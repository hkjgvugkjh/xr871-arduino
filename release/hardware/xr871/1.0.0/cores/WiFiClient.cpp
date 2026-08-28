/**
 * @file WiFiClient.cpp
 * @brief ESP32 Compatible WiFiClient (TCP) Implementation for XR871
 * @author Hermes Agent
 * @date 2026-08-27
 *
 * Implements ESP32 Arduino WiFiClient API using XR871 lwIP TCP stack.
 */

#include "WiFiClient.h"
#include "WiFi.h"
#include <string.h>

// ============================================================
// WiFiClient Implementation
// ============================================================

WiFiClient::WiFiClient() {
    init();
}

WiFiClient::~WiFiClient() {
    cleanup();
}

WiFiClient::WiFiClient(const WiFiClient& other) {
    _pcb = other._pcb;
    _connected = other._connected;
    _timeout = other._timeout;
    _nodelay = other._nodelay;
    _rxBuffer = other._rxBuffer;
    _rxOffset = other._rxOffset;
}

WiFiClient& WiFiClient::operator=(const WiFiClient& other) {
    if (this != &other) {
        cleanup();
        _pcb = other._pcb;
        _connected = other._connected;
        _timeout = other._timeout;
        _nodelay = other._nodelay;
        _rxBuffer = other._rxBuffer;
        _rxOffset = other._rxOffset;
    }
    return *this;
}

void WiFiClient::init() {
    _pcb = NULL;
    _connected = false;
    _timeout = 5000;
    _nodelay = false;
    _rxBuffer = NULL;
    _rxOffset = 0;
}

void WiFiClient::cleanup() {
    if (_pcb) {
        tcp_arg(_pcb, NULL);
        tcp_recv(_pcb, NULL);
        tcp_sent(_pcb, NULL);
        tcp_poll(_pcb, NULL, 0);
        tcp_err(_pcb, NULL);
        tcp_close(_pcb);
        _pcb = NULL;
    }
    if (_rxBuffer) {
        pbuf_free(_rxBuffer);
        _rxBuffer = NULL;
    }
    _connected = false;
}

int WiFiClient::connect(IPAddress ip, uint16_t port) {
    return connect(ip, port, _timeout);
}

int WiFiClient::connect(const char* host, uint16_t port) {
    IPAddress ip;
    if (WiFi.hostByName(host, ip)) {
        return connect(ip, port);
    }
    return 0;
}

int WiFiClient::connect(IPAddress ip, uint16_t port, int32_t timeout) {
    cleanup();
    
    _pcb = tcp_new();
    if (!_pcb) return 0;
    
    ip_addr_t addr;
    addr.addr = (uint32_t)ip;
    
    tcp_arg(_pcb, this);
    tcp_recv(_pcb, _recv);
    tcp_sent(_pcb, _sent);
    tcp_poll(_pcb, _poll, 2);
    tcp_err(_pcb, _err);
    
    err_t err = tcp_connect(_pcb, &addr, port, NULL);
    if (err != ERR_OK) {
        cleanup();
        return 0;
    }
    
    // Wait for connection
    uint32_t start = millis();
    while (!_connected && millis() - start < (uint32_t)timeout) {
        delay(1);
    }
    
    return _connected;
}

size_t WiFiClient::write(uint8_t data) {
    return write(&data, 1);
}

size_t WiFiClient::write(const uint8_t *buf, size_t size) {
    if (!_pcb || !_connected) return 0;
    
    size_t available = tcp_sndbuf(_pcb);
    if (available == 0) return 0;
    
    size_t toWrite = (size < available) ? size : available;
    
    err_t err = tcp_write(_pcb, buf, toWrite, TCP_WRITE_FLAG_COPY);
    if (err != ERR_OK) return 0;
    
    tcp_output(_pcb);
    return toWrite;
}

size_t WiFiClient::write(const char* str) {
    return write((const uint8_t*)str, strlen(str));
}

size_t WiFiClient::print(const char* str) {
    return write(str);
}

