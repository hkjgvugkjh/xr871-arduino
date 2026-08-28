/**
 * @file WiFi.cpp
 * @brief ESP32 Compatible WiFi Implementation for XR871
 * @author Hermes Agent
 * @date 2026-08-27
 *
 * Implements ESP32 Arduino WiFi API using XR871 wlan driver.
 */

#include "WiFi.h"
#include "Arduino.h"
#include <string.h>

// XR871 includes
#include "net/wlan/wlan.h"
#include "net/wlan/wlan_defs.h"
#include "lwip/netif.h"
#include "lwip/ip_addr.h"
#include "lwip/dns.h"

// Global WiFi object
WiFiClass WiFi;

// ============================================================
// IPAddress Implementation
// ============================================================

IPAddress::IPAddress() {
    _address.dword = 0;
}

IPAddress::IPAddress(uint8_t first_octet, uint8_t second_octet, uint8_t third_octet, uint8_t fourth_octet) {
    _address.bytes[0] = first_octet;
    _address.bytes[1] = second_octet;
    _address.bytes[2] = third_octet;
    _address.bytes[3] = fourth_octet;
}

IPAddress::IPAddress(uint32_t address) {
    _address.dword = address;
}

IPAddress::IPAddress(const uint8_t *address) {
    memcpy(_address.bytes, address, sizeof(_address.bytes));
}

IPAddress::operator uint32_t() const {
    return _address.dword;
}

bool IPAddress::operator==(const IPAddress& addr) const {
    return _address.dword == addr._address.dword;
}

bool IPAddress::operator==(const uint8_t* addr) const {
    return memcmp(_address.bytes, addr, sizeof(_address.bytes)) == 0;
}

uint8_t IPAddress::operator[](int index) const {
    return _address.bytes[index];
}

uint8_t& IPAddress::operator[](int index) {
    return _address.bytes[index];
}

IPAddress& IPAddress::operator=(uint32_t address) {
    _address.dword = address;
    return *this;
}

IPAddress& IPAddress::operator=(const uint8_t *address) {
    memcpy(_address.bytes, address, sizeof(_address.bytes));
    return *this;
}

IPAddress& IPAddress::operator+=(uint32_t address) {
    _address.dword += address;
    return *this;
}

uint8_t* IPAddress::raw_address() {
    return _address.bytes;
}

String IPAddress::toString() const {
    char buf[16];
    snprintf(buf, sizeof(buf), "%d.%d.%d.%d",
             _address.bytes[0], _address.bytes[1],
             _address.bytes[2], _address.bytes[3]);
    return String(buf);
}

// ============================================================
// WiFiClass Implementation
// ============================================================

WiFiClass::WiFiClass() {
    _mode = WIFI_MODE_NULL;
    _autoReconnect = true;
    _eventCallback = NULL;
}

// Mode control
bool WiFiClass::mode(wifi_mode_t mode) {
    if (mode == _mode) return true;
    
    // Stop current mode
    if (_mode == WIFI_MODE_STA || _mode == WIFI_MODE_APSTA) {
        wlan_sta_disable();
    }
    if (_mode == WIFI_MODE_AP || _mode == WIFI_MODE_APSTA) {
        wlan_ap_disable();
    }
    
    _mode = mode;
    return true;
}

wifi_mode_t WiFiClass::getMode() {
    return _mode;
}

// STA control
bool WiFiClass::begin(const char* ssid, const char* passphrase, int32_t channel, const uint8_t* bssid) {
    if (_mode == WIFI_MODE_NULL) {
        mode(WIFI_MODE_STA);
    }
    
    return initSTA(ssid, passphrase, channel, bssid);
}

bool WiFiClass::begin(char* ssid, char* passphrase, int32_t channel, const uint8_t* bssid) {
    return begin((const char*)ssid, (const char*)passphrase, channel, bssid);
}

bool WiFiClass::begin() {
    // Auto-connect with saved credentials
    return wlan_sta_connect() == 0;
}

