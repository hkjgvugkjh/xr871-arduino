#ifndef _LWIP_IP_ADDR_H_
#define _LWIP_IP_ADDR_H_
#include <stdint.h>
typedef uint32_t ip_addr_t;
typedef ip_addr_t ip4_addr_t;
typedef struct { ip_addr_t addr; } ip4_addr;
typedef uint16_t u16_t;
typedef uint8_t u8_t;
typedef uint32_t u32_t;
#define IP4_ADDR(ipaddr, a,b,c,d) ((ipaddr)->addr = ((u32_t)((d) & 0xff) << 24) | ((u32_t)((c) & 0xff) << 16) | ((u32_t)((b) & 0xff) << 8) | (u32_t)((a) & 0xff))
#endif
