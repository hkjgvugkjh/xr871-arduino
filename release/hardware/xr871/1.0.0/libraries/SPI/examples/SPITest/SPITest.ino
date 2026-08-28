/*
 * SPI Example
 * 
 * Tests SPI0 in master mode.
 * Compatible with Arduino SPI API.
 */

#include <SPI.h>

void setup() {
    Serial.begin(115200);
    SPI.begin();
    SPI.setDataMode(SPI_MODE0);
    SPI.setBitOrder(MSBFIRST);
    SPI.setClockDivider(SPI_CLOCK_DIV4);
    
    Serial.println("SPI Test");
    
    uint8_t txData = 0xA5;
    uint8_t rxData = SPI.transfer(txData);
    
    Serial.print("Sent: 0x");
    Serial.println(txData, HEX);
    Serial.print("Received: 0x");
    Serial.println(rxData, HEX);
}

void loop() {
    delay(1000);
}
