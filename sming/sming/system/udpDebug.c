/*
Code to redirect both os_printf and debugf to UDP connection.
Created for use with Sming and SmingRTOS
Padelis Floudas 2016
*/

#include "udpDebug.h"
#include "../include/sming_global.h"
#include "../core/stringconversion.h"
#include "../include/lwip_includes.h"

#define debugPort 5000
#define DBG_BUFFER_LEN 256
#define MPRINTF_BUF_SIZE 256

char dbgBuffer[DBG_BUFFER_LEN];
uint8_t dbg_BufferLen = 0;

/*
tx_udp_char

This function is for use with os_install_putc1();
The original serial function sends each character separately.
To make this work with UDP connection i have to add each character to the char array
and when the NULL char is received, i send the packet.
*/

void tx_udp_char(char c)
{
	dbgBuffer[dbg_BufferLen++] = c;		// add the character to the array
										// and increase the index

	// check for NULL character and send
	if(((c == 0x0A) && (dbg_BufferLen > 1)) || (dbg_BufferLen == DBG_BUFFER_LEN))
    {
		struct udp_pcb *pcb;
		struct pbuf *p;
		struct ip_addr dst;
		
		pcb = udp_new();
		
		// if there was an error creating the pcb then return
		if(pcb == NULL) {
			return;
		}
		p = pbuf_alloc(PBUF_TRANSPORT, dbg_BufferLen, PBUF_RAM);
		memcpy(p->payload, dbgBuffer, dbg_BufferLen);
		IP4_ADDR(&dst,192,168,4,255);
		udp_sendto(pcb, p, &dst, debugPort);
		pbuf_free(p);
		
		dbg_BufferLen = 0;
    }
}

/*
m_printf_udp

This fuction replaces the Sming m_printf() function
It is used in the same way as in Sming
*/
int m_printf_udp(const char *fmt, ...)
{
	char buf[MPRINTF_BUF_SIZE], *tmp;
	va_list args;
	struct udp_pcb *pcb;
	struct pbuf *p;
	struct ip_addr dst;

	va_start(args, fmt);
	m_vsnprintf(buf, sizeof(buf), fmt, args);
	va_end(args);

	// find the null character
	tmp = buf;
	int len = 0;
	while (*tmp)
    {
        tmp++;
        len++;
    }
    // len holds the size of the string that we want to send.
	
	pcb = udp_new();
	
	// if there was an error creating the pcb then return
	if(pcb == NULL) {
		return 0;
	}

	p = pbuf_alloc(PBUF_TRANSPORT, len, PBUF_RAM);		// allocate memory for the buffer
	memcpy(p->payload, buf, len);						// copy data to buffer
	IP4_ADDR(&dst,192,168,4,255);						// create IP address
	udp_sendto(pcb, p, &dst, debugPort);				// broadcast message
	pbuf_free(p);										// free memory

	return 1;
}
