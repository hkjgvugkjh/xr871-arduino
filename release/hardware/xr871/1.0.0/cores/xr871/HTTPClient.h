// Minimal HTTPClient stub
#ifndef _HTTPCLIENT_XR871_H_
#define _HTTPCLIENT_XR871_H_
#include <stdint.h>
class HTTPClient {
public:
    HTTPClient() {}
    ~HTTPClient() {}
    bool begin(const char*) { return false; }
    bool begin(const char*, const char*) { return false; }
    void end() {}
    int GET() { return 0; }
    int POST(const char*) { return 0; }
    int PUT(const char*) { return 0; }
    int PATCH(const char*) { return 0; }
    int DELETE(const char*) { return 0; }
    int sendRequest(const char*) { return 0; }
    void addHeader(const char*, const char*, bool=false, bool=true) {}
    void setAuthorization(const char*, const char*) {}
    void setAuthorization(const char*) {}
    void setUserAgent(const char*) {}
    void setReuse(bool) {}
    void setTimeout(uint16_t) {}
    void setFollowRedirects(bool) {}
    int getSize() { return 0; }
    const char* getString() { return ""; }
    bool connected() { return false; }
    void setInsecure(bool) {}
    void collectHeaders(const char**, const size_t) {}
    const char* header(const char*) { return ""; }
    const char* header(size_t) { return ""; }
    const char* headerName(size_t) { return ""; }
    int headers() { return 0; }
    const char* errorToString(int) { return ""; }
};
#endif
