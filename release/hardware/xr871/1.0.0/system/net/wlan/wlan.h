#ifndef _NET_WLAN_WLAN_H_
#define _NET_WLAN_WLAN_H_
#include <stdint.h>
typedef enum { WLAN_MODE_STA = 0, WLAN_MODE_AP = 1, WLAN_MODE_AP_STA = 2 } WLAN_Mode;
typedef enum { WLAN_STATUS_DISCONNECTED = 0, WLAN_STATUS_CONNECTED = 1 } WLAN_Status;
typedef struct { char ssid[32]; char password[64]; } WLAN_Config;
static inline int WLAN_Init(WLAN_Mode mode, WLAN_Config *config) { return 0; }
static inline int WLAN_DeInit(void) { return 0; }
static inline int WLAN_Connect(void) { return 0; }
static inline int WLAN_Disconnect(void) { return 0; }
static inline WLAN_Status WLAN_GetStatus(void) { return WLAN_STATUS_DISCONNECTED; }
static inline int WLAN_GetRSSI(void) { return -50; }
static inline int WLAN_Scan(void) { return 0; }
#endif
