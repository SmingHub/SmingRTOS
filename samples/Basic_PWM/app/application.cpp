#include <Sming/SmingCore.h>

#define LED_PIN 2 // GPIO12

DriverPWM ledPWM;
Timer procTimer;

int i = 0;
bool countUp = true;
bool countDown = false;

void doPWM()
{
	if(countUp){
		i=i+100;
		if(i == 1000){
			countUp = false;
			countDown = true;
		}
	}
	else{
		i=i-100;
		if(i == 0){
			countUp = true;
			countDown = false;
		}
	}
	Serial.println(i);
	ledPWM.analogWrite(LED_PIN, i);
}

void init()
{
	Serial.begin(115200);
	ledPWM.initialize();
	procTimer.initializeMs(100, doPWM).start();
}

