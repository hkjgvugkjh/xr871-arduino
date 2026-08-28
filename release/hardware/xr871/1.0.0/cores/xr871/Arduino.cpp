/**
 * @file Arduino.cpp
 * @brief Arduino Core Implementation for XR871
 * @author Hermes Agent
 * @date 2026-08-27
 */

#include "Arduino.h"
#include <stdio.h>

// Serial instance
SerialClass Serial;

void init(void) {
    // Board initialization stub
}

void pinMode(uint8_t pin, uint8_t mode) {
    (void)pin;
    (void)mode;
}

void digitalWrite(uint8_t pin, uint8_t val) {
    (void)pin;
    (void)val;
}

int digitalRead(uint8_t pin) {
    (void)pin;
    return LOW;
}

void delay(unsigned long ms) {
    (void)ms;
}

void delayMicroseconds(unsigned int us) {
    (void)us;
}

unsigned long millis(void) {
    return 0;
}

unsigned long micros(void) {
    return 0;
}

void attachInterrupt(uint8_t pin, void (*userFunc)(void), int mode) {
    (void)pin;
    (void)userFunc;
    (void)mode;
}

void detachInterrupt(uint8_t pin) {
    (void)pin;
}

void interrupts(void) {
}

void noInterrupts(void) {
}

void serialBegin(unsigned long baud) {
    (void)baud;
}

void serialEnd(void) {
}

int serialAvailable(void) {
    return 0;
}

int serialRead(void) {
    return -1;
}

void serialWrite(uint8_t c) {
    putchar(c);
}

void serialPrint(const char* str) {
    if (str) {
        while (*str) {
            serialWrite(*str++);
        }
    }
}

void serialPrintln(const char* str) {
    serialPrint(str);
    serialWrite('\r');
    serialWrite('\n');
}

void serialPrintInt(long n, int base) {
    (void)base;
    char buf[32];
    snprintf(buf, sizeof(buf), "%ld", n);
    serialPrint(buf);
}

void randomSeed(unsigned long seed) {
    (void)seed;
}

void shiftOut(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder, uint8_t val) {
    (void)dataPin;
    (void)clockPin;
    (void)bitOrder;
    (void)val;
}

uint8_t shiftIn(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder) {
    (void)dataPin;
    (void)clockPin;
    (void)bitOrder;
    return 0;
}

int analogRead(uint8_t pin) {
    (void)pin;
    return 0;
}

void analogReference(uint8_t mode) {
    (void)mode;
}

void analogWrite(uint8_t pin, int val) {
    (void)pin;
    (void)val;
}
