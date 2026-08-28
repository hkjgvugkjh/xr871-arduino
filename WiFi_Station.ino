/**
 * @file WiFi_Station.ino
 * @brief ESP32 Compatible WiFi Station Example for XR871
 * @author Hermes Agent
 * @date 2026-08-27
 *
 * Connects to WiFi network in STA mode and fetches HTTP data.
 * Compatible with ESP32 WiFi API.
 *
 * Usage:
 *   1. Set SSID and PASSWORD below
 *   2. Upload to XR871
 *   3. Open serial monitor
 */

#include "Arduino.h"
#include "WiFi.h"

// ============================================================
// Configuration
// ============================================================
const char* ssid = "YOUR_SSID";
const char* password = "YOUR_PASSWORD";

// ============================================================
// Setup
// ============================================================
void setup() {
    serialBegin(115200);
    delay(1000);
    
    serialPrintln("=== XR871 WiFi Station Example ===");
    serialPrintln("ESP32 Compatible WiFi API");
    serialPrintln("");
    
    // Set WiFi mode
    WiFi.mode(WIFI_STA);
    
    // Connect to WiFi
    serialPrint("Connecting to ");
    serialPrintln(ssid);
    
    WiFi.begin(ssid, password);
    
    // Wait for connection
    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < 20) {
        delay(500);
        serialPrint(".");
        attempts++;
    }
    
    if (WiFi.status() == WL_CONNECTED) {
        serialPrintln("");
        serialPrintln("WiFi connected!");
        serialPrint("IP address: ");
        serialPrintln(WiFi.localIP().toString().c_str());
        serialPrint("MAC address: ");
        serialPrintln(WiFi.macAddress());
        serialPrint("RSSI: ");
        serialPrintInt(WiFi.getRSSI(), DEC);
        serialPrintln(" dBm");
    } else {
        serialPrintln("");
        serialPrintln("WiFi connection failed!");
    }
}

// ============================================================
// Loop
// ============================================================
void loop() {
    // Print connection status every 10 seconds
    static unsigned long lastPrint = 0;
    if (millis() - lastPrint > 10000) {
        lastPrint = millis();
        
        if (WiFi.status() == WL_CONNECTED) {
            serialPrintln("WiFi still connected");
            serialPrint("IP: ");
            serialPrintln(WiFi.localIP().toString().c_str());
        } else {
            serialPrintln("WiFi disconnected!");
        }
    }
    
    delay(100);
}
