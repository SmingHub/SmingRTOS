/*
 * CommandDelegate.cpp
 *
 *  Created on: 2 jul. 2015
 *      Author: Herman
 */

#include "CommandDelegate.h"

CommandDelegate::CommandDelegate()
{
}

CommandDelegate::CommandDelegate(String reqName, String reqHelp, String reqGroup, CommandProcessDelegate reqProcessDelegate)
: commandName(reqName), commandHelp(reqHelp), commandGroup(reqGroup), commandProcessDelegate(reqProcessDelegate)
{
}

CommandDelegate::CommandDelegate(String reqName, String reqHelp, String reqGroup, commandFunctionDelegate reqFunctionDelegate)
: commandName(reqName), commandHelp(reqHelp), commandGroup(reqGroup), commandFunction(reqFunctionDelegate)
{

}

CommandDelegate::~CommandDelegate()
{
}

