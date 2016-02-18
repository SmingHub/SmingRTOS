/*
 * Command.h
 *
 *  Created on: 18 feb. 2016
 *      Author: Herman
 */

#ifndef SMING_COMMANDPROCESSING_COMMAND_H_
#define SMING_COMMANDPROCESSING_COMMAND_H_

#include "../include/sming_global.h"
#include "../Wiring/WString.h"
#include "../Wiring/WVector.h"
#include "../Services/ArduinoJson/include/ArduinoJson.h"


class Command
{
	friend class CommandExecutor;
	friend class CommandHandler;

public:
	Command();
	Command(String reqString);
	Command(String reqName, String reqString);
	virtual ~Command();
//	Command(const Command&) = delete;

	const String operator[](unsigned int index);

	String getCmdName();
	String getCmdString();

	int getVectorSize();

	JsonObject& getRoot();

private:
	void clear();

	DynamicJsonBuffer *cmdJsonBuffer = NULL;
	JsonObject* cmdRoot = NULL;

	Vector<String>* cmdVector = NULL;
	String cmdName = "";
	String cmdString = "";


};

#endif /* SMING_COMMANDPROCESSING_COMMAND_H_ */
