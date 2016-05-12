#ifndef EXPANDUINO_H
#define EXPANDUINO_H

#include "SmingCore.h"

/*
#if ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif
*/
//#include <Wire.h>

class EXPAND {
  public:
    EXPAND(uint8_t addr);
    ~EXPAND();
    void digitalWrite(byte pin,byte val);
    int digitalRead(byte pin);
    int digitalReadPullup(byte pin);
    void analogWrite(byte pin,byte val);
    int analogRead(byte pin);
	void touchscreenOn();
	int touchscreenReadX();
	int touchscreenReadY();
	int touchscreenReadZ();
    void sendDataPacket();
    int receiveResponse();
	
  private:
    uint8_t _addr;
};


#endif

