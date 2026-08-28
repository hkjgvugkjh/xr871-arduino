/**
 * @file WebServer.h
 * @brief ESP32 Compatible WebServer for XR871
 * @author Hermes Agent
 * @date 2026-08-27
 *
 * Provides ESP32 Arduino WebServer API wrapper on top of XR871 shttpd.
 * Supports URL handlers, POST data, streaming responses.
 */

#ifndef _WEBSERVER_XR871_H_
#define _WEBSERVER_XR871_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include "WiFiClient.h"

// ============================================================
// WebServer Configuration
// ============================================================
#define WEBSERVER_MAX_HANDLERS 20
#define WEBSERVER_MAX_URL_LENGTH 128

// ============================================================
// Handler Function Type
// ============================================================
typedef void (*THandlerFunction)(void);

// ============================================================
// WebServer Class (ESP32 Compatible)
// ============================================================
class WebServer {
public:
    WebServer(uint16_t port = 80);
    ~WebServer();
    
    // Start the server
    void begin();
    void begin(uint16_t port);
    void end();
    
    // Register URL handlers
    void on(const char* uri, THandlerFunction handler);
    void on(const char* uri, THandlerFunction handler, THandlerFunction handler2);
    void onNotFound(THandlerFunction handler);
    
    // Handle client requests (must be called in loop)
    void handleClient();
    
    // Request information
    String hostHeader();
    String uri();
    String method();
    String arg(const char* name);
    String arg(int i);
    String argName(int i);
    int args();
    bool hasArg(const char* name);
    String header(const char* name);
    String header(int i);
    String headerName(int i);
    int headers();
    bool hasHeader(const char* name);
    
    // Response
    void send(int code, const char* content_type = NULL, const String& content = "");
    void send(int code, const char* content_type, const char* content);
    void send(int code, const char* content_type, const uint8_t* content, size_t len);
    void send_P(int code, const char* content_type, const char* content);
    void send_P(int code, const char* content_type, const uint8_t* content, size_t len);
    void setContentLength(size_t len);
    void collectHeaders(const char* headerKeys[], const size_t headerKeysCount);
    
    // Authentication
    bool authenticate(const char* username, const char* password);
    void requestAuthentication(const char* realm = NULL);
    
    // Upload handling
    String upload();
    
private:
    uint16_t _port;
    bool _listening;
    WiFiServer* _server;
    
    // Handler entries
    struct HandlerEntry {
        char url[WEBSERVER_MAX_URL_LENGTH];
        THandlerFunction GET_handler;
        THandlerFunction POST_handler;
    };
    
    HandlerEntry _handlers[WEBSERVER_MAX_HANDLERS];
    int _handlerCount;
    THandlerFunction _notFoundHandler;
    
    // Request state
    WiFiClient _currentClient;
    String _currentUri;
    String _currentMethod;
    String _hostHeader;
    String _responseHeaders;
    
    // Internal functions
    void handleRequest();
    void processHeaders();
    void sendResponse(int code, const char* content_type, const String& content);
    String urlDecode(const String& text);
    String getQueryParam(const String& url, const String& param);
};

#ifdef __cplusplus
}
#endif

#endif /* _WEBSERVER_XR871_H_ */
