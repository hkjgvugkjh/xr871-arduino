/**
 * @file WiFi.cpp
 * @brief WiFi implementation for XR871 - calls wlan driver
 * @author Hermes Agent
 * @date 2026-08-28
 */

#include "Arduino.h"
#include "WiFi.h"

// XR871 wlan driver
#include "net/wlan/wlan.h"
#include "net/wlan/wlan_defs.h"
#include "net/wlan/ethernetif.h"

WiFiClass WiFi;

WiFiClass::WiFiClass() : _mode(WIFI_MODE_NULL), _autoReconnect(true), _eventCallback(NULL) {}

// ============================================================
// STA Mode
// ============================================================

bool WiFiClass::mode(wifi_mode_t mode) {
    _mode = mode;
    
    enum wlan_mode wmode;
    switch (mode) {
        case WIFI_MODE_STA:
            wmode = WLAN_MODE_STA;
            break;
        case WIFI_MODE_AP:
            wmode = WLAN_MODE_HOSTAP;
            break;
        default:
            wmode = WLAN_MODE_STA;
            break;
    }
    
    // Create network interface
    struct netif *nif = ethernetif_create(wmode);
    if (nif == NULL) return false;
    
    return true;
}

wifi_mode_t WiFiClass::getMode() {
    return _mode;
}

bool WiFiClass::begin(const char* ssid, const char* passphrase, int32_t channel, const uint8_t* bssid) {
    if (_mode == WIFI_MODE_NULL) {
        if (!mode(WIFI_MODE_STA)) return false;
    }
    
    // Set SSID and password
    wlan_sta_config_t config;
    memset(&config, 0, sizeof(config));
    strncpy((char*)config.ssid, ssid, WLAN_SSID_MAX_LEN);
    if (passphrase) {
        strncpy((char*)config.psk, passphrase, WLAN_PASSPHRASE_MAX_LEN);
    }
    
    if (wlan_sta_set_config(&config) != 0) return false;
    
    // Connect
    if (wlan_sta_connect() != 0) return false;
    
    return true;
}

bool WiFiClass::begin() {
    return wlan_sta_enable() == 0;
}

bool WiFiClass::disconnect(bool wifioff, bool eraseap) {
    return wlan_sta_disconnect() == 0;
}

// ============================================================
// AP Mode
// ============================================================

bool WiFiClass::softAP(const char* ssid, const char* passphrase, int channel, int ssid_hidden, int max_connection) {
    if (_mode == WIFI_MODE_NULL) {
        if (!mode(WIFI_MODE_AP)) return false;
    }
    
    wlan_sta_config_t config;
    memset(&config, 0, sizeof(config));
    strncpy((char*)config.ssid, ssid, WLAN_SSID_MAX_LEN);
    if (passphrase && strlen(passphrase) > 0) {
        strncpy((char*)config.psk, passphrase, WLAN_PASSPHRASE_MAX_LEN);
    }
    
    if (wlan_sta_set_config(&config) != 0) return false;
    
    return wlan_sta_enable() == 0;
}

bool WiFiClass::softAPconfig(IPAddress local_ip, IPAddress gateway, IPAddress subnet) {
    // TODO: Set AP IP config
    return true;
}

bool WiFiClass::softAPdisconnect(bool wifioff) {
    return wlan_stop() == 0;
}

uint8_t WiFiClass::softAPgetStationNum() {
    return 0;
}

IPAddress WiFiClass::softAPIP() {
    return IPAddress(0,0,0,0);
}

String WiFiClass::softAPSSID() const {
    return String("");
}

// ============================================================
// Status
// ============================================================

wl_status_t WiFiClass::status() {
    wlan_sta_states_t state;
    if (wlan_sta_state(&state) != 0) return WL_DISCONNECTED;
    
    switch (state) {
        case WLAN_STA_STATE_CONNECTED:
            return WL_CONNECTED;
        case WLAN_STA_STATE_CONNECTING:
            return WL_STA_CONNECTING;
        case WLAN_STA_STATE_DISCONNECTED:
        default:
            return WL_DISCONNECTED;
    }
}

