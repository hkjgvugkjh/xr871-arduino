/**
 * @file variant.cpp
 * @brief XR871GT Pin Mapping Implementation
 * @author Hermes Agent
 * @date 2026-08-27
 */

#include "pins_arduino.h"

// ============================================================
// Pin mapping tables
// ============================================================

// Digital pin to GPIO port mapping
const uint8_t digital_pin_to_port[NUM_DIGITAL_PINS] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // PA0-PA22
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1   // PB0-PB15
};

// Digital pin to bit mapping
const uint8_t digital_pin_to_bit[NUM_DIGITAL_PINS] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22,
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15
};

// Digital pin to ADC channel (255 = no ADC)
const uint8_t digital_pin_to_channel[NUM_DIGITAL_PINS] = {
    255, 255, 255, 255, 255, 255, 255, 255, 0, 1, 2, 3, 4, 5, 6, 7, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255
};

// Digital pin to PWM channel (255 = no PWM)
const uint8_t digital_pin_to_pwm[NUM_DIGITAL_PINS] = {
    255, 255, 255, 255, 255, 255, 255, 255, 0, 1, 2, 3, 4, 5, 6, 7, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255
};

// Digital pin to ADC channel
const uint8_t digital_pin_to_adc[NUM_DIGITAL_PINS] = {
    255, 255, 255, 255, 255, 255, 255, 255, 0, 1, 2, 3, 4, 5, 6, 7, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255
};

// Digital pin to SPI capability
const uint8_t digital_pin_to_spi[NUM_DIGITAL_PINS] = {
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 0, 1, 255, 2, 3, 255, 255, 255, 255, 255, 255, 255, 255
};

// Digital pin to I2C capability
const uint8_t digital_pin_to_i2c[NUM_DIGITAL_PINS] = {
    255, 255, 0, 1, 2, 3, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255
};

// Digital pin to UART capability
const uint8_t digital_pin_to_uart[NUM_DIGITAL_PINS] = {
    1, 0, 255, 255, 255, 255, 2, 3, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255
};

// Pin map
const pin_map_t pinMap[NUM_DIGITAL_PINS] = {
    {0, 0, 255, DIGITAL | INTERRUPT},                    // PA0
    {0, 1, 255, DIGITAL | INTERRUPT | UART},             // PA1 - UART0_TX
    {0, 2, 255, DIGITAL | INTERRUPT | I2C},              // PA2 - I2C0_SCL
    {0, 3, 255, DIGITAL | INTERRUPT | I2C},              // PA3 - I2C0_SDA
    {0, 4, 255, DIGITAL | INTERRUPT | I2C},              // PA4 - I2C1_SCL
    {0, 5, 255, DIGITAL | INTERRUPT | I2C},              // PA5 - I2C1_SDA
    {0, 6, 255, DIGITAL | INTERRUPT | UART},             // PA6 - UART1_TX
    {0, 7, 255, DIGITAL | INTERRUPT | UART},             // PA7 - UART1_RX
    {0, 8, 0, DIGITAL | INTERRUPT | PWM | ADC},          // PA8 - PWM0/ADC0
    {0, 9, 1, DIGITAL | INTERRUPT | PWM | ADC},          // PA9 - PWM1/ADC1
    {0, 10, 2, DIGITAL | INTERRUPT | PWM | ADC},         // PA10 - PWM2/ADC2
    {0, 11, 3, DIGITAL | INTERRUPT | PWM | ADC},         // PA11 - PWM3/ADC3
    {0, 12, 4, DIGITAL | INTERRUPT | PWM | ADC},         // PA12 - PWM4/ADC4
    {0, 13, 5, DIGITAL | INTERRUPT | PWM | ADC},         // PA13 - PWM5/ADC5
    {0, 14, 6, DIGITAL | INTERRUPT | PWM | ADC},         // PA14 - PWM6/ADC6
    {0, 15, 7, DIGITAL | INTERRUPT | PWM | ADC},         // PA15 - PWM7/ADC7
    {0, 16, 255, DIGITAL | INTERRUPT},                   // PA16
    {0, 17, 255, DIGITAL | INTERRUPT},                   // PA17
    {0, 18, 255, DIGITAL | INTERRUPT},                   // PA18
    {0, 19, 255, DIGITAL | INTERRUPT},                   // PA19
    {0, 20, 255, DIGITAL | INTERRUPT},                   // PA20
    {0, 21, 255, DIGITAL | INTERRUPT},                   // PA21
    {0, 22, 255, DIGITAL | INTERRUPT},                   // PA22
    {1, 0, 255, DIGITAL | INTERRUPT},                    // PB0
    {1, 1, 255, DIGITAL | INTERRUPT},                    // PB1
    {1, 2, 255, DIGITAL | INTERRUPT},                    // PB2
    {1, 3, 255, DIGITAL | INTERRUPT},                    // PB3
    {1, 4, 255, DIGITAL | INTERRUPT | SPI},              // PB4 - SPI0_MOSI
    {1, 5, 255, DIGITAL | INTERRUPT | SPI},              // PB5 - SPI0_MISO
    {1, 6, 255, DIGITAL | INTERRUPT | SPI},              // PB6 - SPI0_CS0
    {1, 7, 255, DIGITAL | INTERRUPT | SPI},              // PB7 - SPI0_CLK
    {1, 8, 255, DIGITAL | INTERRUPT},                    // PB8
    {1, 9, 255, DIGITAL | INTERRUPT},                    // PB9
    {1, 10, 255, DIGITAL | INTERRUPT},                   // PB10
    {1, 11, 255, DIGITAL | INTERRUPT},                   // PB11
    {1, 12, 255, DIGITAL | INTERRUPT},                   // PB12
    {1, 13, 255, DIGITAL | INTERRUPT},                   // PB13
    {1, 14, 255, DIGITAL | INTERRUPT},                   // PB14
    {1, 15, 255, DIGITAL | INTERRUPT},                   // PB15
};
