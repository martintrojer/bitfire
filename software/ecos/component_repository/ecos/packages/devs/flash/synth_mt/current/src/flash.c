// THe Flash API MTFFS expects
// This is a ram-emulated version for debugging.

#include <pkgconf/devs_flash_synth_mt.h>
#define  _FLASH_PRIVATE_
#include <cyg/io/flash.h>
#include <string.h>

#include "flash.h"

int flash_query(void *data)
{
	return FLASH_ERR_OK;
}

int flash_hwr_init(void)
{
  flash_info.block_size = CYGNUM_FLASH_SYNTH_BLOCKSIZE;
  flash_info.buffer_size = 0;
  flash_info.blocks = CYGNUM_FLASH_SYNTH_NUMBLOCKS;
  flash_info.start = &the_flash;
  flash_info.end = (void*)(&the_flash + sizeof(flash));

  return FLASH_ERR_OK;
}

int flash_read_buf(volatile unsigned char* addr, unsigned long* data, int len)
{
//	int first_block = (int)addr / CYGNUM_FLASH_SYNTH_BLOCKSIZE;
//	int offset = (int)addr % CYGNUM_FLASH_SYNTH_BLOCKSIZE;
//	unsigned char *start_address = the_flash.blocks[first_block].data + offset;
//	unsigned char *start_address = (unsigned char*)addr + offset;
	addr -= CYGNUM_IO_FLASH_BLOCK_OFFSET_1;
	memcpy(data,addr,len);
	
	return FLASH_ERR_OK;
}

int flash_program_buf (volatile unsigned char *addr, unsigned long *data, int len)
{
//	int first_block = (int)addr / CYGNUM_FLASH_SYNTH_BLOCKSIZE;
//	int offset = (int)addr % CYGNUM_FLASH_SYNTH_BLOCKSIZE;
//	unsigned char *start_address = the_flash.blocks[first_block].data + offset;
//	unsigned char *start_address = (unsigned char*)addr + offset;
	addr -= CYGNUM_IO_FLASH_BLOCK_OFFSET_1;
	memcpy(addr,data,len);

	return FLASH_ERR_OK;
}

int flash_erase_block (volatile unsigned long block) 
{
	unsigned long offset = ((unsigned long)&the_flash & (unsigned long)flash_info.block_mask) + CYGNUM_IO_FLASH_BLOCK_OFFSET_1;
	int the_block = (block - offset) / CYGNUM_FLASH_SYNTH_BLOCKSIZE;
	memset(the_flash.blocks[the_block].data,-1,CYGNUM_FLASH_SYNTH_BLOCKSIZE);
	return FLASH_ERR_OK;
}

// Map a hardware status to a package error
int flash_hwr_map_error(int err)
{
	return err;
}

// See if a range of FLASH addresses overlaps currently running code
bool flash_code_overlaps(void *start, void *end)
{
  extern char _stext[], _etext[];

  return ((((unsigned long)&_stext >= (unsigned long)start) &&
      ((unsigned long)&_stext < (unsigned long)end)) ||
      (((unsigned long)&_etext >= (unsigned long)start) &&
      ((unsigned long)&_etext < (unsigned long)end)));
}
