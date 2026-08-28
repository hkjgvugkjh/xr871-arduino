/**
 * @file String.h
 * @brief Arduino String class for XR871
 * @author Hermes Agent
 * @date 2026-08-28
 *
 * Minimal Arduino String class implementation for XR871.
 * Provides compatibility with ESP32 Arduino API.
 */

#ifndef _STRING_XR871_H_
#define _STRING_XR871_H_

#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#ifdef __cplusplus

class String {
public:
    String();
    String(const char* str);
    String(const String& str);
    String(char c);
    String(int val);
    String(unsigned int val);
    String(long val);
    String(unsigned long val);
    String(float val, unsigned int decimalPlaces = 2);
    ~String();

    // Assignment
    String& operator=(const String& str);
    String& operator=(const char* str);

    // Concatenation
    String& operator+=(const String& str);
    String& operator+=(const char* str);
    String& operator+=(char c);
    String& operator+=(int val);
    String& operator+=(unsigned int val);
    String& operator+=(long val);
    String& operator+=(unsigned long val);
    String& operator+=(float val);

    // Comparison
    bool operator==(const String& str) const;
    bool operator==(const char* str) const;
    bool operator!=(const String& str) const;
    bool operator!=(const char* str) const;
    bool operator<(const String& str) const;
    bool operator>(const String& str) const;
    bool operator<=(const String& str) const;
    bool operator>=(const String& str) const;
    bool equals(const String& str) const;

    // Access
    char operator[](unsigned int index) const;
    char charAt(unsigned int index) const;
    void setCharAt(unsigned int index, char c);
    const char* c_str() const;
    int length() const;
    bool isEmpty() const;

    // Modification
    String& concat(const String& str);
    String& concat(const char* str);
    String& concat(char c);
    String& concat(int val);
    String& concat(unsigned int val);
    String& concat(long val);
    String& concat(unsigned long val);
    String& concat(float val);

    String& replace(char find, char replace);
    String& replace(const String& find, const String& replace);
    String& remove(unsigned int index);
    String& remove(unsigned int index, unsigned int count);
    String& toLowerCase();
    String& toUpperCase();
    String& trim();

    // Search
    int indexOf(char ch) const;
    int indexOf(char ch, unsigned int fromIndex) const;
    int indexOf(const String& str) const;
    int indexOf(const String& str, unsigned int fromIndex) const;
    int lastIndexOf(char ch) const;
    int lastIndexOf(char ch, unsigned int fromIndex) const;
    int lastIndexOf(const String& str) const;
    int lastIndexOf(const String& str, unsigned int fromIndex) const;
    String substring(unsigned int beginIndex) const;
    String substring(unsigned int beginIndex, unsigned int endIndex) const;

    // Conversion
    long toInt() const;
    float toFloat() const;
    double toDouble() const;

    // Format
    static String format(const char* fmt, ...);

private:
    char* _buffer;
    unsigned int _length;
    unsigned int _capacity;

    void init();
    void reserve(unsigned int size);
    void copy(const char* str, unsigned int length);
};

// Non-member operators (only those that don't conflict with C linkage)
inline String operator+(const String& lhs, const String& rhs) {
    String result = lhs;
    result += rhs;
    return result;
}

inline String operator+(const String& lhs, const char* rhs) {
    String result = lhs;
    result += rhs;
    return result;
}

inline String operator+(const char* lhs, const String& rhs) {
    String result = lhs;
    result += rhs;
    return result;
}

inline String operator+(const String& lhs, char rhs) {
    String result = lhs;
    result += rhs;
    return result;
}

inline String operator+(char lhs, const String& rhs) {
    String result = lhs;
    result += rhs;
    return result;
}

inline String operator+(const String& lhs, int rhs) {
    String result = lhs;
    result += rhs;
    return result;
}

inline String operator+(int lhs, const String& rhs) {
    String result;
    result += lhs;
    result += rhs;
    return result;
}

inline String operator+(const String& lhs, unsigned int rhs) {
    String result = lhs;
    result += rhs;
    return result;
}

inline String operator+(unsigned int lhs, const String& rhs) {
    String result;
    result += lhs;
    result += rhs;
    return result;
}

inline String operator+(const String& lhs, long rhs) {
    String result = lhs;
    result += rhs;
    return result;
}

inline String operator+(long lhs, const String& rhs) {
    String result;
    result += lhs;
    result += rhs;
    return result;
}

inline String operator+(const String& lhs, unsigned long rhs) {
    String result = lhs;
    result += rhs;
    return result;
}

inline String operator+(unsigned long lhs, const String& rhs) {
    String result;
    result += lhs;
    result += rhs;
    return result;
}

inline String operator+(const String& lhs, float rhs) {
    String result = lhs;
    result += rhs;
    return result;
}

inline String operator+(float lhs, const String& rhs) {
    char buf[32];
    snprintf(buf, sizeof(buf), "%.2f", lhs);
    String result = buf;
    result += rhs;
    return result;
}

#endif /* __cplusplus */

#endif /* _STRING_XR871_H_ */