bool WiFiClass::disconnect(bool wifioff, bool eraseap) {
    int ret = wlan_sta_disconnect();
    if (wifioff) {
        wlan_sta_disable();
    }
    return ret == 0;
}

bool WiFiClass::config(IPAddress local_ip, IPAddress gateway, IPAddress subnet, IPAddress dns1, IPAddress dns2) {
    struct netif *netif = wlan_if_create(WLAN_MODE_STA);
    if (!netif) return false;
    
    ip_addr_t ip, gw, sn, d1, d2;
    
    ip.addr = local_ip;
    gw.addr = gateway;
    sn.addr = subnet;
    
    netif_set_ipaddr(netif, &ip);
    netif_set_gw(netif, &gw);
    netif_set_netmask(netif, &sn);
    
    if (dns1 != 0) {
        d1.addr = dns1;
        dns_setserver(0, &d1);
    }
    if (dns2 != 0) {
        d2.addr = dns2;
        dns_setserver(1, &d2);
    }
    
    wlan_if_delete(netif);
    return true;
}

// STA status
wl_status_t WiFiClass::status() {
    wlan_sta_states_t state;
    if (wlan_sta_state(&state) != 0) {
        return WL_DISCONNECTED;
    }
    
    if (state == WLAN_STA_STATE_CONNECTED) {
        return WL_CONNECTED;
    }
    return WL_DISCONNECTED;
}

bool WiFiClass::isConnected() {
    return status() == WL_CONNECTED;
}

