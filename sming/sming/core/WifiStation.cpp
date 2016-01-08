/*
 * WifiStation.cpp
 *
 *  Created on: 3 dec. 2015
 *      Author: Herman
 */

#include "WifiStation.h"

WifiStation::WifiStation() {
	// TODO Auto-generated constructor stub

}

WifiStation::~WifiStation() {
	// TODO Auto-generated destructor stub
}

void WifiStation::config(char* SSID, char* PWD)
{
    struct station_config *config = (struct station_config *)zalloc(sizeof(struct station_config));
    sprintf((char*)config->ssid, SSID);
    sprintf((char*)config->password,PWD);

    /* need to sure that you are in station mode first,
     * otherwise it will be failed. */
    wifi_station_set_config(config);
    free(config);

}

WifiStation wfStation;

