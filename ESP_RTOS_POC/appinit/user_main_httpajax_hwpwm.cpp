/******************************************************************************
 * Copyright 2013-2014 Espressif Systems (Wuxi)
 *
 * FileName: user_main.c
 *
 * Description: entry file of user application
 *
 * Modification history:
 *     2014/12/1, v1.0 create this file.
*******************************************************************************/


#include "../SmingCore/WifiStation.h"
#include "esp_common.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

//#include "lwip/sockets.h"
//#include "lwip/dns.h"
//#include "lwip/netdb.h"

//#include "../system/include/lwip_includes.h"

#include "../SmingCore/Network/UdpConnection.h"
#include "../SmingCore/Network/TcpServer.h"
#include "../SmingCore/Network/TcpClient.h"
#include "../SmingCore/Network/FtpServer.h"
#include "../SmingCore/Network/HttpServer.h"
#include "../SmingCore/Network/TelnetServer.h"
#include "../SmingCore/HardwareSerial.h"
#include "../SmingCore/HardwarePWM.h"

#include "../SmingCore/FileSystem.h"

//#define __forceinline __attribute__((always_inline)) inline

#include "ESP8266EX.h"
#include "Digital.h"
#include "Delegate.h"

//#include "../RTOS_EC/cqueue.h"
//#include "../RTOS_EC/ctask.h"
#include "../Wiring/WString.h"
#include "../Wiring/Stream.h"
#include "../Wiring/WVector.h"
#include "../Wiring/IPAddress.h"

#include "Timer.h"
//#include "Station.h"


//#include "c_types_compatible.h"



void procTask(void *pvParameters)
{
    while (1) {

    }
}

// If you want, you can define WiFi settings globally in Eclipse Environment Variables
#ifndef WIFI_SSID
	#define WIFI_SSID "PleaseEnterSSID" // Put you SSID and Password here
	#define WIFI_PWD "PleaseEnterPass"
#endif

HttpServer server;
FTPServer ftp;

int inputs[] = {0, 2}; // Set input GPIO pins here
Vector<String> namesInput;
const int countInputs = sizeof(inputs) /  sizeof(inputs[0]);

void onIndex(HttpRequest &request, HttpResponse &response)
{
	TemplateFileStream *tmpl = new TemplateFileStream("index.html");
	auto &vars = tmpl->variables();
	//vars["counter"] = String(counter);
	response.sendTemplate(tmpl); // this template object will be deleted automatically
}

void onFile(HttpRequest &request, HttpResponse &response)
{
	String file = request.getPath();
	if (file[0] == '/')
		file = file.substring(1);

	if (file[0] == '.')
		response.forbidden();
	else
	{
		response.setCache(86400, true); // It's important to use cache for better performance.
		response.sendFile(file);
	}
}

void onAjaxInput(HttpRequest &request, HttpResponse &response)
{
	JsonObjectStream* stream = new JsonObjectStream();
	JsonObject& json = stream->getRoot();
	json["status"] = (bool)true;

	String stringKey = "StringKey";
	String stringValue = "StringValue";

	json[stringKey] = stringValue;

    for( int i = 0; i < 11; i++)
    {
        char buff[3];
        itoa(i, buff, 10);
        String desiredString = "sensor_";
        desiredString += buff;
        json[desiredString] = desiredString;
    }


	JsonObject& gpio = json.createNestedObject("gpio");
	for (int i = 0; i < countInputs; i++)
		gpio[namesInput[i].c_str()] = digitalRead(inputs[i]);

	response.sendJsonObject(stream);
}

void onAjaxFrequency(HttpRequest &request, HttpResponse &response)
{
	int freq = request.getQueryParameter("value").toInt();

	System.setCpuFrequency((CpuFrequency)freq);

	JsonObjectStream* stream = new JsonObjectStream();
	JsonObject& json = stream->getRoot();
	json["status"] = (bool)true;
	json["value"] =  (int)System.getCpuFrequency();

	response.sendJsonObject(stream);
}

void startWebServer()
{
	printf("FreeHeapSize StartWebserver = %d\r\n",xPortGetFreeHeapSize());
	server.listen(80);
	server.addPath("/", onIndex);
	server.addPath("/ajax/input", onAjaxInput);
	server.addPath("/ajax/frequency", onAjaxFrequency);
	server.setDefaultHandler(onFile);

	Serial.println("\r\n=== WEB SERVER STARTED ===");
	Serial.println(WifiStation.getIP());
	Serial.println("==============================\r\n");
}

void startFTP()
{
	if (!fileExist("index.html"))
		fileSetContent("index.html", "<h3>Please connect to FTP and upload files from folder 'web/build' (details in code)</h3>");

	// Start FTP server
	ftp.listen(21);
	ftp.addUser("me", "123"); // FTP account
}


uint8_t pins[8] = { 4, 5, 0, 2, 15, 13, 12, 14 }; // List of pins that you want to connect to pwm
HardwarePWM HW_pwm(pins, 8);

Timer procTimer;
int32 i = 0;
int32 inc = 1000;
bool countUp = true;
void doPWM() {
	if (countUp == true) {
		i += inc;
		if (i >= 22222) {
			i = 22222;
			countUp = false;
		}
	} else {
		i -= inc;
		if (i <= 0) {
			i = 0;
			countUp = true;
		}
	}
	HW_pwm.analogWrite(2, i);
}

void HWPWM_init() {
	// Setting PWM values on 8 different pins
	HW_pwm.analogWrite(4, 22222);
	HW_pwm.analogWrite(5, 11111);
	HW_pwm.analogWrite(0, 22222);
	HW_pwm.analogWrite(2, 11111);
	HW_pwm.analogWrite(15, 22222);
	HW_pwm.analogWrite(13, 11111);
	HW_pwm.analogWrite(12, 22222);
	HW_pwm.analogWrite(14, 11111);

	debugf("PWM output set on all 8 Pins. Kindly check...");
	debugf("Now Pin 2 will go from 0 to VCC to 0 in cycles.");
	procTimer.initializeMs(100, doPWM).start();
}




// Will be called when WiFi station was connected to AP
void connectOk()
{
	Serial.println("I'm CONNECTED");

	startFTP();
	startWebServer();
}


void init()
{
	Serial.begin(115200);

    Serial.printf("Mounting FS\r\n");
    spiffs_mount_manual(0x40500000, 65536);
    Serial.printf("Mounted\r\n");

    if (!fileExist("index.html"))
    {
    	debugf("creating index file");
		fileSetContent("index.html", "<h3>Please connect to FTP and upload files from folder 'web/build' (details in code)</h3>");
		debugf("index created");
    }

    int fd = fileOpen ("index.html",eFO_ReadOnly);

	for (int i = 0; i < countInputs; i++)
	{
		namesInput.add(String(inputs[i]));
		pinMode(inputs[i], INPUT);
	}

	WifiStation.enable(true);
	WifiStation.config(WIFI_SSID, WIFI_PWD);
//	WifiAccessPoint.enable(false);
/*
	for (int i = 0; i < countInputs; i++)
	{
		namesInput.add(String(inputs[i]));
		pinMode(inputs[i], INPUT);
	}
*/
	// Run our method when station was connected to AP
	WifiStation.waitConnection(connectOk);

	HWPWM_init();

}


extern "C" void user_init(void)
{
    printf("Sming RTOS Proof of Concept \r\n");
	printf("SDK version:%s\n", system_get_sdk_version());
    printf("FreeHeapSize = %d\r\n",xPortGetFreeHeapSize());
    init();
    xTaskCreate(procTask,(signed char *) "procTask", 256, NULL, 2, NULL);
}
