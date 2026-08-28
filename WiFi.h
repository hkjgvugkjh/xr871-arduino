/**
 * @file WiFi.h
 * @brief ESP32 Compatible WiFi Library for XR871
 * @author Hermes Agent
 * @date 2026-08-27
 *
 * Provides ESP32 Arduino WiFi API wrapper on top of XR871 wlan driver.
 * Supports STA, AP, and STA+AP modes with TCP/UDP networking.
 *
 * ESP32 API Compatibility:
 *   - WiFi.begin(), WiFi.disconnect(), WiFi.status()
 *   - WiFi.localIP(), WiFi.macAddress(), WiFi.scanNetworks()
 *   - WiFi.mode(WIFI_STA/WIFI_AP/WIFI_AP_STA)
 *   - WiFi.softAP(), WiFi.softAPIP(), WiFi.softAPmacAddress()
 *   - WiFiClient (TCP), WiFiServer (TCP), WiFiUDP (UDP)
 *   - HTTPClient, WebServer (optional)
 */

#ifndef _WIFI_XR871_H_
#define _WIFI_XR871_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>

// ============================================================
// WiFi Mode Definitions
// ============================================================
typedef enum {
    WIFI_MODE_NULL = 0,
    WIFI_MODE_STA,
    WIFI_MODE_AP,
    WIFI_MODE_APSTA,
    WIFI_MODE_MAX
} wifi_mode_t;

typedef enum {
    WIFI_IF_STA = 0,
    WIFI_IF_AP,
} wifi_interface_t;

typedef enum {
    WIFI_AUTH_OPEN = 0,
    WIFI_AUTH_WEP,
    WIFI_AUTH_WPA_PSK,
    WIFI_AUTH_WPA2_PSK,
    WIFI_AUTH_WPA_WPA2_PSK,
    WIFI_AUTH_WPA2_ENTERPRISE,
    WIFI_AUTH_WPA3_PSK,
    WIFI_AUTH_WPA2_WPA3_PSK,
    WIFI_AUTH_MAX
} wifi_auth_mode_t;

typedef enum {
    WIFI_SCAN_FAILED = -1,
} wifi_scan_status_t;

// ============================================================
// WiFi Event Definitions
// ============================================================
typedef enum {
    SYSTEM_EVENT_WIFI_READY = 0,
    SYSTEM_EVENT_SCAN_DONE,
    SYSTEM_EVENT_STA_START,
    SYSTEM_EVENT_STA_STOP,
    SYSTEM_EVENT_STA_CONNECTED,
    SYSTEM_EVENT_STA_DISCONNECTED,
    SYSTEM_EVENT_STA_AUTHMODE_CHANGE,
    SYSTEM_EVENT_STA_GOT_IP,
    SYSTEM_EVENT_STA_LOST_IP,
    SYSTEM_EVENT_STA_WPS_ER_SUCCESS,
    SYSTEM_EVENT_STA_WPS_ER_FAILED,
    SYSTEM_EVENT_STA_WPS_ER_TIMEOUT,
    SYSTEM_EVENT_STA_WPS_ER_PIN,
    SYSTEM_EVENT_STA_WPS_ER_PBC_OVERLAP,
    SYSTEM_EVENT_AP_START,
    SYSTEM_EVENT_AP_STOP,
    SYSTEM_EVENT_AP_STACONNECTED,
    SYSTEM_EVENT_AP_STADISCONNECTED,
    SYSTEM_EVENT_AP_STAIPASSIGNED,
    SYSTEM_EVENT_AP_PROBEREQRECVED,
    SYSTEM_EVENT_GOT_IP6,
    SYSTEM_EVENT_ETH_START,
    SYSTEM_EVENT_ETH_STOP,
    SYSTEM_EVENT_ETH_CONNECTED,
    SYSTEM_EVENT_ETH_DISCONNECTED,
    SYSTEM_EVENT_ETH_GOT_IP,
    SYSTEM_EVENT_MAX
} system_event_id_t;

