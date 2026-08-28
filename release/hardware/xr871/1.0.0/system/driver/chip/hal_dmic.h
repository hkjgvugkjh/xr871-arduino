#ifndef _DRIVER_CHIP_HAL_DMIC_H_
#define _DRIVER_CHIP_HAL_DMIC_H_
#include <stdint.h>
typedef struct { uint32_t sampleRate; uint8_t bits; } DMIC_InitParam;
static inline void HAL_DMIC_Init(DMIC_InitParam *param) {}
static inline void HAL_DMIC_DeInit(void) {}
static inline void HAL_DMIC_Start(void) {}
static inline void HAL_DMIC_Stop(void) {}
#endif
