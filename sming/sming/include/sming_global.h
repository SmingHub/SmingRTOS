// Based on mziwisky espmissingincludes.h && ESP8266_IoT_SDK_Programming Guide_v0.9.1.pdf && ESP SDK defines

#ifndef __SMING_GOLBAL_H__
#define __SMING_GLOBAL_H__

#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>

#include "limits.h"

#include <espressif/c_types.h>
#include <espressif/esp8266/ets_sys.h>
#include <espressif/esp_libc.h>

#include "sming/include/sming_config.h"

#include "sming/system/m_printf.h"

#define __ESP8266_EX__ // System definition ESP8266 SOC

//#define IRAM_ATTR __attribute__((section(".iram.text")))
#define __forceinline __attribute__((always_inline)) inline
#define STORE_TYPEDEF_ATTR __attribute__((aligned(4),packed))
#define STORE_ATTR __attribute__((aligned(4)))

//#undef assert
#define debugf(fmt, ...) m_printf(fmt"\r\n", ##__VA_ARGS__)
//#define assert(condition) if (!(condition)) SYSTEM_ERROR("ASSERT: %s %d", __FUNCTION__, __LINE__)
#define SYSTEM_ERROR(fmt, ...) printf("ERROR: " fmt "\r\n", ##__VA_ARGS__)

typedef signed short file_t;

#endif
