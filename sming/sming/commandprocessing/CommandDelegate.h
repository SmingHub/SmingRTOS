/*
 * CommandDelegate.h
 *
 *  Created on: 2 jul. 2015
 *      Author: Herman
 */

#ifndef SERVICES_COMMANDPROCESSING_COMMANDDELEGATE_H_
#define SERVICES_COMMANDPROCESSING_COMMANDDELEGATE_H_

#include "../wiring/WString.h"
#include "../core/Delegate.h"
#include "../network/TcpClient.h"
#include "CommandOutput.h"
#include "Command.h"

typedef Delegate<void(Command command ,CommandOutput* commandOutput)> CommandProcessDelegate;
typedef Delegate<void(String commandLine, CommandOutput* commandOutput)> commandFunctionDelegate;

class CommandDelegate
{
	// Hashmap uses CommandDelegate() contructor when extending size
	friend class HashMap<String, CommandDelegate>;
	friend class CommandHandler;
public:
	CommandDelegate(String reqName, String reqHelp, String reqGroup, CommandProcessDelegate reqFunctionNew);
	CommandDelegate(String reqName, String reqHelp, String reqGroup, commandFunctionDelegate reqFunction);
	~CommandDelegate();

	String commandName = "";
	String commandHelp = "";
	String commandGroup = "";
	CommandProcessDelegate commandProcessDelegate = NULL;
	commandFunctionDelegate commandFunction = NULL;

private :
	CommandDelegate();

};

#endif /* SERVICES_COMMANDPROCESSING_COMMANDDELEGATE_H_ */
