#include <user_config.h>
#include <SmingCore.h>

typedef struct
{
	int value;
} MessageStruct;

typedef Delegate<void(MessageStruct)> MessageDelegate;

class MessageServer
{
public :
	MessageServer()  {
		qd = new QueuedDelegate<MessageStruct>(MessageDelegate(&MessageServer::messageHandler,this));
	};
	~MessageServer(){
		delete qd;
	}

	void initiateMessage(int reqValue)
	{
		MessageStruct ms = {reqValue};
		qd->sendQueue(ms);
	}

	void messageHandler(MessageStruct messageStruct)
	{
		// This is a class function called using queue executed in a separate task
		Serial.printf("Class function called, value =  %d\r\n",messageStruct.value);
	}

private :
	QueuedDelegate<MessageStruct> * qd;
};

void handleMessage(MessageStruct messageStruct)
{
	// This is a static function called using queue, executed in a separate task
	Serial.printf("Static function called, value =  %d\r\n",messageStruct.value);
}

#define GPIO2 2
#define GPIO4 4

MessageServer* classServer;
QueuedDelegate<MessageStruct>* staticServer;

Timer t1;
Timer t2;

void staticTimer()
{
	debugf("static called");
	MessageStruct m = {(int) millis() };
	staticServer->sendQueue(m);
}

void classTimer()
{
	debugf("class called");
	classServer->initiateMessage(millis());
}

void init()
{
	Serial.begin(SERIAL_BAUD_RATE);
	classServer  = new MessageServer();
	staticServer = new QueuedDelegate<MessageStruct>(handleMessage);

	t1.initializeMs(2000, staticTimer).start();
	t2.initializeMs(3000, classTimer).start();
}

