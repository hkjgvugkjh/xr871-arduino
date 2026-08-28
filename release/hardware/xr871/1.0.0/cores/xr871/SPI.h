// Minimal SPI stub for compilation
#ifndef _SPI_XR871_H_
#define _SPI_XR871_H_
#include <stdint.h>
class SPIClass {
public:
    void begin(int=-1, int=-1, int=-1, int=-1) {}
    void end() {}
    void setFrequency(uint32_t) {}
    void setClockDivider(uint32_t) {}
    void setDataMode(uint8_t) {}
    void setBitOrder(uint8_t) {}
    void beginTransaction(void*) {}
    void endTransaction() {}
    uint8_t transfer(uint8_t) { return 0; }
    void transfer(const void*, void*, size_t) {}
    void write(uint8_t) {}
    void write(const uint8_t*, size_t) {}
    int read() { return -1; }
};
extern SPIClass SPI;
extern SPIClass SPI1;
#endif
