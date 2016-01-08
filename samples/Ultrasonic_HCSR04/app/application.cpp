#include <SmingCore.h>
#include <libraries/Ultrasonic/Ultrasonic.h>

#define TRIG_PIN 4
#define ECHO_PIN 5

Timer measureTimer;
Ultrasonic ultrasonic = Ultrasonic();

void measure()
{
	// get distance
	uint16_t dist = ultrasonic.rangeCM();

	// print the distance
	Serial.println(dist);
}

void init()
{
	Serial.begin(115200);

	ultrasonic.begin(TRIG_PIN, ECHO_PIN);
	measureTimer.initializeMs(500,measure).start();

}
