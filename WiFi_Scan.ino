/**
 * @file WiFi_Scan.ino
 * @brief ESP32 Compatible WiFi Scanner for XR871
 * @author Hermes Agent
 * @date 2026-08-27
 *
 * Scans for available WiFi networks and displays results.
 * Compatible with ESP32 WiFi API.
 */

#include "Arduino.h"
#include "WiFi.h"

void setup() {
    serialBegin(115200);
    delay(1000);
    
    serialPrintln("=== XR871 WiFi Scanner ===");
    serialPrintln("ESP32 Compatible WiFi API");
    serialPrintln("");
    
    // Set WiFi mode to STA
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    delay(100);
    
    serialPrintln("Scanning for WiFi networks...");
    
    // Scan for networks
    int numNetworks = WiFi.scanNetworks();
    
    if (numNetworks == WIFI_SCAN_FAILED) {
        serialPrintln("Scan failed!");
        return;
    }
    
    if (numNetworks == 0) {
        serialPrintln("No networks found.");
        return;
    }
    
    serialPrint("Found ");
    serialPrintInt(numNetworks, DEC);
    serialPrintln(" networks:");
    serialPrintln("");
    
    // Print header
    serialPrintln("No.  SSID                             RSSI  Channel  Encryption");
    serialPrintln("----------------------------------------------------------------");
    
    // Print network info
    for (int i = 0; i < numNetworks; i++) {
        serialPrint(" ");
        serialPrintInt(i + 1, DEC);
        serialPrint("   ");
        
        // SSID
        String ssid = WiFi.SSID(i);
        serialPrint(ssid.c_str());
        
        // Pad to 32 chars
        for (int j = ssid.length(); j < 32; j++) {
            serialPrint(" ");
        }
        
        // RSSI
        serialPrint("  ");
        serialPrintInt(WiFi.RSSI(i), DEC);
        serialPrint(" dBm  ");
        
        // Channel
        serialPrintInt(WiFi.channel(i), DEC);
        serialPrint("        ");
        
        // Encryption type
        wifi_auth_mode_t auth = WiFi.encryptionType(i);
        switch (auth) {
            case WIFI_AUTH_OPEN: serialPrint("Open"); break;
            case WIFI_AUTH_WEP: serialPrint("WEP"); break;
            case WIFI_AUTH_WPA_PSK: serialPrint("WPA-PSK"); break;
            case WIFI_AUTH_WPA2_PSK: serialPrint("WPA2-PSK"); break;
            case WIFI_AUTH_WPA_WPA2_PSK: serialPrint("WPA/WPA2-PSK"); break;
            case WIFI_AUTH_WPA2_ENTERPRISE: serialPrint("WPA2-Enterprise"); break;
            case WIFI_AUTH_WPA3_PSK: serialPrint("WPA3-PSK"); break;
            case WIFI_AUTH_WPA2_WPA3_PSK: serialPrint("WPA2/WPA3-PSK"); break;
            default: serialPrint("Unknown"); break;
        }
        
        serialPrintln("");
    }
    
    serialPrintln("");
    serialPrintln("Scan complete.");
}

void loop() {
    delay(10000);
}
