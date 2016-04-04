/******************** (C) COPYRIGHT 2003 STMicroelectronics ********************
* File Name          : emi.h
* Author             : MCD Application Team
* Date First Issued  : 08/08/2003
* Description        : This file contains all the functions prototypes for the
*                      EMI software library.
********************************************************************************
* History:
*  08/08/03 : creation
*******************************************************************************/

#ifndef _EMI_H
#define _EMI_H

#include "71x_map.h"

#define EMI_ENABLE       0x8000
#define EMI_DISABLE      0x0000
#define EMI_WAITSTATE(n) ((n) << 2)
#define EMI_SIZE_8       0x0000
#define EMI_SIZE_16      0x0001

#define EMI_BANK0  EXTMEM_BASE
#define EMI_BANK1  (EXTMEM_BASE + 0x2000000)
#define EMI_BANK2  (EXTMEM_BASE + 0x4000000)

/*******************************************************************************
* Function Name  : EMI_Config
* Description    : This routine is used, for each bank, to configure enabling,
*                : cycle length and bus size.
* Input          : Bank (u8)
*                : Config (u16)
* Return         : None
*******************************************************************************/
inline void EMI_Config(u8 Bank, u16 Config)
{
  *(u16 *)(EMI_BASE + (4*Bank)) = Config;
}

#endif // _EMI_H

/******************* (C) COPYRIGHT 2003 STMicroelectronics *****END OF FILE****/
