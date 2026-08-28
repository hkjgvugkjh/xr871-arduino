#ifndef _DRIVER_CHIP_HAL_ADC_H_
#define _DRIVER_CHIP_HAL_ADC_H_
#include <stdint.h>
typedef enum { ADC_CH0 = 0, ADC_CH1 = 1, ADC_CH2 = 2, ADC_CH3 = 3, ADC_CH4 = 4, ADC_CH5 = 5, ADC_CH6 = 6, ADC_CH7 = 7 } ADC_CH_ID;
static inline void HAL_ADC_Init(void) {}
static inline uint16_t HAL_ADC_Read(ADC_CH_ID ch) { return 0; }
#endif
