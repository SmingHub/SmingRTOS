#include <user_config.h>
#include <SmingCore.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "espressif/smartconfig.h"


#define LED_PIN 2 // GPIO2

Timer procTimer;
bool state = true;

#define server_ip "192.168.101.142"
#define server_port 9669

void ICACHE_FLASH_ATTR
smartconfig_done(sc_status status, void *pdata)
{
    switch(status) {
        case SC_STATUS_WAIT:
            printf("SC_STATUS_WAIT\n");
            break;
        case SC_STATUS_FIND_CHANNEL:
            printf("SC_STATUS_FIND_CHANNEL\n");
            break;
        case SC_STATUS_GETTING_SSID_PSWD:
        	{
				printf("SC_STATUS_GETTING_SSID_PSWD\n");
				sc_type *type = (sc_type *) pdata;
				if (*type == SC_TYPE_ESPTOUCH) {
					printf("SC_TYPE:SC_TYPE_ESPTOUCH\n");
				} else {
					printf("SC_TYPE:SC_TYPE_AIRKISS\n");
				}
				break;
        	}
        case SC_STATUS_LINK:
			{
				printf("SC_STATUS_LINK\n");
				struct station_config *sta_conf = (station_config *) pdata;

				wifi_station_set_config(sta_conf);
				wifi_station_disconnect();
				wifi_station_connect();
				break;
			}
        case SC_STATUS_LINK_OVER:
            printf("SC_STATUS_LINK_OVER\n");
            if (pdata != NULL) {
                uint8 phone_ip[4] = {0};

                memcpy(phone_ip, (uint8*)pdata, 4);
                printf("Phone ip: %d.%d.%d.%d\n",phone_ip[0],phone_ip[1],phone_ip[2],phone_ip[3]);
            }
            smartconfig_stop();
            break;
    }

}


void ICACHE_FLASH_ATTR
smartconfig_task(void *pvParameters)
{
    smartconfig_start(smartconfig_done);

    vTaskDelete(NULL);
}


void init()
{

	Serial.begin(115200); // 115200 by default
	Serial.systemDebugOutput(true); // Allow debug output to serial

    printf("SDK version:%s\n", system_get_sdk_version());

    /* need to set opmode before you set config */
    wifi_set_opmode(STATION_MODE);

    xTaskCreate(smartconfig_task, (const signed char*) "smartconfig_task", 256, NULL, 2, NULL);


}
