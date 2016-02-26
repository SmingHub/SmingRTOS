/*
 * Command.cpp
 *
 *  Created on: 18 feb. 2016
 *      Author: Herman
 */

#include <sming/commandprocessing/Command.h>

Command::Command() {
}

Command::Command(String reqString) : cmdString(reqString) {
}

Command::Command(String reqName, String reqString) : cmdName(reqName), cmdString(reqString) {
}

const String Command::operator[](unsigned int index)
{
	if (!cmdVector)
	{
		cmdVector = new Vector<String>;
		splitString(cmdString, ' ' , *cmdVector);
	}
	if (index >= cmdVector->size())
	{
		return "";
	}
	else
	{
		return cmdVector->elementAt(index);
	}
}

Command::~Command() {
	delete cmdVector;
	delete cmdJsonBuffer;
	delete &cmdRoot;
}

JsonObject& Command::getRoot()
{
	if (!cmdJsonBuffer)
	{
		cmdJsonBuffer = new DynamicJsonBuffer;
		cmdRoot =  &cmdJsonBuffer->parseObject(cmdString);
	}
	return *cmdRoot;
}

String Command::getCmdName(){
	return cmdName;
}

String Command::getCmdString()
{
	return cmdString;
}

void Command::clear()
{
	cmdString = "";
	cmdName = "";
	delete cmdVector;
	delete cmdJsonBuffer;
//	delete cmdRoot;
	cmdVector = NULL;
	cmdJsonBuffer = NULL;
	cmdRoot = NULL;
}


