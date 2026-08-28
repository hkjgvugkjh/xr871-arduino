#ifndef _DRIVER_CHIP_HAL_GPIO_H_
#define _DRIVER_CHIP_HAL_GPIO_H_

#include <stdint.h>
#include <stdbool.h>

typedef enum {
    GPIO_PORT_A = 0,
    GPIO_PORT_B = 1,
    GPIO_PORT_C = 2,
} GPIO_Port;

typedef enum {
    GPIO_PIN_0 = 0,
    GPIO_PIN_1 = 1,
    GPIO_PIN_2 = 2,
    GPIO_PIN_3 = 3,
    GPIO_PIN_4 = 4,
    GPIO_PIN_5 = 5,
    GPIO_PIN_6 = 6,
    GPIO_PIN_7 = 7,
    GPIO_PIN_8 = 8,
    GPIO_PIN_9 = 9,
    GPIO_PIN_10 = 10,
    GPIO_PIN_11 = 11,
    GPIO_PIN_12 = 12,
    GPIO_PIN_13 = 13,
    GPIO_PIN_14 = 14,
    GPIO_PIN_15 = 15,
    GPIO_PIN_16 = 16,
    GPIO_PIN_17 = 17,
    GPIO_PIN_18 = 18,
    GPIO_PIN_19 = 19,
    GPIO_PIN_20 = 20,
    GPIO_PIN_21 = 21,
    GPIO_PIN_22 = 22,
    GPIO_PIN_23 = 23,
    GPIO_PIN_ALL = 31,
} GPIO_Pin;

typedef enum {
    GPIO_PIN_LOW = 0,
    GPIO_PIN_HIGH = 1,
} GPIO_PinState;

typedef enum {
    GPIOx_Pn_F0_INPUT = 0,
    GPIOx_Pn_F1_OUTPUT = 1,
    GPIOx_Pn_F2_FUNCTION2 = 2,
    GPIOx_Pn_F3_FUNCTION3 = 3,
} GPIO_Func;

typedef enum {
    GPIO_PULL_NONE = 0,
    GPIO_PULL_UP = 1,
    GPIO_PULL_DOWN = 2,
} GPIO_Pull;

typedef enum {
    GPIO_DRIVING_LEVEL_0 = 0,
    GPIO_DRIVING_LEVEL_1 = 1,
    GPIO_DRIVING_LEVEL_2 = 2,
    GPIO_DRIVING_LEVEL_3 = 3,
} GPIO_Driving;

typedef enum {
    GPIO_IRQ_EVT_RISING_EDGE = 0,
    GPIO_IRQ_EVT_FALLING_EDGE = 1,
    GPIO_IRQ_EVT_BOTH_EDGE = 2,
    GPIO_IRQ_EVT_LOW_LEVEL = 3,
    GPIO_IRQ_EVT_HIGH_LEVEL = 4,
} GPIO_IrqEvent;

typedef void (*GPIO_IRQCallback)(void *arg);

typedef struct {
    GPIO_Func mode;
    GPIO_Pull pull;
    GPIO_Driving driving;
} GPIO_InitParam;

typedef struct {
    GPIO_IrqEvent event;
    GPIO_IRQCallback callback;
    void *arg;
} GPIO_IrqParam;

// Function stubs
static inline void HAL_GPIO_Init(GPIO_Port port, GPIO_Pin pin, GPIO_InitParam *param) {}
static inline void HAL_GPIO_DeInit(GPIO_Port port, GPIO_Pin pin) {}
static inline void HAL_GPIO_WritePin(GPIO_Port port, GPIO_Pin pin, GPIO_PinState state) {}
static inline GPIO_PinState HAL_GPIO_ReadPin(GPIO_Port port, GPIO_Pin pin) { return GPIO_PIN_LOW; }
static inline void HAL_GPIO_EnableIRQ(GPIO_Port port, GPIO_Pin pin, GPIO_IrqParam *param) {}
static inline void HAL_GPIO_DisableIRQ(GPIO_Port port, GPIO_Pin pin) {}
static inline void HAL_GPIO_TogglePin(GPIO_Port port, GPIO_Pin pin) {}

#endif
