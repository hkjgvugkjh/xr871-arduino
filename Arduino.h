/**
 * @file Arduino.h
 * @brief Arduino Core for XR871 - Header File
 * @author Hermes Agent
 * @date 2026-08-27
 *
 * This file provides the Arduino API wrapper on top of XR871 HAL.
 * Part of the XR871 Arduino porting project (方案C: FreeRTOS+Arduino库).
 * ESP32 Compatible API.
 */

#ifndef _ARDUINO_H_
#define _ARDUINO_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// XR871 HAL includes
#include "driver/chip/hal_gpio.h"
#include "driver/chip/hal_uart.h"
#include "driver/chip/hal_clock.h"
#include "kernel/os/os.h"

// Arduino version
#define ARDUINO 10819
#define ARDUINO_XR871 1
#define XR871_ARDUINO_VERSION "1.0.0"
#define XR871_ARDUINO_VERSION_MAJOR 1
#define XR871_ARDUINO_VERSION_MINOR 0
#define XR871_ARDUINO_VERSION_PATCH 0

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
#define SERIAL_8E1      0x0E
#define SERIAL_8O1      0x0B

// Built-in LED pin (PA6 on XR871GT)
#define LED_BUILTIN     6
#define LED_BUILTIN_PA  GPIO_PIN_6
#define LED_BUILTIN_PORT GPIO_PORT_A

// Analog settings
#define ADC_RESOLUTION_BITS  12
#define PWM_RESOLUTION_BITS  8

// System clock
#define F_CPU 192000000L  // 192 MHz

// Arduino API functions
void init(void);
void setup(void);
void loop(void);

// Digital I/O
void pinMode(uint8_t pin, uint8_t mode);
void digitalWrite(uint8_t pin, uint8_t val);
int digitalRead(uint8_t pin);

// Analog I/O
int analogRead(uint8_t pin);
void analogReference(uint8_t mode);
void analogWrite(uint8_t pin, int val);
void analogWriteResolution(uint8_t bits);
void analogReadResolution(uint8_t bits);

// Advanced I/O
void shiftOut(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder, uint8_t val);
uint8_t shiftIn(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder);
unsigned long pulseIn(uint8_t pin, uint8_t state, unsigned long timeout);
unsigned long pulseInLong(uint8_t pin, uint8_t state, unsigned long timeout);

// Time functions
void delay(unsigned long ms);
void delayMicroseconds(unsigned int us);
unsigned long millis(void);
unsigned long micros(void);

// Math functions
#define min(a,b) ((a)<(b)?(a):(b))
#define max(a,b) ((a)>(b)?(a):(b))
#define abs(x) ((x)>0?(x):-(x))
#define constrain(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))
#define round(x)     ((x)>=0?(long)((x)+0.5):(long)((x)-0.5))
#define radians(deg) ((deg)*DEG_TO_RAD)
#define degrees(rad) ((rad)*RAD_TO_DEG)
#define sq(x) ((x)*(x))

// Trigonometric constants
#define PI 3.1415926535897932384626433832795
#define HALF_PI 1.5707963267948966192313216916398
#define TWO_PI 6.283185307179586476925286766559
#define DEG_TO_RAD 0.017453292519943295769236907684886
#define RAD_TO_DEG 57.295779513082320876798154814105

// Bit manipulation
#define lowByte(w) ((uint8_t) ((w) & 0xff))
#define highByte(w) ((uint8_t) ((w) >> 8))
#define bitRead(value, bit) (((value) >> (bit)) & 0x01)
#define bitWrite(value, bit, bitvalue) (bitvalue ? bitSet(value, bit) : bitClear(value, bit))
#define bitSet(value, bit) ((value) |= (1UL << (bit)))
#define bitClear(value, bit) ((value) &= ~(1UL << (bit)))
#define bitToggle(value, bit) ((value) ^= (1UL << (bit)))
#define bit(b) (1UL << (b))

// Interrupt functions
void attachInterrupt(uint8_t pin, void (*userFunc)(void), int mode);
void detachInterrupt(uint8_t pin);
void interrupts(void);
void noInterrupts(void);

// Tone functions
void tone(uint8_t pin, unsigned int frequency, unsigned long duration);
void noTone(uint8_t pin);

// Random functions
long random(long max);
long random(long min, long max);
void randomSeed(unsigned long seed);

// Serial communication
void serialBegin(unsigned long baud);
void serialEnd(void);
int serialAvailable(void);
int serialPeek(void);
int serialRead(void);
void serialFlush(void);
void serialWrite(uint8_t c);
void serialPrint(const char* str);
void serialPrintln(const char* str);
void serialPrintInt(long n, int base);
void serialPrintFloat(double n, int digits);

// Pin mapping structure
typedef struct {
    GPIO_Port port;
    GPIO_Pin pin;
    uint8_t arduino_pin;
} PinMap;

// Pin mapping table
extern const PinMap g_pinMap[];
extern const uint8_t g_pinCount;

// Helper functions
GPIO_Port getPinPort(uint8_t pin);
GPIO_Pin getPinIndex(uint8_t pin);
uint8_t pinToArduino(GPIO_Port port, GPIO_Pin pin);

// Include peripheral libraries
#include "Wire.h"
#include "SPI.h"
#include "PWM.h"
#include "WiFi.h"
#include "WiFiClient.h"
#include "WiFiUDP.h"
#include "HTTPClient.h"
#include "OTA.h"
#include "WebServer.h"
#include "DNSServer.h"
#include "MDNS.h"
#include "Camera.h"
#include "Audio.h"

#ifdef __cplusplus
}
#endif

#endif /* _ARDUINO_H_ */