typedef struct {
    system_event_id_t event_id;
    union {
        struct {
            uint8_t number;
            uint8_t scan_id;
        } scan_done;
        struct {
            uint8_t mac[6];
            uint8_t aid;
        } sta_connected;
        struct {
            uint8_t mac[6];
            uint8_t aid;
        } sta_disconnected;
        struct {
            char if_ip[16];
            uint8_t mac[6];
        } sta_got_ip;
        struct {
            char if_ip[16];
        } ap_staipassigned;
        struct {
            uint8_t ssid[32];
            uint8_t ssid_len;
            uint8_t bssid[6];
            uint8_t channel;
            wifi_auth_mode_t authmode;
        } sta_scan_done;
        struct {
            uint8_t mac[6];
        } ap_sta_connected;
    } event_info;
} system_event_t;

typedef void (*system_event_cb_t)(system_event_t *event);

// ============================================================
// IP Address Type (ESP32 Compatible)
// ============================================================
class IPAddress {
private:
    union {
        uint8_t bytes[4];
        uint32_t dword;
    } _address;

public:
    IPAddress();
    IPAddress(uint8_t first_octet, uint8_t second_octet, uint8_t third_octet, uint8_t fourth_octet);
    IPAddress(uint32_t address);
    IPAddress(const uint8_t *address);

    operator uint32_t() const;
    bool operator==(const IPAddress& addr) const;
    bool operator==(const uint8_t* addr) const;
    uint8_t operator[](int index) const;
    uint8_t& operator[](int index);

    IPAddress& operator=(uint32_t address);
    IPAddress& operator=(const uint8_t *address);
    IPAddress& operator+=(uint32_t address);

    uint8_t* raw_address();
    String toString() const;

    friend class WiFiClient;
    friend class WiFiUDP;
};

// ============================================================
// WiFi Class
// ============================================================
class WiFiClass {
public:
    // Mode control
    bool mode(wifi_mode_t mode);
    wifi_mode_t getMode();

    // STA control
    bool begin(const char* ssid, const char* passphrase = NULL, int32_t channel = 0, const uint8_t* bssid = NULL);
    bool begin(char* ssid, char* passphrase = NULL, int32_t channel = 0, const uint8_t* bssid = NULL);
    bool begin();
    bool disconnect(bool wifioff = false, bool eraseap = false);
    bool config(IPAddress local_ip, IPAddress gateway, IPAddress subnet, 
                IPAddress dns1 = (uint32_t)0x00000000, IPAddress dns2 = (uint32_t)0x00000000);

    // STA status
    wl_status_t status();
    bool isConnected();
    String macAddress();
    IPAddress localIP();
    IPAddress subnetMask();
    IPAddress gatewayIP();
    IPAddress dnsIP(uint8_t dns_no = 0);
    String getBSSID();
    int8_t getChannel();
    int8_t getRSSI();
    bool getAutoConnect();
    bool setAutoConnect(bool autoConnect);
    bool getAutoReconnect();
    bool setAutoReconnect(bool autoReconnect);
    bool setHostname(const char* hostname);
    uint8_t* macAddress(uint8_t* mac);
    const char* getHostname();

    // AP control
    bool softAP(const char* ssid, const char* passphrase = NULL, int channel = 1, int ssid_hidden = 0, int max_connection = 4);
    bool softAPconfig(IPAddress local_ip, IPAddress gateway, IPAddress subnet);
    bool softAPdisconnect(bool wifioff = false);
    String softAPmacAddress();
    uint8_t softAPgetStationNum();
    IPAddress softAPIP();
    String softAPSSID() const;

    // Scan
    int8_t scanNetworks(bool async = false, bool show_hidden = false, bool passive = false, uint32_t max_ms_per_chan = 300);
    int16_t scanComplete();
    void scanDelete();
    String SSID(uint8_t networkItem);
    wifi_auth_mode_t encryptionType(uint8_t networkItem);
    int32_t RSSI(uint8_t networkItem);
    uint8_t* BSSID(uint8_t networkItem);
    int32_t channel(uint8_t networkItem);
    bool getNetworkInfo(uint8_t networkItem, String &ssid, uint8_t &encryptionType, int32_t &RSSI, uint8_t* &BSSID, int32_t &channel);

    // Event
    wifi_err_reason_t disconnectReason(system_event_id_t event_id);
    void onEvent(system_event_cb_t cbEvent, system_event_id_t event = SYSTEM_EVENT_MAX);
    system_event_cb_t onEvent(system_event_cb_t cbEvent);

    // Power
    bool setTxPower(wifi_power_t power);
    wifi_power_t getTxPower();

    // Sleep
    bool setSleep(bool enable);
    bool getSleep();

