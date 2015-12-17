# Sming_RTOS_POC

Proof of Concept for Sming based on Espressif RTOS SDK.

Based on : 

Sming Develop branch dd 26/11/15

- Espressif RTOS_SDK 1.3.0
- UDK 2.0.9 for toolchain and SDK
- Only used on Windows, unknow Unix/Mac behavior

Usage : 

- Clone this repository.
- Copy files from sdk_updates directory into corresponding SDK directories
- Set SDK location in makefile
- Set WIFI_SSID & WIFI_PWD in environment variables
- Compile & flash
- Serial baudrate 115200 (when using example application)

Applications : 

- Application which includes HttpAjax & HWPwm example source.
- HttpAjax -> works
- HWPWm, -> compiles, not tested on hardware

Known limitations : 

- No framework/application but one "all in one" 
- HardwareSerial -> only print(f)(ln).. and begin()
- No AccessPoint
- Interrupts not yet implemented
- Only limited testing done




