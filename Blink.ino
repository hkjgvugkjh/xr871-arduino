/**
 * @file Blink.ino
 * @brief Arduino Blink Example for XR871
 * @author Hermes Agent
 * @date 2026-08-27
 *
 * Blinks the built-in LED (PA6) on XR871GT development board.
 * This is the "Hello World" of Arduino - if this works, the port is successful!
 */

#include "Arduino.h"

// Built-in LED is connected to PA6 on XR871GT
#define LED_PIN LED_BUILTIN

void setup() {
    // Initialize serial for debugging
    serialBegin(115200);
    serialPrintln("=== XR871 Arduino Blink Demo ===");
    serialPrintln("LED will blink on PA6");
    
    // Set LED pin as output
    pinMode(LED_PIN, OUTPUT);
}

void loop() {
    digitalWrite(LED_PIN, HIGH);  // Turn LED on
    serialPrintln("LED ON");
    delay(500);                    // Wait 500ms
    
    digitalWrite(LED_PIN, LOW);   // Turn LED off
    serialPrintln("LED OFF");
    delay(500);                    // Wait 500ms
}
