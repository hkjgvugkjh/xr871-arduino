#ifndef _DRIVER_CHIP_HAL_I2C_H_
#define _DRIVER_CHIP_HAL_I2C_H_
#include <stdint.h>
typedef enum { I2C0_ID = 0, I2C1_ID = 1 } I2C_ID;
typedef enum { I2C_SPEED_100K = 100000, I2C_SPEED_400K = 400000 } I2C_Speed;
typedef struct { I2C_Speed speed; uint8_t addr; } I2C_InitParam;
static inline void HAL_I2C_Init(I2C_ID id, I2C_InitParam *param) {}
static inline void HAL_I2C_DeInit(I2C_ID id) {}
static inline int HAL_I2C_MasterSend(I2C_ID id, uint8_t addr, const uint8_t *data, uint16_t len) { return 0; }
static inline int HAL_I2C_MasterReceive(I2C_ID id, uint8_t addr, uint8_t *data, uint16_t len) { return 0; }
#endif
