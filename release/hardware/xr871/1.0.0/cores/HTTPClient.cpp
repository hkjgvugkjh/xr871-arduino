/**
 * @file HTTPClient.cpp
 * @brief ESP32 Compatible HTTPClient Implementation for XR871
 * @author Hermes Agent
 * @date 2026-08-27
 *
 * Implements ESP32 Arduino HTTPClient API using XR871 HTTP client.
 */

#include "HTTPClient.h"
#include "WiFi.h"
#include <string.h>

// ============================================================
// HTTPClient Implementation
// ============================================================

HTTPClient::HTTPClient() {
    clear();
}

HTTPClient::~HTTPClient() {
    end();
}

void HTTPClient::clear() {
    _url = "";
    _host = "";
    _path = "/";
    _port = 80;
    _https = false;
    _connected = false;
    _statusCode = 0;
    _contentLength = 0;
    _responseSize = 0;
    _response = "";
    _reuse = false;
    _timeout = 5000;
    _followRedirects = false;
    _userAgent = "XR871-HTTPClient/1.0";
    _authorization = "";
    _requestHeaders = "";
    _responseHeaders = "";
    _headerCount = 0;
    _headerIndex = 0;
}

bool HTTPClient::begin(String url) {
    clear();
    _url = url;
    parseUrl(url);
    return true;
}

bool HTTPClient::begin(String url, const char* CAcert) {
    return begin(url);
}

bool HTTPClient::begin(String host, uint16_t port, String path) {
    clear();
    _host = host;
    _port = port;
    _path = path;
    _url = host + ":" + String(port) + path;
    return true;
}

bool HTTPClient::begin(String host, uint16_t port, String path, const char* CAcert) {
    return begin(host, port, path);
}

bool HTTPClient::begin(String host, uint16_t port, String path, bool https, const char* CAcert) {
    begin(host, port, path);
    _https = https;
    return true;
}

void HTTPClient::end() {
    disconnect();
    clear();
}

void HTTPClient::disconnect() {
    _connected = false;
}

String HTTPClient::parseUrl(String url) {
    // Parse URL into host, port, path
    int protoIndex = url.indexOf("://");
    if (protoIndex > 0) {
        String protocol = url.substring(0, protoIndex);
        if (protocol == "https") {
            _https = true;
            _port = 443;
        } else {
            _https = false;
            _port = 80;
        }
        url = url.substring(protoIndex + 3);
    }
    
    int slashIndex = url.indexOf("/");
    if (slashIndex > 0) {
        _host = url.substring(0, slashIndex);
        _path = url.substring(slashIndex);
    } else {
        _host = url;
        _path = "/";
    }
    
    int colonIndex = _host.indexOf(":");
    if (colonIndex > 0) {
        _port = _host.substring(colonIndex + 1).toInt();
        _host = _host.substring(0, colonIndex);
    }
    
    return _host;
}

int HTTPClient::GET() {
    return sendRequest("GET");
}

int HTTPClient::POST(String payload) {
    return sendRequest("POST", payload);
}

int HTTPClient::POST(const uint8_t* payload, size_t size) {
    return sendRequest("POST", payload, size);
}

int HTTPClient::PUT(String payload) {
    return sendRequest("PUT", payload);
}

int HTTPClient::PUT(const uint8_t* payload, size_t size) {
    return sendRequest("PUT", payload, size);
}

int HTTPClient::PATCH(String payload) {
    return sendRequest("PATCH", payload);
}

int HTTPClient::PATCH(const uint8_t* payload, size_t size) {
    return sendRequest("PATCH", payload, size);
}

int HTTPClient::DELETE(String payload) {
    return sendRequest("DELETE", payload);
}

int HTTPClient::sendRequest(const char* type, String payload) {
    return sendRequest(type, (const uint8_t*)payload.c_str(), payload.length());
}

