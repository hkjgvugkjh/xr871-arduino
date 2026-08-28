/**
 * @file String.cpp
 * @brief Arduino String class implementation for XR871
 */

#include "String.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>

// Constructors
String::String() {
    init();
}

String::String(const char* str) {
    init();
    if (str) {
        copy(str, strlen(str));
    }
}

String::String(const String& str) {
    init();
    copy(str._buffer, str._length);
}

String::String(char c) {
    init();
    char buf[2] = {c, '\0'};
    copy(buf, 1);
}

String::String(int val) {
    init();
    char buf[16];
    snprintf(buf, sizeof(buf), "%d", val);
    copy(buf, strlen(buf));
}

String::String(unsigned int val) {
    init();
    char buf[16];
    snprintf(buf, sizeof(buf), "%u", val);
    copy(buf, strlen(buf));
}

String::String(long val) {
    init();
    char buf[20];
    snprintf(buf, sizeof(buf), "%ld", val);
    copy(buf, strlen(buf));
}

String::String(unsigned long val) {
    init();
    char buf[20];
    snprintf(buf, sizeof(buf), "%lu", val);
    copy(buf, strlen(buf));
}

String::String(float val, unsigned int decimalPlaces) {
    init();
    char buf[32];
    char fmt[16];
    snprintf(fmt, sizeof(fmt), "%%.%uf", decimalPlaces);
    snprintf(buf, sizeof(buf), fmt, val);
    copy(buf, strlen(buf));
}

String::~String() {
    if (_buffer) {
        free(_buffer);
    }
}

void String::init() {
    _capacity = 16;
    _buffer = (char*)malloc(_capacity);
    if (_buffer) {
        _buffer[0] = '\0';
    }
    _length = 0;
}

void String::reserve(unsigned int size) {
    if (size <= _capacity) return;
    while (_capacity < size) {
        _capacity *= 2;
    }
    char* newBuf = (char*)realloc(_buffer, _capacity);
    if (newBuf) {
        _buffer = newBuf;
    }
}

void String::copy(const char* str, unsigned int length) {
    reserve(length + 1);
    if (_buffer) {
        memcpy(_buffer, str, length);
        _buffer[length] = '\0';
        _length = length;
    }
}

// Assignment
String& String::operator=(const String& str) {
    if (this != &str) {
        copy(str._buffer, str._length);
    }
    return *this;
}

String& String::operator=(const char* str) {
    if (str) {
        copy(str, strlen(str));
    } else {
        _length = 0;
        if (_buffer) _buffer[0] = '\0';
    }
    return *this;
}

// Concatenation
String& String::operator+=(const String& str) {
    reserve(_length + str._length + 1);
    if (_buffer && str._buffer) {
        memcpy(_buffer + _length, str._buffer, str._length);
        _length += str._length;
        _buffer[_length] = '\0';
    }
    return *this;
}

String& String::operator+=(const char* str) {
    if (str) {
        unsigned int len = strlen(str);
        reserve(_length + len + 1);
        if (_buffer) {
            memcpy(_buffer + _length, str, len);
            _length += len;
            _buffer[_length] = '\0';
        }
    }
    return *this;
}

String& String::operator+=(char c) {
    reserve(_length + 2);
    if (_buffer) {
        _buffer[_length] = c;
        _length++;
        _buffer[_length] = '\0';
    }
    return *this;
}

String& String::operator+=(int val) {
    char buf[16];
    snprintf(buf, sizeof(buf), "%d", val);
    *this += buf;
    return *this;
}

String& String::operator+=(unsigned int val) {
    char buf[16];
    snprintf(buf, sizeof(buf), "%u", val);
    *this += buf;
    return *this;
}

String& String::operator+=(long val) {
    char buf[20];
    snprintf(buf, sizeof(buf), "%ld", val);
    *this += buf;
    return *this;
}

String& String::operator+=(unsigned long val) {
    char buf[20];
    snprintf(buf, sizeof(buf), "%lu", val);
    *this += buf;
    return *this;
}

String& String::operator+=(float val) {
    char buf[32];
    snprintf(buf, sizeof(buf), "%.2f", val);
    *this += buf;
    return *this;
}

// Comparison
bool String::operator==(const String& str) const {
    return equals(str);
}

bool String::operator==(const char* str) const {
    if (!_buffer || !str) return false;
    return strcmp(_buffer, str) == 0;
}

bool String::operator!=(const String& str) const {
    return !equals(str);
}

bool String::operator!=(const char* str) const {
    return !(*this == str);
}

bool String::operator<(const String& str) const {
    if (!_buffer || !str._buffer) return false;
    return strcmp(_buffer, str._buffer) < 0;
}

bool String::operator>(const String& str) const {
    if (!_buffer || !str._buffer) return false;
    return strcmp(_buffer, str._buffer) > 0;
}

bool String::operator<=(const String& str) const {
    return !(*this > str);
}

bool String::operator>=(const String& str) const {
    return !(*this < str);
}

bool String::equals(const String& str) const {
    if (_length != str._length) return false;
    if (!_buffer || !str._buffer) return false;
    return memcmp(_buffer, str._buffer, _length) == 0;
}

// Access
char String::operator[](unsigned int index) const {
    return charAt(index);
}

char String::charAt(unsigned int index) const {
    if (index >= _length || !_buffer) return 0;
    return _buffer[index];
}

void String::setCharAt(unsigned int index, char c) {
    if (index < _length && _buffer) {
        _buffer[index] = c;
    }
}

const char* String::c_str() const {
    return _buffer ? _buffer : "";
}

