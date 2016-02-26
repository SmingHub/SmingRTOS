/*
 * CommandOutput.cpp
 *
 *  Created on: 5 jul. 2015
 *      Author: Herman
 */

#include "CommandOutput.h"

CommandOutput::CommandOutput(TcpClient* reqClient)
: outputTcpClient(reqClient)
{
}

CommandOutput::CommandOutput(Stream* reqStream)
: outputStream(reqStream)
{
}

CommandOutput::CommandOutput(WebSocket* reqSocket)
:  outputSocket(reqSocket)
{
}

CommandOutput::CommandOutput(MemoryDataStream* reqMemoryStream)
: outputMemoryStream(reqMemoryStream)
{
}

CommandOutput::~CommandOutput()
{
}

void CommandOutput::flush()
{
	if (cmdJsonBuffer) // There is a jsonObject created in the object.
	{
		cmdRoot->printTo(*this);
	}

	if (outputTcpClient)
	{
		outputTcpClient->sendString(outputString);
	}
	else if (outputStream)
	{
		outputStream->printf("%s",outputString.c_str());
	}
	else if (outputSocket)
	{
		outputSocket->sendString(tempSocket);
	}
	else if (outputMemoryStream)
	{
		outputMemoryStream->printf("%s",outputString.c_str());
	}
	outputString = "";
}

JsonObject& CommandOutput::getRoot()
{
	if (!cmdJsonBuffer)
	{
		cmdJsonBuffer = new DynamicJsonBuffer;
		cmdRoot =  &cmdJsonBuffer->createObject();
	}
	return *cmdRoot;
}

size_t CommandOutput::write(uint8_t outChar)
{
	outputString = outputString+String(char(outChar));

	/*
	if (outputTcpClient)
	{
		char outBuf[1] = { outChar };
		outputTcpClient->write(outBuf,1);
	}
	else if (outputStream)
	{
		outputStream->write(outChar);
	}
	else if (outputSocket)
	{
		if (outChar == '\r')
		{
			outputSocket->sendString(tempSocket);
			tempSocket = "";
		}
		else
		{
			tempSocket = tempSocket+String(char(outChar));
		}
	}
	else if (outputMemoryStream)
	{
		outputMemoryStream->write(outChar);
	}
	*/
}

