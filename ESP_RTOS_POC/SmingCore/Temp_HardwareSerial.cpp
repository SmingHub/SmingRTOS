/****
 * Sming Framework Project - Open Source framework for high efficiency native ESP8266 development.
 * Created 2015 by Skurydin Alexey
 * http://github.com/anakod/Sming
 * All files of the Sming Core are provided under the LGPL v3 license.
 ****/

// HardwareSerial based on Espressif Systems code

#include "Temp_HardwareSerial.h"
//#include "../Wiring/WiringFrameworkIncludes.h"
#include <cstdarg>

#include "../SmingCore/Clock.h"
#include "../SmingCore/Interrupts.h"
#include "uart.h"


//set m_printf callback
extern void setMPrintfPrinterCbc(void (*callback)(char));

// StreamDataAvailableDelegate HardwareSerial::HWSDelegates[2];

HWSerialMemberData HardwareSerial::memberData[NUMBER_UARTS];

HardwareSerial::HardwareSerial(const int uartPort)
	: uart(uartPort)
{
	resetCallback();
}

void HardwareSerial::begin(const uint32_t baud/* = 9600*/)
{
}

size_t HardwareSerial::write(uint8_t oneChar)
{
	//if (oneChar == '\0') return 0;

//	uart_tx_one_char(oneChar);
//	LOCAL STATUS
 	uart_tx_one_char(0, oneChar);
//	uart0_write_char(oneChar);
//	UART_ResetFifo(UART0);

	return 1;
}

int HardwareSerial::available()
{
}

int HardwareSerial::read()
{
}

int HardwareSerial::readMemoryBlock(char* buf, int max_len)
{
}

int HardwareSerial::peek()
{
}

void HardwareSerial::flush()
{
}


/*void HardwareSerial::printf(const char *fmt, ...)
{
	// Doesn't work :(
	va_list va;
	va_start(va, fmt);
	ets_uart_printf(fmt, va);
	va_end(va);
}*/

void HardwareSerial::systemDebugOutput(bool enabled)
{

//	if (uart == UART_ID_0)
//		setMPrintfPrinterCbc(enabled ? uart_tx_one_char : NULL);
	//else
	//	os_install_putc1(enabled ? (void *)uart1_tx_one_char : NULL); //TODO: Debug serial
}

void HardwareSerial::setCallback(StreamDataReceivedDelegate reqDelegate, bool useSerialRxBuffer /* = true */)
{
	memberData[uart].HWSDelegate = reqDelegate;
	memberData[uart].useRxBuff = useSerialRxBuffer;
}

void HardwareSerial::resetCallback()
{
	memberData[uart].HWSDelegate = nullptr;
	memberData[uart].useRxBuff = true;
}

void HardwareSerial::commandProcessing(bool reqEnable)
{
	/*
	if (reqEnable)
	{
		if (!memberData[uart].commandExecutor)
		{
			memberData[uart].commandExecutor = new CommandExecutor(&Serial);
		}
	}
	else
	{
		delete memberData[uart].commandExecutor;
		memberData[uart].commandExecutor = nullptr;
	}
	*/
}


void HardwareSerial::uart0_rx_intr_handler(void *para)
{
}


HardwareSerial Serial(UART_ID_0);
