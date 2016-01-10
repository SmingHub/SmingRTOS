/****
 * Sming Framework Project - Open Source framework for high efficiency native ESP8266 development.
 * Created 2015 by Skurydin Alexey
 * http://github.com/anakod/Sming
 * All files of the Sming Core are provided under the LGPL v3 license.
 ****/

#ifndef _NET_WIRING_
#define _NET_WIRING_

#define SMING_VERSION 140 // Major Minor Sub

#include "FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/task.h"

#include "../wiring/WiringFrameworkIncludes.h"

#include "Delegate.h"
//#include "Boards.h"
#include "Clock.h"
#include "SystemClock.h"
#include "Digital.h"
#include "ESP8266EX.h"
#include "FileSystem.h"
#include "HardwareSerial.h"
#include "Interrupts.h"
//#include "DriverPWM.h"
#include "HardwarePWM.h"
#include "Timer.h"
#include "Wire.h"
#include "SPISoft.h"

#include "platform/System.h"
#include "platform/Station.h"
#include "platform/AccessPoint.h"
//#include "platform/WDT.h"

#include "network/HttpClient.h"
#include "network/MqttClient.h"
#include "network/NtpClient.h"
#include "network/HttpServer.h"
#include "network/HttpRequest.h"
#include "network/HttpResponse.h"
#include "network/FtpServer.h"
#include "network/NetUtils.h"
#include "network/TcpClient.h"
#include "network/TcpConnection.h"
#include "network/UdpConnection.h"
//#include "network/HttpFirmwareUpdate.h"
#include "network/rBootHttpUpdate.h"
#include "network/URL.h"
#include "network/TcpServer.h"
#include "network/TelnetServer.h"

#include "../services/ArduinoJson/include/ArduinoJson.h"
#include "../services/DateTime/DateTime.h"
#include "../services/libemqtt/libemqtt.h"
#include "../services/FATFS/ff.h"

#endif