size_t WiFiClient::println(const char* str) {
    size_t n = write(str);
    n += write("\r\n");
    return n;
}

size_t WiFiClient::print(int val, int base) {
    char buf[32];
    itoa(val, buf, base);
    return write(buf);
}

size_t WiFiClient::println(int val, int base) {
    size_t n = print(val, base);
    n += write("\r\n");
    return n;
}

size_t WiFiClient::printFloat(double val, int format) {
    char buf[32];
    dtostrf(val, 0, format, buf);
    return write(buf);
}

size_t WiFiClient::printlnFloat(double val, int format) {
    size_t n = printFloat(val, format);
    n += write("\r\n");
    return n;
}

int WiFiClient::read() {
    if (!_rxBuffer) return -1;
    
    if (_rxOffset >= _rxBuffer->tot_len) {
        pbuf_free(_rxBuffer);
        _rxBuffer = NULL;
        _rxOffset = 0;
        return -1;
    }
    
    uint8_t* data = (uint8_t*)_rxBuffer->payload;
    return data[_rxOffset++];
}

int WiFiClient::read(uint8_t *buf, size_t size) {
    if (!_rxBuffer) return 0;
    
    size_t available = _rxBuffer->tot_len - _rxOffset;
    size_t toRead = (size < available) ? size : available;
    
    uint8_t* data = (uint8_t*)_rxBuffer->payload;
    memcpy(buf, data + _rxOffset, toRead);
    _rxOffset += toRead;
    
    if (_rxOffset >= _rxBuffer->tot_len) {
        pbuf_free(_rxBuffer);
        _rxBuffer = NULL;
        _rxOffset = 0;
    }
    
    return toRead;
}

int WiFiClient::peek() {
    if (!_rxBuffer || _rxOffset >= _rxBuffer->tot_len) return -1;
    uint8_t* data = (uint8_t*)_rxBuffer->payload;
    return data[_rxOffset];
}

int WiFiClient::available() {
    if (!_rxBuffer) return 0;
    return _rxBuffer->tot_len - _rxOffset;
}

void WiFiClient::flush() {
    if (_pcb) {
        tcp_output(_pcb);
    }
}

void WiFiClient::stop() {
    cleanup();
}

uint8_t WiFiClient::connected() {
    return _connected;
}

WiFiClient::operator bool() {
    return _connected;
}

bool WiFiClient::operator==(const WiFiClient& other) {
    return _pcb == other._pcb;
}

IPAddress WiFiClient::remoteIP() {
    if (!_pcb) return IPAddress(0, 0, 0, 0);
    return IPAddress(_pcb->remote_ip.addr);
}

uint16_t WiFiClient::remotePort() {
    if (!_pcb) return 0;
    return _pcb->remote_port;
}

IPAddress WiFiClient::localIP() {
    if (!_pcb) return IPAddress(0, 0, 0, 0);
    return IPAddress(_pcb->local_ip.addr);
}

uint16_t WiFiClient::localPort() {
    if (!_pcb) return 0;
    return _pcb->local_port;
}

void WiFiClient::setNoDelay(bool nodelay) {
    _nodelay = nodelay;
    if (_pcb) {
        if (nodelay) {
            tcp_nagle_disable(_pcb);
        } else {
            tcp_nagle_enable(_pcb);
        }
    }
}

bool WiFiClient::getNoDelay() {
    return _nodelay;
}

void WiFiClient::setTimeout(uint32_t timeout) {
    _timeout = timeout;
}

WiFiClient WiFiClient::accept() {
    return WiFiClient();
}

// ============================================================
// Callback functions
// ============================================================

err_t WiFiClient::_recv(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err) {
    WiFiClient* client = (WiFiClient*)arg;
    if (!client) return ERR_OK;
    return client->recvCallback(tpcb, p, err);
}

err_t WiFiClient::_sent(void *arg, struct tcp_pcb *tpcb, u16_t len) {
    WiFiClient* client = (WiFiClient*)arg;
    if (!client) return ERR_OK;
    return client->sentCallback(tpcb, len);
}

