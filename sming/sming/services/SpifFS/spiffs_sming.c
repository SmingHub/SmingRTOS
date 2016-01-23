#include "spiffs_sming.h"

#define LOG_PAGE_SIZE       256

spiffs _filesystemStorageHandle;

static u8_t spiffs_work_buf[LOG_PAGE_SIZE*2];
static u8_t spiffs_fds[32*7]; // sizeof(spiffs_fd) * K
static u8_t spiffs_cache[(LOG_PAGE_SIZE+32)*4];

static s32_t api_spiffs_read(u32_t addr, u32_t size, u8_t *dst)
{
  flashmem_read(dst, addr, size);
  return SPIFFS_OK;
}

static s32_t api_spiffs_write(u32_t addr, u32_t size, u8_t *src)
{
  //debugf("api_spiffs_write");
  flashmem_write(src, addr, size);
  return SPIFFS_OK;
}

static s32_t api_spiffs_erase(u32_t addr, u32_t size)
{
  debugf("api_spiffs_erase");
  u32_t sect_first = flashmem_get_sector_of_address(addr);
  u32_t sect_last = sect_first;
  while( sect_first <= sect_last )
    if( !flashmem_erase_sector( sect_first ++ ) )
      return SPIFFS_ERR_INTERNAL;
  return SPIFFS_OK;
} 

/*******************
The W25Q32BV array is organized into 16,384 programmable pages of 256-bytes each. Up to 256 bytes can be programmed at a time.
Pages can be erased in groups of 16 (4KB sector erase), groups of 128 (32KB block erase), groups of 256 (64KB block erase) or
the entire chip (chip erase). The W25Q32BV has 1,024 erasable sectors and 64 erasable blocks respectively.
The small 4KB sectors allow for greater flexibility in applications that require data and parameter storage.
********************/


u32_t spiffs_get_startaddress()
{
	s32_t phys_addr = flashmem_get_first_free_block_address();
	phys_addr &= 0xFFFFF000;
	return phys_addr;
}

s32_t spiffs_mount()
{
  spiffs_mount_manual (spiffs_get_startaddress(), 0);
}

s32_t spiffs_mount_manual(u32_t phys_addr, u32_t phys_size)
{
  spiffs_config cfg = {0};

  // Set Spiffs HAL functions
  cfg.hal_read_f = api_spiffs_read;
  cfg.hal_write_f = api_spiffs_write;
  cfg.hal_erase_f = api_spiffs_erase;

  // Set Spiffs physical layout
  cfg.phys_erase_block = INTERNAL_FLASH_SECTOR_SIZE; // according to datasheet
  cfg.log_block_size = INTERNAL_FLASH_SECTOR_SIZE * 2; // Important to make large
  cfg.log_page_size = LOG_PAGE_SIZE; // as we said

  // Set spiffs location
  cfg.phys_addr = phys_addr;
  cfg.phys_size = phys_size;

  if (cfg.phys_size == 0)
   {
	  s32_t probeSize = SPIFFS_probe_fs(&cfg);
	  if (probeSize < 0)
      {
 	  	  return probeSize;  // Requested automatic sizing but not possible to detect
      }
	  else
 	  {
 	  	  cfg.phys_size = probeSize;
 	  }
   }

  int spiffsResult = SPIFFS_mount(&_filesystemStorageHandle,
    &cfg,
    spiffs_work_buf,
    spiffs_fds,
    sizeof(spiffs_fds),
    spiffs_cache,
    sizeof(spiffs_cache),
    NULL);
  debugf("mount res: %d\n", spiffsResult);

  debugf("fs.start: address %X, size: %d Kb, result : %d \n", cfg.phys_addr, cfg.phys_size / 1024, spiffsResult );

  return spiffsResult;
}

void spiffs_unmount()
{
	SPIFFS_unmount(&_filesystemStorageHandle);
}

bool spiffs_format_internal(spiffs_config *cfg)
{
	// should we use _filestorage handle
	SPIFFS_format(&_filesystemStorageHandle);
}

// FS formatting function
bool spiffs_format()
{
	if (_filesystemStorageHandle.user_data)
	{
		return SPIFFS_ERR_NOT_MOUNTED;
	}
	if (SPIFFS_mounted(&_filesystemStorageHandle))
	{
		return SPIFFS_ERR_MOUNTED;
	}
	SPIFFS_format(&_filesystemStorageHandle);

	return true;
}

