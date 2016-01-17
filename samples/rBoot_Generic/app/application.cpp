#include <user_config.h>
#include "include/SmingCore.h"
#include "sming/core/RBootClass.h"

// If you want, you can define WiFi settings globally in Eclipse Environment Variables
#ifndef WIFI_SSID
	#define WIFI_SSID "PleaseEnterSSID" // Put you SSID and Password here
	#define WIFI_PWD "PleaseEnterPass"
#endif

RBoot rBoot;
TelnetServer telnetServer;
FTPServer ftpServer;

void rbootDelegate(int resultCode) {

	Serial.printf("rBoot resultcode =  %d\r\n", resultCode );
}

void addSpiffsRoms()
{
	rboot_config bootconf;
	bootconf = rboot_get_config();
	bootconf.count = 4;
	bootconf.roms[2] = 0x100000;
	bootconf.roms[3] = 0x300000;
	rboot_set_config(&bootconf);
}


void processApplicationCommands(String commandLine, CommandOutput* commandOutput)
{
	commandOutput->printf("This command is handled by the application\r\n");

	Vector<String> commandToken;
	int numToken = splitString(commandLine, ' ' , commandToken);

	if ((numToken == 2) && (commandToken[1] == "loop"))
	{
		commandOutput->printf("Entering loop for wdt reset");
		int i =0;
		do
		{
			i ++;
		} while (true);
	}
	if ((numToken == 3) && (commandToken[1] == "div"))
	{
		int a = 56;
		int b  = a / commandToken[2].toInt();
		commandOutput->printf("Divide test %d, %d ");

	}
	else if (numToken = 2)
	{
		int rom = commandToken[1].toInt();
		rBoot.loadRom(1,rom,"http://10.0.0.201/esptest/rom0.bin");
		commandOutput->printf("Loading Rom %d\r\n",rom);
	}
}


void init() {
	

	Serial.begin(115200); // 115200 by default
	Serial.systemDebugOutput(true); // Debug output to serial

	WifiAccessPoint.enable(false);
	WifiStation.enable(true);
	WifiStation.config(WIFI_SSID, WIFI_PWD);
	WifiAccessPoint.enable(false);
	
	telnetServer.listen(23);
	telnetServer.enableDebug(true);

	// Start FTP server
	ftpServer.listen(21);
	ftpServer.addUser("me", "123"); // FTP account
	ftpServer.setFlashDirectory("flash");

	Serial.commandProcessing(true);
	commandHandler.registerCommand(CommandDelegate("app","rBoot_Generic example","Application",processApplicationCommands));

	Serial.printf("add spiff roms\r\n");

	addSpiffsRoms();

	rBoot.initCommand();
	rBoot.setDelegate(rbootDelegate);
	int slot = rBoot.getCurrentRom();
	
	rboot_config bootconf;
	bootconf = rboot_get_config();
	Serial.printf("\r\nrBoot Generic running rom %d \r\n", slot,bootconf.current_rom);

	debugf("Mounting SPIFFS from ROM2, address = %X", bootconf.roms[2]);
	rBoot.mountSpiffs(2);

}
