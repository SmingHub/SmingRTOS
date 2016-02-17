#include <user_config.h>
#include <SmingCore.h>

#define LED_PIN 2   // GPIO2
#define INT_PIN 4	// GPIO4

bool ledState = false;
volatile int pinValue;


void IRAM_ATTR interruptHandler()
{
	pinValue = digitalRead(INT_PIN);
	Serial.printf("Interupt on %d, value = %d\r\n", INT_PIN, pinValue);
	ledState = !ledState; digitalWrite(LED_PIN,ledState);
}

void init()
{
	Serial.begin(SERIAL_BAUD_RATE); // 115200 or 9600 by default
	pinMode(LED_PIN, OUTPUT);
	attachInterrupt(INT_PIN, interruptHandler, (GPIO_INT_TYPE)GPIO_PIN_INTR_ANYEDGE);

}
