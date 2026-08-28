/*
 * OTA Update Example
 * 
 * Demonstrates OTA firmware update functionality.
 * Compatible with ESP32 ArduinoOTA API.
 */

#include <WiFi.h>
#include <OTA.h>

const char* ssid = "YOUR_SSID";
const char* password = "YOUR_PASSWORD";

void setup() {
    Serial.begin(115200);
    delay(1000);
    
    Serial.println("OTA Update Example");
    
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    
    Serial.println();
    Serial.print("IP: ");
    Serial.println(WiFi.localIP());
    
    ArduinoOTA.begin();
    
    ArduinoOTA.onStart([]() {
        Serial.println("OTA Start");
    });
    
    ArduinoOTA.onEnd([]() {
        Serial.println("OTA End");
    });
    
    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
        Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
    });
    
    ArduinoOTA.onError([](ota_error_t error) {
        Serial.printf("Error[%u]: ", error);
    });
    
    Serial.println("OTA Ready!");
}

void loop() {
    ArduinoOTA.handle();
    delay(10);
}
