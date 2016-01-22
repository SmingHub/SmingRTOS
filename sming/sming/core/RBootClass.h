/*
 * RBoot.h
 *
 *  Created on: 5 nov. 2015
 *      Author: Herman
 */

#ifndef SMINGCORE_RBOOT_H_
#define SMINGCORE_RBOOT_H_


#include "../Network/rBootHttpUpdate.h"

#include "../Wiring/WiringFrameworkDependencies.h"
#include "../CommandProcessing/CommandProcessingIncludes.h"
#include "../../rboot/appcode/rboot-api.h"
#include "../services/rboot_sming/rboot_sming.h"
#include "FileSystem.h"

#define RBOOT_OK                       0
#define RBOOT_INVALID_ROM				-1
#define RBOOT_INVALID_APPL          	-2
#define RBOOT_INVALID_CURRENTROM		-3

typedef struct
{
	uint32 StartAddress;     // Keep always as first to prevent updates to core rboot
	uint8 RomType;           // -> AppCode, SpiffyFS
	uint32 Size;
	uint8 Status;
} RBootRomInfo;

typedef Delegate<void(int resultCode)> RBootDelegate;


class RBoot
{
public:
	RBoot();
	virtual ~RBoot();
	int loadRom(uint8 loadType, uint8 reqSlot, String reqURL);

	void OtaUpdateDelegate(bool result);
	int getCurrentRom();
	int getCurrentSpiffs();

	int startRom(uint8 reqSlot);

	void initCommand();
	void showRomInfo(CommandOutput* commandOutput);
	void showSpiffs(CommandOutput* commandOutput);

	bool mountSpiffs(uint8 reqRom);
	void unmoutSpiffs();

	void setDelegate (RBootDelegate reqDelegate);

private :

	void processRBootCommands(String commandLine, CommandOutput* commandOutput);

	String baseURL;
	otaUpdateDelegate updateDelegate;
	rBootHttpUpdate* httpUpdate = 0;

	RBootDelegate rbootDelegate;
};

#endif /* SMINGCORE_RBOOT_H_ */
