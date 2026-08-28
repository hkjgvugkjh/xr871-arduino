/**
 * @file HTTP_Client.ino
 * @brief ESP32 Compatible HTTP Client Example for XR871
 * @author Hermes Agent
 * @date 2026-08-27
 *
 * Makes HTTP GET and POST requests.
 * Compatible with ESP32 HTTPClient API.
 */

#include "Arduino.h"
#include "WiFi.h"
#include "HTTPClient.h"

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
    
    serialPrintln("=== XR871 HTTP Client Example ===");
    serialPrintln("ESP32 Compatible HTTPClient API");
    serialPrintln("");
    
    // Connect to WiFi
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    
    serialPrint("Connecting to WiFi");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        serialPrint(".");
    }
    serialPrintln("");
    serialPrintln("WiFi connected!");
    serialPrint("IP: ");
    serialPrintln(WiFi.localIP().toString().c_str());
    serialPrintln("");
    
    // HTTP GET request
    HTTPClient http;
    http.begin("http://httpbin.org/get");
    int httpCode = http.GET();
    
    if (httpCode > 0) {
        serialPrint("GET Response code: ");
        serialPrintInt(httpCode, DEC);
        serialPrintln("");
        serialPrintln(http.getString().c_str());
    } else {
        serialPrintln("GET failed");
    }
    http.end();
    
    // HTTP POST request
    HTTPClient httpPost;
    httpPost.begin("http://httpbin.org/post");
    httpPost.addHeader("Content-Type", "application/json");
    
    String json = "{\"name\":\"XR871\",\"version\":\"1.0\"}";
    int postCode = httpPost.POST(json);
    
    if (postCode > 0) {
        serialPrint("POST Response code: ");
        serialPrintInt(postCode, DEC);
        serialPrintln("");
        serialPrintln(httpPost.getString().c_str());
    } else {
        serialPrintln("POST failed");
    }
    httpPost.end();
}

void loop() {
    delay(10000);
}
