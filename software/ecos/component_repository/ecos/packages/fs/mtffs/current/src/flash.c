// THe Flash API MTFFS expects
// This is a ram-emulated version for debugging.

#include "flash.h"
#include <string.h>

int mtffs_flash_read(unsigned char *addr, unsigned char *data, unsigned int length)
{
	int first_block = (int)addr / BLOCK_SIZE;
	int offset = (int)addr % BLOCK_SIZE;
	unsigned char *start_address = the_flash.blocks[first_block].data + offset;
	memcpy(data,start_address, length);
	
	return ENOERR;
}

int mtffs_flash_write(unsigned char *addr, unsigned char *data, unsigned int length)
{
	int first_block = (int)addr / BLOCK_SIZE;
	int offset = (int)addr % BLOCK_SIZE;
	unsigned char *start_address = the_flash.blocks[first_block].data + offset;
	memcpy(start_address, data,length);

	return ENOERR;
}

int mtffs_flash_erase(unsigned short sector)
{
	memset(the_flash.blocks[sector].data,-1,BLOCK_SIZE);

	return ENOERR;
}
