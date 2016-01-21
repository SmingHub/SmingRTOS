#ifndef __RBOOT_SMING_H__
#define __RBOOT_SMING_H__

//////////////////////////////////////////////////
// rBoot OTA and config API for ESP8266.
// Copyright 2015 Richard A Burton
// richardaburton@gmail.com
// See license.txt for license terms.
// OTA code based on SDK sample from Espressif.
//////////////////////////////////////////////////

#include <rboot.h>

#ifdef __cplusplus
extern "C" {
#endif

#include "appcode/rboot-api.h"
#include "rboot-private.h"

uint32 rboot_check_image(uint32 startaddress);

#ifdef __cplusplus
}
#endif

#endif
