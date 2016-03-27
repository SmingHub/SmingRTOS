#include <user_config.h>
#include <SmingCore.h>

class InterruptServer
{
public :
	InterruptServer(int reqPin) : interruptPin(reqPin) {
		qi = new QueuedInterrupt(interruptPin,
								(GPIO_INT_TYPE)GPIO_PIN_INTR_ANYEDGE,
								QueuedInterruptDelegate(&InterruptServer::interruptHandler,this));
	};
	~InterruptServer(){
		delete qi;
	}

	void interruptHandler(QueuedInterruptMessage queuedInterruptMessage)
	{
		// This is a class function called from interrupt using queue
		Serial.printf("Class function interrupt on pin %d, at %d, pinvalue = %d\r\n",
							queuedInterruptMessage.interruptPin,
							queuedInterruptMessage.interruptMicros,
							queuedInterruptMessage.interruptPinValue);
	}
private :
	int interruptPin;
	QueuedInterrupt* qi;
};

void handleInterrupt(QueuedInterruptMessage queuedInterruptMessage)
{
	// This is a static function called from interrupt using queue
	Serial.printf("Class function interrupt on pin %d, at %d, pinvalue = %d\r\n",
						queuedInterruptMessage.interruptPin,
						queuedInterruptMessage.interruptMicros,
						queuedInterruptMessage.interruptPinValue);
}

#define GPIO2 2
#define GPIO4 4

InterruptServer* GPIO2_Server;
QueuedInterrupt* GPIO4_Server;

Timer t1;

void init()
{
	Serial.begin(SERIAL_BAUD_RATE);
	GPIO2_Server = new InterruptServer(GPIO2);
	GPIO4_Server = new QueuedInterrupt(GPIO4,(GPIO_INT_TYPE)GPIO_PIN_INTR_ANYEDGE,handleInterrupt);

}

