#ifndef _DRIVER_CHIP_HAL_CSI_H_
#define _DRIVER_CHIP_HAL_CSI_H_
#include <stdint.h>
typedef struct { uint32_t width; uint32_t height; uint32_t format; } CSI_InitParam;
static inline void HAL_CSI_Init(CSI_InitParam *param) {}
static inline void HAL_CSI_DeInit(void) {}
static inline void HAL_CSI_Start(void) {}
static inline void HAL_CSI_Stop(void) {}
#endif