String WiFiClass::macAddress() {
    uint8_t mac[6];
    wlan_get_mac_addr(mac, 6);
    char buf[18];
    snprintf(buf, sizeof(buf), "%02X:%02X:%02X:%02X:%02X:%02X",
             mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    return String(buf);
}

IPAddress WiFiClass::localIP() {
    struct netif *netif = wlan_if_create(WIFI_MODE_STA);
    if (!netif) return IPAddress(0, 0, 0, 0);
    
    IPAddress ip(netif->ip_addr.addr);
    wlan_if_delete(netif);
    return ip;
}

IPAddress WiFiClass::subnetMask() {
    struct netif *netif = wlan_if_create(WIFI_MODE_STA);
    if (!netif) return IPAddress(0, 0, 0, 0);
    
    IPAddress mask(netif->netmask.addr);
    wlan_if_delete(netif);
    return mask;
}

IPAddress WiFiClass::gatewayIP() {
    struct netif *netif = wlan_if_create(WIFI_MODE_STA);
    if (!netif) return IPAddress(0, 0, 0, 0);
    
    IPAddress gw(netif->gw.addr);
    wlan_if_delete(netif);
    return gw;
}

IPAddress WiFiClass::dnsIP(uint8_t dns_no) {
    const ip_addr_t *dns = dns_getserver(dns_no);
    if (!dns) return IPAddress(0, 0, 0, 0);
    return IPAddress(dns->addr);
}

String WiFiClass::getBSSID() {
    wlan_sta_ap_t ap;
    if (wlan_sta_ap_info(&ap) != 0) return String();
    
    char buf[18];
    snprintf(buf, sizeof(buf), "%02X:%02X:%02X:%02X:%02X:%02X",
             ap.bssid[0], ap.bssid[1], ap.bssid[2],
             ap.bssid[3], ap.bssid[4], ap.bssid[5]);
    return String(buf);
}

int8_t WiFiClass::getChannel() {
    wlan_sta_ap_t ap;
    if (wlan_sta_ap_info(&ap) != 0) return -1;
    return ap.channel;
}

int8_t WiFiClass::getRSSI() {
    wlan_sta_ap_t ap;
    if (wlan_sta_ap_info(&ap) != 0) return 0;
    return ap.rssi;
}

bool WiFiClass::getAutoConnect() {
    return _autoReconnect;
}

bool WiFiClass::setAutoConnect(bool autoConnect) {
    _autoReconnect = autoConnect;
    return wlan_sta_set_autoconnect(autoConnect ? 1 : 0) == 0;
}

bool WiFiClass::getAutoReconnect() {
    return _autoReconnect;
}

bool WiFiClass::setAutoReconnect(bool autoReconnect) {
    _autoReconnect = autoReconnect;
    return true;
}

bool WiFiClass::setHostname(const char* hostname) {
    // TODO: Implement hostname setting
    return true;
}

uint8_t* WiFiClass::macAddress(uint8_t* mac) {
    wlan_get_mac_addr(mac, 6);
    return mac;
}

const char* WiFiClass::getHostname() {
    return "XR871";
}

// AP control
bool WiFiClass::softAP(const char* ssid, const char* passphrase, int channel, int ssid_hidden, int max_connection) {
    if (_mode == WIFI_MODE_NULL) {
        mode(WIFI_MODE_AP);
    }
    
    return initAP(ssid, passphrase, channel, ssid_hidden, max_connection);
}

bool WiFiClass::softAPconfig(IPAddress local_ip, IPAddress gateway, IPAddress subnet) {
    struct netif *netif = wlan_if_create(WLAN_MODE_AP);
    if (!netif) return false;
    
    ip_addr_t ip, gw, sn;
    ip.addr = local_ip;
    gw.addr = gateway;
    sn.addr = subnet;
    
    netif_set_ipaddr(netif, &ip);
    netif_set_gw(netif, &gw);
    netif_set_netmask(netif, &sn);
    
    wlan_if_delete(netif);
    return true;
}

bool WiFiClass::softAPdisconnect(bool wifioff) {
    int ret = wlan_ap_disable();
    if (wifioff) {
        mode(WIFI_MODE_NULL);
    }
    return ret == 0;
}

String WiFiClass::softAPmacAddress() {
    return macAddress();
}

uint8_t WiFiClass::softAPgetStationNum() {
    int num = 0;
    wlan_ap_sta_num(&num);
    return num;
}

IPAddress WiFiClass::softAPIP() {
    struct netif *netif = wlan_if_create(WIFI_MODE_AP);
    if (!netif) return IPAddress(0, 0, 0, 0);
    
    IPAddress ip(netif->ip_addr.addr);
    wlan_if_delete(netif);
    return ip;
}

String WiFiClass::softAPSSID() const {
    wlan_ap_config_t config;
    if (wlan_ap_get_config(&config) != 0) return String();
    return String((char*)config.u.ssid.ssid);
}

// Scan
int8_t WiFiClass::scanNetworks(bool async, bool show_hidden, bool passive, uint32_t max_ms_per_chan) {
    wlan_sta_scan_param_t param;
    param.scan_only = 0;
    param.scan_passive = passive ? 1 : 0;
    param.scan_ssid = 0;
    
    if (wlan_sta_scan(&param) != 0) {
        return WIFI_SCAN_FAILED;
    }
    
    // Wait for scan complete
    delay(1000);
    
    // Get results
    wlan_sta_scan_results_t results;
    wlan_sta_ap_t aps[20];
    results.ap = aps;
    results.size = 20;
    results.num = 0;
    
    if (wlan_sta_scan_result(&results) != 0) {
        return WIFI_SCAN_FAILED;
    }
    
    return results.num;
}

int16_t WiFiClass::scanComplete() {
    return WL_SCAN_COMPLETED;
}

void WiFiClass::scanDelete() {
    // Clear scan results
}

String WiFiClass::SSID(uint8_t networkItem) {
    wlan_sta_scan_results_t results;
    wlan_sta_ap_t aps[20];
    results.ap = aps;
    results.size = 20;
    results.num = 0;
    
    if (wlan_sta_scan_result(&results) != 0 || networkItem >= results.num) {
        return String();
    }
    
    return String((char*)results.ap[networkItem].ssid.ssid);
}

wifi_auth_mode_t WiFiClass::encryptionType(uint8_t networkItem) {
    wlan_sta_scan_results_t results;
    wlan_sta_ap_t aps[20];
    results.ap = aps;
    results.size = 20;
    results.num = 0;
    
    if (wlan_sta_scan_result(&results) != 0 || networkItem >= results.num) {
        return WIFI_AUTH_OPEN;
    }
    
    // Map XR871 auth to ESP32 auth
    if (results.ap[networkItem].wpa_flags & WPA_FLAGS_WPA2) {
        return WIFI_AUTH_WPA2_PSK;
    }
    if (results.ap[networkItem].wpa_flags & WPA_FLAGS_WPA) {
        return WIFI_AUTH_WPA_PSK;
    }
    if (results.ap[networkItem].wpa_flags & WPA_FLAGS_WEP) {
        return WIFI_AUTH_WEP;
    }
    return WIFI_AUTH_OPEN;
}

int32_t WiFiClass::RSSI(uint8_t networkItem) {
    wlan_sta_scan_results_t results;
    wlan_sta_ap_t aps[20];
    results.ap = aps;
    results.size = 20;
    results.num = 0;
    
    if (wlan_sta_scan_result(&results) != 0 || networkItem >= results.num) {
        return 0;
    }
    
    return results.ap[networkItem].rssi;
}

uint8_t* WiFiClass::BSSID(uint8_t networkItem) {
    wlan_sta_scan_results_t results;
    wlan_sta_ap_t aps[20];
    results.ap = aps;
    results.size = 20;
    results.num = 0;
    
    if (wlan_sta_scan_result(&results) != 0 || networkItem >= results.num) {
        return NULL;
    }
    
    return results.ap[networkItem].bssid;
}

int32_t WiFiClass::channel(uint8_t networkItem) {
    wlan_sta_scan_results_t results;
    wlan_sta_ap_t aps[20];
    results.ap = aps;
    results.size = 20;
    results.num = 0;
    
    if (wlan_sta_scan_result(&results) != 0 || networkItem >= results.num) {
        return 0;
    }
    
    return results.ap[networkItem].channel;
}

bool WiFiClass::getNetworkInfo(uint8_t networkItem, String &ssid, uint8_t &encryptionType, int32_t &RSSI, uint8_t* &BSSID, int32_t &channel) {
    wlan_sta_scan_results_t results;
    wlan_sta_ap_t aps[20];
    results.ap = aps;
    results.size = 20;
    results.num = 0;
    
    if (wlan_sta_scan_result(&results) != 0 || networkItem >= results.num) {
        return false;
    }
    
    ssid = String((char*)results.ap[networkItem].ssid.ssid);
    encryptionType = (uint8_t)this->encryptionType(networkItem);
    RSSI = results.ap[networkItem].rssi;
    BSSID = results.ap[networkItem].bssid;
    channel = results.ap[networkItem].channel;
    
    return true;
}

// Event
wifi_err_reason_t WiFiClass::disconnectReason(system_event_id_t event_id) {
    switch (event_id) {
        case SYSTEM_EVENT_STA_DISCONNECTED:
            return WIFI_REASON_UNSPECIFIED;
        default:
            return WIFI_REASON_UNSPECIFIED;
    }
}

void WiFiClass::onEvent(system_event_cb_t cbEvent, system_event_id_t event) {
    _eventCallback = cbEvent;
}

system_event_cb_t WiFiClass::onEvent(system_event_cb_t cbEvent) {
    _eventCallback = cbEvent;
    return _eventCallback;
}

// Power
bool WiFiClass::setTxPower(wifi_power_t power) {
    // TODO: Implement TX power setting
    return true;
}

wifi_power_t WiFiClass::getTxPower() {
    return WIFI_POWER_19_5dBm;
}

// Sleep
bool WiFiClass::setSleep(bool enable) {
    return wlan_set_ps_mode(NULL, enable ? 1 : 0) == 0;
}

bool WiFiClass::getSleep() {
    return false;
}

// Misc
void WiFiClass::persistent(bool persistent) {
    // TODO
}

bool WiFiClass::enableSTA(bool enable) {
    if (enable) {
        return wlan_sta_enable() == 0;
    } else {
        return wlan_sta_disable() == 0;
    }
}

void WiFiClass::enableAP(bool enable) {
    if (enable) {
        wlan_ap_enable();
    } else {
        wlan_ap_disable();
    }
}

int WiFiClass::hostByName(const char* aHostname, IPAddress& aResult) {
    return hostByName(aHostname, aResult, 5000);
}

int WiFiClass::hostByName(const char* aHostname, IPAddress& aResult, uint32_t timeout_ms) {
    ip_addr_t addr;
    err_t err = dns_gethostbyname(aHostname, &addr, NULL, NULL);
    if (err == ERR_OK) {
        aResult = IPAddress(addr.addr);
        return 1;
    }
    return 0;
}

// Status codes
const char* WiFiClass::statusToString(wl_status_t status) {
    switch (status) {
        case WL_NO_SHIELD: return "No Shield";
        case WL_IDLE_STATUS: return "Idle";
        case WL_NO_SSID_AVAIL: return "No SSID Available";
        case WL_SCAN_COMPLETED: return "Scan Completed";
        case WL_CONNECTED: return "Connected";
        case WL_CONNECT_FAILED: return "Connect Failed";
        case WL_CONNECTION_LOST: return "Connection Lost";
        case WL_DISCONNECTED: return "Disconnected";
        case WL_AP_LISTENING: return "AP Listening";
        case WL_AP_CONNECTED: return "AP Connected";
        case WL_AP_FAILED: return "AP Failed";
        case WL_AP_SUCCESS: return "AP Success";
        case WL_STA_GOT_IP: return "STA Got IP";
        case WL_STA_CONNECTING: return "STA Connecting";
        case WL_STA_DISCONNECTED: return "STA Disconnected";
        default: return "Unknown";
    }
}

const char* WiFiClass::eventTypeToString(system_event_id_t event_id) {
    switch (event_id) {
        case SYSTEM_EVENT_WIFI_READY: return "WiFi Ready";
        case SYSTEM_EVENT_SCAN_DONE: return "Scan Done";
        case SYSTEM_EVENT_STA_START: return "STA Start";
        case SYSTEM_EVENT_STA_STOP: return "STA Stop";
        case SYSTEM_EVENT_STA_CONNECTED: return "STA Connected";
        case SYSTEM_EVENT_STA_DISCONNECTED: return "STA Disconnected";
        case SYSTEM_EVENT_STA_GOT_IP: return "STA Got IP";
        case SYSTEM_EVENT_AP_START: return "AP Start";
        case SYSTEM_EVENT_AP_STOP: return "AP Stop";
        case SYSTEM_EVENT_AP_STACONNECTED: return "AP STA Connected";
        case SYSTEM_EVENT_AP_STADISCONNECTED: return "AP STA Disconnected";
        default: return "Unknown";
    }
}

// Private functions
bool WiFiClass::initSTA(const char* ssid, const char* passphrase, int32_t channel, const uint8_t* bssid) {
    // Set SSID and PSK
    if (wlan_sta_set((uint8_t*)ssid, strlen((char*)ssid), (uint8_t*)passphrase) != 0) {
        return false;
    }
    
    // Enable STA
    if (wlan_sta_enable() != 0) {
        return false;
    }
    
    // Connect
    if (wlan_sta_connect() != 0) {
        return false;
    }
    
    return true;
}

bool WiFiClass::initAP(const char* ssid, const char* passphrase, int channel, int ssid_hidden, int max_connection) {
    // Set AP config
    wlan_ap_config_t config;
    memset(&config, 0, sizeof(config));
    
    config.field = WLAN_AP_FIELD_SSID;
    memcpy(config.u.ssid.ssid, ssid, strlen(ssid));
    config.u.ssid.ssid_len = strlen(ssid);
    wlan_ap_set_config(&config);
    
    if (passphrase && strlen(passphrase) > 0) {
        config.field = WLAN_AP_FIELD_PSK;
        memcpy(config.u.psk, passphrase, strlen(passphrase));
        wlan_ap_set_config(&config);
    }
    
    config.field = WLAN_AP_FIELD_CHANNEL;
    config.u.channel = channel;
    wlan_ap_set_config(&config);
    
    // Enable AP
    return wlan_ap_enable() == 0;
}
