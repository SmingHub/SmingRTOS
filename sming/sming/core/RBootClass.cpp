/*
 * RBoot.cpp
 *
 *  Created on: 5 nov. 2015
 *      Author: Herman
 */

#include "RBootClass.h"
#include "../platform/System.h"
#include "../system/flashmem.h"

RBoot::RBoot()
{
}

RBoot::~RBoot()
{
	// TODO Auto-generated destructor stub
}

int RBoot::getCurrentRom()
{
	return rboot_get_current_rom();
}

int RBoot::getCurrentSpiffs()
{
	int retValue = -1;
	if (SPIFFS_mounted(&_filesystemStorageHandle))
	{
		rboot_config bootconf;
		bootconf = rboot_get_config();
		int i = 0;
		for (int i = 0; i< bootconf.count;i++)
		{
			if (_filesystemStorageHandle.cfg.phys_addr == bootconf.roms[i] )
			{
				retValue = i;
				break;
			}
		}
	}
	return retValue;
}

int RBoot::startRom(uint8 reqRom )
{
	debugf("StartRom : req = %d", reqRom);

	rboot_config bootconf;
	bootconf = rboot_get_config();
	if (reqRom >= bootconf.count)
	{
		return RBOOT_INVALID_ROM;
	};
	if (rboot_check_image(bootconf.roms[reqRom]) == 0)
	{
		return RBOOT_INVALID_APPL;
	};
	if (bootconf.current_rom == reqRom)
	{
		return RBOOT_INVALID_CURRENTROM;
	}

	bootconf.current_rom = reqRom;
	rboot_set_config(&bootconf);
	System.restart();
	return RBOOT_OK; // Will never be executed;
}

int RBoot::loadRom(uint8 loadType, uint8 reqSlot, String reqURL)
{
	debugf("Rboot loadrom Rom = %d, url = %s",reqSlot, reqURL.c_str());
	// romStatus = loading
	rboot_config bootconf;
	bootconf = rboot_get_config();

	if (reqSlot >= bootconf.count)
	{
		debugf("Invalid rom");
		return RBOOT_INVALID_ROM;
	}
	else if ( (reqSlot == bootconf.current_rom) ||
		(SPIFFS_mounted(&_filesystemStorageHandle) && (_filesystemStorageHandle.cfg.phys_addr == bootconf.roms[reqSlot] )))
	{
		debugf("Current rom");
		return RBOOT_INVALID_CURRENTROM;
	}
	debugf("start http");
	httpUpdate = new rBootHttpUpdate();
	httpUpdate->setCallback(otaUpdateDelegate(&RBoot::OtaUpdateDelegate,this));
	httpUpdate->addItem(bootconf.roms[reqSlot], reqURL);
	httpUpdate->start();

	return RBOOT_OK;
}

void RBoot::setDelegate (RBootDelegate reqDelegate)
{
	rbootDelegate = reqDelegate;
}

bool RBoot::mountSpiffs(uint8 reqSlot)
{
	rboot_config bootconf;
	bootconf = rboot_get_config();

	if ((reqSlot >= bootconf.count) || (getCurrentRom() == reqSlot))
	{
		return false;
	};

	debugf("trying to mount spiffs at %x, length %d", bootconf.roms[reqSlot], 65536);
	spiffs_mount_manual(bootconf.roms[reqSlot], 65536);
//	spiffs_mount_manual(bootconf.roms[reqSlot] , 65536);
	return true;
}

void RBoot::unmoutSpiffs()
{
	spiffs_unmount();
}

void RBoot::OtaUpdateDelegate(bool result)
{
	int romStatus;
	romStatus = result ? 0 : 1; // valid : error
	debugf("Delegate Called result = %d", result);

	if (rbootDelegate)
	{
		rbootDelegate(result ? 0 : 1);
	}
}

