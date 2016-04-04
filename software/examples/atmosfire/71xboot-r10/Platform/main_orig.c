//Main.c
#include "71x_lib.h"
#include "platform.h"

#define UART0_Rx_Pin (0x0001<<8)   // TQFP 64: pin N° 63 , TQFP 144 pin N° 143
#define UART0_Tx_Pin (0x0001<<9)   // TQFP 64: pin N° 64 , TQFP 144 pin N° 144

#define  UARTX_Rx_Pin  UART0_Rx_Pin
#define  UARTX_Tx_Pin  UART0_Tx_Pin
//#define  UARTX_Periph  UART0_Periph
#define  CSn_0_Pin  (0x1<<0)	//p2.0
#define  CSn_1_Pin	(0x1<<1)	//p2.1

static void PlatformInit(void)
{
	//Set Main Freq to 48MHz
	RCCU_Div2Config(ENABLE);
	RCCU_PLL1Config(RCCU_Mul_12,RCCU_Div_2);
	RCCU_RCLKSourceConfig(RCCU_PLL1_Output);
	RCCU_MCLKConfig(RCCU_DEFAULT);	//48M
	RCCU_FCLKConfig(RCCU_RCLK_2);	//24M for PCLK1
	RCCU_PCLKConfig(RCCU_RCLK_4);	//12M for PCLK2
	
	//Config EMI bank 0(Ext Flash)--Tune WaitState 
	//GPIO_Config(GPIO2, CSn_0_Pin, GPIO_AF_PP);
	//EMI->BCR0 |= ;
	//Config EMI bank 1(Ext SRAM)
  	GPIO_Config(GPIO2, CSn_1_Pin, GPIO_AF_PP);
	EMI->BCR1 = 0x800D;
	//A.20/P2.4  A.21/P2.5
	GPIO_Config(GPIO2, (0x1 << 4 | 0x1 << 5), GPIO_AF_PP);
	
  	// Configure the GPIO pins as alternate function push pull
  	GPIO_Config(GPIO0, UARTX_Rx_Pin, GPIO_IN_TRI_CMOS);
  	GPIO_Config(GPIO0, UARTX_Tx_Pin, GPIO_AF_PP);
  	//GPIO_Config(GPIO0, UARTX_Rx_Pin|UARTX_Tx_Pin, GPIO_AF_PP);

  	// Configure the UART X
  	UART_OnOffConfig(UARTX, ENABLE);       // Turn UARTX baud generator on
  	UART_FifoConfig (UARTX, DISABLE);      // Disable FIFOs
  	UART_FifoReset(UARTX , UART_RxFIFO);   // Reset the UART_RxFIFO
  	UART_FifoReset(UARTX , UART_TxFIFO);   // Reset the UART_TxFIFO
  	UART_LoopBackConfig(UARTX , DISABLE);  // Disable Loop Back
 	/* Configure the UARTX as following:
	    - Baudrate = 115200 Bps
	    - No parity
	    - 8 data bits
	    - 1 stop bit */
	UART_Config(UARTX,115200,UART_NO_PARITY,UART_1_StopBits,UARTM_8D);
	UART_RxConfig(UARTX ,DISABLE);    
	UART_RxConfig(UARTX ,ENABLE);    
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