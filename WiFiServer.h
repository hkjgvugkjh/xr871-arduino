/**
 * @file WiFiServer.h
 * @brief ESP32 Compatible WiFiServer for XR871
 */

#ifndef _WIFISERVER_XR871_H_
#define _WIFISERVER_XR871_H_

#include <stdint.h>
#include <stdbool.h>
#include "WiFiClient.h"

class WiFiServer {
public:
    WiFiServer(uint16_t port = 80);
    ~WiFiServer();

    void begin();
    void end();

    WiFiClient available();
    bool hasClient();

private:
    uint16_t _port;
    int _sock;
    bool _listening;
};

#endif /* _WIFISERVER_XR871_H_ */
