// THe Flash API MTFFS expects
// This is a ram-emulated version for debugging.

#ifndef __FLASH_H
#define __FLASH_H

#include <errno.h>

// These are also defined in mtffs.h
#define BLOCK_NUMBER 512
#define BLOCK_SIZE 0x1000
#define FLASH_START_ADDRESS 0x0


// Read/Write must support access that spans more than one block

int mtffs_flash_read(unsigned char *addr, unsigned char *data, unsigned int length);
int mtffs_flash_write(unsigned char *addr, unsigned char *data, unsigned int length);
int mtffs_flash_erase(unsigned short sector);

typedef struct
{
	unsigned char data[BLOCK_SIZE];
} flash_block;

typedef struct
{
	flash_block blocks[BLOCK_NUMBER];
} flash;

static flash the_flash;

#endif
