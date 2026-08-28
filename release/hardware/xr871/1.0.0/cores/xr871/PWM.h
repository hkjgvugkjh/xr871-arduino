// Minimal PWM stub for compilation
#ifndef _PWM_XR871_H_
#define _PWM_XR871_H_
#include <stdint.h>
class PWMClass {
public:
    void begin(uint8_t, uint32_t) {}
    void end(uint8_t) {}
    void setFreq(uint8_t, uint32_t) {}
    void setDuty(uint8_t, uint16_t) {}
    void enable(uint8_t) {}
    void disable(uint8_t) {}
};
extern PWMClass PWM;
#endif