int String::length() const {
    return _length;
}

bool String::isEmpty() const {
    return _length == 0;
}

// Modification
String& String::concat(const String& str) {
    *this += str;
    return *this;
}

String& String::concat(const char* str) {
    *this += str;
    return *this;
}

String& String::concat(char c) {
    *this += c;
    return *this;
}

String& String::concat(int val) {
    *this += val;
    return *this;
}

String& String::concat(unsigned int val) {
    *this += val;
    return *this;
}

String& String::concat(long val) {
    *this += val;
    return *this;
}

String& String::concat(unsigned long val) {
    *this += val;
    return *this;
}

String& String::concat(float val) {
    *this += val;
    return *this;
}

String& String::replace(char find, char replace) {
    if (_buffer) {
        for (unsigned int i = 0; i < _length; i++) {
            if (_buffer[i] == find) {
                _buffer[i] = replace;
            }
        }
    }
    return *this;
}

String& String::replace(const String& find, const String& replace) {
    if (!_buffer || find._length == 0) return *this;
    
    String result;
    unsigned int pos = 0;
    while (pos < _length) {
        int idx = indexOf(find, pos);
        if (idx < 0) {
            result += substring(pos);
            break;
        }
        result += substring(pos, idx);
        result += replace;
        pos = idx + find._length;
    }
    *this = result;
    return *this;
}

String& String::remove(unsigned int index) {
    if (index < _length && _buffer) {
        memmove(_buffer + index, _buffer + index + 1, _length - index);
        _length--;
    }
    return *this;
}

String& String::remove(unsigned int index, unsigned int count) {
    if (index < _length && _buffer) {
        if (index + count > _length) count = _length - index;
        memmove(_buffer + index, _buffer + index + count, _length - index - count + 1);
        _length -= count;
    }
    return *this;
}

String& String::toLowerCase() {
    if (_buffer) {
        for (unsigned int i = 0; i < _length; i++) {
            if (_buffer[i] >= 'A' && _buffer[i] <= 'Z') {
                _buffer[i] += 32;
            }
        }
    }
    return *this;
}

String& String::toUpperCase() {
    if (_buffer) {
        for (unsigned int i = 0; i < _length; i++) {
            if (_buffer[i] >= 'a' && _buffer[i] <= 'z') {
                _buffer[i] -= 32;
            }
        }
    }
    return *this;
}

String& String::trim() {
    if (!_buffer || _length == 0) return *this;
    unsigned int start = 0;
    while (start < _length && _buffer[start] == ' ') start++;
    unsigned int end = _length;
    while (end > start && _buffer[end-1] == ' ') end--;
    
    if (start > 0 || end < _length) {
        String tmp;
        tmp.copy(_buffer + start, end - start);
        *this = tmp;
    }
    return *this;
}

// Search
int String::indexOf(char ch) const {
    for (unsigned int i = 0; i < _length; i++) {
        if (_buffer[i] == ch) return i;
    }
    return -1;
}

int String::indexOf(char ch, unsigned int fromIndex) const {
    if (fromIndex >= _length) return -1;
    for (unsigned int i = fromIndex; i < _length; i++) {
        if (_buffer[i] == ch) return i;
    }
    return -1;
}

int String::indexOf(const String& str) const {
    return indexOf(str, 0);
}

int String::indexOf(const String& str, unsigned int fromIndex) const {
    if (str._length == 0 || fromIndex >= _length) return -1;
    if (str._length > _length - fromIndex) return -1;
    
    for (unsigned int i = fromIndex; i <= _length - str._length; i++) {
        if (memcmp(_buffer + i, str._buffer, str._length) == 0) {
            return i;
        }
    }
    return -1;
}

int String::lastIndexOf(char ch) const {
    for (int i = _length - 1; i >= 0; i--) {
        if (_buffer[i] == ch) return i;
    }
    return -1;
}

int String::lastIndexOf(char ch, unsigned int fromIndex) const {
    if (_length == 0) return -1;
    if (fromIndex >= _length) fromIndex = _length - 1;
    for (int i = fromIndex; i >= 0; i--) {
        if (_buffer[i] == ch) return i;
    }
    return -1;
}

int String::lastIndexOf(const String& str) const {
    return lastIndexOf(str, _length - str._length);
}

int String::lastIndexOf(const String& str, unsigned int fromIndex) const {
    if (str._length == 0 || _length == 0 || str._length > _length) return -1;
    if (fromIndex > _length - str._length) fromIndex = _length - str._length;
    for (int i = fromIndex; i >= 0; i--) {
        if (memcmp(_buffer + i, str._buffer, str._length) == 0) {
            return i;
        }
    }
    return -1;
}

String String::substring(unsigned int beginIndex) const {
    return substring(beginIndex, _length);
}

String String::substring(unsigned int beginIndex, unsigned int endIndex) const {
    if (beginIndex >= _length) return String();
    if (endIndex > _length) endIndex = _length;
    if (beginIndex >= endIndex) return String();
    String result;
    result.copy(_buffer + beginIndex, endIndex - beginIndex);
    return result;
}

// Conversion
long String::toInt() const {
    return _buffer ? atol(_buffer) : 0;
}

float String::toFloat() const {
    return _buffer ? atof(_buffer) : 0.0f;
}

double String::toDouble() const {
    return _buffer ? atof(_buffer) : 0.0;
}

String String::format(const char* fmt, ...) {
    char buf[256];
    va_list args;
    va_start(args, fmt);
    vsnprintf(buf, sizeof(buf), fmt, args);
    va_end(args);
    return String(buf);
}
