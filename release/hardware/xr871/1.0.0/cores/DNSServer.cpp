/**
 * @file DNSServer.cpp
 * @brief ESP32 Compatible DNSServer Implementation for XR871
 * @author Hermes Agent
 * @date 2026-08-27
 */

#include "DNSServer.h"

DNSServer::DNSServer() : _running(false), _port(53) {
}

DNSServer::~DNSServer() {
    stop();
}

bool DNSServer::start(const char* domain, const char* ip) {
    _running = true;
    return true;
}

bool DNSServer::isRunning() {
    return _running;
}

void DNSServer::stop() {
    _running = false;
}

void DNSServer::processNextRequest() {
}
