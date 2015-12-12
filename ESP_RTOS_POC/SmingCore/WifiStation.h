/*
 * WifiStation.h
 *
 *  Created on: 3 dec. 2015
 *      Author: Herman
 */

#ifndef USER_WIFISTATION_H_
#define USER_WIFISTATION_H_

#include "esp_common.h"

class WifiStation {
public:
	WifiStation();
	virtual ~WifiStation();

	void config(char* SSID, char* PWD);
};

extern WifiStation wfStation;

#endif /* USER_WIFISTATION_H_ */
