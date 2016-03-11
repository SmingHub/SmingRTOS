#ifndef __SMING_CONFIG_H__
#define __SMING_CONFIG_H__

#define SMING_VERSION "SmingRTOS V0.1.0" // Major Minor Sub

#ifndef __ESP8266_EX__
  #define __ESP8266_EX__
#endif

#ifndef SERIAL_BAUD_RATE
  #define SERIAL_BAUD_RATE 115200
#endif

// For Sming framework always enable RTC
#define BOOT_RTC_ENABLED

#endif
