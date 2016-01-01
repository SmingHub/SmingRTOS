/****
 * Sming Framework Project - Open Source framework for high efficiency native ESP8266 development.
 * Created 2015 by Skurydin Alexey
 * http://github.com/anakod/Sming
 * All files of the Sming Core are provided under the LGPL v3 license.
 ****/

// HardwareSerial based on Espressif Systems code

#include "HardwareSerial.h"
//#include "../Wiring/WiringFrameworkIncludes.h"
#include <cstdarg>

#include "../SmingCore/Clock.h"
#include "../SmingCore/Interrupts.h"
#include "uart.h"


//set m_printf callback
extern void setMPrintfPrinterCbc(void (*callback)(char));

HardwareSerial* HardwareSerial::hardwareSerialObjects[NUMBER_UARTS];

HardwareSerial::HardwareSerial(const int uartPort)
	: uart(uartPort)
{
	resetCallback();
	hardwareSerialObjects[uartPort] = this;
}

void HardwareSerial::begin(const uint32_t baud/* = 9600*/)
{
	UART_SetBaudrate(UART0,baud);
    UART_intr_handler_register((void *) &uart0_rx_intr_handler,NULL);
    ETS_UART_INTR_ENABLE();
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
	return rxBuffer.Len();
}

int HardwareSerial::read()
{
	char rcvChar;

	noInterrupts();
	if (!rxBuffer.Pop(rcvChar))
	{
		rcvChar = -1;
	}
	interrupts();

	return rcvChar;
}

int HardwareSerial::readMemoryBlock(char* buf, int max_len)
{
	int numChar = 0;
	char tempChar;
	noInterrupts();
	while ((rxBuffer.Pop(tempChar)) && (max_len-- > 0)) {
		*buf = tempChar;		// Read data from Buffer
		numChar++;						// Increase counter of read bytes
		buf++;						// Increase Buffer pointer
	}
	interrupts();

	return numChar;
}

int HardwareSerial::peek()
{
	char peekChar;

	noInterrupts();
	if (!rxBuffer.Peek(peekChar))
	{
		peekChar = -1;
	}
	interrupts();

	return peekChar;
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
	HWSDelegate = reqDelegate;
	useRxBuff = useSerialRxBuffer;
}

void HardwareSerial::resetCallback()
{
	HWSDelegate = nullptr;
	useRxBuff = true;
}

void HardwareSerial::commandProcessing(bool reqEnable)
{

	if (reqEnable)
	{
		if (!commandExecutor)
		{
			commandExecutor = new CommandExecutor(&Serial);
		}
	}
	else
	{
		delete commandExecutor;
		commandExecutor = nullptr;
	}

}

void HardwareSerial::uart0_rx_intr_handler(void *para)
{
    /* uart0 and uart1 intr combine togther, when interrupt occur, see reg 0x3ff20020, bit2, bit0 represents
     * uart1 and uart0 respectively
     */
	HardwareSerial* Self = hardwareSerialObjects[UART_ID_0];
	uint8 RcvChar;

    if (UART_RXFIFO_FULL_INT_ST != (READ_PERI_REG(UART_INT_ST(UART_ID_0)) & UART_RXFIFO_FULL_INT_ST))
        return;

    WRITE_PERI_REG(UART_INT_CLR(UART_ID_0), UART_RXFIFO_FULL_INT_CLR);

    while (READ_PERI_REG(UART_STATUS(UART_ID_0)) & (UART_RXFIFO_CNT << UART_RXFIFO_CNT_S))
    {
        RcvChar = READ_PERI_REG(UART_FIFO(UART_ID_0)) & 0xFF;

        /* you can add your handle code below.*/
      if (Self->useRxBuff)
      {
    	Self->rxBuffer.Push(RcvChar);
      }

      if (Self->HWSDelegate)
        {
        	Self->HWSDelegate(Serial, RcvChar, Self->rxBuffer.Len());
        }
      if (Self->commandExecutor)
      {
    	  Self->commandExecutor->executorReceive(RcvChar);
      }
    }
}


HardwareSerial Serial(UART_ID_0);
