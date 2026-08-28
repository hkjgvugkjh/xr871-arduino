/**
 * @file pins_arduino.h
 * @brief XR871GT Arduino Pin Definitions
 * @author Hermes Agent
 * @date 2026-08-27
 *
 * Pin mapping for XR871GT development board.
 * Maps Arduino pin numbers to XR871 GPIO ports.
 */

#ifndef _PINS_ARDUINO_XR871_H_
#define _PINS_ARDUINO_XR871_H_

#include <stdint.h>

// ============================================================
// Pin count
// ============================================================
#define NUM_DIGITAL_PINS 39    // 0-38
#define NUM_ANALOG_INPUTS  8   // A0-A7
#define NUM_PWM_PINS       8   // PWM0-PWM7
#define NUM_SPI_PINS       6   // MOSI, MISO, SCK, CS, MOSI1, MISO1
#define NUM_I2C_PINS       4   // SDA, SCL, SDA1, SCL1
#define NUM_UART_PINS      4   // TX0, RX0, TX1, RX1

// ============================================================
// Digital pins (Arduino pin -> XR871 GPIO)
// ============================================================
// GPIOA: pin 0-22
// GPIOB: pin 23-38
#define PA0  0
#define PA1  1
#define PA2  2
#define PA3  3
#define PA4  4
#define PA5  5
#define PA6  6
#define PA7  7
#define PA8  8
#define PA9  9
#define PA10 10
#define PA11 11
#define PA12 12
#define PA13 13
#define PA14 14
#define PA15 15
#define PA16 16
#define PA17 17
#define PA18 18
#define PA19 19
#define PA20 20
#define PA21 21
#define PA22 22

#define PB0  23
#define PB1  24
#define PB2  25
#define PB3  26
#define PB4  27
#define PB5  28
#define PB6  29
#define PB7  30
#define PB8  31
#define PB9  32
#define PB10 33
#define PB11 34
#define PB12 35
#define PB13 36
#define PB14 37
#define PB15 38

// ============================================================
// Analog inputs (ADC channels)
// ============================================================
#define A0  39  // ADC_CH0 - PA8
#define A1  40  // ADC_CH1 - PA9
#define A2  41  // ADC_CH2 - PA10
#define A3  42  // ADC_CH3 - PA11
#define A4  43  // ADC_CH4 - PA12
#define A5  44  // ADC_CH5 - PA13
#define A6  45  // ADC_CH6 - PA14
#define A7  46  // ADC_CH7 - PA15

// ============================================================
// PWM channels
// ============================================================
#define PWM0 8   // PA8
#define PWM1 9   // PA9
#define PWM2 10  // PA10
#define PWM3 11  // PA11
#define PWM4 12  // PA12
#define PWM5 13  // PA13
#define PWM6 14  // PA14
#define PWM7 15  // PA15

// ============================================================
// SPI pins (default SPI0)
// ============================================================
#define MOSI 27  // PB4 - SPI0_MOSI
#define MISO 28  // PB5 - SPI0_MISO
#define SCK  30  // PB7 - SPI0_CLK
#define CS   29  // PB6 - SPI0_CS0

// ============================================================
// I2C pins (default I2C0)
// ============================================================
#define SDA  2   // PA2 - I2C0_SDA
#define SCL  3   // PA3 - I2C0_SCL
#define SDA1 4   // PA4 - I2C1_SDA
#define SCL1 5   // PA5 - I2C1_SCL

// ============================================================
// UART pins
// ============================================================
#define TX0  1   // PA1 - UART0_TX
#define RX0  0   // PA0 - UART0_RX
#define TX1  6   // PA6 - UART1_TX
#define RX1  7   // PA7 - UART1_RX

// ============================================================
// Built-in LED
// ============================================================
#define LED_BUILTIN 6   // PA6 - onboard LED
#define LED_ON      LOW
#define LED_OFF     HIGH

// ============================================================
// Special pins
// ============================================================
#define USER_BTN  0  // PA0 - user button
#define WAKEUP_PIN 4 // PA4 - wakeup pin

// ============================================================
// Pin capabilities lookup
// ============================================================
extern const uint8_t digital_pin_to_port[];
extern const uint8_t digital_pin_to_bit[];
extern const uint8_t digital_pin_to_channel[];
extern const uint8_t digital_pin_to_pwm[];
extern const uint8_t digital_pin_to_adc[];
extern const uint8_t digital_pin_to_spi[];
extern const uint8_t digital_pin_to_i2c[];
extern const uint8_t digital_pin_to_uart[];

// Pin capabilities
#define DIGITAL            0x01
#define ANALOG             0x02
#define PWM                0x04
#define SPI                0x08
#define I2C                0x10
#define UART               0x20
#define INTERRUPT          0x40
#define ADC                0x80

// Pin state
#define PIN_STATE_INPUT    0
#define PIN_STATE_OUTPUT   1
#define PIN_STATE_PWM      2
#define PIN_STATE_ANALOG   3
#define PIN_STATE_SPI      4
#define PIN_STATE_I2C      5
#define PIN_STATE_UART     6

// ============================================================
// Pin mapping structure
// ============================================================
typedef struct {
    uint8_t port;       // GPIO port (0=PA, 1=PB)
    uint8_t pin;        // Pin number within port
    uint8_t channel;    // ADC/PWM channel
    uint8_t capabilities; // Pin capabilities
} pin_map_t;

extern const pin_map_t pinMap[];

#endif /* _PINS_ARDUINO_XR871_H_ */
