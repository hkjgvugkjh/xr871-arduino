#ifndef _KERNEL_OS_OS_H_
#define _KERNEL_OS_OS_H_

#include <stdint.h>

typedef uint32_t OS_Time_t;

static inline void OS_MSleep(uint32_t ms) {}
static inline uint32_t OS_GetTimeMs(void) { return 0; }
static inline void OS_StartScheduler(void) {}

#endif
