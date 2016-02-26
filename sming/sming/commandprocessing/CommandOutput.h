/*
 * CommandOutput.h
 *
 *  Created on: 5 jul. 2015
 *      Author: Herman
 */

#ifndef SERVICES_COMMANDPROCESSING_COMMANDOUTPUT_H_
#define SERVICES_COMMANDPROCESSING_COMMANDOUTPUT_H_

#include "../network/TcpClient.h"
#include "../wiring/Stream.h"
#include "../wiring/Print.h"
#include "../wiring/WiringFrameworkDependencies.h"
#include "../network/WebSocket.h"

class CommandOutput: public Print
{
public:
	CommandOutput(TcpClient* reqClient);
	CommandOutput(Stream* reqStream);
	CommandOutput(WebSocket* reqSocket);
	CommandOutput(MemoryDataStream* reqMemoryStream);
	virtual ~CommandOutput();

	size_t write(uint8_t outChar);
	JsonObject& getRoot();
	void flush();

private:
	TcpClient* outputTcpClient = nullptr;
	Stream*    outputStream = nullptr;
	WebSocket* outputSocket = nullptr;
	MemoryDataStream* outputMemoryStream = nullptr;
	String outputString = "";
	DynamicJsonBuffer *cmdJsonBuffer = NULL;
	JsonObject* cmdRoot = NULL;
};

#endif /* SERVICES_COMMANDPROCESSING_COMMANDOUTPUT_H_ */
