/**
 * @file Arduino.c
 * @brief Arduino Core Implementation for XR871
 * @author Hermes Agent
 * @date 2026-08-27
 */

#include "Arduino.h"
#include <stdio.h>

const PinMap g_pinMap[] = {
    {GPIO_PORT_A, GPIO_PIN_0, 0},
    {GPIO_PORT_A, GPIO_PIN_1, 1},
    {GPIO_PORT_A, GPIO_PIN_2, 2},
    {GPIO_PORT_A, GPIO_PIN_3, 3},
    {GPIO_PORT_A, GPIO_PIN_4, 4},
    {GPIO_PORT_A, GPIO_PIN_5, 5},
    {GPIO_PORT_A, GPIO_PIN_6, 6},
    {GPIO_PORT_A, GPIO_PIN_7, 7},
    {GPIO_PORT_A, GPIO_PIN_8, 8},
    {GPIO_PORT_A, GPIO_PIN_9, 9},
    {GPIO_PORT_A, GPIO_PIN_10, 10},
    {GPIO_PORT_A, GPIO_PIN_11, 11},
    {GPIO_PORT_A, GPIO_PIN_12, 12},
    {GPIO_PORT_A, GPIO_PIN_13, 13},
    {GPIO_PORT_A, GPIO_PIN_14, 14},
    {GPIO_PORT_A, GPIO_PIN_15, 15},
    {GPIO_PORT_A, GPIO_PIN_16, 16},
    {GPIO_PORT_A, GPIO_PIN_17, 17},
    {GPIO_PORT_A, GPIO_PIN_18, 18},
    {GPIO_PORT_A, GPIO_PIN_19, 19},
    {GPIO_PORT_A, GPIO_PIN_20, 20},
    {GPIO_PORT_A, GPIO_PIN_21, 21},
    {GPIO_PORT_A, GPIO_PIN_22, 22},
    {GPIO_PORT_B, GPIO_PIN_0, 23},
    {GPIO_PORT_B, GPIO_PIN_1, 24},
    {GPIO_PORT_B, GPIO_PIN_2, 25},
    {GPIO_PORT_B, GPIO_PIN_3, 26},
    {GPIO_PORT_B, GPIO_PIN_4, 27},
    {GPIO_PORT_B, GPIO_PIN_5, 28},
    {GPIO_PORT_B, GPIO_PIN_6, 29},
    {GPIO_PORT_B, GPIO_PIN_7, 30},
    {GPIO_PORT_B, GPIO_PIN_8, 31},
    {GPIO_PORT_B, GPIO_PIN_9, 32},
    {GPIO_PORT_B, GPIO_PIN_10, 33},
    {GPIO_PORT_B, GPIO_PIN_11, 34},
    {GPIO_PORT_B, GPIO_PIN_12, 35},
    {GPIO_PORT_B, GPIO_PIN_13, 36},
    {GPIO_PORT_B, GPIO_PIN_14, 37},
    {GPIO_PORT_B, GPIO_PIN_15, 38},
};

const uint8_t g_pinCount = sizeof(g_pinMap) / sizeof(g_pinMap[0]);

GPIO_Port getPinPort(uint8_t pin) {
    if (pin < g_pinCount) return g_pinMap[pin].port;
    return GPIO_PORT_A;
}

GPIO_Pin getPinIndex(uint8_t pin) {
    if (pin < g_pinCount) return g_pinMap[pin].pin;
    return GPIO_PIN_0;
}

uint8_t pinToArduino(GPIO_Port port, GPIO_Pin pin) {
    for (uint8_t i = 0; i < g_pinCount; i++) {
        if (g_pinMap[i].port == port && g_pinMap[i].pin == pin) {
            return g_pinMap[i].arduino_pin;
        }
    }
    return 0xFF;
}

void init(void) {
    HAL_BoardIoctl(HAL_BIR_PINMUX_INIT, 0, 0);
    HAL_BoardIoctl(HAL_BIR_CHIP_CLOCK_INIT, 0, 0);
}

void pinMode(uint8_t pin, uint8_t mode) {
    if (pin >= g_pinCount) return;
    
    GPIO_InitParam param;
    param.driving = GPIO_DRIVING_LEVEL_1;
    param.pull = GPIO_PULL_NONE;
    
    switch (mode) {
        case INPUT: param.mode = GPIOx_Pn_F0_INPUT; break;
        case OUTPUT: param.mode = GPIOx_Pn_F1_OUTPUT; break;
        case INPUT_PULLUP:
            param.mode = GPIOx_Pn_F0_INPUT;
            param.pull = GPIO_PULL_UP;
            break;
        case INPUT_PULLDOWN:
            param.mode = GPIOx_Pn_F0_INPUT;
            param.pull = GPIO_PULL_DOWN;
            break;
        default: param.mode = GPIOx_Pn_F0_INPUT; break;
    }
    
    HAL_GPIO_Init(g_pinMap[pin].port, g_pinMap[pin].pin, &param);
}

void digitalWrite(uint8_t pin, uint8_t val) {
    if (pin >= g_pinCount) return;
    GPIO_PinState state = (val == HIGH) ? GPIO_PIN_HIGH : GPIO_PIN_LOW;
    HAL_GPIO_WritePin(g_pinMap[pin].port, g_pinMap[pin].pin, state);
}

