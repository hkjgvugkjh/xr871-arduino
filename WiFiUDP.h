/**
 * @file WiFiUDP.h
 * @brief ESP32 Compatible WiFiUDP for XR871
 * @author Hermes Agent
 * @date 2026-08-27
 *
 * Provides ESP32 Arduino WiFiUDP API using XR871 lwIP UDP stack.
 */

#ifndef _WIFI_UDP_XR871_H_
#define _WIFI_UDP_XR871_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "lwip/udp.h"
#include "WiFi.h"

// ============================================================
// WiFiUDP Class
// ============================================================
class WiFiUDP {
public:
    WiFiUDP();
    ~WiFiUDP();
    
    // Initialize UDP
    uint8_t begin(uint16_t port);
    uint8_t beginMulticast(IPAddress interfaceAddr, IPAddress multicast, uint16_t port);
    
    // Stop UDP
    void stop();
    
    // Send data
    int beginPacket(IPAddress ip, uint16_t port);
    int beginPacket(const char* host, uint16_t port);
    int beginPacketMulticast(IPAddress multicast, uint16_t port, IPAddress interfaceAddr, int ttl);
    int endPacket();
    
    // Write data
    size_t write(uint8_t data);
    size_t write(const uint8_t *buffer, size_t size);
    
    // Parse received packet
    int parsePacket();
    
    // Read received data
    int read();
    int read(uint8_t* buffer, size_t size);
    int read(char* buffer, size_t size);
    
    // Peek at next byte
    int peek();
    
    // Flush buffers
    void flush();
    
    // Remote info (from last parsed packet)
    IPAddress remoteIP();
    uint16_t remotePort();
    
    // Local info
    IPAddress localIP();
    uint16_t localPort();
    
    // Available data
    int available();
    
    // Multicast group management
    static uint8_t beginMulticast(IPAddress interfaceAddr, IPAddress multicast);
    static uint8_t joinMulticastGroup(IPAddress interfaceAddr, IPAddress multicast);
    static uint8_t leaveMulticastGroup(IPAddress interfaceAddr, IPAddress multicast);
    
private:
    struct udp_pcb* _pcb;
    bool _initialized;
    uint16_t _port;
    
    // Receive buffer
    struct pbuf* _rxBuffer;
    size_t _rxOffset;
    
    // Current packet info
    IPAddress _remoteIP;
    uint16_t _remotePort;
    
    // Transmit buffer
    struct pbuf* _txBuffer;
    
    // Callback function
    static void _recv(void *arg, struct udp_pcb *upcb, struct pbuf *p, const ip_addr_t *addr, u16_t port);
    
    void recvCallback(struct udp_pcb *upcb, struct pbuf *p, const ip_addr_t *addr, u16_t port);
    void cleanup();
};

#ifdef __cplusplus
}
#endif

#endif /* _WIFI_UDP_XR871_H_ */
