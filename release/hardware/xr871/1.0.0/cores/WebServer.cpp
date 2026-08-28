/**
 * @file WebServer.cpp
 * @brief ESP32 Compatible WebServer Implementation for XR871
 * @author Hermes Agent
 * @date 2026-08-27
 */

#include "WebServer.h"
#include "WiFi.h"
#include <string.h>

WebServer::WebServer(uint16_t port) {
    _port = port;
    _listening = false;
    _server = NULL;
    _handlerCount = 0;
    _notFoundHandler = NULL;
}

WebServer::~WebServer() {
    end();
}

void WebServer::begin() {
    begin(_port);
}

void WebServer::begin(uint16_t port) {
    _port = port;
    _server = new WiFiServer(_port);
    _server->begin();
    _listening = true;
}

void WebServer::end() {
    if (_server) {
        _server->end();
        delete _server;
        _server = NULL;
    }
    _listening = false;
}

void WebServer::on(const char* uri, THandlerFunction handler) {
    if (_handlerCount >= WEBSERVER_MAX_HANDLERS) return;
    strncpy(_handlers[_handlerCount].url, uri, WEBSERVER_MAX_URL_LENGTH-1);
    _handlers[_handlerCount].GET_handler = handler;
    _handlers[_handlerCount].POST_handler = NULL;
    _handlerCount++;
}

void WebServer::on(const char* uri, THandlerFunction handler, THandlerFunction handler2) {
    if (_handlerCount >= WEBSERVER_MAX_HANDLERS) return;
    strncpy(_handlers[_handlerCount].url, uri, WEBSERVER_MAX_URL_LENGTH-1);
    _handlers[_handlerCount].GET_handler = handler;
    _handlers[_handlerCount].POST_handler = handler2;
    _handlerCount++;
}

void WebServer::onNotFound(THandlerFunction handler) {
    _notFoundHandler = handler;
}

void WebServer::handleClient() {
    if (!_listening || !_server) return;
    
    WiFiClient client = _server->available();
    if (client) {
        _currentClient = client;
        handleRequest();
        client.stop();
    }
}

void WebServer::handleRequest() {
    // Read request line
    String request = _currentClient.readStringUntil('\r');
    _currentClient.read(); // consume '\n'
    
    // Parse method and URI
    int spaceIndex = request.indexOf(' ');
    if (spaceIndex < 0) return;
    _currentMethod = request.substring(0, spaceIndex);
    _currentUri = request.substring(spaceIndex + 1);
    
    int space2 = _currentUri.indexOf(' ');
    if (space2 > 0) {
        _currentUri = _currentUri.substring(0, space2);
    }
    
    // Read headers
    processHeaders();
    
    // Find handler
    bool handled = false;
    for (int i = 0; i < _handlerCount; i++) {
        if (strcmp(_currentUri.c_str(), _handlers[i].url) == 0) {
            if (_currentMethod == "GET" && _handlers[i].GET_handler) {
                _handlers[i].GET_handler();
                handled = true;
            } else if (_currentMethod == "POST" && _handlers[i].POST_handler) {
                _handlers[i].POST_handler();
                handled = true;
            }
            break;
        }
    }
    
    if (!handled && _notFoundHandler) {
        _notFoundHandler();
    }
}

void WebServer::processHeaders() {
    while (_currentClient.available()) {
        String line = _currentClient.readStringUntil('\r');
        _currentClient.read(); // consume '\n'
        if (line.length() == 0) break; // End of headers
        
        int colon = line.indexOf(':');
        if (colon > 0) {
            String name = line.substring(0, colon);
            String value = line.substring(colon + 1);
            value.trim();
            if (name == "Host") {
                _hostHeader = value;
            }
        }
    }
}

String WebServer::hostHeader() {
    return _hostHeader;
}

String WebServer::uri() {
    return _currentUri;
}

String WebServer::method() {
    return _currentMethod;
}

