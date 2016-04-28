#include <user_config.h>
#include <SmingCore.h>


// Will be called when WiFi station was connected to AP
void connectOk()
{
	printf("I'm CONNECTED\n");
}

// Will be called when WiFi station timeout was reached
void connectFail()
{
	printf("I'm NOT CONNECTED. Need help :(\n");
	// .. some you code for device configuration ..
}

void smartconfig_done(sc_status status, void *pdata)
{
    switch(status) {
        case SC_STATUS_WAIT:
            printf("SC_STATUS_WAIT\n");
            break;
        case SC_STATUS_FIND_CHANNEL:
            printf("SC_STATUS_FIND_CHANNEL\n");
            break;
        case SC_STATUS_GETTING_SSID_PSWD: {
				printf("SC_STATUS_GETTING_SSID_PSWD\n");
				sc_type *type = (sc_type *) pdata;
				if (*type == SC_TYPE_ESPTOUCH) {
					printf("SC_TYPE:SC_TYPE_ESPTOUCH\n");
				} else {
					printf("SC_TYPE:SC_TYPE_AIRKISS\n");
				}
				break;
        	}
        case SC_STATUS_LINK: {
				printf("SC_STATUS_LINK\n");
				struct station_config *sta_conf = (station_config *) pdata;

				printf("Trying to connect to %s\n", sta_conf->ssid);
				printf("Using password to %s\n", sta_conf->password);

				wifi_station_set_config(sta_conf);
				wifi_station_disconnect();
				wifi_station_connect();

				// Run our method when station was connected to AP (or not connected)
				WifiStation.waitConnection(connectOk, 20, connectFail); // We recommend 20+ seconds for connection timeout at start

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


void smartconfig_task(void *pvParameters)
{
    smartconfig_start(smartconfig_done);

    vTaskDelete(NULL);
}


void init()
{

	Serial.begin(115200); // 115200 by default
	Serial.systemDebugOutput(false); // Allow debug output to serial

	WifiAccessPoint.enable(false);
	WifiStation.enable(true);

    printf("SDK version:%s\n", system_get_sdk_version());

    /* need to set opmode before you set config */
    wifi_set_opmode(STATION_MODE);

    xTaskCreate(smartconfig_task, (const signed char*) "smartconfig_task", 256, NULL, 2, NULL);

}
