#ifndef _DRIVER_CHIP_HAL_SPI_H_
#define _DRIVER_CHIP_HAL_SPI_H_
#include <stdint.h>
typedef enum { SPI0 = 0, SPI1 = 1 } SPI_ID;
typedef enum { SPI_MODE0 = 0, SPI_MODE1 = 1, SPI_MODE2 = 2, SPI_MODE3 = 3 } SPI_Mode;
typedef enum { SPI_MSB_FIRST = 0, SPI_LSB_FIRST = 1 } SPI_BitOrder;
typedef struct { SPI_Mode mode; SPI_BitOrder bitOrder; uint32_t clock; } SPI_InitParam;
static inline void HAL_SPI_Init(SPI_ID id, SPI_InitParam *param) {}
static inline void HAL_SPI_DeInit(SPI_ID id) {}
static inline uint8_t HAL_SPI_SendReceive(SPI_ID id, uint8_t data) { return 0; }
static inline void HAL_SPI_SendData(SPI_ID id, const uint8_t *data, uint16_t len) {}
static inline void HAL_SPI_ReceiveData(SPI_ID id, uint8_t *data, uint16_t len) {}
static inline void HAL_SPI_SetCS(SPI_ID id, uint8_t cs, uint8_t state) {}
#endif
