/**
 * @file Wire.h
 * @brief Arduino Wire (I2C) Library for XR871
 * @author Hermes Agent
 * @date 2026-08-27
 *
 * Provides Arduino Wire API wrapper on top of XR871 HAL I2C driver.
 * Supports I2C0 and I2C1 with master mode only.
 */

#ifndef _WIRE_XR871_H_
#define _WIRE_XR871_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include "driver/chip/hal_i2c.h"
#include "driver/chip/hal_gpio.h"
#include "kernel/os/os.h"

// I2C clock frequencies
#define WIRE_CLOCK_100K  100000
#define WIRE_CLOCK_400K  400000
#define WIRE_CLOCK_1M    1000000

// Default I2C timeout (ms)
#define WIRE_DEFAULT_TIMEOUT 1000

// Buffer size for I2C transfers
#define WIRE_BUFFER_LENGTH 32

// I2C0 pin definitions (default)
#define I2C0_SCL_PORT  GPIO_PORT_A
#define I2C0_SCL_PIN   GPIO_PIN_2
#define I2C0_SDA_PORT  GPIO_PORT_A
#define I2C0_SDA_PIN   GPIO_PIN_3

// I2C1 pin definitions
#define I2C1_SCL_PORT  GPIO_PORT_A
#define I2C1_SCL_PIN   GPIO_PIN_4
#define I2C1_SDA_PORT  GPIO_PORT_A
#define I2C1_SDA_PIN   GPIO_PIN_5

/**
 * @brief Wire class - Arduino I2C interface
 */
class TwoWire {
public:
    /**
     * @brief Initialize I2C master
     * @param i2cID I2C0_ID or I2C1_ID
     * @param clockFreq Clock frequency (100000 or 400000)
     */
    void begin(I2C_ID i2cID = I2C0_ID, uint32_t clockFreq = WIRE_CLOCK_400K);
    
    /**
     * @brief Deinitialize I2C
     */
    void end();
    
    /**
     * @brief Set clock frequency
     * @param freq Clock frequency in Hz
     */
    void setClock(uint32_t freq);
    
    /**
     * @brief Begin transmission to slave device
     * @param address 7-bit slave address
     */
    void beginTransmission(uint8_t address);
    void beginTransmission(int address);
    
    /**
     * @brief End transmission
     * @return 0=success, 1=data too long, 2=NACK on addr, 3=NACK on data, 4=other error
     */
    uint8_t endTransmission(void);
    uint8_t endTransmission(uint8_t sendStop);
    
    /**
     * @brief Request data from slave device
     * @param address 7-bit slave address
     * @param quantity Number of bytes to request
     * @param sendStop Whether to send stop condition
     * @return Number of bytes returned
     */
    uint8_t requestFrom(uint8_t address, uint8_t quantity, uint8_t sendStop);
    uint8_t requestFrom(uint8_t address, uint8_t quantity);
    uint8_t requestFrom(int address, int quantity);
    uint8_t requestFrom(int address, int quantity, int sendStop);
    
    /**
     * @brief Write a byte
     * @param data Byte to write
     * @return Number of bytes written
     */
    virtual size_t write(uint8_t data);
    virtual size_t write(const uint8_t *data, size_t quantity);
    
    /**
     * @brief Read a byte
     * @return Byte read, or -1 if no data available
     */
    virtual int read(void);
    
    /**
     * @brief Get number of bytes available for reading
     * @return Number of bytes available
     */
    virtual int available(void);
    
    /**
     * @brief Peek at next byte without consuming it
     * @return Next byte, or -1 if no data available
     */
    virtual int peek(void);
    
    /**
     * @brief Flush the transmit buffer
     */
    virtual void flush(void);
    
    /**
     * @brief Set callback function for slave receive
     */
    void onReceive(void (*function)(int));
    
    /**
     * @brief Set callback function for slave request
     */
    void onRequest(void (*function)(void));
    
    /**
     * @brief Get I2C ID
     */
    I2C_ID getI2CId(void) { return _i2cID; }
    
    /**
     * @brief Check if I2C is initialized
     */
    bool isInitialized(void) { return _initialized; }

private:
    I2C_ID _i2cID;
    uint32_t _clockFreq;
    bool _initialized;
    uint8_t _slaveAddress;
    
    // Transmit buffer
    uint8_t _txBuffer[WIRE_BUFFER_LENGTH];
    uint8_t _txBufferIndex;
    uint8_t _txBufferLength;
    
    // Receive buffer
    uint8_t _rxBuffer[WIRE_BUFFER_LENGTH];
    uint8_t _rxBufferIndex;
    uint8_t _rxBufferLength;
    
    // Callbacks
    void (*_onReceiveCallback)(int);
    void (*_onRequestCallback)(void);
    
    // Internal functions
    int32_t i2cWrite(uint8_t address, uint8_t *data, int32_t length);
    int32_t i2cRead(uint8_t address, uint8_t *data, int32_t length);
};

// Global Wire objects
extern TwoWire Wire;
extern TwoWire Wire1;

#ifdef __cplusplus
}
#endif

#endif /* _WIRE_XR871_H_ */
