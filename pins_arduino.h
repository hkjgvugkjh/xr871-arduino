/**
 * @file pins_arduino.h
 * @brief Arduino Pin Mapping for XR871 EVB
 *
 * XR871 GPIO: PA0-PA22, PB0-PB15
 * Arduino pin numbers: pin 0-22 = PA0-PA22, 23-38 = PB0-PB15
 */

#ifndef _PINS_ARDUINO_H_
#define _PINS_ARDUINO_H_

#include <stdint.h>
#include "driver/chip/hal_gpio.h"
#include "driver/chip/hal_adc.h"
#include "driver/chip/hal_pwm.h"

#ifdef __cplusplus
extern "C" {
#endif

// ============================================================
// Pin Count
// ============================================================
#define PINS_COUNT          39
#define NUM_DIGITAL_PINS    39
#define NUM_ANALOG_INPUTS   9
#define NUM_PWM_PINS        8

// ============================================================
// Pin Mapping Table
// ============================================================
typedef struct {
    uint8_t     arduino_pin;
    GPIO_Port   port;
    GPIO_Pin    pin;
    uint8_t     adc_channel;    // ADC_CHANNEL_* or 255
    uint8_t     pwm_channel;    // PWM_CH_ID or 255
} pin_map_t;

static const pin_map_t pinMap[] = {
    // Pin | Port       | Pin         | ADC Ch      | PWM Ch
    { 0,  GPIO_PORT_A, GPIO_PIN_0,  255,         255                },
    { 1,  GPIO_PORT_A, GPIO_PIN_1,  255,         255                },
    { 2,  GPIO_PORT_A, GPIO_PIN_2,  255,         255                },
    { 3,  GPIO_PORT_A, GPIO_PIN_3,  255,         255                },
    { 4,  GPIO_PORT_A, GPIO_PIN_4,  255,         255                },
    { 5,  GPIO_PORT_A, GPIO_PIN_5,  255,         255                },
    { 6,  GPIO_PORT_A, GPIO_PIN_6,  255,         255                },
    { 7,  GPIO_PORT_A, GPIO_PIN_7,  255,         255                },
    { 8,  GPIO_PORT_A, GPIO_PIN_8,  ADC_CHANNEL_0, PWM_GROUP0_CH0  },
    { 9,  GPIO_PORT_A, GPIO_PIN_9,  ADC_CHANNEL_1, PWM_GROUP0_CH1  },
    { 10, GPIO_PORT_A, GPIO_PIN_10, ADC_CHANNEL_2, PWM_GROUP1_CH2  },
    { 11, GPIO_PORT_A, GPIO_PIN_11, ADC_CHANNEL_3, PWM_GROUP1_CH3  },
    { 12, GPIO_PORT_A, GPIO_PIN_12, ADC_CHANNEL_4, PWM_GROUP2_CH4  },
    { 13, GPIO_PORT_A, GPIO_PIN_13, ADC_CHANNEL_5, PWM_GROUP2_CH5  },
    { 14, GPIO_PORT_A, GPIO_PIN_14, ADC_CHANNEL_6, PWM_GROUP3_CH6  },
    { 15, GPIO_PORT_A, GPIO_PIN_15, ADC_CHANNEL_7, PWM_GROUP3_CH7  },
    { 16, GPIO_PORT_A, GPIO_PIN_16, 255,         255                },
    { 17, GPIO_PORT_A, GPIO_PIN_17, 255,         255                },
    { 18, GPIO_PORT_A, GPIO_PIN_18, 255,         255                },
    { 19, GPIO_PORT_A, GPIO_PIN_19, 255,         255                },
    { 20, GPIO_PORT_A, GPIO_PIN_20, 255,         255                },
    { 21, GPIO_PORT_A, GPIO_PIN_21, 255,         255                },
    { 22, GPIO_PORT_A, GPIO_PIN_22, 255,         255                },
    { 23, GPIO_PORT_B, GPIO_PIN_0,  255,         255                },
    { 24, GPIO_PORT_B, GPIO_PIN_1,  255,         255                },
    { 25, GPIO_PORT_B, GPIO_PIN_2,  255,         255                },
    { 26, GPIO_PORT_B, GPIO_PIN_3,  255,         255                },
    { 27, GPIO_PORT_B, GPIO_PIN_4,  255,         255                },
    { 28, GPIO_PORT_B, GPIO_PIN_5,  255,         255                },
    { 29, GPIO_PORT_B, GPIO_PIN_6,  255,         255                },
    { 30, GPIO_PORT_B, GPIO_PIN_7,  255,         255                },
    { 31, GPIO_PORT_B, GPIO_PIN_8,  255,         255                },
    { 32, GPIO_PORT_B, GPIO_PIN_9,  255,         255                },
    { 33, GPIO_PORT_B, GPIO_PIN_10, 255,         255                },
    { 34, GPIO_PORT_B, GPIO_PIN_11, 255,         255                },
    { 35, GPIO_PORT_B, GPIO_PIN_12, 255,         255                },
    { 36, GPIO_PORT_B, GPIO_PIN_13, 255,         255                },
    { 37, GPIO_PORT_B, GPIO_PIN_14, 255,         255                },
    { 38, GPIO_PORT_B, GPIO_PIN_15, 255,         255                },
};

// ============================================================
// Helper Functions (extern for use in Arduino.c)
// ============================================================
#define pinToPort(p)     ((p) < 23 ? GPIO_PORT_A : GPIO_PORT_B)
#define pinToPin(p)      ((GPIO_Pin)((p) < 23 ? (p) : ((p) - 23)))
#define pinToADC(p)      (((p) >= 8 && (p) <= 15) ? ((p) - 8) : 255)
#define pinToPWM(p)      (((p) >= 8 && (p) <= 15) ? ((p) - 8) : 255)

// ============================================================
// Special Pin Definitions
// ============================================================
#define LED_BUILTIN     6       // PA6 - onboard LED
#define LED_ON          LOW     // Active low

// UART0 (Console)
#define PIN_UART0_TX    0       // PA0
#define PIN_UART0_RX    1       // PA1

// UART1
#define PIN_UART1_TX    6       // PA6
#define PIN_UART1_RX    7       // PA7

// I2C0
#define PIN_I2C0_SCL    4       // PA4
#define PIN_I2C0_SDA    5       // PA5

// I2C1
#define PIN_I2C1_SCL    2       // PA2
#define PIN_I2C1_SDA    3       // PA3

// SPI0
#define PIN_SPI0_CS     29      // PB6
#define PIN_SPI0_MOSI   27      // PB4
#define PIN_SPI0_MISO   28      // PB5
#define PIN_SPI0_SCK    30      // PB7

// SPI1
#define PIN_SPI1_CS     3       // PA3
#define PIN_SPI1_MOSI   0       // PA0
#define PIN_SPI1_MISO   1       // PA1
#define PIN_SPI1_SCK    2       // PA2

#ifdef __cplusplus
}
#endif

#endif /* _PINS_ARDUINO_H_ */
