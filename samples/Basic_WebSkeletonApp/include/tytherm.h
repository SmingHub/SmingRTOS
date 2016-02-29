#ifndef INCLUDE_TYTHERM_H_
#define INCLUDE_TYTHERM_H_
#include <configuration.h>
#include <SmingCore.h>

extern unsigned long counter; // Kind of heartbeat counter

const uint8_t ConfigJsonBufferSize = 200; // Application configuration JsonBuffer size ,increase it if you have large config
const uint16_t ConfigFileBufferSize = 2048; // Application configuration FileBuffer size ,increase it if you have large config

//Webserver
void startWebServer();

#endif /* INCLUDE_HEATCONTROL_H_ */
