#include <user_config.h>
#include <SmingCore.h>

#include "Arduino_I2C_Port_Expander.h"
#include <libraries/Adafruit_SSD1306/Adafruit_SSD1306.h>

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

Adafruit_SSD1306 display(16);

void Demo2()
{
	Serial.println("Display: some text");
	display.clearDisplay();
	// text display tests
	display.setTextSize(1);
	display.setTextColor(WHITE);
	display.setCursor(0,0);
	display.println("Sming Framework");
	display.setTextColor(BLACK, WHITE); // 'inverted' text
	display.setCursor(104, 7);
	display.println("v1.0");
	//----
	display.setTextColor(WHITE);
	display.println("Let's do smart things");
	display.setTextSize(3);
	display.print("IoT");
	display.display();
}

void Demo1()
{
        Serial.println("Display: circle");
	// Clear the buffer.
	display.clearDisplay();
	// draw a circle, 10 pixel radius
	display.fillCircle(display.width()/2, display.height()/2, 10, WHITE);
	display.display();
}



EXPAND Arduino(3);

EXPAND A(2);



void blink()
{
	state = !state;

	if (state) {Demo1();} else {Demo2();}
	//	Arduino = new EXPAND(3);
//	Wire.beginTransmission(3);
//	int error = Wire.endTransmission();
//	Serial.printf("Errorcode = %d\r\n", error);
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
	Wire.begin(4,5);

//	scanBus();
	Serial.println("Display: start");
	display.begin(SSD1306_SWITCHCAPVCC);
	display.display();

	procTimer.initializeMs(1000, blink).start();
}
