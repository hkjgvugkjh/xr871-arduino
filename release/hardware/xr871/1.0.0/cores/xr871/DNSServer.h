// Minimal DNSServer stub
#ifndef _DNSSERVER_XR871_H_
#define _DNSSERVER_XR871_H_
#include <stdint.h>
class DNSServer {
public:
    DNSServer() {}
    ~DNSServer() {}
    bool start(const char*, const char*) { return false; }
    bool isRunning() { return false; }
    void stop() {}
    void processNextRequest() {}
};
#endif
