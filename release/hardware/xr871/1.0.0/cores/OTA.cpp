/**
 * @file OTA.cpp
 * @brief ESP32 Compatible OTA Implementation for XR871
 * @author Hermes Agent
 * @date 2026-08-27
 *
 * Implements ESP32 Arduino OTA API using XR871 OTA driver.
 */

#include "OTA.h"
#include "WiFi.h"
#include <string.h>

// XR871 includes
#include "ota/ota.h"

// Global ArduinoOTA object
ArduinoOTAClass ArduinoOTA;

// ============================================================
// ArduinoOTAClass Implementation
// ============================================================

ArduinoOTAClass::ArduinoOTAClass() {
    _status = OTA_IDLE;
    _error = OTA_ERROR_NONE;
    _progress = 0;
    _hostname = "XR871-OTA";
    _password = "";
    _port = 8266;
    _timeout = 30000;
    _onStartCallback = NULL;
    _onEndCallback = NULL;
    _onProgressCallback = NULL;
    _onErrorCallback = NULL;
}

ArduinoOTAClass::~ArduinoOTAClass() {
    abort();
}

bool ArduinoOTAClass::begin() {
    // Initialize OTA subsystem
    if (ota_init() != OTA_STATUS_OK) {
        error(OTA_ERROR_BEGIN_FAILED);
        return false;
    }
    
    _status = OTA_IDLE;
    return true;
}

void ArduinoOTAClass::handle() {
    // In this implementation, OTA is synchronous
    // This function is for compatibility with ESP32 API
}

bool ArduinoOTAClass::update(const char* url) {
    if (_status == OTA_START) {
        return false;
    }
    
    _status = OTA_START;
    _progress = 0;
    _error = OTA_ERROR_NONE;
    
    // Call onStart callback
    if (_onStartCallback) {
        _onStartCallback();
    }
    
    // Download image
    if (!downloadImage(url)) {
        error(OTA_ERROR_DOWNLOAD_FAILED);
        return false;
    }
    
    // Verify image
    if (!verifyImage()) {
        error(OTA_ERROR_VERIFY_FAILED);
        return false;
    }
    
    // Write image
    if (!writeImage()) {
        error(OTA_ERROR_WRITE_FAILED);
        return false;
    }
    
    // Call onEnd callback
    if (_onEndCallback) {
        _onEndCallback();
    }
    
    _status = OTA_SUCCESS;
    return true;
}

void ArduinoOTAClass::setHostname(const char* hostname) {
    _hostname = hostname;
}

String ArduinoOTAClass::getHostname() {
    return _hostname;
}

void ArduinoOTAClass::setPassword(const char* password) {
    _password = password;
}

void ArduinoOTAClass::setPort(uint16_t port) {
    _port = port;
}

void ArduinoOTAClass::setTimeout(uint16_t timeout) {
    _timeout = timeout;
}

void ArduinoOTAClass::onStart(void (*callback)(void)) {
    _onStartCallback = callback;
}

void ArduinoOTAClass::onEnd(void (*callback)(void)) {
    _onEndCallback = callback;
}

void ArduinoOTAClass::onProgress(void (*callback)(unsigned int progress, unsigned int total)) {
    _onProgressCallback = callback;
}

void ArduinoOTAClass::onError(void (*callback)(ota_error_t error)) {
    _onErrorCallback = callback;
}

ota_status_t ArduinoOTAClass::getStatus() {
    return _status;
}

ota_error_t ArduinoOTAClass::getError() {
    return _error;
}

int ArduinoOTAClass::getProgress() {
    return _progress;
}

void ArduinoOTAClass::abort() {
    _status = OTA_IDLE;
    _progress = 0;
}

// ============================================================
// Private functions
// ============================================================

bool ArduinoOTAClass::startOTA() {
    if (ota_init() != OTA_STATUS_OK) {
        return false;
    }
    return true;
}

void ArduinoOTAClass::endOTA() {
    ota_deinit();
}

bool ArduinoOTAClass::downloadImage(const char* url) {
    // Use XR871 OTA to download image from URL
    ota_status_t status = ota_get_image(OTA_PROTOCOL_HTTP, (void*)url);
    
    if (status != OTA_STATUS_OK) {
        return false;
    }
    
    // Update progress
    _progress = 50;
    if (_onProgressCallback) {
        _onProgressCallback(50, 100);
    }
    
    return true;
}

bool ArduinoOTAClass::verifyImage() {
    // Verify downloaded image
    ota_verify_data_t verify_data;
    if (ota_get_verify_data(&verify_data) != OTA_STATUS_OK) {
        return false;
    }
    
    // Update progress
    _progress = 75;
    if (_onProgressCallback) {
        _onProgressCallback(75, 100);
    }
    
    return true;
}

bool ArduinoOTAClass::writeImage() {
    // Write image to flash and reboot
    ota_reboot();
    return true;
}

void ArduinoOTAClass::error(ota_error_t err) {
    _error = err;
    _status = OTA_ERROR;
    
    if (_onErrorCallback) {
        _onErrorCallback(err);
    }
}

// ============================================================
// Helper Functions
// ============================================================

bool OTA_Available() {
    return true;
}

bool OTA_GetPartitionInfo(uint32_t* address, uint32_t* size) {
    // TODO: Get partition info
    *address = 0x0;
    *size = 0x100000; // 1MB
    return true;
}

bool OTA_VerifyImage(uint32_t address) {
    // TODO: Verify image at address
    return true;
}

void OTA_Reboot() {
    ota_reboot();
}
