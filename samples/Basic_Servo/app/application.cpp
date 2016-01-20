/**
 * Basic Servo test application
 * A simplied servo demo based on the classical arduino Sweep
 *
 * v1.0 Robotiko
 *
*/

#include <user_config.h>
#include <libraries/Servo/ServoChannel.h>


#define SERVO_PIN 2 // GPIO2

#define SERVO_DELAY 20 //Delay in ms  between calls to new servo position

ServoChannel *servoChannel; //Servo instance

Timer servoTimer; //Timer to call servo with new positions.



int degree=0; //initial position
bool countUp = true;//Initial movement direction


/*
 * This function is called by the timer to compute next position and move the servo.
 * It moves the servo from one side to the other and back (sweep) continuously
 * Min/Max values [-90,90] are servo library constrains but can also work with precise timings
 */
void doSweep()
{

	if(countUp){ //Check movement direction
		degree++;
		if(degree == 90){ //If reached max position, change direction
			countUp = false;
		}
	}
	else{
		degree--;
		if(degree == -90){ //If reached max position, change direction
			countUp = true;
		}
	}

	servoChannel->setDegree(degree); //Move the servo
	Serial.println(degree); //Output current servo position
}

void init()
{
	Serial.println("Init Basic Servo Sample");
	System.setCpuFrequency(eCF_80MHz);

	servoChannel = new(ServoChannel); //Create the servo channel instance
	servoChannel->attach(SERVO_PIN); //attach the servo channel to the servo gpio

	servoTimer.initializeMs(SERVO_DELAY, doSweep).start();//Lest call the servo movement

}

