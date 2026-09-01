/**
 * @file WiFiServer.cpp
 * @brief WiFiServer implementation for XR871 using lwIP sockets
 */

#include "Arduino.h"
#include "WiFiServer.h"
#include "WiFiClient.h"
#include "net/lwip-2.0.3/lwip/sockets.h"

WiFiServer::WiFiServer(uint16_t port) : _port(port), _sock(-1), _listening(false) {}

WiFiServer::~WiFiServer() {
    end();
}

void WiFiServer::begin() {
    if (_listening) return;

    _sock = lwip_socket(AF_INET, SOCK_STREAM, 0);
    if (_sock < 0) return;

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons((u16_t)_port);

    if (lwip_bind(_sock, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        lwip_close(_sock);
        _sock = -1;
        return;
    }

    if (lwip_listen(_sock, 5) < 0) {
        lwip_close(_sock);
        _sock = -1;
        return;
    }

    _listening = true;
}

void WiFiServer::end() {
    if (_sock >= 0) {
        lwip_close(_sock);
        _sock = -1;
    }
    _listening = false;
}

WiFiClient WiFiServer::available() {
    WiFiClient client;

    if (!_listening) return client;

    struct sockaddr_in addr;
    socklen_t addr_len = sizeof(addr);
    int client_sock = lwip_accept(_sock, (struct sockaddr*)&addr, &addr_len);

    if (client_sock >= 0) {
        // Create a new WiFiClient with the accepted socket
        // Note: This is a simplified implementation
        // In a full implementation, we'd need to modify WiFiClient to accept a socket
    }

    return client;
}

bool WiFiServer::hasClient() {
    if (!_listening) return false;
    // Check if there's a pending connection
    return false;
}
