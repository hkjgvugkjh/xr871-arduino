// Minimal WiFiUDP stub
#ifndef _WIFIUDP_XR871_H_
#define _WIFIUDP_XR871_H_
#include <stdint.h>
class WiFiUDP {
public:
    WiFiUDP() {}
    ~WiFiUDP() {}
    uint8_t begin(uint16_t) { return 0; }
    uint8_t beginMulticast(uint32_t, uint16_t) { return 0; }
    void stop() {}
    int beginPacket(const char*, uint16_t) { return 0; }
    int beginPacket(uint32_t, uint16_t) { return 0; }
    int beginMulticastPacket() { return 0; }
    int endPacket() { return 0; }
    int write(uint8_t) { return 0; }
    int write(const uint8_t*, size_t) { return 0; }
    int parsePacket() { return 0; }
    int available() { return 0; }
    int read() { return -1; }
    int read(uint8_t*, size_t) { return 0; }
    int read(char*, size_t) { return 0; }
    int peek() { return -1; }
    void flush() {}
    uint32_t remoteIP() { return 0; }
    uint16_t remotePort() { return 0; }
    uint32_t destinationIP() { return 0; }
    uint16_t localPort() { return 0; }
    void multicastIP(uint32_t) {}
    uint32_t multicastIP() { return 0; }
    void setMulticastInterface(uint32_t) {}
    size_t write(const char*) { return 0; }
    int availableForWrite() { return 0; }
    void setTTL(uint8_t) {}
    uint8_t getTTL() { return 0; }
};
#endif