int digitalRead(uint8_t pin) {
    if (pin >= g_pinCount) return LOW;
    GPIO_PinState state = HAL_GPIO_ReadPin(g_pinMap[pin].port, g_pinMap[pin].pin);
    return (state == GPIO_PIN_HIGH) ? HIGH : LOW;
}

void delay(unsigned long ms) {
    OS_MSleep(ms);
}

void delayMicroseconds(unsigned int us) {
    volatile unsigned long count = us * 192 / 10;
    while (count--) {
        __asm__ volatile ("nop");
    }
}

unsigned long millis(void) {
    return OS_GetTimeMs();
}

unsigned long micros(void) {
    return OS_GetTimeMs() * 1000;
}

typedef struct {
    void (*callback)(void);
    uint8_t enabled;
} InterruptHandler;

static InterruptHandler g_irqHandlers[38] = {0};

void attachInterrupt(uint8_t pin, void (*userFunc)(void), int mode) {
    if (pin >= g_pinCount) return;
    
    GPIO_IrqEvent event;
    switch (mode) {
        case RISING: event = GPIO_IRQ_EVT_RISING_EDGE; break;
        case FALLING: event = GPIO_IRQ_EVT_FALLING_EDGE; break;
        case CHANGE: event = GPIO_IRQ_EVT_BOTH_EDGE; break;
        case LOW_LEVEL: event = GPIO_IRQ_EVT_LOW_LEVEL; break;
        case HIGH_LEVEL: event = GPIO_IRQ_EVT_HIGH_LEVEL; break;
        default: event = GPIO_IRQ_EVT_FALLING_EDGE; break;
    }
    
    GPIO_IrqParam irqParam;
    irqParam.event = event;
    irqParam.callback = (GPIO_IRQCallback)userFunc;
    irqParam.arg = NULL;
    
    HAL_GPIO_EnableIRQ(g_pinMap[pin].port, g_pinMap[pin].pin, &irqParam);
    g_irqHandlers[pin].callback = userFunc;
    g_irqHandlers[pin].enabled = 1;
}

void detachInterrupt(uint8_t pin) {
    if (pin >= g_pinCount) return;
    HAL_GPIO_DisableIRQ(g_pinMap[pin].port, g_pinMap[pin].pin);
    g_irqHandlers[pin].enabled = 0;
}

void interrupts(void) {
    __asm__ volatile ("cpsie i");
}

void noInterrupts(void) {
    __asm__ volatile ("cpsid i");
}

static UART_ID g_serialUart = UART0_ID;

void serialBegin(unsigned long baud) {
    HAL_UART_Init(g_serialUart, baud);
}

void serialEnd(void) {
    HAL_UART_DeInit(g_serialUart);
}

int serialAvailable(void) {
    return HAL_UART_IsRxReady(g_serialUart);
}

int serialRead(void) {
    if (HAL_UART_IsRxReady(g_serialUart)) {
        return HAL_UART_ReceiveChar(g_serialUart);
    }
    return -1;
}

void serialWrite(uint8_t c) {
    HAL_UART_SendChar(g_serialUart, c);
}

void serialPrint(const char* str) {
    while (*str) {
        serialWrite(*str++);
    }
}

void serialPrintln(const char* str) {
    serialPrint(str);
    serialWrite('\r');
    serialWrite('\n');
}

void serialPrintInt(long n, int base) {
    char buf[32];
    itoa(n, buf, base);
    serialPrint(buf);
}

long random(long max) {
    return rand() % max;
}

long random(long min, long max) {
    return min + (rand() % (max - min));
}

void randomSeed(unsigned long seed) {
    srand(seed);
}

void shiftOut(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder, uint8_t val) {
    for (uint8_t i = 0; i < 8; i++) {
        if (bitOrder == LSBFIRST) {
            digitalWrite(dataPin, !!(val & (1 << i)));
        } else {
            digitalWrite(dataPin, !!(val & (1 << (7 - i))));
        }
        digitalWrite(clockPin, HIGH);
        digitalWrite(clockPin, LOW);
    }
}

uint8_t shiftIn(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder) {
    uint8_t value = 0;
    for (uint8_t i = 0; i < 8; i++) {
        digitalWrite(clockPin, HIGH);
        if (bitOrder == LSBFIRST) {
            value |= digitalRead(dataPin) << i;
        } else {
            value |= digitalRead(dataPin) << (7 - i);
        }
        digitalWrite(clockPin, LOW);
    }
    return value;
}

int analogRead(uint8_t pin) {
    return 0;
}

void analogReference(uint8_t mode) {
}

void analogWrite(uint8_t pin, int val) {
    PWM_CH_ID channel = pinToPWMChannel(pin);
    if (channel == PWM_CH_NULL) return;
    
    if (!g_pwmChannels[channel].enabled) {
        pwmBegin(channel, PWM_DEFAULT_FREQ);
    }
    
    uint16_t duty = (uint16_t)(val * 65535 / 255);
    pwmSetDuty(channel, duty);
    pwmEnable(channel);
}
