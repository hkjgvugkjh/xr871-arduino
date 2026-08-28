#ifndef _DRIVER_CHIP_HAL_PWM_H_
#define _DRIVER_CHIP_HAL_PWM_H_
#include <stdint.h>
typedef enum { PWM_CH0 = 0, PWM_CH1 = 1, PWM_CH2 = 2, PWM_CH3 = 3, PWM_CH4 = 4, PWM_CH5 = 5, PWM_CH6 = 6, PWM_CH7 = 7, PWM_CH_NULL = 255 } PWM_CH_ID;
typedef struct { uint32_t freq; uint16_t duty; uint8_t polarity; } PWM_InitParam;
static inline void HAL_PWM_Init(PWM_CH_ID ch, PWM_InitParam *param) {}
static inline void HAL_PWM_DeInit(PWM_CH_ID ch) {}
static inline void HAL_PWM_Start(PWM_CH_ID ch) {}
static inline void HAL_PWM_Stop(PWM_CH_ID ch) {}
static inline void HAL_PWM_SetDuty(PWM_CH_ID ch, uint16_t duty) {}
#endif
