/*
 * CommandHandler.h
 *
 *  Created on: 2 jul. 2015
 *      Author: Herman
 */

#ifndef SERVICES_COMMANDPROCESSING_COMMANDHANDLER_H_
#define SERVICES_COMMANDPROCESSING_COMMANDHANDLER_H_

//#include "../wiring/WiringFrameworkIncludes.h"
#include "CommandDelegate.h"
#include "../wiring/WHashMap.h"
//#include "SystemClock.h"
#include <stdio.h>
#include "../core/HardwareSerial.h"

typedef enum {VERBOSE,SILENT} VerboseMode;

class CommandHandler
{
public:
	CommandHandler();
	~CommandHandler();
	bool registerCommand(CommandDelegate);
	bool unregisterCommand(CommandDelegate);
	void registerSystemCommands();
	CommandDelegate getCommandDelegate(String commandString);
	VerboseMode getVerboseMode();
	void setVerboseMode(VerboseMode reqVerboseMode);
	String getCommandPrompt();
	void setCommandPrompt(String reqPrompt);
	char getCommandEOL();
	void setCommandEOL(char reqEOL);
	String getCommandWelcomeMessage();
	void setCommandWelcomeMessage(String reqWelcomeMessage);

//	int deleteGroup(String reqGroup);

private :
	HashMap<String, CommandDelegate> *registeredCommands;
	void procesHelpCommand(Command reqCommand, CommandOutput* commandOutput);
	void procesStatusCommand(Command reqCommand, CommandOutput* commandOutput);
	void procesEchoCommand(Command reqCommand, CommandOutput* commandOutput);
	void procesDebugOnCommand(Command reqCommand, CommandOutput* commandOutput);
	void procesDebugOffCommand(Command reqCommand, CommandOutput* commandOutput);
	void processCommandOptions(Command reqCommand  ,CommandOutput* commandOutput);

	VerboseMode verboseMode = VERBOSE;
	String currentPrompt = "Sming>";
	char currentEOL = '\r';
	String currentWelcomeMessage = "Welcome to the Sming CommandProcessing\r\n";
};

extern CommandHandler commandHandler;

#endif /* SERVICES_COMMANDPROCESSING_COMMANDHANDLER_H_ */
