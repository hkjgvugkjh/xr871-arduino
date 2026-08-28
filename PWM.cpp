/**
 * @file PWM.cpp
 * @brief Arduino PWM Library Implementation for XR871
 * @author Hermes Agent
 * @date 2026-08-27
 *
 * Implements Arduino analogWrite PWM support using XR871 HAL PWM driver.
 */

#include "PWM.h"
#include <string.h>

// PWM channel status
static PWM_Channel g_pwmChannels[PWM_CH_NUM];
static bool g_pwmInitialized = false;

// Pin to PWM channel mapping
typedef struct {
    uint8_t arduinoPin;
    PWM_CH_ID pwmChannel;
    PWM_GROUP_ID pwmGroup;
} PWM_PinMap;

static const PWM_PinMap g_pwmPinMap[] = {
    // Arduino pin, PWM channel, PWM group
    {8,  PWM_GROUP0_CH0, PWM_GROUP_0},  // PA8
    {9,  PWM_GROUP0_CH1, PWM_GROUP_0},  // PA9
    {10, PWM_GROUP1_CH2, PWM_GROUP_1},  // PA10
    {11, PWM_GROUP1_CH3, PWM_GROUP_1},  // PA11
    {12, PWM_GROUP2_CH4, PWM_GROUP_2},  // PA12
    {13, PWM_GROUP2_CH5, PWM_GROUP_2},  // PA13
    {14, PWM_GROUP3_CH6, PWM_GROUP_3},  // PA14
    {15, PWM_GROUP3_CH7, PWM_GROUP_3},  // PA15
};

// ============================================================
// PWM functions
// ============================================================

int pwmBegin(PWM_CH_ID channel, uint32_t frequency) {
    if (channel >= PWM_CH_NUM) return -1;
    
    // Initialize PWM if not already done
    if (!g_pwmInitialized) {
        memset(g_pwmChannels, 0, sizeof(g_pwmChannels));
        g_pwmInitialized = true;
    }
    
    // Determine group from channel
    PWM_GROUP_ID group = (PWM_GROUP_ID)(channel / 2);
    
    // Configure group clock
    PWM_ClkParam clkParam;
    clkParam.clk = PWM_CLK_APB1;  // Use APB1 clock
    clkParam.div = PWM_SRC_CLK_DIV_1;
    HAL_PWM_GroupClkCfg(group, &clkParam);
    
    // Initialize channel
    PWM_ChInitParam chParam;
    chParam.mode = PWM_CYCLE_MODE;
    chParam.polarity = PWM_HIGHLEVE;
    chParam.hz = frequency;
    
    if (HAL_PWM_ChInit(channel, &chParam) != 0) {
        return -1;
    }
    
    // Store channel info
    g_pwmChannels[channel].channel = channel;
    g_pwmChannels[channel].group = group;
    g_pwmChannels[channel].frequency = frequency;
    g_pwmChannels[channel].duty = 0;
    g_pwmChannels[channel].enabled = false;
    
    return 0;
}

void pwmEnd(PWM_CH_ID channel) {
    if (channel >= PWM_CH_NUM) return;
    
    HAL_PWM_ChDeinit(channel);
    g_pwmChannels[channel].enabled = false;
}

void pwmSetDuty(PWM_CH_ID channel, uint16_t duty) {
    if (channel >= PWM_CH_NUM) return;
    
    HAL_PWM_ChSetDutyRatio(channel, duty);
    g_pwmChannels[channel].duty = duty;
}

void pwmSetDutyPercent(PWM_CH_ID channel, float percent) {
    if (channel >= PWM_CH_NUM) return;
    
    // Clamp to 0-100%
    if (percent < 0.0f) percent = 0.0f;
    if (percent > 100.0f) percent = 100.0f;
    
    uint16_t duty = (uint16_t)(percent * 65535.0f / 100.0f);
    pwmSetDuty(channel, duty);
}

void pwmSetFrequency(PWM_CH_ID channel, uint32_t frequency) {
    if (channel >= PWM_CH_NUM) return;
    
    // Deinit and reinit with new frequency
    HAL_PWM_ChDeinit(channel);
    
    PWM_ChInitParam chParam;
    chParam.mode = PWM_CYCLE_MODE;
    chParam.polarity = PWM_HIGHLEVE;
    chParam.hz = frequency;
    HAL_PWM_ChInit(channel, &chParam);
    
    g_pwmChannels[channel].frequency = frequency;
    
    // Re-enable if it was enabled
    if (g_pwmChannels[channel].enabled) {
        HAL_PWM_EnableCh(channel, PWM_CYCLE_MODE, 1);
    }
}

void pwmEnable(PWM_CH_ID channel) {
    if (channel >= PWM_CH_NUM) return;
    
    HAL_PWM_EnableCh(channel, PWM_CYCLE_MODE, 1);
    g_pwmChannels[channel].enabled = true;
}

void pwmDisable(PWM_CH_ID channel) {
    if (channel >= PWM_CH_NUM) return;
    
    HAL_PWM_EnableCh(channel, PWM_CYCLE_MODE, 0);
    g_pwmChannels[channel].enabled = false;
}

PWM_CH_ID pinToPWMChannel(uint8_t pin) {
    for (uint8_t i = 0; i < sizeof(g_pwmPinMap) / sizeof(g_pwmPinMap[0]); i++) {
        if (g_pwmPinMap[i].arduinoPin == pin) {
            return g_pwmPinMap[i].pwmChannel;
        }
    }
    return PWM_CH_NULL;
}

uint8_t pwmChannelToPin(PWM_CH_ID channel) {
    for (uint8_t i = 0; i < sizeof(g_pwmPinMap) / sizeof(g_pwmPinMap[0]); i++) {
        if (g_pwmPinMap[i].pwmChannel == channel) {
            return g_pwmPinMap[i].arduinoPin;
        }
    }
    return 0xFF;
}

// ============================================================
// Arduino analogWrite compatibility
// ============================================================

/**
 * @brief Arduino analogWrite function
 * @param pin Arduino pin number
 * @param val 0-255 (8-bit resolution)
 */
void analogWrite(uint8_t pin, int val) {
    PWM_CH_ID channel = pinToPWMChannel(pin);
    if (channel == PWM_CH_NULL) return;
    
    // Initialize PWM if not already done
    if (!g_pwmChannels[channel].enabled) {
        pwmBegin(channel, PWM_DEFAULT_FREQ);
    }
    
    // Convert 8-bit (0-255) to 16-bit (0-65535)
    uint16_t duty = (uint16_t)(val * 65535 / 255);
    pwmSetDuty(channel, duty);
    pwmEnable(channel);
}
