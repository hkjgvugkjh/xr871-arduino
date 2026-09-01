/**
 * @file Arduino.c
 * @brief Arduino API implementation for XR871 - Core functions
 *
 * Implements: pinMode, digitalWrite, digitalRead, analogRead, analogWrite,
 *             delay, delayMicroseconds, millis, micros, attachInterrupt
 */

#include "Arduino.h"
#include "pins_arduino.h"
#include "driver/chip/hal_gpio.h"
#include "driver/chip/hal_timer.h"
#include "driver/chip/hal_uart.h"
#include "driver/chip/hal_pwm.h"
#include "driver/chip/hal_adc.h"

// ============================================================
// Timer for millis/micros
// ============================================================
static volatile uint32_t millis_counter = 0;

/**
 * @brief Initialize timer for millis/micros
 */
static void initMillisTimer(void) {
    TIMER_InitParam param;
    param.cfg = HAL_TIMER_MakeInitCfg(TIMER_MODE_REPEAT,
                                       TIMER_CLK_PRESCALER_1,
                                       TIMER_CLK_SRC_HFCLK);
    // 192MHz / 192 = 1MHz = 1us tick
    param.period = 192;
    param.arg = NULL;
    
    HAL_TIMER_Init(TIMER0_ID, &param);
    HAL_TIMER_Start(TIMER0_ID);
}

// ============================================================
// Digital I/O
// ============================================================

void pinMode(uint8_t pin, uint8_t mode) {
    if (pin >= PINS_COUNT) return;
    
    GPIO_Port port = pinToPort(pin);
    GPIO_Pin gpioPin = pinToPin(pin);
    
    GPIO_InitParam config;
    config.driving = GPIO_DRIVING_LEVEL_1;
    
    switch (mode) {
        case INPUT:
            config.mode = GPIOx_Pn_F0_INPUT;
            config.pull = GPIO_PULL_NONE;
            break;
        case OUTPUT:
            config.mode = GPIOx_Pn_F1_OUTPUT;
            config.pull = GPIO_PULL_NONE;
            break;
        case INPUT_PULLUP:
            config.mode = GPIOx_Pn_F0_INPUT;
            config.pull = GPIO_PULL_UP;
            break;
        case INPUT_PULLDOWN:
            config.mode = GPIOx_Pn_F0_INPUT;
            config.pull = GPIO_PULL_DOWN;
            break;
        default:
            config.mode = GPIOx_Pn_F0_INPUT;
            config.pull = GPIO_PULL_NONE;
            break;
    }
    
    HAL_GPIO_Init(port, gpioPin, &config);
}

void digitalWrite(uint8_t pin, uint8_t val) {
    if (pin >= PINS_COUNT) return;
    
    GPIO_Port port = pinToPort(pin);
    GPIO_Pin gpioPin = pinToPin(pin);
    
    HAL_GPIO_WritePin(port, gpioPin, (val == HIGH) ? GPIO_PIN_HIGH : GPIO_PIN_LOW);
}

int digitalRead(uint8_t pin) {
    if (pin >= PINS_COUNT) return LOW;
    
    GPIO_Port port = pinToPort(pin);
    GPIO_Pin gpioPin = pinToPin(pin);
    
    return (HAL_GPIO_ReadPin(port, gpioPin) == GPIO_PIN_HIGH) ? HIGH : LOW;
}

// ============================================================
// Analog I/O
// ============================================================

int analogRead(uint8_t pin) {
    uint8_t channel = pinToADC(pin);
    if (channel >= 8) return 0;
    
    uint32_t value = 0;
    HAL_ADC_Conv_Polling((ADC_Channel)channel, &value, 1000);
    return (int)value;
}

