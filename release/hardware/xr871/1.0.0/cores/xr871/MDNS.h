// Minimal MDNS stub
#ifndef _MDNS_XR871_H_
#define _MDNS_XR871_H_
#include <stdint.h>
class MDNS {
public:
    MDNS() {}
    ~MDNS() {}
    bool begin(const char*) { return false; }
    void end() {}
    bool addService(const char*, const char*, uint16_t) { return false; }
    bool removeService(const char*, const char*) { return false; }
    void update() {}
    int queryService(const char*, const char*) { return 0; }
    const char* hostname(int) { return ""; }
    const char* service(int) { return ""; }
    const char* protocol(int) { return ""; }
    uint16_t port(int) { return 0; }
    const char* IP(int) { return ""; }
    void setInstanceName(const char*) {}
    void enableArduino(uint16_t=3232, bool=false) {}
};
#endif
