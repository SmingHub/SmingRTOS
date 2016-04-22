/*
Author: (github.com/)ADiea
Project: Sming for ESP8266 - https://github.com/anakod/Sming
License: MIT
Date: 15.07.2015
Descr: Low-level SDCard functions
*/
#ifndef _SD_CARD_
#define _SD_CARD_

#include "../../sming/include/SmingIncludes.h"

void SDCard_begin(uint8 PIN_CARD_SS);

extern SPIBase	*SDCardSPI;

#endif /*_SD_CARD_*/
