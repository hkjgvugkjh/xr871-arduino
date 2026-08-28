/**
 * @file I2C_Scan.ino
 * @brief Arduino I2C Scanner for XR871
 * @author Hermes Agent
 * @date 2026-08-27
 *
 * Scans I2C bus for connected devices.
 * Outputs found serial addresses to serial monitor.
 *
 * Hardware connections:
 *   SCL -> PA2 (I2C0) or PA4 (I2C1)
 *   SDA -> PA3 (I2C0) or PA5 (I2C1)
 */

#include "Arduino.h"

void setup() {
    serialBegin(115200);
    delay(1000);
    
    serialPrintln("=== XR871 I2C Scanner ===");
    serialPrintln("Scanning I2C0 bus (SCL=PA2, SDA=PA3)...");
    
    // Initialize I2C0 at 100kHz
    Wire.begin(I2C0_ID, WIRE_CLOCK_100K);
    
    uint8_t devicesFound = 0;
    
    for (uint8_t addr = 1; addr < 127; addr++) {
        Wire.beginTransmission(addr);
        uint8_t error = Wire.endTransmission();
        
        if (error == 0) {
            serialPrint("Device found at 0x");
            serialPrintInt(addr, HEX);
            serialPrintln("");
            devicesFound++;
        }
    }
    
    if (devicesFound == 0) {
        serialPrintln("No I2C devices found.");
    } else {
        serialPrint("Total devices found: ");
        serialPrintInt(devicesFound, DEC);
        serialPrintln("");
    }
    
    serialPrintln("\nScan complete.");
}

void loop() {
    delay(5000);
}
