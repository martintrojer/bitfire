#include "71x_lib.h"
#include "platform.h"

#define UART3_Rx_Pin (0x0001<<1)   /*  TQFP 64: pin N° 52 , TQFP 144 pin N° 123 */
#define UART3_Tx_Pin (0x0001<<0)   /*  TQFP 64: pin N° 53 , TQFP 144 pin N° 124 */

//#define  UARTX  UART3
#define  UARTX_Rx_Pin  UART3_Rx_Pin
#define  UARTX_Tx_Pin  UART3_Tx_Pin
#define  UARTX_Periph  UART3_Periph

static void PlatformInit(void)
{
  RCCU_Div2Config(ENABLE);
  RCCU_MCLKConfig(RCCU_DEFAULT);
  RCCU_FCLKConfig(RCCU_RCLK_2);
  RCCU_PCLKConfig(RCCU_RCLK_2);
  RCCU_PLL1Config(RCCU_Mul_16,RCCU_Div_2);
  RCCU_RCLKSourceConfig(RCCU_PLL1_Output);

  GPIO_Config(GPIO0, UARTX_Tx_Pin, GPIO_AF_PP);
  GPIO_Config(GPIO0, UARTX_Rx_Pin, GPIO_IN_TRI_CMOS);

  UART_OnOffConfig(UARTX, ENABLE); 
  UART_FifoConfig (UARTX, DISABLE); 
  UART_FifoReset  (UARTX , UART_RxFIFO); 
  UART_FifoReset  (UARTX , UART_TxFIFO); 
  UART_LoopBackConfig(UARTX , DISABLE);
  UART_Config(UARTX,115200,UART_NO_PARITY,UART_1_StopBits,UARTM_8D);
  UART_RxConfig(UARTX ,ENABLE);

  {
    u8 bBuffer[4]={'S','T','R','7'};
    int i;
    for(i=0;i<4;i++)
    {
      UART_ByteSend(UARTX, (u8 *)&bBuffer[i]);
      /*  wait until the data transmission is finished */
      while(!((UART_FlagStatus(UARTX)) & UART_TxEmpty)); 
    }
  }
}

void platformmain(void)
{
    int cnt = 0;
    unsigned char key;

	PlatformInit();
	
	Printf("\r\nSTR710-EVAL Bootloader Version %d.%d\r\n",MAJOR_VER,MINOR_VER);

    Printf("\r\nPress ESC to enter main menu or other keys to run uClinux directly.\
    		\r\nOtherwise, linux will be automatically loaded in 5 seconds... 5");
    while(cnt < 25)
    {
    	WaitMs(200);
    	cnt ++;
    	if (cnt % 5 == 0)
    	{
    		PutChar('\b');
    		PutChar('0'+(5-cnt/5));
    	}
    	if (cnt >= 25) goto runlinux;
    	if (KeyPressed((int *)&key))
    	{
    		if (key == ESC_KEY) break;
    		else 
    		{
	runlinux:
    		Printf("\r\n");
   			ExecCommandline(PlatformCmdList,"loadlinux");
    		}	
    	} 
    }
    Printf("\r\n");

	for (;;)
	{
		BuildMenu (&MainMenu,NULL);
	}
}