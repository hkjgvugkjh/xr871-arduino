/**
 * @file WiFi.h
 * @brief ESP32 Compatible WiFi Library for XR871
 * @author Hermes Agent
 * @date 2026-08-28
 *
 * Provides ESP32 Arduino WiFi API wrapper on top of XR871 wlan driver.
 * Supports STA, AP, and STA+AP modes with TCP/UDP networking.
 */

#ifndef _WIFI_XR871_H_
#define _WIFI_XR871_H_

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

// Include XR871 wlan driver
#include "net/wlan/wlan.h"
#include "net/wlan/wlan_defs.h"
#include "net/wlan/ethernetif.h"

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
    WIFI_POWER_19_5dBm = 78,
    WIFI_POWER_19dBm = 76,
    WIFI_POWER_18_5dBm = 74,
    WIFI_POWER_17dBm = 68,
    WIFI_POWER_15dBm = 60,
    WIFI_POWER_13dBm = 52,
    WIFI_POWER_11dBm = 44,
    WIFI_POWER_8_5dBm = 34,
    WIFI_POWER_7dBm = 28,
    WIFI_POWER_5dBm = 20,
    WIFI_POWER_2dBm = 8,
    WIFI_POWER_MINUS_1dBm = -4
} wifi_power_t;

#ifdef __cplusplus
}
#endif

#endif /* _WIFI_XR871_H_ */
