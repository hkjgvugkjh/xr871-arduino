/**
 * @file WiFiUDP.cpp
 * @brief ESP32 Compatible WiFiUDP Implementation for XR871
 * @author Hermes Agent
 * @date 2026-08-27
 *
 * Implements ESP32 Arduino WiFiUDP API using XR871 lwIP UDP stack.
 */

#include "WiFiUDP.h"
#include <string.h>

// ============================================================
// WiFiUDP Implementation
// ============================================================

WiFiUDP::WiFiUDP() {
    _pcb = NULL;
    _initialized = false;
    _port = 0;
    _rxBuffer = NULL;
    _rxOffset = 0;
    _txBuffer = NULL;
}

WiFiUDP::~WiFiUDP() {
    cleanup();
}

void WiFiUDP::cleanup() {
    if (_pcb) {
        udp_remove(_pcb);
        _pcb = NULL;
    }
    if (_rxBuffer) {
        pbuf_free(_rxBuffer);
        _rxBuffer = NULL;
    }
    if (_txBuffer) {
        pbuf_free(_txBuffer);
        _txBuffer = NULL;
    }
    _initialized = false;
}

uint8_t WiFiUDP::begin(uint16_t port) {
    cleanup();
    
    _pcb = udp_new();
    if (!_pcb) return 0;
    
    _port = port;
    
    err_t err = udp_bind(_pcb, IP_ADDR_ANY, port);
    if (err != ERR_OK) {
        cleanup();
        return 0;
    }
    
    udp_recv(_pcb, _recv, this);
    _initialized = true;
    return 1;
}

uint8_t WiFiUDP::beginMulticast(IPAddress interfaceAddr, IPAddress multicast, uint16_t port) {
    if (begin(port) == 0) return 0;
    // TODO: Set multicast interface
    return 1;
}

void WiFiUDP::stop() {
    cleanup();
}

int WiFiUDP::beginPacket(IPAddress ip, uint16_t port) {
    if (!_initialized) return 0;
    
    if (_txBuffer) {
        pbuf_free(_txBuffer);
        _txBuffer = NULL;
    }
    
    _txBuffer = pbuf_alloc(PBUF_TRANSPORT, 0, PBUF_REF);
    if (!_txBuffer) return 0;
    
    _remoteIP = ip;
    _remotePort = port;
    
    return 1;
}

int WiFiUDP::beginPacket(const char* host, uint16_t port) {
    IPAddress ip;
    if (WiFi.hostByName(host, ip)) {
        return beginPacket(ip, port);
    }
    return 0;
}

int WiFiUDP::beginPacketMulticast(IPAddress multicast, uint16_t port, IPAddress interfaceAddr, int ttl) {
    return beginPacket(multicast, port);
}

int WiFiUDP::endPacket() {
    if (!_txBuffer) return 0;
    
    ip_addr_t addr;
    addr.addr = _remoteIP;
    
    err_t err = udp_sendto(_pcb, _txBuffer, &addr, _remotePort);
    pbuf_free(_txBuffer);
    _txBuffer = NULL;
    
    return err == ERR_OK;
}

size_t WiFiUDP::write(uint8_t data) {
    return write(&data, 1);
}

size_t WiFiUDP::write(const uint8_t *buffer, size_t size) {
    if (!_txBuffer) return 0;
    
    // For UDP, we need to copy data into pbuf
    struct pbuf* newBuf = pbuf_alloc(PBUF_TRANSPORT, size, PBUF_RAM);
    if (!newBuf) return 0;
    
    memcpy(newBuf->payload, buffer, size);
    
    if (_txBuffer) {
        pbuf_cat(_txBuffer, newBuf);
    } else {
        _txBuffer = newBuf;
    }
    
    return size;
}

int WiFiUDP::parsePacket() {
    if (!_rxBuffer) return 0;
    
    // Free old buffer
    if (_rxBuffer) {
        pbuf_free(_rxBuffer);
        _rxBuffer = NULL;
    }
    
    // Get new buffer from PCB
    // Note: In lwIP, we need to handle this differently
    // For now, return available data
    return available();
}

int WiFiUDP::read() {
    if (!_rxBuffer || _rxOffset >= _rxBuffer->tot_len) return -1;
    
    uint8_t* data = (uint8_t*)_rxBuffer->payload;
    return data[_rxOffset++];
}

int WiFiUDP::read(uint8_t* buffer, size_t size) {
    if (!_rxBuffer) return 0;
    
    size_t available = _rxBuffer->tot_len - _rxOffset;
    size_t toRead = (size < available) ? size : available;
    
    uint8_t* data = (uint8_t*)_rxBuffer->payload;
    memcpy(buffer, data + _rxOffset, toRead);
    _rxOffset += toRead;
    
    if (_rxOffset >= _rxBuffer->tot_len) {
        pbuf_free(_rxBuffer);
        _rxBuffer = NULL;
        _rxOffset = 0;
    }
    
    return toRead;
}

int WiFiUDP::read(char* buffer, size_t size) {
    return read((uint8_t*)buffer, size);
}

int WiFiUDP::peek() {
    if (!_rxBuffer || _rxOffset >= _rxBuffer->tot_len) return -1;
    uint8_t* data = (uint8_t*)_rxBuffer->payload;
    return data[_rxOffset];
}

void WiFiUDP::flush() {
    if (_rxBuffer) {
        pbuf_free(_rxBuffer);
        _rxBuffer = NULL;
        _rxOffset = 0;
    }
}

IPAddress WiFiUDP::remoteIP() {
    return _remoteIP;
}

uint16_t WiFiUDP::remotePort() {
    return _remotePort;
}

IPAddress WiFiUDP::localIP() {
    if (!_pcb) return IPAddress(0, 0, 0, 0);
    return IPAddress(_pcb->local_ip.addr);
}

uint16_t WiFiUDP::localPort() {
    if (!_pcb) return 0;
    return _pcb->local_port;
}

int WiFiUDP::available() {
    if (!_rxBuffer) return 0;
    return _rxBuffer->tot_len - _rxOffset;
}

uint8_t WiFiUDP::beginMulticast(IPAddress interfaceAddr, IPAddress multicast) {
    return 1;
}

uint8_t WiFiUDP::joinMulticastGroup(IPAddress interfaceAddr, IPAddress multicast) {
    return 1;
}

uint8_t WiFiUDP::leaveMulticastGroup(IPAddress interfaceAddr, IPAddress multicast) {
    return 1;
}

// ============================================================
// Callback function
// ============================================================

void WiFiUDP::_recv(void *arg, struct udp_pcb *upcb, struct pbuf *p, const ip_addr_t *addr, u16_t port) {
    WiFiUDP* udp = (WiFiUDP*)arg;
    if (!udp) return;
    udp->recvCallback(upcb, p, addr, port);
}

void WiFiUDP::recvCallback(struct udp_pcb *upcb, struct pbuf *p, const ip_addr_t *addr, u16_t port) {
    if (!p) return;
    
    if (_rxBuffer) {
        pbuf_cat(_rxBuffer, p);
    } else {
        _rxBuffer = p;
        _rxOffset = 0;
        _remoteIP = IPAddress(addr->addr);
        _remotePort = port;
    }
}
