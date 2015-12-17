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


#include "../SmingCore/WifiStation.h"
#include "esp_common.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

//#include "lwip/sockets.h"
//#include "lwip/dns.h"
//#include "lwip/netdb.h"

//#include "../system/include/lwip_includes.h"

#include "../SmingCore/Network/UdpConnection.h"
#include "../SmingCore/Network/TcpServer.h"
#include "../SmingCore/Network/TcpClient.h"
#include "../SmingCore/Network/FtpServer.h"
#include "../SmingCore/Network/HttpServer.h"
#include "../SmingCore/Network/TelnetServer.h"
#include "../SmingCore/Temp_HardwareSerial.h"

#include "../SmingCore/FileSystem.h"

#define __forceinline __attribute__((always_inline)) inline

#include "ESP8266EX.h"
#include "Digital.h"
#include "Delegate.h"

#include "../RTOS_EC/cqueue.h"
#include "../RTOS_EC/ctask.h"
#include "../Wiring/WString.h"
#include "../Wiring/Stream.h"
#include "../Wiring/WVector.h"
#include "../Wiring/IPAddress.h"

#include "Timer.h"
//#include "Station.h"


//#include "c_types_compatible.h"

#define server_ip "192.168.101.142"
#define server_port 9669

typedef Delegate<void()> DemoDelegate;

class DelegateDemo
{
public :
	void setDelegate(DemoDelegate reqDelegate)
	{
	  myDemoDelegate = reqDelegate;
	}

	void callDelegate()
	{
	  printf("testDelegate called\r\n");
	  if (myDemoDelegate)
	  {
		  myDemoDelegate();
		  printf("Delegated\r\n");
	  }
	  else
	  {
		  printf("No delegation defined\r\n");
	  }
    }


private :
	DemoDelegate myDemoDelegate;

};


class Counter
{
private:
  uint32_t _count;
public:

  Counter(uint32_t initial_count)
  {
    this->_count = initial_count;
    printf("Counter initialised with count %d\r\n", initial_count);
  }

  void Increment()
  {
    _count++;
  }

  uint32_t getCount()
  {
    return _count;
  }

};

static Counter static_counter(99);

void task1(void *pvParameters)
{
  Counter local_counter = Counter(12);
  Counter *new_counter = new Counter(24);
  while(1) {
    Counter *counter = NULL;
    switch(rand() % 3) {
    case 0:
      counter = &local_counter;
      break;
    case 1:
      counter = &static_counter;
      break;
    default:
      counter = new_counter;
      break;
    }
    counter->Increment();
    printf("local counter %d static counter %d newly allocated counter %d\r\n", local_counter.getCount(),
	   static_counter.getCount(), new_counter->getCount());
    vTaskDelay(100);
  }
}


void blink();

