/**
 * @file HTTPClient.h
 * @brief ESP32 Compatible HTTPClient for XR871
 * @author Hermes Agent
 * @date 2026-08-27
 *
 * Provides ESP32 Arduino HTTPClient API wrapper on top of XR871 HTTP client.
 * Supports GET, POST, PUT with HTTPS (optional).
 */

#ifndef _HTTP_CLIENT_XR871_H_
#define _HTTP_CLIENT_XR871_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include "WiFiClient.h"

// ============================================================
// HTTP Status Codes
// ============================================================
#define HTTP_CODE_OK 200
#define HTTP_CODE_CREATED 201
#define HTTP_CODE_ACCEPTED 202
#define HTTP_CODE_NO_CONTENT 204
#define HTTP_CODE_MOVED_PERMANENTLY 301
#define HTTP_CODE_FOUND 302
#define HTTP_CODE_NOT_MODIFIED 304
#define HTTP_CODE_BAD_REQUEST 400
#define HTTP_CODE_UNAUTHORIZED 401
#define HTTP_CODE_FORBIDDEN 403
#define HTTP_CODE_NOT_FOUND 404
#define HTTP_CODE_METHOD_NOT_ALLOWED 405
#define HTTP_CODE_INTERNAL_SERVER_ERROR 500
#define HTTP_CODE_NOT_IMPLEMENTED 501
#define HTTP_CODE_BAD_GATEWAY 502
#define HTTP_CODE_SERVICE_UNAVAILABLE 503

// ============================================================
// HTTPClient Class
// ============================================================
class HTTPClient {
public:
    HTTPClient();
    ~HTTPClient();
    
    // Begin HTTP session
    bool begin(String url);
    bool begin(String url, const char* CAcert);
    bool begin(String host, uint16_t port, String path);
    bool begin(String host, uint16_t port, String path, const char* CAcert);
    bool begin(String host, uint16_t port, String path, bool https, const char* CAcert = NULL);
    
    // End HTTP session
    void end();
    
    // HTTP methods
    int GET();
    int POST(String payload);
    int POST(const uint8_t* payload, size_t size);
    int PUT(String payload);
    int PUT(const uint8_t* payload, size_t size);
    int PATCH(String payload);
    int PATCH(const uint8_t* payload, size_t size);
    int DELETE(String payload);
    
    // Send HTTP request (generic)
    int sendRequest(const char* type, String payload);
    int sendRequest(const char* type, const uint8_t* payload, size_t size);
    int sendRequest(const char* type);
    
    // Headers
    void addHeader(const String& name, const String& value, bool first = false, bool replace = true);
    void setAuthorization(const char* user, const char* password);
    void setAuthorization(const char* auth);
    void setUserAgent(const String& userAgent);
    void setReuse(bool reuse);
    void setTimeout(uint16_t timeout);
    void setFollowRedirects(bool follow);
    
    // Response handling
    int getSize();
    String getString();
    Stream& getStream();
    WiFiClient& getStreamPtr();
    
    // Connection management
    bool connected();
    void setInsecure(bool insecure);
    
    // Collect headers
    void collectHeaders(const char* headerKeys[], const size_t headerKeysCount);
    String header(const char* name);
    String header(size_t i);
    String headerName(size_t i);
    int headers();
    
    // Error handling
    String errorToString(int code);

private:
    String _url;
    String _host;
    String _path;
    uint16_t _port;
    bool _https;
    bool _connected;
    int _statusCode;
    int _contentLength;
    int _responseSize;
    String _response;
    bool _reuse;
    uint16_t _timeout;
    bool _followRedirects;
    String _userAgent;
    String _authorization;
    
    // Headers
    String _requestHeaders;
    String _responseHeaders;
    static const int HTTP_MAX_HEADERS = 10;
    String _headerKeys[HTTP_MAX_HEADERS];
    String _headerValues[HTTP_MAX_HEADERS];
    int _headerCount;
    int _headerIndex;
    
    // Internal functions
    void disconnect();
    void clear();
    String parseUrl(String url);
    int handleHeaderResponse();
};

#ifdef __cplusplus
}
#endif

#endif /* _HTTP_CLIENT_XR871_H_ */
