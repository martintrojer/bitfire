/******************** (C) COPYRIGHT 2006 STMicroelectronics ********************
* File Name          : memory.c
* Author             : MCD Application Team
* Date First Issued  : 10/01/2006 : V1.0
* Description        : Memory management layer
********************************************************************************
* History:
* 10/01/2006 : V1.0
********************************************************************************
* THE PRESENT SOFTWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH SOFTWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/
/* Includes ------------------------------------------------------------------*/
#include "memory.h"
#include "usb_scsi.h"
#include "usb_bot.h"
#include "usb_regs.h"
#include "hw_config.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
u32 Memory_Offset;                    /* Memory Offset */
u32 Transfer_Length;                  /* Transfer Length */
/* Extern variables ----------------------------------------------------------*/
extern u8 Bulk_Data_Buff[BULK_MAX_PACKET_SIZE];  // data buffer
extern u32 Data_Len;
extern u32 Bot_State;
extern Bulk_Only_CBW CBW;
extern Bulk_Only_CSW CSW;
extern u32 MASS_MEMORY_SIZE;
extern u32 MASS_BLOCK_SIZE;
/* Private function prototypes -----------------------------------------------*/
/* Extern function prototypes ------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/*******************************************************************************
* Function Name  : Read_Memory
* Description    :
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void Read_Memory(void)
{
  UserToPMABufferCopy((u8 *)(MASS_MEMORY_START + Memory_Offset),ENDP1_TXADDR, BULK_MAX_PACKET_SIZE);
  SetEPTxCount(ENDP1,BULK_MAX_PACKET_SIZE);
  SetEPTxStatus(ENDP1, EP_TX_VALID);


  Memory_Offset += BULK_MAX_PACKET_SIZE;
  Transfer_Length -= BULK_MAX_PACKET_SIZE;

  CSW.dDataResidue -= BULK_MAX_PACKET_SIZE;
  Led_RW_ON();

  if (Transfer_Length == 0) {
    Bot_State = BOT_DATA_IN_LAST;
   Led_RW_OFF();
  }
}
/*******************************************************************************
* Function Name  : Write_Memory
* Description    :
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void Write_Memory(void)
{
  u32 Counter;
   for (Counter = 0; Counter < Data_Len; Counter++) {
    *((u8 *)(MASS_MEMORY_START + Memory_Offset + Counter)) = Bulk_Data_Buff[Counter];
  }

  Memory_Offset += Data_Len;
  Transfer_Length -= Data_Len;

  CSW.dDataResidue -= Data_Len;
  SetEPRxStatus(ENDP2, EP_RX_VALID);/* enable the nest transaction*/
   Led_RW_ON();
  if ((Transfer_Length == 0) || (Bot_State == BOT_CSW_Send)) {
    Set_CSW (CSW_CMD_PASSED,SEND_CSW_ENABLE);
    Led_RW_OFF();
  }
}
/*******************************************************************************
* Function Name  : Address_Management_Test
* Description    :
* Input          : u8 Cmd : the command can be SCSI_READ10 or SCSI_WRITE10.
* Output         : None.
* Return         : Read\Write status (bool).
*******************************************************************************/
bool Address_Management_Test(u8 Cmd)
{
u32 temp1;
u32 temp2;

  /* Logical Block Address of First Block */
  temp1 = (CBW.CB[2] << 24) |
           (CBW.CB[3] << 16) |
           (CBW.CB[4] <<  8) |
           (CBW.CB[5] <<  0);
  /* Number of Blocks to transfer */
  temp2 = (CBW.CB[7] <<  8) |
           (CBW.CB[8] <<  0);

  Memory_Offset = temp1 * MASS_BLOCK_SIZE;
  Transfer_Length = temp2 * MASS_BLOCK_SIZE;

    if ((Memory_Offset + Transfer_Length) > MASS_MEMORY_SIZE) {
      if (Cmd == SCSI_WRITE10)
         Bot_Abort(BOTH_DIR);
     Bot_Abort(DIR_IN);
     Set_Scsi_Sense_Data(ILLEGAL_REQUEST,ADDRESS_OUT_OF_RANGE);
     Set_CSW (CSW_CMD_FAILED,SEND_CSW_DISABLE);
     return (FALSE);
   }


   if (CBW.dDataLength != Transfer_Length) {
     if (Cmd == SCSI_WRITE10)
         Bot_Abort(BOTH_DIR);
     else
         Bot_Abort(DIR_IN);
     Set_Scsi_Sense_Data(ILLEGAL_REQUEST,INVALID_FIELED_IN_COMMAND);
     Set_CSW (CSW_CMD_FAILED,SEND_CSW_DISABLE);
     return (FALSE);
  }
  return (TRUE);
}
/******************* (C) COPYRIGHT 2006 STMicroelectronics *****END OF FILE****/