void analogWrite(uint8_t pin, int val) {
    uint8_t pwmCh = pinToPWM(pin);
    if (pwmCh >= 8) return;
    
    // val: 0-255 (8-bit resolution)
    if (val < 0) val = 0;
    if (val > 255) val = 255;
    
    // Convert 0-255 to duty ratio (0-65535 for 16-bit PWM)
    uint16_t duty = (uint16_t)((val * 65535) / 255);
    
    HAL_PWM_ChSetDutyRatio((PWM_CH_ID)pwmCh, duty);
}

// ============================================================
// Time functions
// ============================================================

void delay(unsigned long ms) {
    uint32_t start = millis();
    while ((millis() - start) < ms) {
        // Wait
    }
}

void delayMicroseconds(unsigned int us) {
    if (us == 0) return;
    
    uint32_t start = HAL_TIMER_GetCurrentValue(TIMER0_ID);
    while ((start - HAL_TIMER_GetCurrentValue(TIMER0_ID)) < us) {
        // Wait
    }
}

unsigned long millis(void) {
    return millis_counter;
}

unsigned long micros(void) {
    uint32_t current = HAL_TIMER_GetCurrentValue(TIMER0_ID);
    return (millis_counter * 1000) + (192 - current);
}

// ============================================================
// Interrupt functions
// ============================================================

typedef struct {
    void (*callback)(void);
    GPIO_Port port;
    GPIO_Pin pin;
} interrupt_t;

static interrupt_t interrupts[PINS_COUNT];

void attachInterrupt(uint8_t pin, void (*userFunc)(void), int mode) {
    if (pin >= PINS_COUNT) return;
    
    GPIO_Port port = pinToPort(pin);
    GPIO_Pin gpioPin = pinToPin(pin);
    
    GPIO_IrqParam irqParam;
    irqParam.callback = (GPIO_IRQCallback)userFunc;
    irqParam.arg = NULL;
    
    switch (mode) {
        case RISING:
            irqParam.event = GPIO_IRQ_EVT_RISING_EDGE;
            break;
        case FALLING:
            irqParam.event = GPIO_IRQ_EVT_FALLING_EDGE;
            break;
        case CHANGE:
            irqParam.event = GPIO_IRQ_EVT_BOTH_EDGE;
            break;
        case LOW_LEVEL:
            irqParam.event = GPIO_IRQ_EVT_LOW_LEVEL;
            break;
        case HIGH_LEVEL:
            irqParam.event = GPIO_IRQ_EVT_HIGH_LEVEL;
            break;
        default:
            irqParam.event = GPIO_IRQ_EVT_RISING_EDGE;
            break;
    }
    
    HAL_GPIO_EnableIRQ(port, gpioPin, &irqParam);
    
    interrupts[pin].callback = userFunc;
    interrupts[pin].port = port;
    interrupts[pin].pin = gpioPin;
}

void detachInterrupt(uint8_t pin) {
    if (pin >= PINS_COUNT) return;
    
    GPIO_Port port = pinToPort(pin);
    GPIO_Pin gpioPin = pinToPin(pin);
    
    HAL_GPIO_DisableIRQ(port, gpioPin);
    interrupts[pin].callback = NULL;
}

// ============================================================
// Serial communication (deprecated - use Serial object)
// ============================================================

void serialBegin(unsigned long baud) {
    // Use Serial.begin() instead
}

void serialWrite(uint8_t c) {
    // Use Serial.write() instead
}

void serialPrint(const char* str) {
    // Use Serial.print() instead
}

// ============================================================
// Random functions
// ============================================================

long xr871_random(long max) {
    if (max <= 0) return 0;
    return rand() % max;
}

long xr871_random_range(long min, long max) {
    if (max <= min) return min;
    return min + (rand() % (max - min));
}

void xr871_randomSeed(unsigned long seed) {
    srand(seed);
}

// ============================================================
// Arduino Framework Entry Point
// ============================================================

/**
 * @brief Initialize Arduino framework (called before setup())
 */
void xr871ArduinoInit(void) {
    initMillisTimer();
    
    for (int i = 0; i < PINS_COUNT; i++) {
        interrupts[i].callback = NULL;
    }
}
