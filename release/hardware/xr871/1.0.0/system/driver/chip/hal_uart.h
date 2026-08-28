#ifndef _DRIVER_CHIP_HAL_UART_H_
#define _DRIVER_CHIP_HAL_UART_H_

#include <stdint.h>

typedef enum {
    UART0_ID = 0,
    UART1_ID = 1,
    UART2_ID = 2,
} UART_ID;

typedef enum {
    UART_MODE_TX_RX = 0,
    UART_MODE_TX_ONLY = 1,
    UART_MODE_RX_ONLY = 2,
} UART_Mode;

typedef enum {
    UART_BAUD_9600 = 9600,
    UART_BAUD_115200 = 115200,
} UART_Baud;

typedef struct {
    UART_Mode mode;
    UART_Baud baud;
} UART_InitParam;

static inline void HAL_UART_Init(UART_ID id, unsigned long baud) {}
static inline void HAL_UART_DeInit(UART_ID id) {}
static inline void HAL_UART_SendChar(UART_ID id, uint8_t c) {}
static inline uint8_t HAL_UART_ReceiveChar(UART_ID id) { return 0; }
static inline int HAL_UART_IsRxReady(UART_ID id) { return 0; }
static inline int HAL_UART_SendData(UART_ID id, const uint8_t *data, uint16_t len) { return 0; }
static inline int HAL_UART_ReceiveData(UART_ID id, uint8_t *data, uint16_t len) { return 0; }

#endif
