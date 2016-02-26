/*
 * CommandExecutor.cpp
 *
 *  Created on: 2 jul. 2015
 *      Author: Herman
 */

#include "CommandExecutor.h"
#include "../core/HardwareSerial.h"

CommandExecutor::CommandExecutor()
{
	commandHandler.registerSystemCommands();
}

CommandExecutor::CommandExecutor(TcpClient* cmdClient) : CommandExecutor()
{
	commandOutput = new CommandOutput(cmdClient);
	if (commandHandler.getVerboseMode() != SILENT)
	{
		commandOutput->printf("Welcome to the Tcp Command executor\r\n");
	}
}

CommandExecutor::CommandExecutor(Stream* reqStream) : CommandExecutor()
{
	commandOutput = new CommandOutput(reqStream);
	if (commandHandler.getVerboseMode() != SILENT)
	{
		commandOutput->printf("Welcome to the Stream Command executor\r\n");
	}
}

CommandExecutor::CommandExecutor(WebSocket* reqSocket)
{
	commandOutput = new CommandOutput(reqSocket);
	if (commandHandler.getVerboseMode() != SILENT)
	{
		reqSocket->sendString("Welcome to the Websocket Command Executor");
	}

}

CommandExecutor::CommandExecutor(MemoryDataStream* reqMemoryStream)
{
	commandOutput = new CommandOutput(reqMemoryStream);
}

CommandExecutor::~CommandExecutor()
{
	delete commandOutput;
}

int CommandExecutor::executorReceive(Command reqCommand)
{
	processCommand(reqCommand);
}

int CommandExecutor::executorReceive(char *recvData, int recvSize)
{
	int receiveReturn = 0;
	for (int recvIdx=0;recvIdx<recvSize;recvIdx++)
	{
		receiveReturn = executorReceive(recvData[recvIdx]);
		if (receiveReturn)
		{
			break;
		}
	}
	return receiveReturn;
}

int CommandExecutor::executorReceive(String recvString, bool completeCommand /* = false */)
{
	if (completeCommand)
	{
		processCommand(Command(recvString));
	}
	else // only partly command -> split in chars and add to current commandLine
	{
		int receiveReturn = 0;
		for (int recvIdx=0;recvIdx<recvString.length();recvIdx++)
		{
			receiveReturn = executorReceive(recvString[recvIdx]);
			if (receiveReturn)
			{
				break;
			}
		}
	}
}

int CommandExecutor::executorReceive(char recvChar)
{
	if (recvChar == 27) // ESC -> delete current commandLine
	{
		recvCommand.clear();
		if (commandHandler.getVerboseMode() == VERBOSE)
		{
			commandOutput->printf("\r\n%s",commandHandler.getCommandPrompt().c_str());
		}
	}
	else if (recvChar == commandHandler.getCommandEOL())
	{
		processCommand(recvCommand);
		recvCommand.clear();
	}
	else
	{
		if (isprint(recvChar))
		{
			recvCommand.cmdString += recvChar;
		}
	}
	return 0;
}

void CommandExecutor::processCommand(Command cmdCommand)
{
	debugf("Received Command, size = %d,cmd = %s",cmdCommand.cmdString.length(),cmdCommand.cmdString.c_str());


	if (cmdCommand.cmdName == "")
	{
		// Need to extract command from inputline
		// Check if we have a json input
		DynamicJsonBuffer jsonBuffer;
		JsonObject& root = jsonBuffer.parseObject(cmdCommand.cmdString);

		if (root.success())
		{
		   Serial.printf("ParseObject() -> cmdString is Json object");
		   cmdCommand.cmdName = root["command"].asString();
		}
		else	// First word of cmdString is cmdCommand
		{
			int cmdLen = cmdCommand.cmdString.indexOf(' ');
			if (cmdLen == -1)
			{
				cmdCommand.cmdName = cmdCommand.cmdString;
			}
			else
			{
				cmdCommand.cmdName = cmdCommand.cmdString.substring(0,cmdLen);
			}
		}
	}

	debugf("CommandExecutor : executing command %s",cmdCommand.cmdName.c_str());

	CommandDelegate cmdDelegate = commandHandler.getCommandDelegate(cmdCommand.cmdName);

	if ((!cmdDelegate.commandProcessDelegate) && (!cmdDelegate.commandFunction))
	{
		commandOutput->printf("Command not found, cmd = '");
		commandOutput->printf(cmdCommand.cmdString.c_str());
		commandOutput->printf("'\r\n");
	}
	else
	{
		if (cmdDelegate.commandProcessDelegate)
		{
			cmdDelegate.commandProcessDelegate(cmdCommand,commandOutput);
		}
		else
		{
			cmdDelegate.commandFunction(cmdCommand.cmdString,commandOutput);
		}
		commandOutput->flush();
	}
	if (commandHandler.getVerboseMode() == VERBOSE)
	{
		commandOutput->printf(commandHandler.getCommandPrompt().c_str());
	}
}
