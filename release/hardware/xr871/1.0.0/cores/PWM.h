/**
 * @file PWM.h
 * @brief Arduino PWM Library for XR871
 * @author Hermes Agent
 * @date 2026-08-27
 *
 * Provides Arduino analogWrite PWM support on top of XR871 HAL PWM driver.
 * Supports 8 PWM channels across 4 groups.
 */

#ifndef _PWM_XR871_H_
#define _PWM_XR871_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include "driver/chip/hal_pwm.h"
#include "driver/chip/hal_gpio.h"

// PWM channels (Arduino pin mapping)
// PWM0 = PA8 or PA19, PWM1 = PA9 or PA20, PWM2 = PA10 or PA21
// PWM3 = PA11 or PA22, PWM4 = PA12 or PB0, PWM5 = PA13 or PB1
// PWM6 = PA14 or PB2, PWM7 = PA15 or PB3

// Default PWM frequency
#define PWM_DEFAULT_FREQ 1000  // 1kHz
#define PWM_MAX_FREQ 1000000   // 1MHz
#define PWM_MIN_FREQ 1         // 1Hz

// PWM resolution (duty ratio is 16-bit: 0-65535)
#define PWM_RESOLUTION 16
#define PWM_MAX_DUTY 65535

/**
 * @brief PWM channel configuration
 */
typedef struct {
    PWM_CH_ID channel;
    PWM_GROUP_ID group;
    uint32_t frequency;
    uint16_t duty;  // 0-65535
    bool enabled;
} PWM_Channel;

/**
 * @brief Initialize PWM channel
 * @param channel PWM channel ID (PWM_GROUP0_CH0 to PWM_GROUP3_CH7)
 * @param frequency PWM frequency in Hz
 * @return 0 on success, -1 on failure
 */
int pwmBegin(PWM_CH_ID channel, uint32_t frequency);

/**
 * @brief Deinitialize PWM channel
 * @param channel PWM channel ID
 */
void pwmEnd(PWM_CH_ID channel);

/**
 * @brief Set PWM duty cycle
 * @param channel PWM channel ID
 * @param duty Duty cycle (0-65535, where 0=0%, 65535=100%)
 */
void pwmSetDuty(PWM_CH_ID channel, uint16_t duty);

/**
 * @brief Set PWM duty cycle as percentage
 * @param channel PWM channel ID
 * @param percent Duty cycle percentage (0.0-100.0)
 */
void pwmSetDutyPercent(PWM_CH_ID channel, float percent);

/**
 * @brief Set PWM frequency
 * @param channel PWM channel ID
 * @param frequency Frequency in Hz
 */
void pwmSetFrequency(PWM_CH_ID channel, uint32_t frequency);

/**
 * @brief Enable PWM output
 * @param channel PWM channel ID
 */
void pwmEnable(PWM_CH_ID channel);

/**
 * @brief Disable PWM output
 * @param channel PWM channel ID
 */
void pwmDisable(PWM_CH_ID channel);

/**
 * @brief Get PWM channel from Arduino pin number
 * @param pin Arduino pin number
 * @return PWM channel ID, or PWM_CH_NULL if not a PWM pin
 */
PWM_CH_ID pinToPWMChannel(uint8_t pin);

/**
 * @brief Get Arduino pin number from PWM channel
 * @param channel PWM channel ID
 * @return Arduino pin number, or 0xFF if invalid
 */
uint8_t pwmChannelToPin(PWM_CH_ID channel);

#ifdef __cplusplus
}
#endif

#endif /* _PWM_XR871_H_ */
