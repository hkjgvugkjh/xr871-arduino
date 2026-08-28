/**
 * @file Wire.cpp
 * @brief Arduino Wire (I2C) Library Implementation for XR871
 * @author Hermes Agent
 * @date 2026-08-27
 *
 * Implements Arduino Wire API using XR871 HAL I2C driver.
 */

#include "Wire.h"
#include <string.h>

// Global Wire objects
TwoWire Wire;
TwoWire Wire1;

// ============================================================
// TwoWire class implementation
// ============================================================

void TwoWire::begin(I2C_ID i2cID, uint32_t clockFreq) {
    _i2cID = i2cID;
    _clockFreq = clockFreq;
    _initialized = true;
    _txBufferIndex = 0;
    _txBufferLength = 0;
    _rxBufferIndex = 0;
    _rxBufferLength = 0;
    _onReceiveCallback = NULL;
    _onRequestCallback = NULL;
    
    // Initialize I2C
    I2C_InitParam initParam;
    initParam.addrMode = I2C_ADDR_MODE_7BIT;
    initParam.clockFreq = clockFreq;
    HAL_I2C_Init(i2cID, &initParam);
}

void TwoWire::end() {
    if (_initialized) {
        HAL_I2C_DeInit(_i2cID);
        _initialized = false;
    }
}

void TwoWire::setClock(uint32_t freq) {
    _clockFreq = freq;
    if (_initialized) {
        HAL_I2C_DeInit(_i2cID);
        I2C_InitParam initParam;
        initParam.addrMode = I2C_ADDR_MODE_7BIT;
        initParam.clockFreq = freq;
        HAL_I2C_Init(_i2cID, &initParam);
    }
}

void TwoWire::beginTransmission(uint8_t address) {
    _slaveAddress = address;
    _txBufferIndex = 0;
    _txBufferLength = 0;
}

void TwoWire::beginTransmission(int address) {
    beginTransmission((uint8_t)address);
}

uint8_t TwoWire::endTransmission(void) {
    return endTransmission(true);
}

uint8_t TwoWire::endTransmission(uint8_t sendStop) {
    if (!_initialized) return 4;
    
    int32_t result = HAL_I2C_Master_Transmit_IT(
        _i2cID, 
        _slaveAddress, 
        _txBuffer, 
        _txBufferLength
    );
    
    // Reset buffer
    _txBufferIndex = 0;
    _txBufferLength = 0;
    
    if (result == 0) return 0;  // success
    if (result == -2) return 2; // NACK on address
    return 4; // other error
}

uint8_t TwoWire::requestFrom(uint8_t address, uint8_t quantity, uint8_t sendStop) {
    if (!_initialized) return 0;
    
    // Clamp to buffer size
    if (quantity > WIRE_BUFFER_LENGTH) {
        quantity = WIRE_BUFFER_LENGTH;
    }
    
    int32_t result = HAL_I2C_Master_Receive_IT(
        _i2cID, 
        address, 
        _rxBuffer, 
        quantity
    );
    
    if (result > 0) {
        _rxBufferLength = result;
        _rxBufferIndex = 0;
        return result;
    }
    
    _rxBufferLength = 0;
    _rxBufferIndex = 0;
    return 0;
}

uint8_t TwoWire::requestFrom(uint8_t address, uint8_t quantity) {
    return requestFrom(address, quantity, true);
}

uint8_t TwoWire::requestFrom(int address, int quantity) {
    return requestFrom((uint8_t)address, (uint8_t)quantity, true);
}

uint8_t TwoWire::requestFrom(int address, int quantity, int sendStop) {
    return requestFrom((uint8_t)address, (uint8_t)quantity, (uint8_t)sendStop);
}

size_t TwoWire::write(uint8_t data) {
    if (_txBufferLength >= WIRE_BUFFER_LENGTH) {
        return 0; // buffer full
    }
    _txBuffer[_txBufferIndex++] = data;
    _txBufferLength++;
    return 1;
}

size_t TwoWire::write(const uint8_t *data, size_t quantity) {
    size_t written = 0;
    for (size_t i = 0; i < quantity; i++) {
        if (write(data[i]) == 0) break;
        written++;
    }
    return written;
}

int TwoWire::read(void) {
    if (_rxBufferIndex >= _rxBufferLength) {
        return -1; // no data available
    }
    return _rxBuffer[_rxBufferIndex++];
}

int TwoWire::available(void) {
    return _rxBufferLength - _rxBufferIndex;
}

int TwoWire::peek(void) {
    if (_rxBufferIndex >= _rxBufferLength) {
        return -1; // no data available
    }
    return _rxBuffer[_rxBufferIndex];
}

void TwoWire::flush(void) {
    _txBufferIndex = 0;
    _txBufferLength = 0;
    _rxBufferIndex = 0;
    _rxBufferLength = 0;
}

void TwoWire::onReceive(void (*function)(int)) {
    _onReceiveCallback = function;
}

void TwoWire::onRequest(void (*function)(void)) {
    _onRequestCallback = function;
}

// ============================================================
// C wrapper functions for Arduino compatibility
// ============================================================

/**
 * @brief Initialize I2C0 master (for Arduino Wire)
 */
void i2c0Begin(uint32_t clockFreq) {
    Wire.begin(I2C0_ID, clockFreq);
}

/**
 * @brief Initialize I2C1 master (for Arduino Wire1)
 */
void i2c1Begin(uint32_t clockFreq) {
    Wire1.begin(I2C1_ID, clockFreq);
}
