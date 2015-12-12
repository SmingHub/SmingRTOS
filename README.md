# Sming_RTOS_POC

Very early Proof of Concept for Sming based on Espressif RTOS SDK.

Based on : 

Sming Develop branch dd 26/11/15
Espressif RTOS_SDK 1.3.0
UDK 2.0.9 for toolchain and SDK
Only used on Windows, unknow Unix/Mac behavior

Usage : 

Clone this repository.
Set SDK location in makefile
Set WIFI_SSID & WIFI_PWD in environment variables
Compile & flash
Serial baudrate 74880

Applications : 

Minimal RTOS example, TCP Connection -> for bugfixing network issue
Trial Sming examples, showing some Sming basic fuctionalities -> Network not running correct
Focus first on networking but basic_blink working


