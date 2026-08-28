/**
 * @file SPI_Test.ino
 * @brief Arduino SPI Test for XR871
 * @author Hermes Agent
 * @date 2026-08-27
 *
 * Tests SPI0 in master mode.
 * Sends and receives data via SPI bus.
 *
 * Hardware connections:
 *   MOSI -> PB4 (SPI0)
 *   MISO -> PB5 (SPI0)
 *   CLK  -> PB7 (SPI0)
 *   CS   -> PB6 (SPI0)
 */

#include "Arduino.h"

void setup() {
    serialBegin(115200);
    delay(1000);
    
    serialPrintln("=== XR871 SPI Test ===");
    serialPrintln("Initializing SPI0 (MOSI=PB4, MISO=PB5, CLK=PB7, CS=PB6)");
    
    // Initialize SPI0
    SPI.begin(SPI0, SPI_TCTRL_SS_SEL_SS0);
    SPI.setDataMode(SPI_MODE0);
    SPI.setBitOrder(MSBFIRST);
    SPI.setClockDivider(SPI_CLOCK_DIV4);
    
    serialPrintln("SPI0 initialized at 48MHz/4 = 12MHz");
    serialPrintln("");
    
    // Test 1: Single byte transfer
    serialPrintln("Test 1: Single byte transfer");
    SPI.beginTransaction();
    uint8_t txData = 0xA5;
    uint8_t rxData = SPI.transfer(txData);
    SPI.endTransaction();
    serialPrint("Sent: 0x");
    serialPrintInt(txData, HEX);
    serialPrint(", Received: 0x");
    serialPrintInt(rxData, HEX);
    serialPrintln("");
    
    // Test 2: Multiple byte transfer
    serialPrintln("\nTest 2: Multiple byte transfer");
    uint8_t txBuf[] = {0x01, 0x02, 0x03, 0x04, 0x05};
    uint8_t rxBuf[5];
    
    SPI.beginTransaction();
    for (uint8_t i = 0; i < 5; i++) {
        rxBuf[i] = SPI.transfer(txBuf[i]);
    }
    SPI.endTransaction();
    
    serialPrint("Sent: ");
    for (uint8_t i = 0; i < 5; i++) {
        serialPrintInt(txBuf[i], HEX);
        serialPrint(" ");
    }
    serialPrintln("");
    
    serialPrint("Received: ");
    for (uint8_t i = 0; i < 5; i++) {
        serialPrintInt(rxBuf[i], HEX);
        serialPrint(" ");
    }
    serialPrintln("");
    
    // Test 3: 16-bit transfer
    serialPrintln("\nTest 3: 16-bit transfer");
    uint16_t tx16 = 0xABCD;
    uint16_t rx16 = SPI.transfer16(tx16);
    serialPrint("Sent: 0x");
    serialPrintInt(tx16, HEX);
    serialPrint(", Received: 0x");
    serialPrintInt(rx16, HEX);
    serialPrintln("");
    
    serialPrintln("\nSPI Test complete.");
}

void loop() {
    delay(5000);
}
