/*
 * Web Server Example
 * 
 * Creates a simple web server with LED control.
 * Compatible with ESP32 WebServer API.
 */

#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "YOUR_SSID";
const char* password = "YOUR_PASSWORD";

WebServer server(80);

void handleRoot() {
    String html = "<html><head><title>XR871</title></head>";
    html += "<body><h1>XR871 Web Server</h1>";
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

void setup() {
    Serial.begin(115200);
    pinMode(LED_BUILTIN, OUTPUT);
    
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    
    Serial.println();
    Serial.print("IP: ");
    Serial.println(WiFi.localIP());
    
    server.on("/", handleRoot);
    server.on("/led", handleLED);
    server.begin();
}

void loop() {
    server.handleClient();
}