err_t WiFiClient::_poll(void *arg, struct tcp_pcb *tpcb) {
    WiFiClient* client = (WiFiClient*)arg;
    if (!client) return ERR_OK;
    return client->pollCallback(tpcb);
}

void WiFiClient::_err(void *arg, err_t err) {
    WiFiClient* client = (WiFiClient*)arg;
    if (!client) return;
    client->errCallback(err);
}

err_t WiFiClient::recvCallback(struct tcp_pcb *tpcb, struct pbuf *p, err_t err) {
    if (p == NULL) {
        // Connection closed
        _connected = false;
        return ERR_OK;
    }
    
    if (_rxBuffer) {
        // Chain to existing buffer
        pbuf_cat(_rxBuffer, p);
    } else {
        _rxBuffer = p;
        _rxOffset = 0;
    }
    
    tcp_recved(tpcb, p->tot_len);
    return ERR_OK;
}

err_t WiFiClient::sentCallback(struct tcp_pcb *tpcb, u16_t len) {
    return ERR_OK;
}

err_t WiFiClient::pollCallback(struct tcp_pcb *tpcb) {
    return ERR_OK;
}

void WiFiClient::errCallback(err_t err) {
    _connected = false;
    cleanup();
}

// ============================================================
// WiFiServer Implementation
// ============================================================

WiFiServer::WiFiServer(uint16_t port) {
    _port = port;
    _listenPcb = NULL;
    _listening = false;
    _timeout = 5000;
    _pendingClient = NULL;
}

WiFiServer::~WiFiServer() {
    end();
}

void WiFiServer::begin() {
    begin(_port);
}

void WiFiServer::begin(uint16_t port) {
    _port = port;
    
    _listenPcb = tcp_new();
    if (!_listenPcb) return;
    
    err_t err = tcp_bind(_listenPcb, IP_ADDR_ANY, _port);
    if (err != ERR_OK) {
        tcp_close(_listenPcb);
        _listenPcb = NULL;
        return;
    }
    
    _listenPcb = tcp_listen(_listenPcb);
    tcp_arg(_listenPcb, this);
    tcp_accept(_listenPcb, _accept);
    
    _listening = true;
}

void WiFiServer::end() {
    if (_listenPcb) {
        tcp_arg(_listenPcb, NULL);
        tcp_accept(_listenPcb, NULL);
        tcp_close(_listenPcb);
        _listenPcb = NULL;
    }
    _listening = false;
    
    if (_pendingClient) {
        delete _pendingClient;
        _pendingClient = NULL;
    }
}

WiFiClient WiFiServer::available() {
    if (!_listening) return WiFiClient();
    
    if (_pendingClient) {
        WiFiClient client = *_pendingClient;
        delete _pendingClient;
        _pendingClient = NULL;
        return client;
    }
    
    return WiFiClient();
}

bool WiFiServer::hasClient() {
    return _pendingClient != NULL;
}

void WiFiServer::setTimeout(uint32_t timeout) {
    _timeout = timeout;
}

err_t WiFiServer::_accept(void *arg, struct tcp_pcb *newpcb, err_t err) {
    WiFiServer* server = (WiFiServer*)arg;
    if (!server) return ERR_OK;
    return server->acceptCallback(newpcb, err);
}

err_t WiFiServer::acceptCallback(struct tcp_pcb *newpcb, err_t err) {
    WiFiClient* client = new WiFiClient();
    client->_pcb = newpcb;
    client->_connected = true;
    
    tcp_arg(newpcb, client);
    tcp_recv(newpcb, WiFiClient::_recv);
    tcp_sent(newpcb, WiFiClient::_sent);
    tcp_poll(newpcb, WiFiClient::_poll, 2);
    tcp_err(newpcb, WiFiClient::_err);
    
    if (_pendingClient) {
        delete _pendingClient;
    }
    _pendingClient = client;
    
    return ERR_OK;
}
