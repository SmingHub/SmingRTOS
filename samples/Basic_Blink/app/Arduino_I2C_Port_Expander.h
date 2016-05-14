#ifndef EXPANDUINO_H
#define EXPANDUINO_H

#include "SmingCore.h"

class EXPAND {
  public:
    EXPAND(uint8_t addr);
    ~EXPAND();
    void digitalWrite(byte pin,byte val);
    int digitalRead(byte pin);
    void analogWrite(byte pin,byte val);
    int analogRead(byte pin);
    void pinMode(byte mode);
    int status(byte reqValue);

  private:
    uint8_t _addr;
    byte io_DataPacket[3];
    byte io_pin;
    byte io_method;
    byte io_value;
    void sendDataPacket(int reqBytes);
    int receiveResponse();
};


#endif