bool WiFiClass::isConnected() {
    return status() == WL_CONNECTED;
}

String WiFiClass::macAddress() {
    uint8_t mac[6];
    wlan_set_mac_addr(mac, 6);
    char mac_str[18];
    snprintf(mac_str, sizeof(mac_str), "%02X:%02X:%02X:%02X:%02X:%02X",
             mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    return String(mac_str);
}

IPAddress WiFiClass::localIP() {
    // Get IP from netif
    return IPAddress(0,0,0,0);
}

IPAddress WiFiClass::subnetMask() {
    return IPAddress(0,0,0,0);
}

IPAddress WiFiClass::gatewayIP() {
    return IPAddress(0,0,0,0);
}

IPAddress WiFiClass::dnsIP(uint8_t dns_no) {
    return IPAddress(0,0,0,0);
}

String WiFiClass::getBSSID() {
    return String("");
}

int8_t WiFiClass::getChannel() {
    return 0;
}

int8_t WiFiClass::getRSSI() {
    return 0;
}

// ============================================================
// Scan
// ============================================================

int8_t WiFiClass::scanNetworks(bool async, bool show_hidden, bool passive, uint32_t max_ms_per_chan) {
    if (wlan_sta_scan_once() != 0) return 0;
    return scanComplete();
}

int16_t WiFiClass::scanComplete() {
    wlan_sta_scan_results_t results;
    if (wlan_sta_scan_result(&results) != 0) return 0;
    return results.num;
}

void WiFiClass::scanDelete() {
    wlan_sta_bss_flush(0);
}

String WiFiClass::SSID(uint8_t networkItem) {
    wlan_sta_scan_results_t results;
    if (wlan_sta_scan_result(&results) != 0) return String("");
    if (networkItem >= results.num) return String("");
    return String((char*)results.bss[networkItem].ssid);
}

wifi_auth_mode_t WiFiClass::encryptionType(uint8_t networkItem) {
    wlan_sta_scan_results_t results;
    if (wlan_sta_scan_result(&results) != 0) return WIFI_AUTH_OPEN;
    if (networkItem >= results.num) return WIFI_AUTH_OPEN;
    
    // Map XR871 auth mode to WiFi auth mode
    return WIFI_AUTH_OPEN;
}

int32_t WiFiClass::RSSI(uint8_t networkItem) {
    wlan_sta_scan_results_t results;
    if (wlan_sta_scan_result(&results) != 0) return 0;
    if (networkItem >= results.num) return 0;
    return results.bss[networkItem].rssi;
}

uint8_t* WiFiClass::BSSID(uint8_t networkItem) {
    wlan_sta_scan_results_t results;
    if (wlan_sta_scan_result(&results) != 0) return NULL;
    if (networkItem >= results.num) return NULL;
    return results.bss[networkItem].bssid;
}

int32_t WiFiClass::channel(uint8_t networkItem) {
    wlan_sta_scan_results_t results;
    if (wlan_sta_scan_result(&results) != 0) return 0;
    if (networkItem >= results.num) return 0;
    return results.bss[networkItem].channel;
}

bool WiFiClass::getNetworkInfo(uint8_t networkItem, String &ssid, uint8_t &encryptionType, int32_t &RSSI, uint8_t* &BSSID, int32_t &channel) {
    wlan_sta_scan_results_t results;
    if (wlan_sta_scan_result(&results) != 0) return false;
    if (networkItem >= results.num) return false;
    
    ssid = String((char*)results.bss[networkItem].ssid);
    RSSI = results.bss[networkItem].rssi;
    BSSID = results.bss[networkItem].bssid;
    channel = results.bss[networkItem].channel;
    return true;
}

// ============================================================
// Configuration
// ============================================================

bool WiFiClass::config(IPAddress local_ip, IPAddress gateway, IPAddress subnet, IPAddress dns1, IPAddress dns2) {
    return true;
}

bool WiFiClass::setHostname(const char* hostname) {
    return true;
}

const char* WiFiClass::getHostname() {
    return "xr871";
}

uint8_t* WiFiClass::macAddress(uint8_t* mac) {
    wlan_set_mac_addr(mac, 6);
    return mac;
}

bool WiFiClass::setTxPower(wifi_power_t power) {
    return true;
}

wifi_power_t WiFiClass::getTxPower() {
    return WIFI_POWER_15dBm;
}

bool WiFiClass::setSleep(bool enable) {
    return true;
}

bool WiFiClass::getSleep() {
    return false;
}

void WiFiClass::persistent(bool persistent) {}

bool WiFiClass::enableSTA(bool enable) {
    if (enable) return wlan_sta_enable() == 0;
    return wlan_sta_disable() == 0;
}

void WiFiClass::enableAP(bool enable) {}

int WiFiClass::hostByName(const char* aHostname, IPAddress& aResult) {
    return 0;
}

int WiFiClass::hostByName(const char* aHostname, IPAddress& aResult, uint32_t timeout_ms) {
    return 0;
}

// ============================================================
// Event Handling
// ============================================================

wifi_err_reason_t WiFiClass::disconnectReason(system_event_id_t event_id) {
    return WIFI_REASON_UNSPECIFIED;
}

void WiFiClass::onEvent(system_event_cb_t cbEvent, system_event_id_t event) {
    _eventCallback = cbEvent;
}

system_event_cb_t WiFiClass::onEvent(system_event_cb_t cbEvent) {
    _eventCallback = cbEvent;
    return _eventCallback;
}

const char* WiFiClass::statusToString(wl_status_t status) {
    switch (status) {
        case WL_CONNECTED: return "Connected";
        case WL_DISCONNECTED: return "Disconnected";
        case WL_CONNECT_FAILED: return "Connect Failed";
        case WL_CONNECTION_LOST: return "Connection Lost";
        case WL_NO_SSID_AVAIL: return "No SSID Available";
        case WL_IDLE_STATUS: return "Idle";
        case WL_SCAN_COMPLETED: return "Scan Completed";
        case WL_STA_GOT_IP: return "Got IP";
        case WL_STA_CONNECTING: return "Connecting";
        default: return "Unknown";
    }
}

const char* WiFiClass::eventTypeToString(system_event_id_t event_id) {
    switch (event_id) {
        case SYSTEM_EVENT_STA_CONNECTED: return "STA Connected";
        case SYSTEM_EVENT_STA_DISCONNECTED: return "STA Disconnected";
        case SYSTEM_EVENT_STA_GOT_IP: return "STA Got IP";
        case SYSTEM_EVENT_AP_START: return "AP Start";
        case SYSTEM_EVENT_AP_STOP: return "AP Stop";
        case SYSTEM_EVENT_SCAN_DONE: return "Scan Done";
        default: return "Unknown";
    }
}

bool WiFiClass::getAutoConnect() {
    return _autoReconnect;
}

bool WiFiClass::setAutoConnect(bool autoConnect) {
    _autoReconnect = autoConnect;
    return true;
}

bool WiFiClass::getAutoReconnect() {
    return _autoReconnect;
}

bool WiFiClass::setAutoReconnect(bool autoReconnect) {
    _autoReconnect = autoReconnect;
    return true;
}

bool WiFiClass::initSTA(const char* ssid, const char* passphrase, int32_t channel, const uint8_t* bssid) {
    return begin(ssid, passphrase, channel, bssid);
}

bool WiFiClass::initAP(const char* ssid, const char* passphrase, int channel, int ssid_hidden, int max_connection) {
    return softAP(ssid, passphrase, channel, ssid_hidden, max_connection);
}
