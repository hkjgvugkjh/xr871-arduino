/**
 * @file OTA.h
 * @brief ESP32 Compatible OTA (Over-The-Air) for XR871
 * @author Hermes Agent
 * @date 2026-08-27
 *
 * Provides ESP32 Arduino OTA API wrapper on top of XR871 OTA driver.
 * Supports HTTP-based OTA updates with verification.
 *
 * ESP32 API Compatibility:
 *   - ArduinoOTA.begin()
 *   - ArduinoOTA.handle()
 *   - ArduinoOTA.onStart()
 *   - ArduinoOTA.onEnd()
 *   - ArduinoOTA.onProgress()
 *   - ArduinoOTA.onError()
 */

#ifndef _OTA_XR871_H_
#define _OTA_XR871_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

// ============================================================
// OTA Status Codes
// ============================================================
typedef enum {
    OTA_IDLE = 0,
    OTA_START,
    OTA_PROGRESS,
    OTA_SUCCESS,
    OTA_ERROR
} ota_status_t;

// ============================================================
// OTA Error Codes
// ============================================================
typedef enum {
    OTA_ERROR_NONE = 0,
    OTA_ERROR_BEGIN_FAILED,
    OTA_ERROR_CONNECT_FAILED,
    OTA_ERROR_DOWNLOAD_FAILED,
    OTA_ERROR_VERIFY_FAILED,
    OTA_ERROR_WRITE_FAILED,
    OTA_ERROR_SPACE_EXHAUSTED,
    OTA_ERROR_UNKNOWN
} ota_error_t;

// ============================================================
// ArduinoOTA Class (ESP32 Compatible)
// ============================================================
class ArduinoOTAClass {
public:
    ArduinoOTAClass();
    ~ArduinoOTAClass();
    
    // Start OTA
    bool begin();
    
    // Handle OTA (must be called in loop)
    void handle();
    
    // OTA update from URL
    bool update(const char* url);
    
    // Set hostname for OTA
    void setHostname(const char* hostname);
    String getHostname();
    
    // Set password for OTA
    void setPassword(const char* password);
    
    // Set port for OTA
    void setPort(uint16_t port);
    
    // Set reconnect timeout
    void setTimeout(uint16_t timeout);
    
    // Callbacks
    void onStart(void (*callback)(void));
    void onEnd(void (*callback)(void));
    void onProgress(void (*callback)(unsigned int progress, unsigned int total));
    void onError(void (*callback)(ota_error_t error));
    
    // Get status
    ota_status_t getStatus();
    ota_error_t getError();
    int getProgress();
    
    // Abort OTA
    void abort();

private:
    ota_status_t _status;
    ota_error_t _error;
    int _progress;
    String _hostname;
    String _password;
    uint16_t _port;
    uint16_t _timeout;
    
    // Callbacks
    void (*_onStartCallback)(void);
    void (*_onEndCallback)(void);
    void (*_onProgressCallback)(void)(unsigned int, unsigned int);
    void (*_onErrorCallback)(void)(ota_error_t);
    
    // Internal functions
    bool startOTA();
    void endOTA();
    bool downloadImage(const char* url);
    bool verifyImage();
    bool writeImage();
    void error(ota_error_t err);
};

extern ArduinoOTAClass ArduinoOTA;

// ============================================================
// OTA Helper Functions
// ============================================================

/**
 * @brief Check if OTA is available
 */
bool OTA_Available();

/**
 * @brief Get OTA partition info
 */
bool OTA_GetPartitionInfo(uint32_t* address, uint32_t* size);

/**
 * @brief Verify OTA image
 */
bool OTA_VerifyImage(uint32_t address);

/**
 * @brief Reboot into new firmware
 */
void OTA_Reboot();

#ifdef __cplusplus
}
#endif

#endif /* _OTA_XR871_H_ */
