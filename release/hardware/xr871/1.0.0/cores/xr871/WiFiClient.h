// Minimal stubs for compilation
#ifndef _WIFICLIENT_XR871_H_
#define _WIFICLIENT_XR871_H_
#include <stdint.h>
class WiFiClient {
public:
    WiFiClient() {}
    WiFiClient(const WiFiClient&) {}
    ~WiFiClient() {}
    int connect(const char*, uint16_t) { return 0; }
    int connect(uint32_t, uint16_t) { return 0; }
    int write(uint8_t) { return 0; }
    int write(const uint8_t*, size_t) { return 0; }
    int available() { return 0; }
    int read() { return -1; }
    int read(uint8_t*, size_t) { return 0; }
    int peek() { return -1; }
    void flush() {}
    void stop() {}
    uint8_t connected() { return 0; }
    uint8_t status() { return 0; }
    operator bool() { return false; }
    WiFiClient* operator->() { return this; }
    bool operator==(const WiFiClient&) const { return false; }
    bool operator!=(const WiFiClient&) const { return true; }
    int fd() { return -1; }
    uint32_t remoteIP() { return 0; }
    uint16_t remotePort() { return 0; }
    uint32_t localIP() { return 0; }
    uint16_t localPort() { return 0; }
    void setNoDelay(bool) {}
    bool getNoDelay() { return false; }
    void setSync(bool) {}
    bool getSync() { return false; }
    void setTimeout(uint32_t) {}
    size_t write(const char*) { return 0; }
    int availableForWrite() { return 0; }
};
#endif
