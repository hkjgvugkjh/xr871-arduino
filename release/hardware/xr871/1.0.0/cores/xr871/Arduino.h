/**
 * @file Arduino.h
 * @brief Arduino Core for XR871 - Header File
 * @author Hermes Agent
 * @date 2026-08-27
 *
 * ESP32 Compatible API for XR871
 * Minimal core - peripheral libraries included on demand
 */

#ifndef _ARDUINO_H_
#define _ARDUINO_H_

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <math.h>

// Arduino version
#define ARDUINO_XR871 1

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

// Built-in LED pin
#define LED_BUILTIN     6

// System clock
#define F_CPU 192000000L

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

#ifdef __cplusplus

// Simple String class for XR871
class String {
public:
    String() : _str(NULL), _len(0) {}
    String(const char* s) : _str(NULL), _len(0) {
        if (s) {
            _len = strlen(s);
            _str = (char*)malloc(_len + 1);
            if (_str) {
                memcpy(_str, s, _len + 1);
            }
        }
    }
    String(const String& other) : _str(NULL), _len(0) {
        if (other._str) {
            _len = other._len;
            _str = (char*)malloc(_len + 1);
            if (_str) {
                memcpy(_str, other._str, _len + 1);
            }
        }
    }
    ~String() {
        if (_str) {
            free(_str);
            _str = NULL;
        }
    }
    
    String& operator=(const String& other) {
        if (this != &other) {
            if (_str) free(_str);
            _str = NULL;
            _len = 0;
            if (other._str) {
                _len = other._len;
                _str = (char*)malloc(_len + 1);
                if (_str) {
                    memcpy(_str, other._str, _len + 1);
                }
            }
        }
        return *this;
    }
    
    String& operator=(const char* s) {
        if (_str) free(_str);
        _str = NULL;
        _len = 0;
        if (s) {
            _len = strlen(s);
            _str = (char*)malloc(_len + 1);
            if (_str) {
                memcpy(_str, s, _len + 1);
            }
        }
        return *this;
    }
    
    operator const char*() const { return _str ? _str : ""; }
    const char* c_str() const { return _str ? _str : ""; }
    int length() const { return _len; }
    bool operator==(const String& other) const {
        if (_len != other._len) return false;
        if (_len == 0) return true;
        return strcmp(_str, other._str) == 0;
    }
    bool operator==(const char* s) const {
        if (!s) return _len == 0;
        return strcmp(_str ? _str : "", s) == 0;
    }
    bool operator!=(const String& other) const { return !(*this == other); }
    bool operator!=(const char* s) const { return !(*this == s); }
    
private:
    char* _str;
    int _len;
};

// Stream base class
class Stream {
public:
    virtual int available() { return 0; }
    virtual int read() { return -1; }
    virtual int peek() { return -1; }
    virtual void flush() {}
    virtual void write(uint8_t c) { (void)c; }
    virtual void print(const char* s) { if (s) { while (*s) { write(*s++); } } }
    virtual void println(const char* s) { print(s); print("\r\n"); }
    virtual void println() { print("\r\n"); }
};

// Serial class for Arduino API compatibility
class SerialClass : public Stream {
public:
    void begin(unsigned long baud) { (void)baud; }
    void end() {}
    int available() override { return 0; }
    int peek() override { return -1; }
    int read() override { return -1; }
    void flush() override {}
    void write(uint8_t c) override { putchar(c); }
    void print(const char* s) override { if (s) { while (*s) { write(*s++); } } }
    void print(int n) { char buf[16]; snprintf(buf, sizeof(buf), "%d", n); print(buf); }
    void print(long n) { char buf[32]; snprintf(buf, sizeof(buf), "%ld", n); print(buf); }
    void print(unsigned long n) { char buf[32]; snprintf(buf, sizeof(buf), "%lu", n); print(buf); }
    void print(float n, int digits = 2) { char buf[32]; snprintf(buf, sizeof(buf), "%.*f", digits, n); print(buf); }
    void println(const char* s) override { print(s); print("\r\n"); }
    void println(int n) { print(n); print("\r\n"); }
    void println(long n) { print(n); print("\r\n"); }
    void println(unsigned long n) { print(n); print("\r\n"); }
    void println(float n, int digits = 2) { print(n, digits); print("\r\n"); }
    void println() override { print("\r\n"); }
};

extern SerialClass Serial;

#endif // __cplusplus

// C-compatible declarations
#ifdef __cplusplus
extern "C" {
#endif

void init(void);
void setup(void);
void loop(void);

void pinMode(uint8_t pin, uint8_t mode);
void digitalWrite(uint8_t pin, uint8_t val);
int digitalRead(uint8_t pin);

int analogRead(uint8_t pin);
void analogReference(uint8_t mode);
void analogWrite(uint8_t pin, int val);
void analogWriteResolution(uint8_t bits);
void analogReadResolution(uint8_t bits);

void delay(unsigned long ms);
void delayMicroseconds(unsigned int us);
unsigned long millis(void);
unsigned long micros(void);

void attachInterrupt(uint8_t pin, void (*userFunc)(void), int mode);
void detachInterrupt(uint8_t pin);
void interrupts(void);
void noInterrupts(void);

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

void randomSeed(unsigned long seed);

void shiftOut(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder, uint8_t val);
uint8_t shiftIn(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder);

#ifdef __cplusplus
}
#endif

#endif /* _ARDUINO_H_ */
