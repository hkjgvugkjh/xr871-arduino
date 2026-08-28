/**
 * @file WiFi_AP.ino
 * @brief ESP32 Compatible WiFi AP Example for XR871
 * @author Hermes Agent
 * @date 2026-08-27
 *
 * Creates a WiFi access point and serves a simple web page.
 * Compatible with ESP32 WiFi API.
 *
 * Usage:
 *   1. Set AP_SSID and AP_PASSWORD below
 *   2. Upload to XR871
 *   3. Connect to the AP from your phone/PC
 *   4. Open browser and go to 192.168.4.1
 */

#include "Arduino.h"
#include "WiFi.h"
#include "WiFiClient.h"

// ============================================================
// Configuration
// ============================================================
const char* ap_ssid = "XR871_AP";
const char* ap_password = "12345678";  // Minimum 8 characters

// Create web server on port 80
WiFiServer server(80);

// ============================================================
// Setup
// ============================================================
void setup() {
    serialBegin(115200);
    delay(1000);
    
    serialPrintln("=== XR871 WiFi AP Example ===");
    serialPrintln("ESP32 Compatible WiFi API");
    serialPrintln("");
    
    // Set WiFi mode to AP
    WiFi.mode(WIFI_AP);
    
    // Start AP
    serialPrint("Starting AP: ");
    serialPrintln(ap_ssid);
    
    WiFi.softAP(ap_ssid, ap_password);
    
    serialPrintln("AP started!");
    serialPrint("AP IP address: ");
    serialPrintln(WiFi.softAPIP().toString().c_str());
    serialPrint("AP MAC address: ");
    serialPrintln(WiFi.softAPmacAddress());
    
    // Start web server
    server.begin();
    serialPrintln("Web server started on port 80");
}

// ============================================================
// Loop
// ============================================================
void loop() {
    // Check for clients
    WiFiClient client = server.available();
    
    if (client) {
        serialPrintln("New client connected!");
        
        // Read request
        String request = "";
        unsigned long timeout = millis() + 3000;
        
        while (client.connected() && millis() < timeout) {
            if (client.available()) {
                char c = client.read();
                request += c;
                
                // End of HTTP request
                if (request.endsWith("\r\n\r\n")) {
                    break;
                }
            }
        }
        
        serialPrintln("Request: ");
        serialPrintln(request.c_str());
        
        // Send HTTP response
        client.println("HTTP/1.1 200 OK");
        client.println("Content-Type: text/html");
        client.println("Connection: close");
        client.println();
        client.println("<!DOCTYPE html>");
        client.println("<html>");
        client.println("<head><title>XR871 WiFi</title></head>");
        client.println("<body>");
        client.println("<h1>XR871 Arduino</h1>");
        client.println("<p>ESP32 Compatible WiFi AP</p>");
        client.println("<p>WiFi library for XR871</p>");
        client.println("<hr>");
        client.println("<h2>System Info</h2>");
        client.print("<p>Uptime: ");
        client.print(millis() / 1000);
        client.println(" seconds</p>");
        client.print("<p>Free heap: ");
        client.print(xPortGetFreeHeapSize());
        client.println(" bytes</p>");
        client.print("<p>Connected stations: ");
        client.print(WiFi.softAPgetStationNum());
        client.println("</p>");
        client.println("</body>");
        client.println("</html>");
        
        delay(1);
        client.stop();
        serialPrintln("Client disconnected");
    }
    
    delay(10);
}
