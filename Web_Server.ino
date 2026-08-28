/**
 * @file Web_Server.ino
 * @brief ESP32 Compatible Web Server Example for XR871
 * @author Hermes Agent
 * @date 2026-08-27
 *
 * Creates a web server that serves a simple page with LED control.
 * Compatible with ESP32 WebServer API.
 */

#include "Arduino.h"
#include "WiFi.h"
#include "WebServer.h"

const char* ssid = "YOUR_SSID";
const char* password = "YOUR_PASSWORD";

WebServer server(80);

void handleRoot();
void handleLED();
void handleNotFound();

void setup() {
    serialBegin(115200);
    delay(1000);
    
    serialPrintln("=== XR871 Web Server Example ===");
    
    pinMode(LED_BUILTIN, OUTPUT);
    
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        serialPrint(".");
    }
    
    serialPrintln("");
    serialPrint("IP: ");
    serialPrintln(WiFi.localIP().toString().c_str());
    
    server.on("/", handleRoot);
    server.on("/led", handleLED);
    server.onNotFound(handleNotFound);
    
    server.begin();
    serialPrintln("Web server started");
}

void loop() {
    server.handleClient();
}

void handleRoot() {
    String html = "<html><head><title>XR871 Web Server</title></head>";
    html += "<body><h1>XR871 Arduino Web Server</h1>";
    html += "<p><a href=\"/led?state=1\">LED ON</a></p>";
    html += "<p><a href=\"/led?state=0\">LED OFF</a></p>";
    html += "</body></html>";
    
    server.send(200, "text/html", html);
}

void handleLED() {
    String state = server.arg("state");
    if (state == "1") {
        digitalWrite(LED_BUILTIN, HIGH);
        server.send(200, "text/plain", "LED ON");
    } else {
        digitalWrite(LED_BUILTIN, LOW);
        server.send(200, "text/plain", "LED OFF");
    }
}

void handleNotFound() {
    server.send(404, "text/plain", "Not Found");
}
