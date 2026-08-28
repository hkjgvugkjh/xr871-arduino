/**
 * @file MDNS.h
 * @brief ESP32 Compatible mDNS for XR871
 * @author Hermes Agent
 * @date 2026-08-27
 *
 * Provides mDNS functionality for service discovery.
 * ESP32 API compatible.
 */

#ifndef _MDNS_XR871_H_
#define _MDNS_XR871_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

class MDNS {
public:
    MDNS();
    ~MDNS();
    
    bool begin(const char* hostname);
    void end();
    
    bool addService(const char* service, const char* proto, uint16_t port);
    bool removeService(const char* service, const char* proto);
    
    void update();
    
    // Query services
    int queryService(const char* service, const char* proto);
    String hostname(int idx);
    String service(int idx);
    String protocol(int idx);
    uint16_t port(int idx);
    String IP(int idx);
    
    // Instance name
    void setInstanceName(const char* instance);
    
    // Enable/Disable
    void enableArduino(uint16_t port = 3232, bool auth = false);
    
private:
    bool _running;
    String _hostname;
    String _instanceName;
};

#ifdef __cplusplus
}
#endif

#endif
