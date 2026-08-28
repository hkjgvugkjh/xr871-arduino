#ifndef _DRIVER_CHIP_HAL_I2S_H_
#define _DRIVER_CHIP_HAL_I2S_H_
#include <stdint.h>
typedef enum { I2S0 = 0, I2S1 = 1 } I2S_ID;
typedef enum { I2S_SAMPLE_8K = 8000, I2S_SAMPLE_16K = 16000, I2S_SAMPLE_44K = 44100 } I2S_SampleRate;
typedef enum { I2S_BITS_8 = 8, I2S_BITS_16 = 16, I2S_BITS_24 = 24 } I2S_Bits;
typedef struct { I2S_SampleRate rate; I2S_Bits bits; uint8_t channels; } I2S_InitParam;
static inline void HAL_I2S_Init(I2S_ID id, I2S_InitParam *param) {}
static inline void HAL_I2S_DeInit(I2S_ID id) {}
static inline void HAL_I2S_SendData(I2S_ID id, const uint8_t *data, uint16_t len) {}
static inline void HAL_I2S_ReceiveData(I2S_ID id, uint8_t *data, uint16_t len) {}
#endif
