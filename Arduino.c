/**
 * @file Arduino.c
 * @brief Arduino API implementation for XR871
 */

#include "Arduino.h"

void setup(void) { /* User defined */ }
void loop(void) { /* User defined */ }

void pinMode(uint8_t pin, uint8_t mode) { /* Stub */ }
void digitalWrite(uint8_t pin, uint8_t val) { /* Stub */ }
int digitalRead(uint8_t pin) { return LOW; }
int analogRead(uint8_t pin) { return 0; }
void analogWrite(uint8_t pin, int val) { /* Stub */ }

void delay(unsigned long ms) { /* Stub */ }
void delayMicroseconds(unsigned int us) { /* Stub */ }
unsigned long millis(void) { return 0; }
unsigned long micros(void) { return 0; }

void attachInterrupt(uint8_t pin, void (*userFunc)(void), int mode) { /* Stub */ }
void detachInterrupt(uint8_t pin) { /* Stub */ }

void serialBegin(unsigned long baud) { /* Stub */ }
void serialWrite(uint8_t c) { /* Stub */ }
void serialPrint(const char* str) { /* Stub */ }

long xr871_random(long max) { return rand() % max; }
long xr871_random_range(long min, long max) { return min + (rand() % (max - min)); }
void xr871_randomSeed(unsigned long seed) { srand(seed); }