void task2(void *pvParameters)
{
    printf("Hello, welcome to client!\r\n");


    while (1) {
    	int count;
    	printf("Message from task2 %d\r\n",count++);
        vTaskDelay(4000 / portTICK_RATE_MS);

    }
}

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

	portTASK_FUNCTION( vBasicTFTPServer, pvParameters ){
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

void procTask(void *pvParameters)
{
    while (1) {

    }
}


#define LED_PIN 2 // GPIO2

//Timer procTimer;
bool state = true;

void blink()
{
	digitalWrite(LED_PIN, state);
	state = !state;
//	printf("Blinking state = %s\r\n", state ? "True":"False");
}


Vector<String> test;

void globalDelegate()
{
	printf("Global Delegate called\r\n");
}

Timer blinkTimer;
Timer textTimer;

DelegateDemo delegateDemo;

TcpClient tc(false);


void onReceive(UdpConnection& connection, char *data, int size, IPAddress remoteIP, uint16_t remotePort); // Declaration


// UDP server
const uint16_t EchoPort = 1234;
UdpConnection udp(onReceive);

TcpServer tserver;


void onReceive(UdpConnection& connection, char *data, int size, IPAddress remoteIP, uint16_t remotePort)
{
	debugf("UDP Sever callback from %s:%d, %d bytes", remoteIP.toString().c_str(), remotePort, size);

	// We implement string mode server for example
//	Serial.print(">\t");
//	Serial.print(data);

	printf(">\t");
	printf(data);
	printf("\r\n");

	String recvData = data;
	if (recvData == "start")
	{
		printf("procTask starting\r\n");
		xTaskCreate(procTask, (signed char *)"procTask", 256, NULL, 2, NULL);
		printf("procTask started\r\n");
	}
	else
	{
		printf("Task not started\r\n");
	}

	if (recvData == "list")
	{
//		char buf [300];
//		vTaskList((signed char*)buf);
		printf("Number of tasks = %d\r\n",uxTaskGetNumberOfTasks());
	}
	if (recvData == "connect")
	{
	    tc.connect(IPAddress("1.0.0.219"),1000);
	    String s = "This is the message";
	    tc.sendString(s,true);
	    printf("connected\r\n");
	}




	// Send echo to remote sender
	String text = String("echo: ") + data;
//	udp.sendStringTo(remoteIP, EchoPort, text);
}

CTask task;

void tcpServerClientConnected (TcpClient* client)
{
	debugf("Application onClientCallback : %s\r\n",client->getRemoteIp().toString().c_str());
}

bool tcpServerClientReceive (TcpClient& client, char *data, int size)
{
	debugf("Application DataCallback : %s, %d bytes \r\n",client.getRemoteIp().toString().c_str(),size );
	debugf("Data : %s", data);
	client.sendString("sendString data\r\n", false);
	client.writeString("writeString data\r\n",0 );
	client.flush();
	if (strcmp(data,"close") == 0)
	{
		debugf("Closing client");
		client.close();
	};
	return true;
}



void tcpServerClientComplete(TcpClient& client, bool succesfull)
{
	debugf("Application CompleteCallback : %s \r\n",client.getRemoteIp().toString().c_str() );
}


void onIndex(HttpRequest &request, HttpResponse &response)
{
	debugf("HTTP OnIndex\r\n");
	response.forbidden();
}

void onDefault(HttpRequest &request, HttpResponse &response)
{
	debugf("HTTP OnDefault\r\n");
	response.notFound();
}


TcpServer tcpServer(tcpServerClientConnected, tcpServerClientReceive, tcpServerClientComplete);



HttpServer http;

TelnetServer tnet;

FTPServer ftp;

void connectOk()
{
	printf("I'm CONNECTED\r\n");

//	startFTP();
//	startWebServer();

}

IPAddress ip;


void init()
{
	taskENABLE_INTERRUPTS();
	pinMode(LED_PIN, OUTPUT);
	blinkTimer.initializeMs(1000, blink).start();

	char buf[1000];
//	vTaskList( (signed char*)buf );


//	spiffs_F_manual(0x40500000, 65536);

	CQueue myQueue;

    String help ="text";
    const char* z = help.c_str();

    printf("This is helptekst %s",help.c_str());

	udp.listen(EchoPort);
//	tserver.listen(8080);
//	tcpServer.listen(80);

	http.listen(80);
	http.addPath("/", onIndex);
	http.setDefaultHandler(onDefault);

	tnet.listen(23);

    int i = TCP_SND_BUF;


    Serial.printf("Mounting FS\r\n");

    spiffs_mount_manual(0x40500000, 65536);

    Serial.printf("Mounted\r\n");

    spiffs_format_manual(0x40500000, 65536);

    Serial.printf("Formatted\r\n");

    if (!fileExist("index.html"))
    {
    	debugf("creating index file");
		fileSetContent("index.html", "<h3>Please connect to FTP and upload files from folder 'web/build' (details in code)</h3>");
		debugf("index created");
    }
    debugf("opening file");

    int fd = fileOpen ("index.html",eFO_ReadOnly);

    Serial.printf("fd after file open : %d\r\n",fd);


	// Start FTP server
	ftp.listen(21);
	ftp.addUser("me", "123"); // FTP account



	delegateDemo.callDelegate();
    delegateDemo.setDelegate(globalDelegate);
    delegateDemo.callDelegate();

    printf("Buf size = %d\r\n",TCP_SND_BUF);
    printf("Queue length = %d\r\n",TCP_SND_QUEUELEN);

    Serial.println("This is a Serial line");
    Serial.printf("This is the printf command\r\n");





    //	textTimer.initializeMs(10000, DemoDelegate(&DelegateDemo::callDelegate,&delegateDemo)).start();
/*
	WifiStation.enable(true);
	WifiStation.config(WIFI_SSID, WIFI_PWD);
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
    wifi_set_opmode(STATIONAP_MODE);


    wfStation.config((char*)WIFI_SSID,(char*)WIFI_PWD);
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
