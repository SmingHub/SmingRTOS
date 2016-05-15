/*
For detailed instructions. Please visit this project on Github.
https://github.com/jaretburkett/Arduino-I2C-Port-Expander

Upload this code onto an Arduino Compatible microcontroller through
the Arduino IDE. Connect to master GND, VCC, SCL, SDA. Be sure to use 
pullup resistors on the I2C lines. View the master example to see how to 
communicate to your new I2C port expander. 

Be sure to make the slave id compatible with your master.

This code is released under a MIT license. 
Created by Jaret Burkett
*/

// Make this unique for each device on the same I2C bus
// can be 0x01 - 0xff
const uint8_t SlaveDeviceId = 0x03;

uint16_t receivedPacket[10]; 
#include <Wire.h>

byte buffer[2];

uint16_t returninfo;

void setup(){
	Wire.begin(SlaveDeviceId);      // join i2c bus with Slave ID
	Wire.onReceive(receiveDataPacket); // register talk event
	Wire.onRequest(slavesRespond);  // register callback event
  Serial.begin(57600);
}

void loop(){  

// All is done within callback functions
}

void receiveDataPacket(int howMany){
	// get bytes from i2c
	for(byte i=0; i < howMany; i++){
		receivedPacket[i] = Wire.read();
  }
    Serial.print("Received packet");
    Serial.println(receivedPacket[0]);
   
   /* 
      Handle write only requests
      receivedPacket[0] = method
   */

    switch(receivedPacket[0]){
  
    case 1:  // digitalWrite
    /*
    receivedPacket[1] = pin
    receivedPacket[2] = value
    */
    digitalWrite(receivedPacket[1], receivedPacket[2]);
    break;

    case 4:  // analogWrite
    /*
    receivedPacket[1] = pin
    receivedPacket[2] = value
    */
    analogWrite(receivedPacket[1], receivedPacket[2]);
    break;

    case 6:  // pinMode
    /*
    receivedPacket[1] = pin
    receivedPacket[2] = mode
    */
    pinMode(receivedPacket[1], receivedPacket[2]); // set pin mode
    break;
    }

}
 
void slavesRespond(){

	  switch(receivedPacket[0]){

		case 2:  // digitalRead
		  /*
			receivedPacket[1] = pin
		  */
		  returninfo = digitalRead(receivedPacket[1]);
		  break;
		case 5:   // analogRead
		  /*
			receivedPacket[1] = pin
		  */
      returninfo = analogRead(receivedPacket[1]);
		  break;
    case 7:   // status
      /*
      receivedPacket[1] = value
      */
      returninfo = receivedPacket[1];
      break;
  	}
	
    buffer[0] = returninfo >> 8;
    buffer[1] = returninfo & 0xff;
    Wire.write(buffer,2);
  
}
 
