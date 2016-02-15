/*
 * File: Esp SDK Hardware PWM demo
 * Original Author: https://github.com/hrsavla
 *
 * This HardwarePWM library enables Sming framework user to use ESP SDK PWM API
 * Period of PWM is fixed to 1000ms / Frequency = 1khz
 * Duty at 100% = 22222. Duty at 0% = 0
 * You can use function setPeriod() to change frequency/period.
 * Calculate the Duty as per the formulae give in ESP8266 SDK
 * Duty = (Period *1000)/45
 *
 * PWM can be generated on upto 8 pins (ie All pins except pin 16)
 * Created on August 17, 2015, 2:27 PM
 */
#include <user_config.h>
#include <SmingCore.h>

uint8_t pins[8] = { 4, 5, 0, 2, 15, 13, 12, 14 }; // List of pins that you want to connect to pwm
HardwarePWM HW_pwm(pins, 8);

Timer procTimer;
int32 i = 0;
int32 inc = 1000;
bool countUp = true;
void doPWM() {
	if (countUp == true) {
		i += inc;
		if (i >= PWM_MAXDUTY) {
			i = PWM_MAXDUTY;
			countUp = false;
		}
	} else {
		i -= inc;
		if (i <= 0) {
			i = 0;
			countUp = true;
		}
	}
	HW_pwm.analogWrite(4, i);  // restart is default value
}

void init() {
	Serial.begin(SERIAL_BAUD_RATE); // 115200 by default
	Serial.systemDebugOutput(true); // Enable debug output to serial

	// WIFI not needed for demo. So disabling WIFI.
	WifiStation.enable(false);
	WifiAccessPoint.enable(false);

	// Setting PWM values on 8 different pins // only start when all are set
	HW_pwm.analogWrite(4, 1023, false);
	HW_pwm.analogWrite(5, 512, false);
	HW_pwm.analogWrite(0, 256, false);
	HW_pwm.analogWrite(2, 768, false);
	HW_pwm.analogWrite(15, 128, false);
	HW_pwm.analogWrite(13, 500, false);
	HW_pwm.analogWrite(12, 900, false);
	HW_pwm.analogWrite(14, 1000,false);

	HW_pwm.restart();

	debugf("PWM output set on all 8 Pins. Kindly check...");
	debugf("Now Pin 2 will go from 0 to VCC to 0 in cycles.");
	procTimer.initializeMs(100, doPWM).start();
}
