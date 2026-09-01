/**
 * @file Wire.cpp
 * @brief Arduino Wire (I2C) Implementation for XR871
 *
 * Implements Arduino Wire API using XR871 HAL I2C driver.
 */

#include "Wire.h"
#include "Arduino.h"
#include <string.h>

// Global Wire object
TwoWire Wire;

// ============================================================
// TwoWire Implementation
// ============================================================

TwoWire::TwoWire()
    : _i2c_id(I2C0_ID), _scl_pin(4), _sda_pin(5),
      _initialized(false), _clock(WIRE_DEFAULT_CLK),
      _tx_address(0), _tx_index(0),
      _rx_index(0), _rx_length(0),
      _onReceiveCallback(NULL), _onRequestCallback(NULL) {
    memset(_tx_buffer, 0, sizeof(_tx_buffer));
    memset(_rx_buffer, 0, sizeof(_rx_buffer));
}

TwoWire::~TwoWire() {
    end();
}

void TwoWire::begin(I2C_ID i2c_id, uint8_t scl_pin, uint8_t sda_pin) {
    if (_initialized) end();

    _i2c_id = i2c_id;
    _scl_pin = scl_pin;
    _sda_pin = sda_pin;

    // Initialize I2C
    I2C_InitParam param;
    memset(&param, 0, sizeof(param));
    param.clockFreq = _clock;
    param.addrMode = I2C_ADDR_MODE_7BIT;
    param.slaveAddr = 0x00;  // Master mode

    HAL_I2C_Init(_i2c_id, &param);

    _initialized = true;
}

void TwoWire::end() {
    if (!_initialized) return;
    HAL_I2C_DeInit(_i2c_id);
    _initialized = false;
}

void TwoWire::setClock(uint32_t freq) {
    _clock = freq;
    if (_initialized) {
        HAL_I2C_DeInit(_i2c_id);
        I2C_InitParam param;
        memset(&param, 0, sizeof(param));
        param.clockFreq = _clock;
        param.addrMode = I2C_ADDR_MODE_7BIT;
        param.slaveAddr = 0x00;
        HAL_I2C_Init(_i2c_id, &param);
    }
}

void TwoWire::beginTransmission(uint8_t address) {
    _tx_address = address;
    _tx_index = 0;
}

uint8_t TwoWire::endTransmission(bool sendStop) {
    if (!_initialized) return 4;

    // Transmit buffer
    int32_t result = HAL_I2C_Master_Transmit_IT(
        _i2c_id, _tx_address, _tx_buffer, (int32_t)_tx_index);

    if (result < 0) return 2;  // NACK
    return 0;  // Success
}

size_t TwoWire::requestFrom(uint8_t address, size_t quantity, bool sendStop) {
    if (!_initialized) return 0;
    if (quantity > WIRE_BUFFER_SIZE) quantity = WIRE_BUFFER_SIZE;

    int32_t result = HAL_I2C_Master_Receive_IT(
        _i2c_id, _tx_address, _rx_buffer, (int32_t)quantity);

    if (result <= 0) {
        _rx_length = 0;
        return 0;
    }

    _rx_length = (uint8_t)result;
    _rx_index = 0;
    return (size_t)_rx_length;
}

size_t TwoWire::requestFrom(uint8_t address, size_t quantity) {
    return requestFrom(address, quantity, true);
}

size_t TwoWire::write(uint8_t data) {
    if (_tx_index >= WIRE_BUFFER_SIZE) return 0;
    _tx_buffer[_tx_index++] = data;
    return 1;
}

size_t TwoWire::write(const uint8_t* data, size_t size) {
    size_t written = 0;
    for (size_t i = 0; i < size; i++) {
        if (write(data[i]) == 1) written++;
    }
    return written;
}

int TwoWire::available() {
    return (int)(_rx_length - _rx_index);
}

int TwoWire::read() {
    if (_rx_index >= _rx_length) return -1;
    return (int)_rx_buffer[_rx_index++];
}

int TwoWire::peek() {
    if (_rx_index >= _rx_length) return -1;
    return (int)_rx_buffer[_rx_index];
}

void TwoWire::flush() {
    _tx_index = 0;
    _rx_index = 0;
    _rx_length = 0;
}

void TwoWire::onReceive(void (*callback)(int)) {
    _onReceiveCallback = callback;
}

void TwoWire::onRequest(void (*callback)(void)) {
    _onRequestCallback = callback;
}
