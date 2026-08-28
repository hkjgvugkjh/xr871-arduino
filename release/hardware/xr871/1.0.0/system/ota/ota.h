#ifndef _OTA_OTA_H_
#define _OTA_OTA_H_
#include <stdint.h>
typedef enum { OTA_STATE_OK = 0, OTA_STATE_ERROR = 1 } OTA_State;
typedef void (*OTA_ProgressCB)(uint32_t progress, uint32_t total);
typedef void (*OTA_ErrorCB)(int error);
static inline int OTA_Init(void) { return 0; }
static inline int OTA_Begin(uint32_t size) { return 0; }
static inline int OTA_Write(const uint8_t *data, uint16_t len) { return 0; }
static inline int OTA_End(void) { return 0; }
#endif
