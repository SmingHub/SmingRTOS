//////////////////////////////////////////////////
// rBoot OTA and config API for ESP8266.
// Copyright 2015 Richard A Burton
// richardaburton@gmail.com
// See license.txt for license terms.
// OTA code based on SDK sample from Espressif.
//////////////////////////////////////////////////

#ifdef RBOOT_INTEGRATION
#include <rboot-integration.h>
#endif

#include "../include/sming_global.h"
#include "spi_flash.h"



#ifdef __cplusplus
extern "C" {
#endif

#include "rboot_sming.h"

uint32 rboot_check_image(uint32 readpos) {

	uint8 buffer[BUFFER_SIZE];
	uint8 sectcount;
	uint8 sectcurrent;
	uint8 *writepos;
	uint8 chksum = CHKSUM_INIT;
	uint32 loop;
	uint32 remaining;
	uint32 romaddr;
//	ets_printf("\r\n Checking Image at  %X\r\n",readpos);

	rom_header_new *header = (rom_header_new*)buffer;
	section_header *section = (section_header*)buffer;

	if (readpos == 0 || readpos == 0xffffffff) {
		return 0;
	}

	// read rom header
	if (spi_flash_read(readpos,(uint32 *) buffer, sizeof(rom_header_new)) != 0) {
		return 0;
	}
//	ets_printf("Rom reading complete readpos = %X\r\n",readpos);
//	ets_printf("Header %X %X %X %X \r\n",header->magic, header->count, header->flags1, header->flags2 );

//	ets_printf("Header : ");
//	for (int i=0;i<sizeof(rom_header_new);i++) { ets_printf("%X ",buffer[i]) ;};
//	ets_printf("\r\n");

	// check header type
	if (header->magic == ROM_MAGIC) {
		// old type, no extra header or irom section to skip over
		romaddr = readpos;
		readpos += sizeof(rom_header);
		sectcount = header->count;
//		ets_printf("Old header\r\n");
	} else if (header->magic == ROM_MAGIC_NEW1 && header->count == ROM_MAGIC_NEW2) {
//		ets_printf("New header\r\n");
		// new type, has extra header and irom section first
		romaddr = readpos + header->len + sizeof(rom_header_new);
#ifdef BOOT_IROM_CHKSUM
		// we will set the real section count later, when we read the header
		sectcount = 0xff;
		// just skip the first part of the header
		// rest is processed for the chksum
		readpos += sizeof(rom_header);
#else
		// skip the extra header and irom section
		readpos = romaddr;
		// read the normal header that follows
		if (spi_flash_read(readpos,(uint32*) buffer, sizeof(rom_header)) != 0) {
			return 0;
		}
//		ets_printf("Second read completed\r\n");
		sectcount = header->count;
		readpos += sizeof(rom_header);
#endif
	} else {
		return 0;
	}

	// test each section
	for (sectcurrent = 0; sectcurrent < sectcount; sectcurrent++) {
		// read section header
		if (spi_flash_read(readpos, (uint32*)section, sizeof(section_header)) != 0) {
			return 0;
		}
		readpos += sizeof(section_header);
//		ets_printf("\r\n Section %d, address = %X, length = %X\r\n",sectcurrent, section->address,section->length);
		// get section address and length
		writepos = section->address;
		remaining = section->length;

		while (remaining > 0) {
			// work out how much to read, up to BUFFER_SIZE
			uint32 readlen = (remaining < BUFFER_SIZE) ? remaining : BUFFER_SIZE;
			// read the block
			if (spi_flash_read(readpos, (uint32*)buffer, readlen) != 0) {
				return 0;
			}
			// increment next read and write positions
			readpos += readlen;
			writepos += readlen;
			// decrement remaining count
			remaining -= readlen;
			// add to chksum
			for (loop = 0; loop < readlen; loop++) {
				chksum ^= buffer[loop];
			}
		}

#ifdef BOOT_IROM_CHKSUM
		if (sectcount == 0xff) {
			// just processed the irom section, now
			// read the normal header that follows
			if (spi_flash_read(readpos,(uint32*) header, sizeof(rom_header)) != 0) {
				return 0;
			}
			sectcount = header->count + 1;
			readpos += sizeof(rom_header);
		}
#endif
	}

	// round up to next 16 and get checksum
	readpos = readpos | 0x0f;
	if (spi_flash_read(readpos,(uint32*) buffer, 1) != 0) {
		return 0;
	}

	// compare calculated and stored checksums
	if (buffer[0] != chksum) {
		return 0;
	}

	return romaddr;
}


#ifdef __cplusplus
}
#endif
