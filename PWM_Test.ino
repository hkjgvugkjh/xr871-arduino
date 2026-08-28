/**
 * @file PWM_Test.ino
 * @brief Arduino PWM Test for XR871
 * @author Hermes Agent
 * @date 2026-08-27
 *
 * Tests PWM output on PA8 (Arduino pin 8).
 * Fades an LED using PWM.
 *
 * Hardware connections:
 *   PWM output -> PA8 (Arduino pin 8)
 *   LED -> PA8 (with current limiting resistor)
 */

#include "Arduino.h"

#define PWM_PIN 8  // PA8 - PWM channel 0

void setup() {
    serialBegin(115200);
    delay(1000);
    
    serialPrintln("=== XR871 PWM Test ===");
    serialPrintln("PWM output on PA8 (pin 8)");
    serialPrintln("");
    
    // Fade LED using PWM
    serialPrintln("Fading LED...");
}

void loop() {
    // Fade in
    for (int i = 0; i <= 255; i++) {
        analogWrite(PWM_PIN, i);
        delay(10);
    }
    
    // Fade out
    for (int i = 255; i >= 0; i--) {
        analogWrite(PWM_PIN, i);
        delay(10);
    }
    
    delay(500);
}
