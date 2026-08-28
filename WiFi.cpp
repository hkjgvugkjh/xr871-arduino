/**
 * @file WiFi.cpp
 * @brief WiFi stub implementation for XR871
 */

#include "Arduino.h"
#include "WiFi.h"

WiFiClass WiFi;

WiFiClass::WiFiClass() : _mode(WIFI_MODE_NULL), _autoReconnect(true), _eventCallback(NULL) {}

bool WiFiClass::mode(wifi_mode_t mode) { _mode = mode; return true; }
wifi_mode_t WiFiClass::getMode() { return _mode; }

bool WiFiClass::begin(const char* ssid, const char* passphrase, int32_t channel, const uint8_t* bssid) { return false; }
bool WiFiClass::begin() { return false; }
bool WiFiClass::disconnect(bool wifioff, bool eraseap) { return false; }
bool WiFiClass::config(IPAddress local_ip, IPAddress gateway, IPAddress subnet, IPAddress dns1, IPAddress dns2) { return false; }

wl_status_t WiFiClass::status() { return WL_DISCONNECTED; }
bool WiFiClass::isConnected() { return false; }
String WiFiClass::macAddress() { return String("00:00:00:00:00:00"); }
IPAddress WiFiClass::localIP() { return IPAddress(0,0,0,0); }
IPAddress WiFiClass::subnetMask() { return IPAddress(0,0,0,0); }
IPAddress WiFiClass::gatewayIP() { return IPAddress(0,0,0,0); }
IPAddress WiFiClass::dnsIP(uint8_t dns_no) { return IPAddress(0,0,0,0); }
String WiFiClass::getBSSID() { return String(""); }
int8_t WiFiClass::getChannel() { return 0; }
int8_t WiFiClass::getRSSI() { return 0; }
bool WiFiClass::getAutoConnect() { return false; }
bool WiFiClass::setAutoConnect(bool autoConnect) { return false; }
bool WiFiClass::getAutoReconnect() { return _autoReconnect; }
bool WiFiClass::setAutoReconnect(bool autoReconnect) { _autoReconnect = autoReconnect; return true; }
bool WiFiClass::setHostname(const char* hostname) { return false; }
uint8_t* WiFiClass::macAddress(uint8_t* mac) { return mac; }
const char* WiFiClass::getHostname() { return "xr871"; }

bool WiFiClass::softAP(const char* ssid, const char* passphrase, int channel, int ssid_hidden, int max_connection) { return false; }
bool WiFiClass::softAPconfig(IPAddress local_ip, IPAddress gateway, IPAddress subnet) { return false; }
bool WiFiClass::softAPdisconnect(bool wifioff) { return false; }
String WiFiClass::softAPmacAddress() { return String("00:00:00:00:00:00"); }
uint8_t WiFiClass::softAPgetStationNum() { return 0; }
IPAddress WiFiClass::softAPIP() { return IPAddress(0,0,0,0); }
String WiFiClass::softAPSSID() const { return String(""); }

int8_t WiFiClass::scanNetworks(bool async, bool show_hidden, bool passive, uint32_t max_ms_per_chan) { return 0; }
int16_t WiFiClass::scanComplete() { return 0; }
void WiFiClass::scanDelete() {}
String WiFiClass::SSID(uint8_t networkItem) { return String(""); }
wifi_auth_mode_t WiFiClass::encryptionType(uint8_t networkItem) { return WIFI_AUTH_OPEN; }
int32_t WiFiClass::RSSI(uint8_t networkItem) { return 0; }
uint8_t* WiFiClass::BSSID(uint8_t networkItem) { return NULL; }
int32_t WiFiClass::channel(uint8_t networkItem) { return 0; }
bool WiFiClass::getNetworkInfo(uint8_t networkItem, String &ssid, uint8_t &encryptionType, int32_t &RSSI, uint8_t* &BSSID, int32_t &channel) { return false; }

wifi_err_reason_t WiFiClass::disconnectReason(system_event_id_t event_id) { return WIFI_REASON_UNSPECIFIED; }
void WiFiClass::onEvent(system_event_cb_t cbEvent, system_event_id_t event) { _eventCallback = cbEvent; }
system_event_cb_t WiFiClass::onEvent(system_event_cb_t cbEvent) { _eventCallback = cbEvent; return _eventCallback; }

bool WiFiClass::setTxPower(wifi_power_t power) { return false; }
wifi_power_t WiFiClass::getTxPower() { return WIFI_POWER_2dBm; }
bool WiFiClass::setSleep(bool enable) { return false; }
bool WiFiClass::getSleep() { return false; }

void WiFiClass::persistent(bool persistent) {}
bool WiFiClass::enableSTA(bool enable) { return false; }
void WiFiClass::enableAP(bool enable) {}
int WiFiClass::hostByName(const char* aHostname, IPAddress& aResult) { return 0; }
int WiFiClass::hostByName(const char* aHostname, IPAddress& aResult, uint32_t timeout_ms) { return 0; }

const char* WiFiClass::statusToString(wl_status_t status) { return "Unknown"; }
const char* WiFiClass::eventTypeToString(system_event_id_t event_id) { return "Unknown"; }

bool WiFiClass::initSTA(const char* ssid, const char* passphrase, int32_t channel, const uint8_t* bssid) { return false; }
bool WiFiClass::initAP(const char* ssid, const char* passphrase, int channel, int ssid_hidden, int max_connection) { return false; }
