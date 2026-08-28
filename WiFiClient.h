/**
 * @file WiFiClient.h
 * @brief ESP32 Compatible WiFiClient for XR871
 * @author Hermes Agent
 * @date 2026-08-27
 */

#ifndef _WIFICLIENT_XR871_H_
#define _WIFICLIENT_XR871_H_

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

class WiFiClient {
public:
    WiFiClient();
    ~WiFiClient();
    
    bool connect(const char* host, uint16_t port);
    bool connect(uint32_t ip, uint16_t port);
    size_t write(uint8_t);
    size_t write(const uint8_t *buf, size_t size);
    int available();
    int read();
    int read(uint8_t *buf, size_t size);
    int peek();
    void flush();
    void stop();
    uint8_t connected();
    bool operator==(const WiFiClient& rhs);
    
    String remoteIP();
    uint16_t remotePort();
    String localIP();
    uint16_t localPort();
    
private:
    bool _connected;
    uint8_t _sock;
};

#endif /* _WIFICLIENT_XR871_H_ */
