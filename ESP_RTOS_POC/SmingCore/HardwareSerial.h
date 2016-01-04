/****
 * Sming Framework Project - Open Source framework for high efficiency native ESP8266 development.
 * Created 2015 by Skurydin Alexey
 * http://github.com/anakod/Sming
 * All files of the Sming Core are provided under the LGPL v3 license.
 ****/

#ifndef _HARDWARESERIAL_H_
#define _HARDWARESERIAL_H_

//#include "../Wiring/WiringFrameworkDependencies.h"
#include "../Wiring/Stream.h"
#include "../SmingCore/Delegate.h"
#include "../Services/CommandProcessing/CommandProcessingIncludes.h"
#include "espressif/esp8266/uart_register.h"
#include "espressif/esp8266/pin_mux_register.h"
#include "../SmingCore/CircularBuffer.h"
//#include "freertos/queue.h"
#include "../SmingCore/SmingCore.h"


#define UART_ID_0   0
#define UART_ID_1   1

#define NUMBER_UARTS 2

#define RX_BUFF_SIZE 0x100

// Delegate constructor usage: (&YourClass::method, this)
typedef Delegate<void(Stream &source, char arrivedChar, uint16_t availableCharsCount)> StreamDataReceivedDelegate;

typedef struct
{
   int uart;
   int type;
   char rcvChar;
   int charCount;
} SerialDelegateMessage;

#define SERIAL_SIGNAL_DELEGATE	0
#define SERIAL_SIGNAL_COMMAND	1
#define SERIAL_QUEUE_LEN		10

class CommandExecutor;

class HardwareSerial : public Stream
{
public:
	HardwareSerial(const int uartPort);

	~HardwareSerial();

	void begin(const uint32_t baud = 9600);

	int available();
	int read();
	int readMemoryBlock(char* buf, int max_len);
	int peek();
	void flush();
	size_t write(uint8_t oneChar);

	//void printf(const char *fmt, ...);
	void systemDebugOutput(bool enabled);
	void commandProcessing(bool reqEnable);
	void setCallback(StreamDataReceivedDelegate reqCallback, bool useSerialRxBuffer = true);
	void resetCallback();

	static void IRAM_ATTR uartReceiveInterruptHandler(void *para);

private:
	int uart;
	StreamDataReceivedDelegate HWSDelegate = NULL;
	bool useRxBuff = true;
	CommandExecutor* commandExecutor = nullptr;
	CircularBuffer<int, char, 256> rxBuffer;

	static HardwareSerial* hardwareSerialObjects[NUMBER_UARTS];
	static void DelegateTask(void *pvParameters);

	static xQueueHandle serialDelegateQueue; // one queue for all uarts, uart is selected in message
	static xTaskHandle  serialDelegateTask;

};

extern HardwareSerial Serial;

#endif /* _HARDWARESERIAL_H_ */
