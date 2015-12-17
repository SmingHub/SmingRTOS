#ifndef __RBOOT_API_H__
#define __RBOOT_API_H__

//////////////////////////////////////////////////
// rBoot OTA and config API for ESP8266.
// Copyright 2015 Richard A Burton
// richardaburton@gmail.com
// See license.txt for license terms.
// OTA code based on SDK sample from Espressif.
//////////////////////////////////////////////////

#include <../rboot.h>

#ifdef __cplusplus
extern "C" {
#endif

#define ROM_MAGIC	   0xe9
#define ROM_MAGIC_NEW1 0xea
#define ROM_MAGIC_NEW2 0x04


// buffer size, must be at least 0x10 (size of rom_header_new structure)
#define BUFFER_SIZE 0x100

// esp8266 built in rom functions
extern uint32 SPIRead(uint32 addr, void *outptr, uint32 len);
extern uint32 SPIEraseSector(int);
extern uint32 SPIWrite(uint32 addr, void *inptr, uint32 len);
extern void ets_printf(char*, ...);

// functions we'll call by address
typedef void stage2a(uint32);
typedef void usercode(void);

// standard rom header
typedef struct {
	// general rom header
	uint8 magic;
	uint8 count;
	uint8 flags1;
	uint8 flags2;
	usercode* entry;
} rom_header;

typedef struct {
	uint8* address;
	uint32 length;
} section_header;

// new rom header (irom section first) there is
// another 8 byte header straight afterward the
// standard header
typedef struct {
	// general rom header
	uint8 magic;
	uint8 count; // second magic for new header
	uint8 flags1;
	uint8 flags2;
	uint32 entry;
	// new type rom, lib header
	uint32 add; // zero
	uint32 len; // length of irom section
} rom_header_new;


typedef struct {
	uint32 start_addr;
	uint32 start_sector;
	//uint32 max_sector_count;
	uint32 last_sector_erased;
	uint8 extra_count;
	uint8 extra_bytes[4];
} rboot_write_status;

uint32 rboot_check_image(uint32 startaddress);
rboot_config ICACHE_FLASH_ATTR rboot_get_config();
bool ICACHE_FLASH_ATTR rboot_set_config(rboot_config *conf);
uint8 ICACHE_FLASH_ATTR rboot_get_current_rom();
bool ICACHE_FLASH_ATTR rboot_set_current_rom(uint8 rom);
rboot_write_status ICACHE_FLASH_ATTR rboot_write_init(uint32 start_addr);
bool ICACHE_FLASH_ATTR rboot_write_flash(rboot_write_status *status, uint8 *data, uint16 len);

#ifdef __cplusplus
}
#endif

#endif
