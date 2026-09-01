/**
 * @file Arduino.h
 * @brief Arduino Core for XR871 - Header File
 *
 * Simplified Arduino API for XR871 - compiles cleanly.
 * Peripheral libraries can be included separately as needed.
 */

#ifndef _ARDUINO_H_
#define _ARDUINO_H_

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

// Arduino version
#define ARDUINO 10819
#define ARDUINO_XR871 1
#define XR871_ARDUINO_VERSION "1.4.0"

// Basic types
typedef bool boolean;
typedef uint8_t byte;
typedef uint16_t word;

// Pin modes
#define INPUT           0x0
#define OUTPUT          0x1
#define INPUT_PULLUP    0x2
#define INPUT_PULLDOWN  0x4

// Pin levels
#define LOW             0x0
#define HIGH            0x1

// Interrupt modes
#define CHANGE          1
#define FALLING         2
#define RISING          3
#define LOW_LEVEL       4
#define HIGH_LEVEL      5

// Serial configuration
#define SERIAL_8N1      0x06

// Built-in LED pin (PA6 on XR871GT)
#define LED_BUILTIN     6

// Analog settings
#define ADC_RESOLUTION_BITS  12
#define PWM_RESOLUTION_BITS  8

// System clock
#define F_CPU 192000000L

// Arduino API functions
void setup(void);
void loop(void);

// Digital I/O
void pinMode(uint8_t pin, uint8_t mode);
void digitalWrite(uint8_t pin, uint8_t val);
int digitalRead(uint8_t pin);

// Analog I/O
int analogRead(uint8_t pin);
void analogWrite(uint8_t pin, int val);

// Time functions
void delay(unsigned long ms);
void delayMicroseconds(unsigned int us);
unsigned long millis(void);
unsigned long micros(void);

// Math macros
#define min(a,b) ((a)<(b)?(a):(b))
#define max(a,b) ((a)>(b)?(a):(b))
#define constrain(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))

// Bit manipulation
#define lowByte(w) ((uint8_t) ((w) & 0xff))
#define highByte(w) ((uint8_t) ((w) >> 8))
#define bitRead(value, bit) (((value) >> (bit)) & 0x01)
#define bitSet(value, bit) ((value) |= (1UL << (bit)))
#define bitClear(value, bit) ((value) &= ~(1UL << (bit)))
#define bit(b) (1UL << (b))

// Interrupt functions
void attachInterrupt(uint8_t pin, void (*userFunc)(void), int mode);
void detachInterrupt(uint8_t pin);

// Serial communication (deprecated - use Serial object)
void serialBegin(unsigned long baud);
void serialWrite(uint8_t c);
void serialPrint(const char* str);

// Random functions
long xr871_random(long max);
long xr871_random_range(long min, long max);
void xr871_randomSeed(unsigned long seed);

// Arduino framework initialization
void xr871ArduinoInit(void);

#ifdef __cplusplus
}
#endif

#endif /* _ARDUINO_H_ */
