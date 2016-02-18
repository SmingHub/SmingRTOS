/*
 * CommandExecutor.h
 *
 *  Created on: 2 jul. 2015
 *      Author: Herman
 */

#ifndef SERVICES_COMMANDPROCESSING_COMMANDEXECUTOR_H_
#define SERVICES_COMMANDPROCESSING_COMMANDEXECUTOR_H_

//#include "WiringFrameworkIncludes.h"
#include "../network/TcpClient.h"
#include "CommandHandler.h"
#include "CommandOutput.h"
#include "Command.h"

#define MAX_COMMANDSIZE 64

class CommandExecutor
{
public:
	CommandExecutor(TcpClient* cmdClient);
	CommandExecutor(Stream* reqStream);
	CommandExecutor(WebSocket* reqSocket);
	CommandExecutor(MemoryDataStream* reqMemoryStream);
	~CommandExecutor();

	int executorReceive(char *recvData, int recvSize);
	int executorReceive(char recvChar);
	int executorReceive(String recvString,  bool completeCommand  = false );
	int executorReceive(Command reqCommand);
	void setCommandPrompt(String reqPrompt);
	void setCommandEOL(char reqEOL);


private :
	CommandExecutor();
	void processCommandLine(String cmdString);
	void processCommand(Command cmdCommand);
	CommandOutput* commandOutput;
	Command recvCommand;
};

#endif /* SERVICES_COMMANDPROCESSING_COMMANDEXECUTOR_H_ */
