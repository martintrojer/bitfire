#ifndef __m28w320b_h
#define __m28w320b_h

/* base address of external flash */
#define EXT_FLASH_BASE      ((volatile unsigned short *)0x60000000)
/* size of external flash (by 16-bit) */
#define EXT_FLASH_SIZE      (0x200000)
/* number of blocks in external flash */
#define EXT_FLASH_BLKNUM    (71)

/* function prototypes */
int ExtFlashBlockUnlock     (unsigned int);
int ExtFlashBlockErase      (unsigned int);
int ExtFlashProgram         (unsigned int, unsigned int, unsigned short *);

#endif
