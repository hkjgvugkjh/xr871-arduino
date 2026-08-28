/**
 * @file ADC.cpp
 * @brief Arduino AnalogRead Implementation for XR871
 * @author Hermes Agent
 * @date 2026-08-28
 */

#include "Arduino.h"
#include "ADC.h"

// Pin to channel mapping
const adc_pin_t ADCDriver::_pinMap[] = {
    { ADC_PIN_0,    ADC_CHANNEL_0, false },
    { ADC_PIN_1,    ADC_CHANNEL_1, false },
    { ADC_PIN_2,    ADC_CHANNEL_2, false },
    { ADC_PIN_3,    ADC_CHANNEL_3, false },
    { ADC_PIN_4,    ADC_CHANNEL_4, false },
    { ADC_PIN_5,    ADC_CHANNEL_5, false },
    { ADC_PIN_6,    ADC_CHANNEL_6, false },
    { ADC_PIN_7,    ADC_CHANNEL_7, false },
    { ADC_PIN_VBAT, ADC_CHANNEL_VBAT, false }
};

bool ADCDriver::_initialized = false;
uint8_t ADCDriver::_resolution = ADC_RESOLUTION_DEFAULT;
uint32_t ADCDriver::_maxValue = ADC_VALUE_MAX;

/**
 * @brief Initialize ADC driver
 */
bool ADCDriver::begin() {
    if (_initialized) return true;
    
    if (!initHardware()) return false;
    
    _initialized = true;
    return true;
}

/**
 * @brief Deinitialize ADC driver
 */
void ADCDriver::end() {
    if (!_initialized) return;
    HAL_ADC_DeInit();
    _initialized = false;
}

/**
 * @brief Read analog value from pin
 */
uint32_t ADCDriver::read(uint8_t pin) {
    if (!_initialized) begin();
    
    ADC_Channel ch = getChannel(pin);
    if (ch >= ADC_CHANNEL_NUM) return 0;
    
    return readChannel(ch);
}

/**
 * @brief Read analog value from ADC channel
 */
uint32_t ADCDriver::readChannel(ADC_Channel channel) {
    if (!_initialized) begin();
    
    uint32_t value = 0;
    HAL_ADC_Conv_Polling(channel, &value, 1000);
    
    return value;
}

/**
 * @brief Read voltage in millivolts
 */
uint32_t ADCDriver::readVoltage(uint8_t pin) {
    uint32_t value = read(pin);
    return valueToVoltage(value);
}

/**
 * @brief Set ADC resolution
 */
void ADCDriver::setResolution(uint8_t bits) {
    if (bits < ADC_RESOLUTION_MIN) bits = ADC_RESOLUTION_MIN;
    if (bits > ADC_RESOLUTION_MAX) bits = ADC_RESOLUTION_MAX;
    
    _resolution = bits;
    _maxValue = (1 << bits) - 1;
}

/**
 * @brief Get ADC resolution
 */
uint8_t ADCDriver::getResolution() {
    return _resolution;
}

/**
 * @brief Get maximum ADC value
 */
uint32_t ADCDriver::getMaxValue() {
    return _maxValue;
}

/**
 * @brief Get ADC channel for a pin
 */
ADC_Channel ADCDriver::getChannel(uint8_t pin) {
    for (int i = 0; i < sizeof(_pinMap) / sizeof(_pinMap[0]); i++) {
        if (_pinMap[i].pin == pin) {
            return _pinMap[i].channel;
        }
    }
    return ADC_CHANNEL_NUM;
}

/**
 * @brief Check if pin is valid ADC pin
 */
bool ADCDriver::isValidPin(uint8_t pin) {
    for (int i = 0; i < sizeof(_pinMap) / sizeof(_pinMap[0]); i++) {
        if (_pinMap[i].pin == pin) {
            return true;
        }
    }
    return false;
}

/**
 * @brief Initialize ADC hardware
 */
bool ADCDriver::initHardware() {
    ADC_InitParam param;
    param.mode = ADC_WORK_MODE_SINGLE;  // Single conversion mode
    param.delay = 0;                    // No delay
    
    HAL_Status status = HAL_ADC_Init(&param);
    if (status != HAL_OK) return false;
    
    return true;
}

/**
 * @brief Convert ADC value to voltage
 */
uint32_t ADCDriver::valueToVoltage(uint32_t value) {
    // Convert 12-bit value to millivolts (3.3V reference)
    return (value * ADC_REF_VOLTAGE_MV) / ADC_VALUE_MAX;
}

// ============================================================
// Arduino API Functions
// ============================================================

void adcInit() {
    ADCDriver::begin();
}

uint32_t analogRead(uint8_t pin) {
    return ADCDriver::read(pin);
}

void analogReadResolution(uint8_t bits) {
    ADCDriver::setResolution(bits);
}

void analogReference(uint8_t ref) {
    // Internal 3.3V reference is always used
    // This is a stub for API compatibility
    (void)ref;
}
