/*
 * I2C Scanner Example
 * 
 * Scans I2C bus for connected devices.
 * Compatible with Arduino Wire API.
 */

#include <Wire.h>

void setup() {
    Serial.begin(115200);
    Wire.begin();
    
    Serial.println("I2C Scanner");
    Serial.println("Scanning...");
    
    for (uint8_t addr = 1; addr < 127; addr++) {
        Wire.beginTransmission(addr);
        uint8_t error = Wire.endTransmission();
        
        if (error == 0) {
            Serial.print("Device found at 0x");
            if (addr < 16) Serial.print("0");
            Serial.println(addr, HEX);
        }
    }
    
    Serial.println("Scan complete.");
}

void loop() {
    delay(5000);
}
