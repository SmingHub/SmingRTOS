# Sming_RTOS_POC

Proof of Concept for Sming based on Espressif RTOS SDK.

Based on: Sming Develop branch 26/11/15

Requirements:
- Espressif RTOS_SDK 1.3.0
- UDK 2.0.9 for toolchain and SDK on Windows or esp-open-sdk on Linux
  - Be sure to have a "clean" sdk without previously applied updates
- Tested on Windows and Linux, BSD/Mac untested but should work

Usage:

- Clone this repository.
- Set SDK location in Makefile-<platform> or as environment variables
- Set WIFI_SSID & WIFI_PWD in environment variables
- Compile & flash
- Serial baudrate 115200 (when using example application)

Applications:

- Application which includes HttpAjax & HWPwm example source.
- HttpAjax -> works
- HWPWm, -> compiles, not tested on hardware

Spiffs/Spiffy

Spiffs Filesystem is included in the POC

The example is made for using on a 32MBit, 4Mbyte ESP.
You need to adapt if using other.

There is a make-target : flashspiffy
This flashes both application & spiffs FS
Spiffs : The file spiffs_rom.rom (beware of the .rom extension) to 0x300000
In the application there is a mount_manual for a spiffs FS of 64K at this location.
If you only update application, no need to flash spiffs again -> use flash target

Known limitations : 

- HardwareSerial -> only print(f)(ln).. and begin()
- No AccessPoint
- Interrupts not yet implemented
- Only limited testing done

Known RTOS/NONOS differences with application consequences

- No WDT routines
- Soft timer only on milli and not on micro level

Open Conversion Questions

- Should we make OneWire a Core functionality instead of library (other libs are dependent).



