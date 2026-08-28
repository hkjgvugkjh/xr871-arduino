/**
 * @file SPI.h
 * @brief Arduino SPI Library for XR871
 * @author Hermes Agent
 * @date 2026-08-27
 *
 * Provides Arduino SPI API wrapper on top of XR871 HAL SPI driver.
 * Supports SPI0 and SPI1 with master mode only.
 */

#ifndef _SPI_XR871_H_
#define _SPI_XR871_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include "driver/chip/hal_spi.h"
#include "driver/chip/hal_gpio.h"

// SPI clock dividers (approximate frequencies)
#define SPI_CLOCK_DIV2   2
#define SPI_CLOCK_DIV4   4
#define SPI_CLOCK_DIV8   8
#define SPI_CLOCK_DIV16  16
#define SPI_CLOCK_DIV32  32
#define SPI_CLOCK_DIV64  64
#define SPI_CLOCK_DIV128 128

// SPI modes
#define SPI_MODE0 0x00  // CPOL=0, CPHA=0
#define SPI_MODE1 0x01  // CPOL=0, CPHA=1
#define SPI_MODE2 0x02  // CPOL=1, CPHA=0
#define SPI_MODE3 0x03  // CPOL=1, CPHA=1

// Default SPI settings
#define SPI_DEFAULT_CLOCK 4000000  // 4MHz
#define SPI_DEFAULT_MODE SPI_MODE0
#define SPI_DEFAULT_BITORDER MSBFIRST

// SPI0 pin definitions (default)
#define SPI0_CS_PORT   GPIO_PORT_B
#define SPI0_CS_PIN    GPIO_PIN_6
#define SPI0_MOSI_PORT GPIO_PORT_B
#define SPI0_MOSI_PIN  GPIO_PIN_4
#define SPI0_MISO_PORT GPIO_PORT_B
#define SPI0_MISO_PIN  GPIO_PIN_5
#define SPI0_CLK_PORT  GPIO_PORT_B
#define SPI0_CLK_PIN   GPIO_PIN_7

// SPI1 pin definitions
#define SPI1_CS_PORT   GPIO_PORT_A
#define SPI1_CS_PIN    GPIO_PIN_3
#define SPI1_MOSI_PORT GPIO_PORT_A
#define SPI1_MOSI_PIN  GPIO_PIN_0
#define SPI1_MISO_PORT GPIO_PORT_A
#define SPI1_MISO_PIN  GPIO_PIN_1
#define SPI1_CLK_PORT  GPIO_PORT_A
#define SPI1_CLK_PIN   GPIO_PIN_2

// Bit order
#define MSBFIRST SPI_TCTRL_FBS_MSB
#define LSBFIRST SPI_TCTRL_FBS_LSB

/**
 * @brief SPI class - Arduino SPI interface
 */
class SPIClass {
public:
    /**
     * @brief Initialize SPI master
     * @param port SPI0 or SPI1
     * @param cs Chip select pin (SPI_CS_0 to SPI_CS_3)
     */
    void begin(SPI_Port port = SPI0, SPI_CS cs = SPI_TCTRL_SS_SEL_SS0);
    
    /**
     * @brief Deinitialize SPI
     */
    void end();
    
    /**
     * @brief Set SPI mode
     * @param mode SPI_MODE0 to SPI_MODE3
     */
    void setDataMode(uint8_t mode);
    
    /**
     * @brief Set bit order
     * @param bitOrder MSBFIRST or LSBFIRST
     */
    void setBitOrder(uint8_t bitOrder);
    
    /**
     * @brief Set clock divider
     * @param clockDiv Clock divider value
     */
    void setClockDivider(uint8_t clockDiv);
    
    /**
     * @brief Transfer a byte (simultaneous send and receive)
     * @param data Byte to send
     * @return Byte received
     */
    uint8_t transfer(uint8_t data);
    
    /**
     * @brief Transfer a 16-bit word
     * @param data Word to send
     * @return Word received
     */
    uint16_t transfer16(uint16_t data);
    
    /**
     * @brief Transfer a buffer
     * @param buf Buffer to send/receive
     * @param count Number of bytes
     */
    void transfer(void *buf, size_t count);
    
    /**
     * @brief Begin SPI transaction (for compatibility)
     */
    void beginTransaction();
    
    /**
     * @brief End SPI transaction (for compatibility)
     */
    void endTransaction();
    
    /**
     * @brief Attach interrupt (stub)
     */
    void usingInterrupt(uint8_t interruptNumber);
    
    /**
     * @brief Detach interrupt (stub)
     */
    void notUsingInterrupt(uint8_t interruptNumber);
    
    /**
     * @brief Get SPI port
     */
    SPI_Port getPort(void) { return _port; }
    
    /**
     * @brief Check if SPI is initialized
     */
    bool isInitialized(void) { return _initialized; }

private:
    SPI_Port _port;
    SPI_CS _cs;
    SPI_Config _config;
    bool _initialized;
    uint8_t _mode;
    uint8_t _bitOrder;
    uint8_t _clockDiv;
    
    // Internal functions
    void updateConfig();
    uint32_t getClockFreq(uint8_t divider);
};

// Global SPI object
extern SPIClass SPI;
extern SPIClass SPI1;

#ifdef __cplusplus
}
#endif

#endif /* _SPI_XR871_H_ */
