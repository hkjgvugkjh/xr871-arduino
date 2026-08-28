/**
 * @file WiFiClient.h
 * @brief ESP32 Compatible WiFiClient (TCP) for XR871
 * @author Hermes Agent
 * @date 2026-08-27
 *
 * Provides ESP32 Arduino WiFiClient API using XR871 lwIP TCP stack.
 */

#ifndef _WIFI_CLIENT_XR871_H_
#define _WIFI_CLIENT_XR871_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "lwip/tcp.h"

// ============================================================
// WiFiClient Class
// ============================================================
class WiFiClient {
public:
    WiFiClient();
    ~WiFiClient();
    
    // Copy constructor
    WiFiClient(const WiFiClient& other);
    WiFiClient& operator=(const WiFiClient& other);
    
    // Connection
    int connect(IPAddress ip, uint16_t port);
    int connect(const char* host, uint16_t port);
    int connect(IPAddress ip, uint16_t port, int32_t timeout);
    
    // Write
    size_t write(uint8_t data);
    size_t write(const uint8_t *buf, size_t size);
    size_t write(const char* str);
    size_t print(const char* str);
    size_t println(const char* str);
    size_t print(int val, int base = DEC);
    size_t println(int val, int base = DEC);
    size_t printFloat(double val, int format = 2);
    size_t printlnFloat(double val, int format = 2);
    
    // Read
    int read();
    int read(uint8_t *buf, size_t size);
    int peek();
    int available();
    void flush();
    
    // Stop
    void stop();
    
    // Status
    uint8_t connected();
    operator bool();
    bool operator==(const WiFiClient& other);
    
    // Remote info
    IPAddress remoteIP();
    uint16_t remotePort();
    
    // Local info
    IPAddress localIP();
    uint16_t localPort();
    
    // Set timeout
    void setNoDelay(bool nodelay);
    bool getNoDelay();
    void setTimeout(uint32_t timeout);
    
    // Get internal PCB
    struct tcp_pcb* getPCB() { return _pcb; }
    
    // Accept connection (for WiFiServer)
    WiFiClient accept();
    
    // Status
    static const int TCP_WRITE_FLAG_COPY = 0x01;
    static const int TCP_WRITE_FLAG_MORE = 0x02;
    
private:
    struct tcp_pcb* _pcb;
    bool _connected;
    uint32_t _timeout;
    bool _nodelay;
    
    // Buffer for received data
    struct pbuf* _rxBuffer;
    size_t _rxOffset;
    
    // Callback functions
    static err_t _recv(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err);
    static err_t _sent(void *arg, struct tcp_pcb *tpcb, u16_t len);
    static err_t _poll(void *arg, struct tcp_pcb *tpcb);
    static void _err(void *arg, err_t err);
    
    void init();
    void cleanup();
    err_t recvCallback(struct tcp_pcb *tpcb, struct pbuf *p, err_t err);
    err_t sentCallback(struct tcp_pcb *tpcb, u16_t len);
    err_t pollCallback(struct tcp_pcb *tpcb);
    void errCallback(err_t err);
};

// ============================================================
// WiFiServer Class
// ============================================================
class WiFiServer {
public:
    WiFiServer(uint16_t port);
    ~WiFiServer();
    
    // Start listening
    void begin();
    void begin(uint16_t port);
    void end();
    
    // Accept connection
    WiFiClient available();
    
    // Status
    bool hasClient();
    uint16_t getPort() { return _port; }
    
    // Set timeout
    void setTimeout(uint32_t timeout);
    
private:
    uint16_t port;
    struct tcp_pcb* _listenPcb;
    bool _listening;
    uint32_t _timeout;
    
    // Pending connections
    WiFiClient* _pendingClient;
    
    // Callback functions
    static err_t _accept(void *arg, struct tcp_pcb *newpcb, err_t err);
    
    err_t acceptCallback(struct tcp_pcb *newpcb, err_t err);
};

#ifdef __cplusplus
}
#endif

#endif /* _WIFI_CLIENT_XR871_H_ */
