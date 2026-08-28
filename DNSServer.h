/**
 * @file DNSServer.h
 * @brief ESP32 Compatible DNSServer for XR871
 * @author Hermes Agent
 * @date 2026-08-27
 *
 * Provides captive portal DNS server functionality.
 */

#ifndef _DNSSERVER_XR871_H_
#define _DNSSERVER_XR871_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

class DNSServer {
public:
    DNSServer();
    ~DNSServer();
    
    bool start(const char* domain, const char* ip);
    bool isRunning();
    void stop();
    void processNextRequest();
    
private:
    bool _running;
    uint16_t _port;
};

#ifdef __cplusplus
}
#endif

#endif
