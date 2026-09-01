/**
 * @file HardwareSerial.h
 * @brief Arduino Hardware Serial for XR871
 *
 * Provides Serial.begin(), Serial.print(), Serial.read(), etc.
 * Uses UART0 (default console) or UART1
 */

#ifndef _HARDWARE_SERIAL_H_
#define _HARDWARE_SERIAL_H_

#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stdio.h>

// Buffer sizes
#define SERIAL_RX_BUFFER_SIZE 64
#define SERIAL_TX_BUFFER_SIZE 64

/**
 * @brief HardwareSerial class - Arduino Serial API
 */
class HardwareSerial {
public:
    HardwareSerial();
    ~HardwareSerial();

    /**
     * @brief Initialize serial port
     * @param baud Baud rate (e.g., 9600, 115200)
     * @param uart_id UART0_ID or UART1_ID
     */
    void begin(unsigned long baud, uint8_t uart_id = 0);

    /**
     * @brief Deinitialize serial port
     */
    void end();

    /**
     * @brief Get number of available bytes to read
     * @return Number of bytes available
     */
    int available();

    /**
     * @brief Peek at next byte without consuming it
     * @return Next byte or -1 if none
     */
    int peek();

    /**
     * @brief Read a byte from serial
     * @return Byte read or -1 if none
     */
    int read();

    /**
     * @brief Wait for transmission to complete
     */
    void flush();

    /**
     * @brief Write a byte
     * @param c Byte to write
     * @return Number of bytes written
     */
    size_t write(uint8_t c);

    /**
     * @brief Write a buffer
     * @param buffer Data buffer
     * @param size Number of bytes
     * @return Number of bytes written
     */
    size_t write(const uint8_t* buffer, size_t size);

    /**
     * @brief Print a string
     */
    size_t print(const char* str);

    /**
     * @brief Print a string with newline
     */
    size_t println(const char* str);

    /**
     * @brief Print an integer
     */
    size_t print(int val, int base = 10);

    /**
     * @brief Print an integer with newline
     */
    size_t println(int val, int base = 10);

    /**
     * @brief Print a long integer
     */
    size_t print(long val, int base = 10);

    /**
     * @brief Print a long integer with newline
     */
    size_t println(long val, int base = 10);

    /**
     * @brief Print an unsigned long
     */
    size_t print(unsigned long val, int base = 10);

    /**
     * @brief Print an unsigned long with newline
     */
    size_t println(unsigned long val, int base = 10);

    /**
     * @brief Print a float
     */
    size_t print(double val, int decimals = 2);

    /**
     * @brief Print a float with newline
     */
    size_t println(double val, int decimals = 2);

    /**
     * @brief Print a single character
     */
    size_t print(char c);

    /**
     * @brief Print a single character with newline
     */
    size_t println(char c);

    /**
     * @brief Print a newline
     */
    size_t println();

    /**
     * @brief Printf-style formatting
     */
    size_t printf(const char* fmt, ...);

private:
    uint8_t _uart_id;
    unsigned long _baud;
    bool _initialized;

    // RX buffer
    uint8_t _rx_buffer[SERIAL_RX_BUFFER_SIZE];
    volatile uint16_t _rx_head;
    volatile uint16_t _rx_tail;

    // TX buffer
    uint8_t _tx_buffer[SERIAL_TX_BUFFER_SIZE];
    volatile uint16_t _tx_head;
    volatile uint16_t _tx_tail;

    void storeRx(uint8_t c);
    static void rxCallback(void* arg);
};

// Global Serial object
extern HardwareSerial Serial;

#endif /* _HARDWARE_SERIAL_H_ */
