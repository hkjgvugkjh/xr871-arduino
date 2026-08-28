// Minimal Wire stub for compilation
#ifndef _WIRE_XR871_H_
#define _WIRE_XR871_H_
#include <stdint.h>
class TwoWire {
public:
    void begin(int=0, uint32_t=0) {}
    void end() {}
    void setClock(uint32_t) {}
    void beginTransmission(uint8_t) {}
    uint8_t endTransmission(void) { return 0; }
    uint8_t endTransmission(uint8_t) { return 0; }
    uint8_t requestFrom(uint8_t, uint8_t, uint8_t) { return 0; }
    uint8_t requestFrom(uint8_t, uint8_t) { return 0; }
    uint8_t requestFrom(int, int, int) { return 0; }
    uint8_t requestFrom(int, int) { return 0; }
    virtual size_t write(uint8_t) { return 0; }
    virtual size_t write(const uint8_t*, size_t) { return 0; }
    virtual int read(void) { return -1; }
    virtual int available(void) { return 0; }
    virtual int peek(void) { return -1; }
    virtual void flush(void) {}
    void onReceive(void(*)(int)) {}
    void onRequest(void(*)(void)) {}
};
extern TwoWire Wire;
extern TwoWire Wire1;
#endif
