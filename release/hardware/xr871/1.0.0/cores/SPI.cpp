/**
 * @file SPI.cpp
 * @brief Arduino SPI Library Implementation for XR871
 * @author Hermes Agent
 * @date 2026-08-27
 *
 * Implements Arduino SPI API using XR871 HAL SPI driver.
 */

#include "SPI.h"
#include <string.h>

// Global SPI objects
SPIClass SPI;
SPIClass SPI1;

// ============================================================
// SPIClass class implementation
// ============================================================

void SPIClass::begin(SPI_Port port, SPI_CS cs) {
    _port = port;
    _cs = cs;
    _initialized = true;
    _mode = SPI_MODE0;
    _bitOrder = MSBFIRST;
    _clockDiv = SPI_CLOCK_DIV4;
    
    // Initialize SPI global config
    SPI_Global_Config gconfig;
    gconfig.mclk = 192000000;  // 192MHz
    gconfig.cs_level = false;  // Active low
    HAL_SPI_Init(port, &gconfig);
    
    // Set default config
    _config.mode = SPI_CTRL_MODE_MASTER;
    _config.opMode = SPI_OPERATION_MODE_POLL;
    _config.firstBit = SPI_TCTRL_FBS_MSB;
    _config.sclk = getClockFreq(_clockDiv);
    _config.sclkMode = SPI_SCLK_Mode0;
    
    // Open SPI
    HAL_SPI_Open(port, cs, &_config, SPI_MAX_WAIT_MS);
}

void SPIClass::end() {
    if (_initialized) {
        HAL_SPI_Close(_port);
        HAL_SPI_Deinit(_port);
        _initialized = false;
    }
}

void SPIClass::setDataMode(uint8_t mode) {
    _mode = mode & 0x03;
    updateConfig();
}

void SPIClass::setBitOrder(uint8_t bitOrder) {
    _bitOrder = bitOrder;
    updateConfig();
}

void SPIClass::setClockDivider(uint8_t clockDiv) {
    _clockDiv = clockDiv;
    updateConfig();
}

uint8_t SPIClass::transfer(uint8_t data) {
    if (!_initialized) return 0;
    
    uint8_t rxData = 0;
    HAL_SPI_TransmitReceive(_port, &data, &rxData, 1);
    return rxData;
}

uint16_t SPIClass::transfer16(uint16_t data) {
    if (!_initialized) return 0;
    
    uint16_t rxData = 0;
    uint8_t txBuf[2], rxBuf[2];
    
    if (_bitOrder == MSBFIRST) {
        txBuf[0] = (data >> 8) & 0xFF;
        txBuf[1] = data & 0xFF;
    } else {
        txBuf[0] = data & 0xFF;
        txBuf[1] = (data >> 8) & 0xFF;
    }
    
    HAL_SPI_TransmitReceive(_port, txBuf, rxBuf, 2);
    
    if (_bitOrder == MSBFIRST) {
        rxData = (rxBuf[0] << 8) | rxBuf[1];
    } else {
        rxData = (rxBuf[1] << 8) | rxBuf[0];
    }
    
    return rxData;
}

void SPIClass::transfer(void *buf, size_t count) {
    if (!_initialized || count == 0) return;
    
    uint8_t *data = (uint8_t *)buf;
    for (size_t i = 0; i < count; i++) {
        data[i] = transfer(data[i]);
    }
}

void SPIClass::beginTransaction() {
    // For compatibility - SPI is already configured
}

void SPIClass::endTransaction() {
    // For compatibility
}

void SPIClass::usingInterrupt(uint8_t interruptNumber) {
    // Stub - not implemented
}

void SPIClass::notUsingInterrupt(uint8_t interruptNumber) {
    // Stub - not implemented
}

// ============================================================
// Internal functions
// ============================================================

void SPIClass::updateConfig() {
    if (!_initialized) return;
    
    // Update mode
    switch (_mode) {
        case SPI_MODE0:
            _config.sclkMode = SPI_SCLK_Mode0;
            break;
        case SPI_MODE1:
            _config.sclkMode = SPI_SCLK_Mode1;
            break;
        case SPI_MODE2:
            _config.sclkMode = SPI_SCLK_Mode2;
            break;
        case SPI_MODE3:
            _config.sclkMode = SPI_SCLK_Mode3;
            break;
    }
    
    // Update bit order
    _config.firstBit = (SPI_FirstBit)_bitOrder;
    
    // Update clock
    _config.sclk = getClockFreq(_clockDiv);
    
    // Reconfigure SPI
    HAL_SPI_Config(_port, SPI_ATTRIBUTION_IO_MODE, 0);
}

uint32_t SPIClass::getClockFreq(uint8_t divider) {
    // Base clock is 192MHz, divide by the divider
    switch (divider) {
        case SPI_CLOCK_DIV2:   return 96000000;
        case SPI_CLOCK_DIV4:   return 48000000;
        case SPI_CLOCK_DIV8:   return 24000000;
        case SPI_CLOCK_DIV16:  return 12000000;
        case SPI_CLOCK_DIV32:  return 6000000;
        case SPI_CLOCK_DIV64:  return 3000000;
        case SPI_CLOCK_DIV128: return 1500000;
        default:               return 4000000;
    }
}

// ============================================================
// C wrapper functions for Arduino compatibility
// ============================================================

/**
 * @brief Initialize SPI0 (for Arduino SPI)
 */
void spi0Begin() {
    SPI.begin(SPI0, SPI_TCTRL_SS_SEL_SS0);
}

/**
 * @brief Initialize SPI1 (for Arduino SPI1)
 */
void spi1Begin() {
    SPI1.begin(SPI1, SPI_TCTRL_SS_SEL_SS0);
}
