#include <user_config.h>
#include <SmingCore.h>

#include "Arduino_I2C_Port_Expander.h"

#define LED_PIN 2 // GPIO2

Timer procTimer;
bool state = true;

void scanBus()
{
  byte error, address;
  int nDevices;

  Serial.println("Scanning...");

  nDevices = 0;
  for(address = 1; address < 127; address++ )
  {
	// The i2c_scanner uses the return value of
	// the Write.endTransmisstion to see if
	// a device did acknowledge to the address.
	Wire.beginTransmission(address);
	error = Wire.endTransmission();

	if (error == 0)
	{
	  Serial.print("I2C device found at address 0x");
	  if (address<16)
		Serial.print("0");
	  Serial.print(address,HEX);
	  Serial.println("  !");

	  nDevices++;
	}
	else if (error==4)
	{
	  Serial.print("Unknow error at address 0x");
	  if (address<16)
		Serial.print("0");
	  Serial.println(address,HEX);
	}
  }
  if (nDevices == 0)
	Serial.println("No I2C devices found\n");
  else
	Serial.println("done\n");
}


EXPAND Arduino(3);



void blink()
{
	state = !state;
//	Arduino = new EXPAND(3);
	Wire.beginTransmission(3);
	int error = Wire.endTransmission();
	Serial.printf("Errorcode = %d\r\n", error);
	uint16_t val = Arduino.analogRead(24);
	Serial.printf("Val = %d\r\n",val);
//	delete Arduino;
/*
	if (state){
		Arduino.digitalWrite(13,HIGH);
	}
	else
	{
		Arduino.digitalWrite(13,LOW);
	}
*/
}

void init()
{
	Serial.begin(74880);
//	Wire.pins(4,5);
	Wire.begin();

	scanBus();

	procTimer.initializeMs(1000, blink).start();
}
