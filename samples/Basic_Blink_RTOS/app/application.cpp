#include <user_config.h>
#include <SmingCore.h>


#define LEDB_PIN 2 // GPIO2
#define LEDR_PIN 15 // GPIO15


bool state = true;
bool stateR = true;

//
//  actualy "portTICK_RATE_MS"  (base RTOS tick for ESP8266) is set to 10 ms
//

//  ******************************
//  ledB task (our first "program")
//  ******************************
void ledB_task(void *pvParameters)
{

  pinMode(LEDB_PIN, OUTPUT);
  for(;;)
  {
	digitalWrite(LEDB_PIN, state);
	state = !state;
	//
	// Perform other action here.
	//
	
     vTaskDelay(500/portTICK_RATE_MS);   //Pause operation for 50 ticks (500 ms)
	                      //create opportunities to launch another task with the same priority
  }

}
//  *******************************
//  ledR task (our second "program")
//  *******************************
void ledR_task(void *pvParameters)
{

  pinMode(LEDR_PIN, OUTPUT);
  while(1)
  {
	digitalWrite(LEDR_PIN, stateR);
	stateR = !stateR;
	//
	// Perform other action here.
	//
	
	vTaskDelay(250/portTICK_RATE_MS);   //Pause operation for 25 ticks (250 ms)
						  //create opportunities to launch another task with the same priority
  }

}


void init()
{

	//  The creation of two task  with the same priority
	xTaskCreate(ledB_task, (const signed char*)"ledB_task", 256, NULL, 2, NULL);
	xTaskCreate(ledR_task, (const signed char*)"ledR_task", 256, NULL, 2, NULL);
}
