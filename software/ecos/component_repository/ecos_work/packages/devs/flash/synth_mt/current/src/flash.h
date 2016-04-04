// THe Flash API MTFFS expects
// This is a ram-emulated version for debugging.

#ifndef __FLASH_H
#define __FLASH_H

// Read/Write must support access that spans more than one block

typedef struct
{
	unsigned char data[CYGNUM_FLASH_SYNTH_BLOCKSIZE];
} flash_block;

typedef struct
{
	flash_block blocks[CYGNUM_FLASH_SYNTH_NUMBLOCKS];
} flash;

static flash the_flash;

#endif
