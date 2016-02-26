# Sming_RTOS Beta version

Sming based on Espressif RTOS SDK.

Based on: Sming Develop, including commits until Sming 2.1.0 release
Based on: Espressif RTOS_SDK 1.3.0

Feedback:

- When finding issues during conversion of applications -> Submit an issue on the Sming_RTOS repository.

Requirements:
- Espressif RTOS_SDK 1.3.0
- UDK 2.0.9 for toolchain and SDK on Windows or esp-open-sdk on Linux
  - Be sure to have a "clean" sdk without previously applied updates
- esp-alt-sdk provides downloads for all platforms, @kireevco to supply links
  
Usage:

- Beware : Some Environment variables names are identical to SMING_NONOS !

- Clone this repository.
- SET environment variables valid for the location
- Set SDK location in Makefile-<platform> or as environment variables
	-> SDK_BASE = ....
- Set WIFI_SSID & WIFI_PWD in environment variables
- Compile & flash

Additional needed software

- Spiffy  : Source included in Sming repository
- [ESPtool2] (https://github.com/raburton/esptool2) esptool2 

Completeness:

- All Core and Network functionality is ported to Sming_RTOS
- Spiffy & Spiffs Filesystem functionality included

Applications:

- Application which includes HttpAjax & HWPwm example source.
- HttpAjax -> works
- HWPWm, -> compiles, not tested on hardware
- Neopixel -> working
- ScannerI2C -> working
- WebSockets -> working
- SDCard -> working
- HCSR04 Ultrasonic -> working
- DHT11-22 -> DHT11 tested and working , DHT22 untested
- SSD1306 Screen -> i2c module tested and working, spi module untested
- DS1820 -> working
- SystemClock&NTP -> working
- BMP180 Pressure -> working
- RBoot -> working
- PCD8544 LCD (Nokia 5110 LCD) -> working

Known RTOS/NONOS differences with application consequences

Generic :
- No WDT routines
- Soft timer only on milli and not on micro level

SPI_SIZE
- The espressif SDK is more restrictive on use of flash
The SPI_SIZE needs to be set correct to prevent issues (especially Spiffs)

Include files :
- The user_config.h file does not contain any framework includes anymore.
	-> Remove the NONOS user_config.h !
	If not removed, there will be unresolved .h files 
- The directory structure has changed.
	The only include necessary is the #include "SmingCore.h"
	When using libraries use #include "libraries/......"
	For case sensitive systems : beware of naming
	
Spiffs
- Spiffs FS'es created on NONOS are not supported -> Create a new spiffs using the RTOS supplied Spiffy !
- When mounting the FS on Flash is checked for "Generic spiffy structure" (blocksize, FS length,..) 
- spiffs_mount() and spiffs_mount_manual return bool based on mount result
- When using spiffs_mount() the size of the spiffs FS is autodetected.
- When using spiffs_mount_manual() 
	- use the actual flash address : For conversion of current spiffs_mount_manual :  subtract 0x40200000 from start address 
	- use filesize is 0 (zero) for autodetected size of FS
- See the remark above on SPI_SIZE. If set not correct the spiffs_mount and spiffs_mount_manual will fail.
rBoot 
	- Works as in Sming but ... See the above on spiffs_mount_manual ! 

HardwarePWM.
	- Duty cycle is now independent on period.
	- Values are from 0 - 1023