    // Misc
    static void persistent(bool persistent);
    static bool enableSTA(bool enable);
    static void enableAP(bool enable);
    static int hostByName(const char* aHostname, IPAddress& aResult);
    static int hostByName(const char* aHostname, IPAddress& aResult, uint32_t timeout_ms);

    // Status codes
    static const char* statusToString(wl_status_t status);
    static const char* eventTypeToString(system_event_id_t event_id);

private:
    wifi_mode_t _mode;
    bool _autoReconnect;
    system_event_cb_t _eventCallback;

    bool initSTA(const char* ssid, const char* passphrase, int32_t channel, const uint8_t* bssid);
    bool initAP(const char* ssid, const char* passphrase, int channel, int ssid_hidden, int max_connection);
};

extern WiFiClass WiFi;

// ============================================================
// Status Codes
// ============================================================
typedef enum {
    WL_NO_SHIELD = 255,
    WL_IDLE_STATUS = 0,
    WL_NO_SSID_AVAIL,
    WL_SCAN_COMPLETED,
    WL_CONNECTED,
    WL_CONNECT_FAILED,
    WL_CONNECTION_LOST,
    WL_DISCONNECTED,
    WL_AP_LISTENING,
    WL_AP_CONNECTED,
    WL_AP_FAILED,
    WL_AP_SUCCESS,
    WL_STA_GOT_IP,
    WL_STA_CONNECTING,
    WL_STA_DISCONNECTED
} wl_status_t;

typedef enum {
    WIFI_REASON_UNSPECIFIED = 1,
    WIFI_REASON_AUTH_EXPIRE,
    WIFI_REASON_AUTH_LEAVE,
    WIFI_REASON_ASSOC_EXPIRE,
    WIFI_REASON_ASSOC_TOOMANY,
    WIFI_REASON_NOT_AUTHED,
    WIFI_REASON_NOT_ASSOCED,
    WIFI_REASON_ASSOC_LEAVE,
    WIFI_REASON_ASSOC_NOT_AUTHED,
    WIFI_REASON_DISASSOC_PWRCAP_BAD,
    WIFI_REASON_DISASSOC_SUPCHAN_BAD,
    WIFI_REASON_IE_INVALID,
    WIFI_REASON_MIC_FAILURE,
    WIFI_REASON_4WAY_HANDSHAKE_TIMEOUT,
    WIFI_REASON_GROUP_KEY_UPDATE_TIMEOUT,
    WIFI_REASON_IE_IN_4WAY_DIFFERS,
    WIFI_REASON_GROUP_CIPHER_INVALID,
    WIFI_REASON_PAIRWISE_CIPHER_INVALID,
    WIFI_REASON_AKMP_INVALID,
    WIFI_REASON_UNSUPP_RSN_IE_VERSION,
    WIFI_REASON_INVALID_RSN_IE_CAP,
    WIFI_REASON_802_1X_AUTH_FAILED,
    WIFI_REASON_CIPHER_SUITE_REJECTED,
    WIFI_REASON_BEACON_TIMEOUT = 200,
    WIFI_REASON_NO_AP_FOUND,
    WIFI_REASON_AUTH_FAIL,
    WIFI_REASON_ASSOC_FAIL,
    WIFI_REASON_HANDSHAKE_TIMEOUT,
} wifi_err_reason_t;

// ============================================================
// TX Power Levels
// ============================================================
typedef enum {
    WIFI_POWER_19_5dBm = 78,// 19.5dBm
    WIFI_POWER_19dBm = 76,// 19dBm
    WIFI_POWER_18_5dBm = 74,// 18.5dBm
    WIFI_POWER_17dBm = 68,// 17dBm
    WIFI_POWER_15dBm = 60,// 15dBm
    WIFI_POWER_13dBm = 52,// 13dBm
    WIFI_POWER_11dBm = 44,// 11dBm
    WIFI_POWER_8_5dBm = 34,// 8.5dBm
    WIFI_POWER_7dBm = 28,// 7dBm
    WIFI_POWER_5dBm = 20,// 5dBm
    WIFI_POWER_2dBm = 8,// 2dBm
    WIFI_POWER_MINUS_1dBm = -4// -1dBm
} wifi_power_t;

#ifdef __cplusplus
}
#endif

#endif /* _WIFI_XR871_H_ */
