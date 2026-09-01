/**
 * @file HardwareSerial.cpp
 * @brief Arduino Hardware Serial Implementation for XR871
 *
 * Uses HAL UART driver for serial communication.
 */

#include "HardwareSerial.h"
#include "Arduino.h"
#include "driver/chip/hal_uart.h"
#include "driver/chip/hal_gpio.h"
#include <string.h>
#include <stdio.h>
#include <stdarg.h>

// Global Serial object
HardwareSerial Serial;

// ============================================================
// HardwareSerial Implementation
// ============================================================

HardwareSerial::HardwareSerial()
    : _uart_id(UART0_ID), _baud(115200), _initialized(false),
      _rx_head(0), _rx_tail(0), _tx_head(0), _tx_tail(0) {
    memset(_rx_buffer, 0, sizeof(_rx_buffer));
    memset(_tx_buffer, 0, sizeof(_tx_buffer));
}

HardwareSerial::~HardwareSerial() {
    end();
}

void HardwareSerial::begin(unsigned long baud, uint8_t uart_id) {
    if (_initialized && _baud == baud && _uart_id == uart_id) return;

    end();

    _uart_id = (UART_ID)uart_id;
    _baud = baud;

    // Initialize UART
    UART_InitParam param;
    memset(&param, 0, sizeof(param));
    param.baudRate = baud;
    param.parity = UART_PARITY_NONE;
    param.stopBits = UART_STOP_BITS_1;
    param.dataBits = UART_DATA_BITS_8;
    param.flowControl = 0;
    param.mode = UART_MODE_TX_RX;

    HAL_UART_Init(_uart_id, &param);

    // Enable RX interrupt callback
    HAL_UART_EnableRxCallback(_uart_id, rxCallback, this);

    _initialized = true;
}

void HardwareSerial::end() {
    if (!_initialized) return;

    HAL_UART_DisableRxCallback(_uart_id);
    HAL_UART_DeInit(_uart_id);

    _rx_head = 0;
    _rx_tail = 0;
    _tx_head = 0;
    _tx_tail = 0;
    _initialized = false;
}

int HardwareSerial::available() {
    return (SERIAL_RX_BUFFER_SIZE + _rx_head - _rx_tail) % SERIAL_RX_BUFFER_SIZE;
}

int HardwareSerial::peek() {
    if (_rx_head == _rx_tail) return -1;
    return _rx_buffer[_rx_tail];
}

int HardwareSerial::read() {
    if (_rx_head == _rx_tail) return -1;

    uint8_t c = _rx_buffer[_rx_tail];
    _rx_tail = (_rx_tail + 1) % SERIAL_RX_BUFFER_SIZE;
    return c;
}

void HardwareSerial::flush() {
    // Wait for TX buffer to empty
    while (_tx_head != _tx_tail) {
        // Wait
    }
}

size_t HardwareSerial::write(uint8_t c) {
    if (!_initialized) return 0;

    // Use interrupt-based transmit
    HAL_UART_Transmit_IT(_uart_id, &c, 1);
    return 1;
}

size_t HardwareSerial::write(const uint8_t* buffer, size_t size) {
    if (!_initialized || size == 0) return 0;

    HAL_UART_Transmit_IT(_uart_id, buffer, (int32_t)size);
    return size;
}

size_t HardwareSerial::print(const char* str) {
    if (!str) return 0;
    return write((const uint8_t*)str, strlen(str));
}

size_t HardwareSerial::println(const char* str) {
    size_t n = print(str);
    n += print("\r\n");
    return n;
}

size_t HardwareSerial::print(int val, int base) {
    char buf[33];
    if (base == 10) {
        snprintf(buf, sizeof(buf), "%d", val);
    } else if (base == 16) {
        snprintf(buf, sizeof(buf), "%X", val);
    } else if (base == 8) {
        snprintf(buf, sizeof(buf), "%o", val);
    } else {
        snprintf(buf, sizeof(buf), "%d", val);
    }
    return print(buf);
}

size_t HardwareSerial::println(int val, int base) {
    size_t n = print(val, base);
    n += print("\r\n");
    return n;
}

size_t HardwareSerial::print(long val, int base) {
    char buf[33];
    if (base == 10) {
        snprintf(buf, sizeof(buf), "%ld", val);
    } else if (base == 16) {
        snprintf(buf, sizeof(buf), "%lX", val);
    } else if (base == 8) {
        snprintf(buf, sizeof(buf), "%lo", val);
    } else {
        snprintf(buf, sizeof(buf), "%ld", val);
    }
    return print(buf);
}

size_t HardwareSerial::println(long val, int base) {
    size_t n = print(val, base);
    n += print("\r\n");
    return n;
}

size_t HardwareSerial::print(unsigned long val, int base) {
    char buf[33];
    if (base == 10) {
        snprintf(buf, sizeof(buf), "%lu", val);
    } else if (base == 16) {
        snprintf(buf, sizeof(buf), "%lX", val);
    } else if (base == 8) {
        snprintf(buf, sizeof(buf), "%lo", val);
    } else {
        snprintf(buf, sizeof(buf), "%lu", val);
    }
    return print(buf);
}

size_t HardwareSerial::println(unsigned long val, int base) {
    size_t n = print(val, base);
    n += print("\r\n");
    return n;
}

size_t HardwareSerial::print(double val, int decimals) {
    char buf[33];
    char fmt[10];
    snprintf(fmt, sizeof(fmt), "%%.%df", decimals);
    snprintf(buf, sizeof(buf), fmt, val);
    return print(buf);
}

size_t HardwareSerial::println(double val, int decimals) {
    size_t n = print(val, decimals);
    n += print("\r\n");
    return n;
}

size_t HardwareSerial::print(char c) {
    return write((uint8_t)c);
}

size_t HardwareSerial::println(char c) {
    size_t n = print(c);
    n += print("\r\n");
    return n;
}

size_t HardwareSerial::println() {
    return print("\r\n");
}

size_t HardwareSerial::printf(const char* fmt, ...) {
    char buf[128];
    va_list args;
    va_start(args, fmt);
    int len = vsnprintf(buf, sizeof(buf), fmt, args);
    va_end(args);

    if (len > 0) {
        return write((const uint8_t*)buf, (size_t)len);
    }
    return 0;
}

void HardwareSerial::storeRx(uint8_t c) {
    uint16_t next = (_rx_head + 1) % SERIAL_RX_BUFFER_SIZE;
    if (next != _rx_tail) {
        _rx_buffer[_rx_head] = c;
        _rx_head = next;
    }
}

void HardwareSerial::rxCallback(void* arg) {
    HardwareSerial* serial = (HardwareSerial*)arg;
    if (serial && serial->_initialized) {
        uint8_t data;
        while (HAL_UART_Receive_IT(serial->_uart_id, &data, 1, 0) > 0) {
            serial->storeRx(data);
        }
    }
}
