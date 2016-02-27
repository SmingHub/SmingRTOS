#Sming CommandHandler

##Summary##
The Sming CommandHandler implementation adds the possibility to handle System and Application Commands over a variety of input media.
The CLI provides a framework in which Basic System Commands can be extended and Application specific commands created. 

Currently implemented are :
- Serial
- Telnet Server
- Websocket Server
- Http POST
- Mqtt Client

##Implementation##
The implementation makes use of a global object CommandHandler, which at the start is just a skeleton without any command active.
System Commands can be activated using 
`Serial.commandProcessing(true)`
`telnetServer.enableCommand(true)`
`HttpServer.commandProcessing(true,"command") -> will enable both Websocket and HTTP Post`
'MqttClient.commandProcessing(true,"inputTopic", "replyTopic")

The additional parameter for websocket is used to allow multiple connections on one websocket server instance, both with or without command proceesing
When the websocket open request has queryparameter "command=true" command processing is enabled

##usage##
The usage of CommandProcessor is the same for all channels, except for specific requirements.
Commandprocesor options implemented are :
- Verbose -> sets display of welcome message
- Prompt -> set prompt string
- EOL -> set end of line character
- welcome message -> set the welcome message

##System Commands##
The base CommandHandler has the following system commands implemented :
- "help", "Displays all available commands"
- "status", "Displays System Information"
- "echo", "Displays command entered"
- "debugon", "Set Serial debug on"
- "debugoff", "Set Serial debug off"
- "command","Usage verbose/silent/prompt for command options"

##Application options##
Applications (and Sming Core functionality) can add commands to the CommandProcesor use a construct like
`registerCommand(CommandDelegate("status", "Displays System Information", "system", CommandProcessDelegate(&CommandHandler::procesStatusCommand,this)));`
"status" -> type String, the name of the command
"Displays...." -> text which will be shown when "help command" is requested.
"system" -> Group to which the command belongs, not yet used in the framework
"CommandProcessingDelegate(...) -> The delegate function which is called by the framework.

The added command will then be available thru every opened command channel


The prototype for a CommandProcessDelegate is : 
`void processCommands(Command inputCommand, CommandOutput* commandOutput)`

inputCommand is an object which holds the information about the requested commands.
Main interface are the functions :

`String inputCommand.getCmdName()`  -> The name of the function
`String inputCommand.getCmdString()` -> The full inputline of the command
`JsonObject& inputCommand.getRoot()` -> If the inputline is valid json the jsonObject of that string
`String inputCommand[n]`         -> The n-th word from the inputline

##The detection of the cmdName##
For all command channels
If the inputline is valid json 
-> The value of inputCommand.getRoot()["cmd"]
-> If not present the key of the first json object

If not valid json -> The first word of the inputline.

For HttpPost, Websocket and MqttClient the commandProcessing() function has the optional parameter "defaultCommand"
If this parameter is set, the CmdName will be set for each Command -> all commands will be delegated to the same ProcessFuction.


##The CommandOutput class##
The CommandOutput class provides the functionality to return data.
The class is derived from Wiring Print class so it has all print/println/printf functions.
Next to that, the getRoot() function gives access to a jsonObject.
On finishing the CommandProcessingDelegate the data gathered is flushed to the originating channel.
If the application wants to send multiple replies, the flush function can be called.
The content of the jsonObject is appended when flushed.


##Example usage##
The capabilities of CommandHandler are shown in the example `Commandprocessing_Debug`

The example will create an application which shows CommandProcessing for Telnet, Websocket and Serial. 
To test Telnet, open telnet client and connect on port 23 to the ESP ip 
To test the Websocket, open a web browser with the ESP
To test Serial, use your "Serial program" and make sure "local echo" is activated.

For all implementations "Type help" to show the available Commands

The possibilities of extending commands with the application are shown in 
- The class ExampleCommand
- The functions startExampleApplicationCommand() and processApplicationCommands()

##NONOS Compatibility##
The implementation of the NONOS commandProcessing is still available.
NONOS applications using commandProcessing will run unaltered.
Converting to the new functionality can be achieved by :
- Update commandFunctionDelegate to CommandProcessDelegate in the registerCommand function
- Update the application processCommand function to the new prototype
- As the first line insert : `String commandLine = inputCommand.getCmdString()`
- The rest of the function can stay the same.


