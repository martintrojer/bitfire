#include "m28w320b.h"
#include "platform.h"

/* address offset of each block in m28w320b flash memory */
const unsigned int M28W320B_Blk_Off[] =
{
      0x000000,  /* Start offset of block 0  */
      0x001000,  /* Start offset of block 1  */
      0x002000,  /* Start offset of block 2  */
      0x003000,  /* Start offset of block 3  */
      0x004000,  /* Start offset of block 4  */
      0x005000,  /* Start offset of block 5  */
      0x006000,  /* Start offset of block 6  */
      0x007000,  /* Start offset of block 7  */
      0x008000,  /* Start offset of block 8  */
      0x010000,  /* Start offset of block 9  */
      0x018000,  /* Start offset of block 10 */
      0x020000,  /* Start offset of block 11 */
      0x028000,  /* Start offset of block 12 */
      0x030000,  /* Start offset of block 13 */
      0x038000,  /* Start offset of block 14 */
      0x040000,  /* Start offset of block 15 */
      0x048000,  /* Start offset of block 16 */
      0x050000,  /* Start offset of block 17 */
      0x058000,  /* Start offset of block 18 */
      0x060000,  /* Start offset of block 19 */
      0x068000,  /* Start offset of block 20 */
      0x070000,  /* Start offset of block 21 */
      0x078000,  /* Start offset of block 22 */
      0x080000,  /* Start offset of block 23 */
      0x088000,  /* Start offset of block 24 */
      0x090000,  /* Start offset of block 25 */
      0x098000,  /* Start offset of block 26 */
      0x0A0000,  /* Start offset of block 27 */
      0x0A8000,  /* Start offset of block 28 */
      0x0B0000,  /* Start offset of block 29 */
      0x0B8000,  /* Start offset of block 30 */
      0x0C0000,  /* Start offset of block 31 */
      0x0C8000,  /* Start offset of block 32 */
      0x0D0000,  /* Start offset of block 33 */
      0x0D8000,  /* Start offset of block 34 */
      0x0E0000,  /* Start offset of block 35 */
      0x0E8000,  /* Start offset of block 36 */
      0x0F0000,  /* Start offset of block 37 */
      0x0F8000,  /* Start offset of block 38 */
      0x100000,  /* Start offset of block 39 */
      0x108000,  /* Start offset of block 40 */
      0x110000,  /* Start offset of block 41 */
      0x118000,  /* Start offset of block 42 */
      0x120000,  /* Start offset of block 43 */
      0x128000,  /* Start offset of block 44 */
      0x130000,  /* Start offset of block 45 */
      0x138000,  /* Start offset of block 46 */
      0x140000,  /* Start offset of block 47 */
      0x148000,  /* Start offset of block 48 */
      0x150000,  /* Start offset of block 49 */
      0x158000,  /* Start offset of block 50 */
      0x160000,  /* Start offset of block 51 */
      0x168000,  /* Start offset of block 52 */
      0x170000,  /* Start offset of block 53 */
      0x178000,  /* Start offset of block 54 */
      0x180000,  /* Start offset of block 55 */
      0x188000,  /* Start offset of block 56 */
      0x190000,  /* Start offset of block 57 */
      0x198000,  /* Start offset of block 58 */
      0x1A0000,  /* Start offset of block 59 */
      0x1A8000,  /* Start offset of block 60 */
      0x1B0000,  /* Start offset of block 61 */
      0x1B8000,  /* Start offset of block 62 */
      0x1C0000,  /* Start offset of block 63 */
      0x1C8000,  /* Start offset of block 64 */
      0x1D0000,  /* Start offset of block 65 */
      0x1D8000,  /* Start offset of block 66 */
      0x1E0000,  /* Start offset of block 67 */
      0x1E8000,  /* Start offset of block 68 */
      0x1F0000,  /* Start offset of block 69 */
      0x1F8000   /* Start offset of block 70 */
};

