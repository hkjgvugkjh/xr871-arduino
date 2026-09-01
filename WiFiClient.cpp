/**
 * @file WiFiClient.cpp
 * @brief WiFiClient implementation for XR871 using lwIP sockets
 */

#include "Arduino.h"
#include "WiFiClient.h"
#include "net/lwip-2.0.3/lwip/sockets.h"

WiFiClient::WiFiClient() : _connected(false), _sock(-1) {}

WiFiClient::~WiFiClient() {
    stop();
}

bool WiFiClient::connect(const char* host, uint16_t port) {
    if (_connected) stop();
    
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons((u16_t)port);
    
    // Resolve hostname
    addr.sin_addr.s_addr = inet_addr(host);
    if (addr.sin_addr.s_addr == INADDR_NONE) {
        // TODO: DNS lookup
        return false;
    }
    
    _sock = lwip_socket(AF_INET, SOCK_STREAM, 0);
    if (_sock < 0) return false;
    
    if (lwip_connect(_sock, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        lwip_close(_sock);
        _sock = -1;
        return false;
    }
    
    _connected = true;
    return true;
}

bool WiFiClient::connect(uint32_t ip, uint16_t port) {
    if (_connected) stop();
    
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons((u16_t)port);
    addr.sin_addr.s_addr = ip;
    
    _sock = lwip_socket(AF_INET, SOCK_STREAM, 0);
    if (_sock < 0) return false;
    
    if (lwip_connect(_sock, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        lwip_close(_sock);
        _sock = -1;
        return false;
    }
    
    _connected = true;
    return true;
}

size_t WiFiClient::write(uint8_t c) {
    if (!_connected) return 0;
    return lwip_send(_sock, &c, 1, 0);
}

size_t WiFiClient::write(const uint8_t *buf, size_t size) {
    if (!_connected) return 0;
    return lwip_send(_sock, buf, size, 0);
}

int WiFiClient::available() {
    if (!_connected) return 0;
    int count = 0;
    #ifdef FIONREAD
    lwip_ioctl(_sock, FIONREAD, &count);
    #endif
    return count;
}

int WiFiClient::read() {
    if (!_connected) return -1;
    uint8_t c;
    int ret = lwip_recv(_sock, &c, 1, 0);
    return (ret > 0) ? c : -1;
}

int WiFiClient::read(uint8_t *buf, size_t size) {
    if (!_connected) return 0;
    return lwip_recv(_sock, buf, size, 0);
}

int WiFiClient::peek() {
    if (!_connected) return -1;
    uint8_t c;
    int ret = lwip_recv(_sock, &c, 1, MSG_PEEK);
    return (ret > 0) ? c : -1;
}

void WiFiClient::flush() {
    // lwIP doesn't have flush, data is sent immediately
}

void WiFiClient::stop() {
    if (_sock >= 0) {
        lwip_close(_sock);
        _sock = -1;
    }
    _connected = false;
}

uint8_t WiFiClient::connected() {
    return _connected;
}

bool WiFiClient::operator==(const WiFiClient& rhs) {
    return _sock == rhs._sock;
}

String WiFiClient::remoteIP() {
    return String("0.0.0.0");
}

uint16_t WiFiClient::remotePort() {
    return 0;
}

String WiFiClient::localIP() {
    return String("0.0.0.0");
}

uint16_t WiFiClient::localPort() {
    return 0;
}
