/**
 * @file WiFiClient.cpp
 * @brief WiFiClient stub for XR871
 */

#include "Arduino.h"
#include "WiFiClient.h"

WiFiClient::WiFiClient() : _connected(false), _sock(0) {}
WiFiClient::~WiFiClient() { stop(); }

bool WiFiClient::connect(const char* host, uint16_t port) { return false; }
bool WiFiClient::connect(uint32_t ip, uint16_t port) { return false; }
size_t WiFiClient::write(uint8_t) { return 0; }
size_t WiFiClient::write(const uint8_t *buf, size_t size) { return 0; }
int WiFiClient::available() { return 0; }
int WiFiClient::read() { return -1; }
int WiFiClient::read(uint8_t *buf, size_t size) { return 0; }
int WiFiClient::peek() { return -1; }
void WiFiClient::flush() {}
void WiFiClient::stop() { _connected = false; }
uint8_t WiFiClient::connected() { return _connected; }
bool WiFiClient::operator==(const WiFiClient& rhs) { return false; }

String WiFiClient::remoteIP() { return String("0.0.0.0"); }
uint16_t WiFiClient::remotePort() { return 0; }
String WiFiClient::localIP() { return String("0.0.0.0"); }
uint16_t WiFiClient::localPort() { return 0; }