/****************************
*   Function: Block Unlock  *
*   Return Value:           *
*       0   -- Success      *
*       -1  -- Failed       *
****************************/
int ExtFlashBlockUnlock (unsigned int u32BlkNum)
{
    int iRetVal = 0;
    unsigned short u16ProtStat;

    /* check the block number */
    if (u32BlkNum >= EXT_FLASH_BLKNUM)
    {
        return -1;
    }

    /* unlock the block */
    EXT_FLASH_BASE[0] = 0x0060;
    EXT_FLASH_BASE[M28W320B_Blk_Off[u32BlkNum]] = 0x00D0;

    /* check the block protection status */
    EXT_FLASH_BASE[0] = 0x0090;
    u16ProtStat = EXT_FLASH_BASE[M28W320B_Blk_Off[u32BlkNum] + 0x00000002];
    if (u16ProtStat & 0x0001)
    {
        /* failed */
        iRetVal = -1;
    }

    /* reset the flash */
    EXT_FLASH_BASE[0] = 0x00FF;
    return iRetVal;
}

/****************************
*   Function: Block Erase   *
*   Return Value:           *
*       0   -- Success      *
*       -1  -- Failed       *
****************************/
int ExtFlashBlockErase (unsigned int u32BlkNum)
{
    int iRetVal = 0;
    unsigned short u16Stat;

    /* check the block number */
    if (u32BlkNum >= EXT_FLASH_BLKNUM)
    {
        return -1;
    }

    /* clear the status register */
    EXT_FLASH_BASE[0] = 0x0050;

    /* erase the block */
    EXT_FLASH_BASE[0] = 0x0020;
    EXT_FLASH_BASE[M28W320B_Blk_Off[u32BlkNum]] = 0x00D0;

    /* wait until the erase operation is finished */
    while (((u16Stat = EXT_FLASH_BASE[0]) & 0x0080) != 0x0080);

    /* check the status register */
    if (u16Stat & 0x002A)
    {
        /* failed */
        iRetVal = -1;
        /* clear the status register */
        EXT_FLASH_BASE[0] = 0x0050;
    }

    /* reset the flash */
    EXT_FLASH_BASE[0] = 0x00FF;
    return iRetVal;
}

/****************************
*   Function: Flash Program *
*   Return Value:           *
*       0   -- Success      *
*       -1  -- Failed       *
****************************/
int ExtFlashProgram (unsigned int u32AddrOff, unsigned int u32SizeInWord, unsigned short *pu16Buf)
{
    int i;
    int iRetVal = 0;
    unsigned short u16Stat;
    int oldpercent=0 , percent=0;

    /* check the data location */
    if ((u32AddrOff >= EXT_FLASH_SIZE) || (u32SizeInWord > (EXT_FLASH_SIZE - u32AddrOff)))
    {
        return -1;
    }

    /* program */
    PutString("Programming ...   0%");
    for (i=0; i<u32SizeInWord; i++)
    {
        /* clear the status register */
        EXT_FLASH_BASE[0] = 0x0050;

        /* program the next word */
        EXT_FLASH_BASE[0] = 0x0040;
        EXT_FLASH_BASE[u32AddrOff] = *pu16Buf++;

        /* wait until the program operation is finished */
        while (((u16Stat = EXT_FLASH_BASE[u32AddrOff]) & 0x0080) != 0x0080);
        u32AddrOff ++;

        /* check the status register */
        if (u16Stat & 0x001A)
        {
            /* failed */
            iRetVal = -1;
            /* clear the status register */
            EXT_FLASH_BASE[0] = 0x0050;
            /* exit */
            break;
        }
        
        //update percentage progress
        percent = (1+i)*100 / (int)u32SizeInWord;
        if (percent > oldpercent)
        {
        	oldpercent = percent;
       		Printf("\b\b\b\b%3d%%",percent);
        }	
    }
    PutString("\r\n");

    /* reset the flash */
    EXT_FLASH_BASE[0] = 0x00FF;
    return iRetVal;
}
