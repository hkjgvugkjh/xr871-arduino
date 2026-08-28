/**
 * @file ADC.h
 * @brief Arduino AnalogRead Library for XR871
 * @author Hermes Agent
 * @date 2026-08-28
 *
 * Provides Arduino analogRead() API wrapper on top of XR871 HAL ADC driver.
 * Supports 9 channels (ADC0-ADC8) with 12-bit resolution.
 * ADC8 is connected for VBAT voltage detection.
 *
 * Arduino API Compatibility:
 *   - analogRead(pin) - Read analog value from pin
 *   - analogReference(ref) - Set reference voltage
 *   - analogReadResolution(res) - Set resolution (bits)
 */

#ifndef _ADC_XR871_H_
#define _ADC_XR871_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include "driver/chip/hal_adc.h"

// ============================================================
// ADC Configuration
// ============================================================

// Default ADC resolution (12-bit)
#define ADC_RESOLUTION_DEFAULT  12
#define ADC_RESOLUTION_MIN      8
#define ADC_RESOLUTION_MAX      12

// ADC reference voltage (internal)
#define ADC_REF_VOLTAGE_MV      3300  // 3.3V reference

// ADC value range
#define ADC_VALUE_MIN           0
#define ADC_VALUE_MAX           4095  // 12-bit resolution

// ============================================================
// Arduino Pin to ADC Channel Mapping
// ============================================================

// XR871 Arduino pin definitions (from pins_arduino.h)
// ADC pins are mapped to specific GPIOs
#define ADC_PIN_0               GPIO_A0    // ADC Channel 0
#define ADC_PIN_1               GPIO_A1    // ADC Channel 1
#define ADC_PIN_2               GPIO_A2    // ADC Channel 2
#define ADC_PIN_3               GPIO_A3    // ADC Channel 3
#define ADC_PIN_4               GPIO_A4    // ADC Channel 4
#define ADC_PIN_5               GPIO_A5    // ADC Channel 5
#define ADC_PIN_6               GPIO_A6    // ADC Channel 6
#define ADC_PIN_7               GPIO_A7    // ADC Channel 7
#define ADC_PIN_VBAT            GPIO_A8    // ADC Channel 8 (VBAT)

// ============================================================
// ADC Pin Structure
// ============================================================

typedef struct {
    uint8_t pin;                // Arduino pin number
    ADC_Channel channel;        // ADC channel
    bool enabled;               // Channel enabled flag
} adc_pin_t;

// ============================================================
// ADC Class
// ============================================================

class ADCDriver {
public:
    /**
     * @brief Initialize ADC driver
     */
    static bool begin();
    
    /**
     * @brief Deinitialize ADC driver
     */
    static void end();
    
    /**
     * @brief Read analog value from pin
     * @param pin Arduino pin number
     * @return Analog value (0 to 4095 for 12-bit)
     */
    static uint32_t read(uint8_t pin);
    
    /**
     * @brief Read analog value from ADC channel
     * @param channel ADC channel number (0-8)
     * @return Analog value (0 to 4095 for 12-bit)
     */
    static uint32_t readChannel(ADC_Channel channel);
    
    /**
     * @brief Read voltage in millivolts
     * @param pin Arduino pin number
     * @return Voltage in mV
     */
    static uint32_t readVoltage(uint8_t pin);
    
    /**
     * @brief Set ADC resolution
     * @param bits Resolution in bits (8-12)
     */
    static void setResolution(uint8_t bits);
    
    /**
     * @brief Get ADC resolution
     * @return Current resolution in bits
     */
    static uint8_t getResolution();
    
    /**
     * @brief Get maximum ADC value for current resolution
     * @return Maximum value (e.g., 4095 for 12-bit)
     */
    static uint32_t getMaxValue();
    
    /**
     * @brief Get ADC channel for a pin
     * @param pin Arduino pin number
     * @return ADC channel or ADC_CHANNEL_NUM if invalid
     */
    static ADC_Channel getChannel(uint8_t pin);
    
    /**
     * @brief Check if pin is valid ADC pin
     * @param pin Arduino pin number
     * @return true if valid ADC pin
     */
    static bool isValidPin(uint8_t pin);

private:
    static bool _initialized;
    static uint8_t _resolution;
    static uint32_t _maxValue;
    
    // Pin to channel mapping
    static const adc_pin_t _pinMap[];
    
    /**
     * @brief Initialize ADC hardware
     */
    static bool initHardware();
    
    /**
     * @brief Convert ADC value to voltage
     */
    static uint32_t valueToVoltage(uint32_t value);
};

// ============================================================
// Arduino API Functions
// ============================================================

/**
 * @brief Initialize ADC (called by Arduino framework)
 */
void adcInit();

/**
 * @brief Read analog value from pin
 * @param pin Arduino pin number
 * @return Analog value (0 to 4095)
 */
uint32_t analogRead(uint8_t pin);

/**
 * @brief Set ADC resolution
 * @param bits Resolution in bits (8-12)
 */
void analogReadResolution(uint8_t bits);

/**
 * @brief Set ADC reference voltage (stub)
 * @param ref Reference type (ignored, internal 3.3V used)
 */
void analogReference(uint8_t ref);

#ifdef __cplusplus
}
#endif

#endif /* _ADC_XR871_H_ */
