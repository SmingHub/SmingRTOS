/*
 * ExampleCommand.h
 *
 */

#ifndef SMINGCORE_EXAMPLE_COMMAND_H_
#define SMINGCORE_EXAMPLE_COMMAND_H_

#include "include/SmingCore.h"

class ExampleCommand
{
public:
	ExampleCommand();
	virtual ~ExampleCommand();
	void initCommand();

private:
	bool status = true;
	void processExampleCommands(String commandLine, CommandOutput* commandOutput);
};


#endif /* SMINGCORE_DEBUG_H_ */