String WebServer::arg(const char* name) {
    // Parse query string from URI
    int queryIndex = _currentUri.indexOf('?');
    if (queryIndex < 0) return String();
    String query = _currentUri.substring(queryIndex + 1);
    return getQueryParam(query, name);
}

String WebServer::arg(int i) {
    int queryIndex = _currentUri.indexOf('?');
    if (queryIndex < 0) return String();
    String query = _currentUri.substring(queryIndex + 1);
    
    int start = 0;
    int argCount = 0;
    for (int j = 0; j <= query.length(); j++) {
        if (query[j] == '&' || query[j] == '\0') {
            if (argCount == i) {
                return query.substring(start, j);
            }
            start = j + 1;
            argCount++;
        }
    }
    return String();
}

String WebServer::argName(int i) {
    String a = arg(i);
    int eq = a.indexOf('=');
    if (eq > 0) return a.substring(0, eq);
    return a;
}

int WebServer::args() {
    int queryIndex = _currentUri.indexOf('?');
    if (queryIndex < 0) return 0;
    String query = _currentUri.substring(queryIndex + 1);
    int count = 1;
    for (int j = 0; j < query.length(); j++) {
        if (query[j] == '&') count++;
    }
    return count;
}

bool WebServer::hasArg(const char* name) {
    return arg(name).length() > 0;
}

String WebServer::header(const char* name) {
    return "";
}

String WebServer::header(int i) {
    return "";
}

String WebServer::headerName(size_t i) {
    return "";
}

int WebServer::headers() {
    return 0;
}

bool WebServer::hasHeader(const char* name) {
    return false;
}

void WebServer::send(int code, const char* content_type, const String& content) {
    sendResponse(code, content_type, content);
}

void WebServer::send(int code, const char* content_type, const char* content) {
    sendResponse(code, content_type, String(content));
}

void WebServer::send(int code, const char* content_type, const uint8_t* content, size_t len) {
    sendResponse(code, content_type, String((const char*)content, len));
}

void WebServer::send_P(int code, const char* content_type, const char* content) {
    send(code, content_type, content);
}

void WebServer::send_P(int code, const char* content_type, const uint8_t* content, size_t len) {
    send(code, content_type, content, len);
}

void WebServer::setContentLength(size_t len) {
    // TODO
}

void WebServer::collectHeaders(const char* headerKeys[], const size_t headerKeysCount) {
    // TODO
}

bool WebServer::authenticate(const char* username, const char* password) {
    return true;
}

void WebServer::requestAuthentication(const char* realm) {
    send(401, "text/plain", "Authentication Required");
}

String WebServer::upload() {
    return String();
}

String WebServer::urlDecode(const String& text) {
    String decoded = "";
    for (int i = 0; i < text.length(); i++) {
        if (text[i] == '%' && i + 2 < text.length()) {
            char hex[3] = { text[i+1], text[i+2], '\0' };
            decoded += (char)strtol(hex, NULL, 16);
            i += 2;
        } else if (text[i] == '+') {
            decoded += ' ';
        } else {
            decoded += text[i];
        }
    }
    return decoded;
}

String WebServer::getQueryParam(const String& query, const String& param) {
    String search = param + "=";
    int start = query.indexOf(search);
    if (start < 0) return String();
    start += search.length();
    int end = query.indexOf('&', start);
    if (end < 0) end = query.length();
    return urlDecode(query.substring(start, end));
}

void WebServer::sendResponse(int code, const char* content_type, const String& content) {
    if (!_currentClient.connected()) return;
    
    _currentClient.print("HTTP/1.1 ");
    _currentClient.print(code);
    _currentClient.println(" OK");
    _currentClient.print("Content-Type: ");
    _currentClient.println(content_type ? content_type : "text/html");
    _currentClient.print("Content-Length: ");
    _currentClient.println(content.length());
    _currentClient.println("Connection: close");
    _currentClient.println();
    _currentClient.println(content);
}
