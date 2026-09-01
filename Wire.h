/**
 * @file Wire.h
 * @brief Arduino Wire (I2C) Library for XR871
 *
 * Provides Arduino Wire API wrapper on top of XR871 HAL I2C driver.
 * Supports I2C0 and I2C1 in master mode.
 */

#ifndef _WIRE_XR871_H_
#define _WIRE_XR871_H_

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

#include "driver/chip/hal_i2c.h"
#include "driver/chip/hal_gpio.h"

#ifdef __cplusplus
}
#endif

// Default I2C buffer size
#define WIRE_BUFFER_SIZE 32
#define WIRE_DEFAULT_CLK 400000  // 400kHz fast mode

/**
 * @brief TwoWire class - Arduino Wire API
 */
class TwoWire {
public:
    TwoWire();
    ~TwoWire();

    /**
     * @brief Initialize I2C master
     * @param i2c_id I2C0_ID or I2C1_ID
     * @param scl_pin SCL pin number (Arduino pin)
     * @param sda_pin SDA pin number (Arduino pin)
     */
    void begin(I2C_ID i2c_id = I2C0_ID, uint8_t scl_pin = 4, uint8_t sda_pin = 5);

    /**
     * @brief Deinitialize I2C
     */
    void end();

    /**
     * @brief Set I2C clock frequency
     * @param freq Frequency in Hz (default 100000)
     */
    void setClock(uint32_t freq);

    /**
     * @brief Begin transmission to slave device
     * @param address 7-bit slave address
     */
    void beginTransmission(uint8_t address);

    /**
     * @brief End transmission
     * @return 0=success, 1=data too long, 2=NACK on addr, 3=NACK on data, 4=other
     */
    uint8_t endTransmission(bool sendStop = true);

    /**
     * @brief Request bytes from slave device
     * @param address 7-bit slave address
     * @param quantity Number of bytes to request
     * @param sendStop Send stop condition after read
     * @return Number of bytes received
     */
    size_t requestFrom(uint8_t address, size_t quantity, bool sendStop = true);

    /**
     * @brief Request bytes from slave (simplified)
     */
    size_t requestFrom(uint8_t address, size_t quantity);

    /**
     * @brief Write a byte
     */
    size_t write(uint8_t data);

    /**
     * @brief Write a buffer
     */
    size_t write(const uint8_t* data, size_t size);

    /**
     * @brief Get number of available bytes to read
     */
    int available();

    /**
     * @brief Read a byte
     */
    int read();

    /**
     * @brief Peek at next byte without consuming it
     */
    int peek();

    /**
     * @brief Flush the transmit buffer
     */
    void flush();

    /**
     * @brief Set callback for slave receive
     */
    void onReceive(void (*callback)(int));

    /**
     * @brief Set callback for slave request
     */
    void onRequest(void (*callback)(void));

private:
    I2C_ID _i2c_id;
    uint8_t _scl_pin;
    uint8_t _sda_pin;
    bool _initialized;
    uint32_t _clock;

    // TX buffer
    uint8_t _tx_buffer[WIRE_BUFFER_SIZE];
    uint8_t _tx_address;
    uint8_t _tx_index;

    // RX buffer
    uint8_t _rx_buffer[WIRE_BUFFER_SIZE];
    uint8_t _rx_index;
    uint8_t _rx_length;

    // Callbacks
    void (*_onReceiveCallback)(int);
    void (*_onRequestCallback)(void);
};

// Global Wire object
extern TwoWire Wire;

#endif /* _WIRE_XR871_H_ */
