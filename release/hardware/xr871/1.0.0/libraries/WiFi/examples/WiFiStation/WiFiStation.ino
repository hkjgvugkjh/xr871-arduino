/*
 * WiFi Station Example
 * 
 * Connects to WiFi network and prints IP address.
 * Compatible with ESP32 WiFi API.
 */

#include <WiFi.h>

const char* ssid = "YOUR_SSID";
const char* password = "YOUR_PASSWORD";

void setup() {
    Serial.begin(115200);
    delay(1000);
    
    Serial.println("WiFi Station Example");
    
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    
    Serial.print("Connecting");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    
    Serial.println();
    Serial.print("Connected, IP: ");
    Serial.println(WiFi.localIP());
}

void loop() {
    // Print RSSI every 5 seconds
    Serial.print("RSSI: ");
    Serial.print(WiFi.RSSI());
    Serial.println(" dBm");
    delay(5000);
}
