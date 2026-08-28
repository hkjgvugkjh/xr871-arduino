#ifndef _DRIVER_CHIP_HAL_CLOCK_H_
#define _DRIVER_CHIP_HAL_CLOCK_H_
#include <stdint.h>
static inline uint32_t HAL_Clock_GetSysClk(void) { return 192000000; }
static inline void HAL_Clock_Init(void) {}
#endif