void RBoot::processRBootCommands(String commandLine, CommandOutput* commandOutput)
{
	Vector<String> commandToken;
	int numToken = splitString(commandLine, ' ' , commandToken);

	if (numToken == 1)
	{
		commandOutput->printf("RBoot Commands available : \r\n");
		commandOutput->printf("info : Show Rominfo\r\n");
		commandOutput->printf("load : Load ROM contents \r\n");
		commandOutput->printf("start : Restart to <ROM>\r\n");
		commandOutput->printf("mount : Mount Spiffs from <ROM>\r\n");
		commandOutput->printf("unmount : Unmount spiffs filesystem\r\n");
		commandOutput->printf("url : set baseURL to <URL>");
	}
	else
	{
		if (commandToken[1] == "load")
		{
			if (numToken != 4)
			{
				commandOutput->printf("Usage : rboot load <Rom#> <URL>\r\n");
			}
			else
			{
				int rom = commandToken[2].toInt();
				debugf("Loading Rom %d from  %s",rom, commandToken[3].c_str());
				String reqURL = commandToken[3];
				if (baseURL != "")
				{
					reqURL = baseURL + reqURL;
				}
				int loadResult = loadRom(1,rom,reqURL);
				debugf("Loadresult = %d ", loadResult);
				if (loadResult == RBOOT_INVALID_ROM)
				{
					commandOutput->printf("Loading to ROM%d failed : invalid romnumber\r\n",rom);
				}
				else if (loadResult == RBOOT_INVALID_CURRENTROM)
				{
					commandOutput->printf("Loading to ROM%d failed : currentrom\r\n",rom);
				}
				else
				{
					commandOutput->printf("Loading %s to ROM%d started\r\n",reqURL.c_str(),rom);
				}
			}
		}
		else if (commandToken[1] == "start")
		{
			int rom = getCurrentRom();
			if (numToken == 3)
			{
				rom = commandToken[2].toInt();
			}
			int startResult = startRom(rom);
			if (startResult == RBOOT_INVALID_ROM)
			{
				commandOutput->printf("%d is an invalid Rom number\r\n", rom);
			}
			else if (startResult == RBOOT_INVALID_APPL)
			{
				commandOutput->printf("ROM%d does not contain a valid application\r\n", rom);
			}
			else
			{
				commandOutput->printf("Restart to ROM%d initiated");
			}

		}
		else if (commandToken[1] == "mount")
		{
			if (SPIFFS_mounted(&_filesystemStorageHandle))
			{
				commandOutput->printf("Spiffs filesystem already mounted\r\n");
			}
			else
			if (numToken != 3)
			{
				commandOutput->printf("Usage : rboot spiffs <slot#> \r\n");
			}
			else
			{
				int slot = commandToken[2].toInt();
				if (mountSpiffs(slot))
				{
					commandOutput->printf("Spiffs on ROM%d mounted\r\n",slot);
				}
				else
				{
					commandOutput->printf("Mounting spiffs on ROM%d failed\r\n",slot);
				}
			}

		}
		else if (commandToken[1] == "unmount")
		{
			unmoutSpiffs();
			commandOutput->printf("Spiffs filesystem unmounted\r\n");
		}
		else if (commandToken[1] == "info")
		{
			showRomInfo(commandOutput);
		}
		else if (commandToken[1] == "ls")
		{
			showSpiffs(commandOutput);
		}
		else if (commandToken[1] == "url")
		{
			if (numToken != 3)
			{
				commandOutput->printf("Usage : rboot url <BaseURL> \r\n");
			}
			else
			{
				baseURL = commandToken[2];
				commandOutput->printf("Base URL set to : %s\r\n", baseURL.c_str());
			}
		}
		else
		{
			commandOutput->printf("Unknown rboot command\r\n");
		}

	}
}

void RBoot::initCommand()
{
	commandHandler.registerCommand(CommandDelegate("rboot","Rboot related commands","RBoot",commandFunctionDelegate(&RBoot::processRBootCommands,this)));
}

void RBoot::showSpiffs(CommandOutput* commandOutput)
{
	if (!SPIFFS_mounted(&_filesystemStorageHandle))
	{
		commandOutput->printf("No Spiffs filesystem mounted\r\n");
	}
	else
	{
		Vector<String> files = fileList();
		commandOutput->printf("filecount %d\r\n", files.count());
		for (unsigned int i = 0; i < files.count(); i++)
		{
			commandOutput->println(files[i]);
		}
	}

}

void RBoot::showRomInfo(CommandOutput* commandOutput)
{

	rboot_config bootconf;
	bootconf = rboot_get_config();
	commandOutput->printf("\r\nROM Information\r\n");
	commandOutput->printf("Current ROM = %d\r\n",getCurrentRom());
	commandOutput->printf("Rboot Version %d\r\n",bootconf.version);

	for (int i = 0; i < bootconf.count;i++)
	{
		commandOutput->printf("ROM%d : Address = %06x", i, bootconf.roms[i]) ;
		if (i == bootconf.current_rom) {
			commandOutput->printf("  CurrentRom,    Size = %d Kb", (flashmem_get_first_free_block_address() - bootconf.roms[i]) /1024 );
		}
		else if (i == getCurrentSpiffs())
		{
			commandOutput->printf("  CurrentSpiffs, Size = %d Kb",_filesystemStorageHandle.cfg.phys_size / 1024);
		}
		else if (rboot_check_image(bootconf.roms[i]))
		{
			commandOutput->printf("  Application");
		}
		commandOutput->printf("\r\n");
	}

	rboot_rtc_data rbootRTC;

	if (!rboot_get_rtc_data(&rbootRTC))
	{
		commandOutput->printf("No valid rBoot RTC data \r\n");
	}
	else
	{
		commandOutput->printf("rBoot RTC = Next mode : %d, Last mode : %d, Last rom : %d, Temp rom %d\r\n",rbootRTC.next_mode,rbootRTC.last_mode,rbootRTC.last_rom,rbootRTC.temp_rom);
	}

	if (baseURL != "")
	{
		commandOutput->printf("\r\nbaseURL : %s\r\n",baseURL.c_str());
	}


}

