/******************** (C) COPYRIGHT 2003 STMicroelectronics ********************
********************************************************************************
* File Name          : 71x_type.h
* Author             : MCD Application Team
* Date First Issued  : 16/05/2003
* Description        : Common data types
********************************************************************************
* History:
*  16/05/03 : creation
*******************************************************************************/
#ifndef _71x_type_H
#define _71x_type_H

typedef volatile unsigned long   u32;
typedef volatile unsigned short  u16;
typedef volatile unsigned char   u8;

/*===================================================================*/
typedef enum { FALSE = 0, TRUE  = !FALSE } bool;
/*===================================================================*/
typedef enum { RESET = 0, SET   = !RESET } flagstatus;
/*===================================================================*/
typedef enum { DISABLE = 0, ENABLE  = !DISABLE} functionalstate;
/*===================================================================*/
typedef enum { INDIRECT = 0, DIRECT  = !INDIRECT} registeraccess;
/*===================================================================*/

#endif /* _71x_type_H */

/******************* (C) COPYRIGHT 2003 STMicroelectronics *****END OF FILE****/
