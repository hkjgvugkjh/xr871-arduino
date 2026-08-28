// Minimal WebServer stub
#ifndef _WEBSERVER_XR871_H_
#define _WEBSERVER_XR871_H_
#include <stdint.h>
typedef void (*THandlerFunction)(void);
class WebServer {
public:
    WebServer(uint16_t port=80) {}
    ~WebServer() {}
    void begin() {}
    void begin(uint16_t) {}
    void end() {}
    void on(const char*, THandlerFunction) {}
    void on(const char*, THandlerFunction, THandlerFunction) {}
    void onNotFound(THandlerFunction) {}
    void handleClient() {}
    const char* hostHeader() { return ""; }
    const char* uri() { return ""; }
    const char* method() { return ""; }
    const char* arg(const char*) { return ""; }
    const char* arg(int) { return ""; }
    const char* argName(int) { return ""; }
    int args() { return 0; }
    bool hasArg(const char*) { return false; }
    const char* header(const char*) { return ""; }
    const char* header(int) { return ""; }
    const char* headerName(int) { return ""; }
    int headers() { return 0; }
    bool hasHeader(const char*) { return false; }
    void send(int, const char*, const char*) {}
    void send_P(int, const char*, const char*) {}
    void setContentLength(size_t) {}
    void collectHeaders(const char**, const size_t) {}
    bool authenticate(const char*, const char*) { return true; }
    void requestAuthentication(const char*) {}
    const char* upload() { return ""; }
};
#endif