int HTTPClient::sendRequest(const char* type, const uint8_t* payload, size_t size) {
    // Connect to server
    if (!WiFiClient::connect(_host.c_str(), _port)) {
        return -1;
    }
    
    _connected = true;
    
    // Build HTTP request
    String request = type;
    request += " ";
    request += _path;
    request += " HTTP/1.1\r\n";
    request += "Host: ";
    request += _host;
    request += "\r\n";
    request += "User-Agent: ";
    request += _userAgent;
    request += "\r\n";
    request += "Connection: close\r\n";
    
    if (_authorization.length() > 0) {
        request += "Authorization: ";
        request += _authorization;
        request += "\r\n";
    }
    
    if (payload && size > 0) {
        request += "Content-Length: ";
        request += String((unsigned int)size);
        request += "\r\n";
        request += "Content-Type: application/x-www-form-urlencoded\r\n";
    }
    
    request += _requestHeaders;
    request += "\r\n";
    
    // Send request
    WiFiClient::write((const uint8_t*)request.c_str(), request.length());
    
    if (payload && size > 0) {
        WiFiClient::write(payload, size);
    }
    
    // Wait for response
    unsigned long timeout = millis() + _timeout;
    while (WiFiClient::connected() && millis() < timeout) {
        if (WiFiClient::available()) {
            break;
        }
        delay(1);
    }
    
    // Read response
    String response = "";
    while (WiFiClient::connected() && millis() < timeout) {
        if (WiFiClient::available()) {
            response += (char)WiFiClient::read();
        }
    }
    
    // Parse response
    int httpIndex = response.indexOf("HTTP/1.1 ");
    if (httpIndex >= 0) {
        _statusCode = response.substring(httpIndex + 9, httpIndex + 12).toInt();
    }
    
    int bodyIndex = response.indexOf("\r\n\r\n");
    if (bodyIndex > 0) {
        _response = response.substring(bodyIndex + 4);
    }
    
    WiFiClient::stop();
    _connected = false;
    
    return _statusCode;
}

int HTTPClient::sendRequest(const char* type) {
    return sendRequest(type, (const uint8_t*)NULL, 0);
}

void HTTPClient::addHeader(const String& name, const String& value, bool first, bool replace) {
    _requestHeaders += name;
    _requestHeaders += ": ";
    _requestHeaders += value;
    _requestHeaders += "\r\n";
}

void HTTPClient::setAuthorization(const char* user, const char* password) {
    // TODO: Base64 encode
    _authorization = "Basic ";
    _authorization += user;
    _authorization += ":";
    _authorization += password;
}

void HTTPClient::setAuthorization(const char* auth) {
    _authorization = auth;
}

void HTTPClient::setUserAgent(const String& userAgent) {
    _userAgent = userAgent;
}

void HTTPClient::setReuse(bool reuse) {
    _reuse = reuse;
}

void HTTPClient::setTimeout(uint16_t timeout) {
    _timeout = timeout;
}

void HTTPClient::setFollowRedirects(bool follow) {
    _followRedirects = follow;
}

int HTTPClient::getSize() {
    return _response.length();
}

String HTTPClient::getString() {
    return _response;
}

WiFiClient& HTTPClient::getStreamPtr() {
    return *this;
}

Stream& HTTPClient::getStream() {
    return *this;
}

bool HTTPClient::connected() {
    return _connected;
}

void HTTPClient::setInsecure(bool insecure) {
    // TODO
}

void HTTPClient::collectHeaders(const char* headerKeys[], const size_t headerKeysCount) {
    // TODO
}

String HTTPClient::header(const char* name) {
    return "";
}

String HTTPClient::header(size_t i) {
    return "";
}

String HTTPClient::headerName(size_t i) {
    return "";
}

int HTTPClient::headers() {
    return 0;
}

String HTTPClient::errorToString(int code) {
    switch (code) {
        case -1: return "Connection failed";
        case -2: return "Send failed";
        case -3: return "No response";
        default: return "Unknown error";
    }
}
