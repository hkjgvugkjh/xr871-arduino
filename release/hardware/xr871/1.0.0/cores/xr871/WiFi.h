// Minimal WiFi stub - no duplicate overloads
#ifndef _WIFI_XR871_H_
#define _WIFI_XR871_H_
#include <stdint.h>
typedef int wl_status_t;
typedef int wifi_mode_t;
typedef int wifi_power_t;
class WiFiClass {
public:
    void mode(wifi_mode_t) {}
    int begin(const char*, const char*) { return 0; }
    int begin(const char*) { return 0; }
    int disconnect() { return 0; }
    wl_status_t status() { return 0; }
    void setAutoConnect(bool) {}
    void setAutoReconnect(bool) {}
    int scanNetworks() { return 0; }
    void scanDelete() {}
    const char* SSID(uint8_t) { return ""; }
    int32_t RSSI(uint8_t) { return 0; }
    uint8_t* macAddress(uint8_t*) { return 0; }
    void macAddress(const uint8_t*) {}
    uint8_t channel(uint8_t) { return 0; }
    const char* psk(uint8_t) { return ""; }
    const char* BSSIDstr(uint8_t) { return ""; }
    int32_t RSSI() { return 0; }
    void setHostname(const char*) {}
    const char* getHostname() { return ""; }
    void setSleep(bool) {}
    bool getSleep() { return false; }
    void setTxPower(wifi_power_t) {}
    wifi_power_t getTxPower() { return 0; }
    void enableSTA(bool) {}
    void enableAP(bool) {}
    int softAP(const char*, const char*, int=1, int=0, int=4) { return 0; }
    int softAPConfig(uint32_t, uint32_t, uint32_t) { return 0; }
    int softAPdisconnect(bool) { return 0; }
    uint32_t softAPIP() { return 0; }
    uint8_t softAPStationNum() { return 0; }
    void enableIpV6() {}
    uint32_t localIP() { return 0; }
    uint32_t subnetMask() { return 0; }
    uint32_t gatewayIP() { return 0; }
    uint32_t dnsIP(uint8_t=0) { return 0; }
    const char* SSID() { return ""; }
    uint8_t* BSSID() { return 0; }
    uint8_t encryptionType() { return 0; }
    void setDNS(uint32_t, uint32_t=0) {}
};
extern WiFiClass WiFi;
#endif
