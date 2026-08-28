// Minimal OTA stub
#ifndef _OTA_XR871_H_
#define _OTA_XR871_H_
#include <stdint.h>
typedef enum { OTA_IDLE = 0, OTA_START, OTA_PROGRESS, OTA_SUCCESS, OTA_ERROR } ota_status_t;
typedef enum { OTA_ERROR_NONE = 0, OTA_ERROR_BEGIN_FAILED, OTA_ERROR_CONNECT_FAILED, OTA_ERROR_DOWNLOAD_FAILED, OTA_ERROR_VERIFY_FAILED, OTA_ERROR_WRITE_FAILED, OTA_ERROR_SPACE_EXHAUSTED, OTA_ERROR_UNKNOWN } ota_error_t;
class ArduinoOTAClass {
public:
    ArduinoOTAClass() {}
    ~ArduinoOTAClass() {}
    bool begin() { return false; }
    void handle() {}
    bool update(const char*) { return false; }
    void setHostname(const char*) {}
    const char* getHostname() { return ""; }
    void setPassword(const char*) {}
    void setPort(uint16_t) {}
    void setTimeout(uint16_t) {}
    void onStart(void(*)(void)) {}
    void onEnd(void(*)(void)) {}
    void onProgress(void(*)(unsigned int, unsigned int)) {}
    void onError(void(*)(ota_error_t)) {}
    ota_status_t getStatus() { return OTA_IDLE; }
    ota_error_t getError() { return OTA_ERROR_NONE; }
    int getProgress() { return 0; }
    void abort() {}
};
extern ArduinoOTAClass ArduinoOTA;
bool OTA_Available() { return false; }
bool OTA_GetPartitionInfo(uint32_t*, uint32_t*) { return false; }
bool OTA_VerifyImage(uint32_t) { return false; }
void OTA_Reboot() {}
#endif
