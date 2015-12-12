/******************************************************************************
 * Copyright 2013-2014 Espressif Systems (Wuxi)
 *
 * FileName: user_main.c
 *
 * Description: entry file of user application
 *
 * Modification history:
 *     2014/12/1, v1.0 create this file.
*******************************************************************************/


//#include "../SmingCore/WifiStation.h"
#include "esp_common.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "lwip/sockets.h"
#include "lwip/dns.h"
#include "lwip/netdb.h"

//#include "../system/include/lwip_includes.h"


//#include "Station.h"


//#include "c_types_compatible.h"

#include "lwip/tcp.h"


char mydata[1024];

static void close_conn(struct tcp_pcb *pcb){
      tcp_arg(pcb, NULL);
      tcp_sent(pcb, NULL);
      tcp_recv(pcb, NULL);
      tcp_close(pcb);
}


static err_t staticOnSent(void *arg, tcp_pcb *tcp, uint16_t len)
{
	printf("OnSent called len = %d\r\n",len);
}

static err_t echo_recv(void *arg, struct tcp_pcb *pcb, struct pbuf *p, err_t err){
      int i;
      int len;
      char *pc;
      int sndlen;

      printf("echo recv, sendbuf = %d\r\n",tcp_sndbuf(pcb));

      if (err == ERR_OK && p != NULL) {
            /* Inform TCP that we have taken the data. */
            tcp_recved(pcb, p->tot_len);

            //pointer to the pay load
            pc=(char *)p->payload;

            //size of the pay load
            len =p->tot_len;

            //copy to our own buffer
            for (i=0; i<len; i++)mydata[i]= pc[i];

             //Close TCP when receiving "X"
            if (mydata[0]=='X')close_conn(pcb);

           //Free the packet buffer
            pbuf_free(p);

            printf("Received %d\r\n",len);

            //check output buffer capacity
//            if (len >tcp_sndbuf(pcb)) len= tcp_sndbuf(pcb);

            printf("Sending %d, sndbuf = %d\r\n",len,tcp_sndbuf(pcb));

            //Send out the data
            err = tcp_write(pcb, mydata, len, 0);
            tcp_sent(pcb, staticOnSent); /* No need to call back */
      } else {
            pbuf_free(p);
      }

      if (err == ERR_OK && p == NULL) {
            close_conn(pcb);
      }
      return ERR_OK;
}

static err_t echo_accept(void *arg, struct tcp_pcb *pcb, err_t err){

	printf("Echo accepst, sndbuf = %d\r\n",tcp_sndbuf(pcb));
		LWIP_UNUSED_ARG(arg);
      LWIP_UNUSED_ARG(err);
      tcp_setprio(pcb, TCP_PRIO_MIN);
      tcp_recv(pcb, echo_recv);
      tcp_err(pcb, NULL); //Don't care about error here
      tcp_poll(pcb, NULL, 4); //No polling here
      return ERR_OK;
}


void vBasicTFTPServer(void *pvParameters){
//	portTASK_FUNCTION( vBasicTFTPServer, pvParameters ){
	      struct tcp_pcb *ptel_pcb;
	      struct tcp_pcb *recv_pcb;
	      ptel_pcb = tcp_new();
	      printf("After tcpnew %d\r\n",tcp_sndbuf(ptel_pcb));
	      tcp_bind(ptel_pcb, IP_ADDR_ANY, 8000);
	      printf("After bind %d\r\n",tcp_sndbuf(ptel_pcb));
          recv_pcb = tcp_listen(ptel_pcb);
          printf("After listen %d",tcp_sndbuf(recv_pcb));
          tcp_accept(recv_pcb, echo_accept);
          printf("After accept %d",tcp_sndbuf(recv_pcb));

	      while (1){

	    	  vTaskDelay(1000);
	      }
	}

void init()
{
//	taskENABLE_INTERRUPTS();


    printf("Buf size = %d\r\n",TCP_SND_BUF);
    printf("Queue length = %d\r\n",TCP_SND_QUEUELEN);



    //	textTimer.initializeMs(10000, DemoDelegate(&DelegateDemo::callDelegate,&delegateDemo)).start();
/*
	WifiStation.enable(true);
	WifiStation.configWIFI_SSID, WIFI_PWD);
	WifiStation.waitConnection(connectOk);
*/
}


/******************************************************************************
 * FunctionName : user_init
 * Description  : entry of user application, init user function here
 * Parameters   : none
 * Returns      : none
*******************************************************************************/






extern "C" void user_init(void)
{
    printf("SDK version:%s\n", system_get_sdk_version());

    init();
//    wifi_set_opmode(NULL_MODE);
//    wifi_set_opmode(STATIONAP_MODE);


//    wfStation.config((char*)"FRITZ!Box Fon WLAN 7360",(char*)"00163262000955166948");
/*
    {
        struct station_config *config = (struct station_config *)zalloc(sizeof(struct station_config));
        sprintf((char*)config->ssid, WIFI_SSID);
        sprintf((char*)config->password,WIFI_PWD);
        wifi_station_set_config(config);
        free(config);
    }
*/
//    xTaskCreate(task1, (signed char *)"tsk1", 256, NULL, 2, NULL);
//    xTaskCreate(task2,(signed char *) "tsk2", 256, NULL, 2, NULL);
    xTaskCreate(vBasicTFTPServer,(signed char *) "TFTF", 256, NULL, 2, NULL);
}
/*
extern "C" void user_init(void)
{
    uart_set_baud(0, 115200);
    printf("SDK version:%s\n", sdk_system_get_sdk_version());
    xTaskCreate(task1, (signed char *)"tsk1", 256, NULL, 2, NULL);
}

*/
