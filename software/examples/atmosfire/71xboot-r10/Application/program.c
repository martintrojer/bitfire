#include "platform.h"
#include "common.h"
#include "m28w320b.h"

extern const unsigned int M28W320B_Blk_Off[];

/****************************************
*   Function: Program External Flash    *
*   Return Value:                       *
*       0   -- Success                  *
*       -1  -- Address and/or Size Error*
*       -2  -- Block Unlock Failed      *
*       -3  -- Block Erase Failed       *
*       -4  -- Flash Program Failed     *
*       -5  -- Data Verification Failed *
****************************************/
static int ProgramExternalFlash (unsigned int u32FlashAddr, unsigned int u32SizeInByte, unsigned short *pu16Buf)
{
    int i;
    unsigned int u32FirstOff, u32LastOff, u32SizeInWord;
    unsigned int u32FirstBlk, u32LastBlk, u32CurrBlk;
    volatile unsigned short *pu16FlashPtr, *pu16RamPtr;

    /* calculate the start offset into external flash */
    u32FirstOff = u32FlashAddr - (unsigned int)EXT_FLASH_BASE;
    u32FirstOff >>= 1;

    /* check the data size */
    if ((u32SizeInByte % 2) != 0)
    {
        u32SizeInByte ++;
    }
    u32SizeInWord = u32SizeInByte >> 1;
    if (u32SizeInWord > (EXT_FLASH_SIZE - u32FirstOff))
    {
        return -1;
    }

    /* calculate the end offset into external flash */
    u32LastOff = u32FirstOff + u32SizeInWord - 1;

    /* calculate the block number where the program starts */
    for (u32FirstBlk=0; u32FirstBlk<EXT_FLASH_BLKNUM-1; u32FirstBlk++)
    {
        if (u32FirstOff < M28W320B_Blk_Off[u32FirstBlk+1])
        {
            break;
        }
    }

    /* calculate the block number where the program ends */
    for (u32LastBlk=u32FirstBlk; u32LastBlk<EXT_FLASH_BLKNUM-1; u32LastBlk++)
    {
        if (u32LastOff < M28W320B_Blk_Off[u32LastBlk+1])
        {
            break;
        }
    }

    /* unlock blocks */
    for (u32CurrBlk=u32FirstBlk; u32CurrBlk<=u32LastBlk; u32CurrBlk++)
    {
        if (ExtFlashBlockUnlock(u32CurrBlk) != 0)
        {
            return -2;
        }
    }

    /* erase blocks */
    Printf("Erasing Block...");
    for (u32CurrBlk=u32FirstBlk; u32CurrBlk<=u32LastBlk; u32CurrBlk++)
    {
        if (ExtFlashBlockErase(u32CurrBlk) != 0)
        {
            return -3;
        }
    }
    PutString("OK\r\n");

    /* program data */
    if (ExtFlashProgram(u32FirstOff, u32SizeInWord, pu16Buf) != 0)
    {
        return -4;
    }

    /* verify data */
    PutString("Verifying...");
    pu16FlashPtr = (volatile unsigned short *)u32FlashAddr;
    pu16RamPtr = (volatile unsigned short *)pu16Buf;
    for (i=0; i<u32SizeInByte; i=i+2)
    {
        if (*pu16FlashPtr++ != *pu16RamPtr++)
        {
            return -5;
        }
    }
    PutString("OK\r\n");

    return 0;
}

/************************************************************************************
	Command Handler:
			ProgramExtFlash
**************************************************************************************/
void ProgramExtFlash(int argc, const char **argv)
{
	unsigned int destflashaddr,srcramaddr;
	int errorno = 0;

	if (argc == 1)
	{
		unsigned int destoffset;
		PutString("Please input the offset of target flash: \r\n");
		
		if (!GetIntegerInput(&destoffset)) 
		{
			errorno = 1;
			goto out;
		}
		
		destflashaddr = destoffset + 0x60000000;//base + offset
		
		if (gimagesize == 0)
		{
			PutString("Plase input the image size(in bytes): \r\n");
			if (!GetIntegerInput(&gimagesize))
			{
				errorno = 1;
				goto out;
			}
		}
		srcramaddr = 0x62000000;	//default the beginning of ext SRAM 
	}	
	else if (argc == 4) 
	{
		//Check the arguments
		if (!errorno)
		if (!Str2Int(argv[1],&destflashaddr)) 	errorno = 1;

		if (!errorno)
		if (!Str2Int(argv[2],&gimagesize)) errorno = 1;
		
		if (!errorno)
		if (!Str2Int(argv[3],&srcramaddr))	errorno = 1;
	}
	else
	{
		errorno = 1;
	}

	//Now programming
	if (errorno == 0)
	{
		errorno = ProgramExternalFlash(destflashaddr, gimagesize, (unsigned short*)srcramaddr);
	}
out:	
	gimagesize = 0;//reset gimagesize
	
	//Message output
	switch (errorno)
	{
	case 0:
		PutString("Successfully completed!\r\n");
		break;
	case 1:	
		PutString("Invalid Command Arguments!\r\n");
		break;
	case -1:
		PutString("Address and/or Size Error!\r\n");
		break;
	case -2:
		PutString("Block Unlock Failed!\r\n");
		break;
	case -3:
		PutString("Block Erase Failed!\r\n");
		break;
	case -4:
		PutString("Flash Program Failed!\r\n");
		break;
	case -5:
		PutString("Data Verification Failed!\r\n");
		break;
	default:
		PutString("Unkonw failure!\r\n");
		break;
	}
}