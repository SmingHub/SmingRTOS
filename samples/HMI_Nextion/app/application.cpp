#include <SmingCore.h>
#include <libraries/Nextion/Nextion.h>



/*
 * Declare a button object [page id:0,component id:1, component name: "b0"].
 */
NexButton b0 = NexButton(0, 1, "b0");

char buffer[100] = {0};

/*
 * Register a button object to the touch event list.
 */
NexTouch *nex_listen_list[] =
{
    &b0,
    NULL
};

/*
 * Button component pop callback function.
 * In this example,the button's text value will plus one every time when it is released.
 */
void b0PopCallback(void *ptr)
{
    uint16_t len;
    uint16_t number;
    NexButton *btn = (NexButton *)ptr;
    dbSerialPrintln("b0PopCallback");
    dbSerialPrint("ptr=");
    dbSerialPrintln((uint32_t)ptr);
    memset(buffer, 0, sizeof(buffer));

    /* Get the text value of button component [the value is string type]. */
    btn->getText(buffer, sizeof(buffer));

    number = atoi(buffer);
    number += 1;

    memset(buffer, 0, sizeof(buffer));
    itoa(number, buffer, 10);

    /* Set the text value of button component [the value is string type]. */
    btn->setText(buffer);
}




void IRAM_ATTR nextionLoop(void *pvParameters ){
	while(1){
		nexLoop(nex_listen_list);
	vTaskDelay(10);
	}
}


void init()
{

	Serial.begin(115200); // 115200 by default
	Serial.systemDebugOutput(false); // Disable debug output to serial

	Serial.print("\nNextion demo will start\n");
	system_uart_swap();
	nexInit();

	/* Register the pop event callback function of the current button component. */
	b0.attachPop(b0PopCallback, &b0);

	xTaskCreate(nextionLoop, (const signed char*)"nextionLoop", 256*2, NULL, tskIDLE_PRIORITY, NULL);

}



