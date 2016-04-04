/******************** (C) COPYRIGHT 2003 STMicroelectronics ********************
* File Name          : main.c
* Author             : MCD Application Team
* Date First Issued  : 16/05/2003
* Description        : This file demonstrates how to use the BSPI software library
********************************************************************************
* History:
* 24/05/05 : V3.0
* 30/11/04 : V2.0
* 16/05/03 : Created
*******************************************************************************/
#include "71x_lib.h"

#define FPGACMD_shdn_ledsup_1  0x00
#define FPGACMD_shdn_ledsup_0  0x01
#define FPGACMD_shdnn_muxsup_0 0x02
#define FPGACMD_shdnn_muxsup_1 0x03
#define FPGACMD_lbn_rsx_1      0x04
#define FPGACMD_lbn_rsx_0      0x05
#define FPGACMD_on_rsx_1       0x06
#define FPGACMD_on_rsx_0       0x07
#define FPGACMD_sel1_rsx_0     0x08
#define FPGACMD_sel1_rsx_1     0x09
#define FPGACMD_sel2_rsx_0     0x0A
#define FPGACMD_sel2_rsx_1     0x0B
#define FPGACMD_DIMMER_PRE0    0x10
#define FPGACMD_DIMMER_PRE1    0x11
#define FPGACMD_DIMMER_PRE2    0x12
#define FPGACMD_DIMMER_PRE3    0x13
#define FPGACMD_DIMMER_PRE4    0x14
#define FPGACMD_DIMMER_PRE5    0x15
#define FPGACMD_SHOW_BUF_0     0x20
#define FPGACMD_SHOW_BUF_1     0x21
#define FPGACMD_WORK_BUF_0     0x22
#define FPGACMD_WORK_BUF_1     0x23

#define X_SIZE 40
#define Y_SIZE 16

void setup_bspi(void)
{
  BSPI_Enable ( BSPI1 , ENABLE );
  BSPI_Init   ( BSPI1 ) ;
  BSPI_ClockDividerConfig ( BSPI1, 8);
  BSPI_Enable ( BSPI1 , ENABLE );
  BSPI_MasterEnable ( BSPI1,ENABLE);
  BSPI_ClkActiveHigh(BSPI1,DISABLE);
  BSPI_ClkFEdge(BSPI1,DISABLE);
  BSPI_8bLEn(BSPI1,ENABLE);
  BSPI_TrFifoDepth(BSPI1,1);
  BSPI_RcFifoDepth(BSPI1,1);
}

void fpga_init()
{
  GPIO_Config (GPIO0, 0x00FF, GPIO_AF_PP);
  setup_bspi();
  GPIO_Config(GPIO0, 1<<12, GPIO_OUT_PP);    // GPIO0.12: Sync on level high
  GPIO_Config(GPIO2, 1<<12, GPIO_OUT_PP);    // GPIO2.12: Data/Command line
  GPIO_Config(GPIO0, 1<<7, GPIO_OUT_PP);     // SPI SSEL?
  GPIO_Config(GPIO1, 1<<8, GPIO_OUT_PP);     // CSEL (should be low when writing)
}

void spi_send(unsigned char x)
{
  int i=0;
  while (BSPI_FlagStatus(BSPI1,BSPI_TFF)==SET)
    ;
  BSPI_WordSend(BSPI1, x);
  //  while (BSPI_FlagStatus(BSPI1,BSPI_TFE)==RESET) // Do not return until SPI transfer complete (FPGA sync pins)
  //    i++;
}

void fpga_data()
{
  GPIO_BitWrite(GPIO2,12,0);  // P2.12: 0=Data, 1=Command
  GPIO_BitWrite(GPIO0,12,1);  // Toggle sync
  GPIO_BitWrite(GPIO0,12,0);
}

void fpga_command()
{
  GPIO_BitWrite(GPIO2,12,1);  // P2.12: 0=Data, 1=Command
  GPIO_BitWrite(GPIO0,12,1);  // Toggle sync
  GPIO_BitWrite(GPIO0,12,0);
}

void bsp_init_gfx(void)
{
  fpga_init();
  fpga_command();
  spi_send(FPGACMD_shdnn_muxsup_1);
  fpga_command();
  spi_send(FPGACMD_shdn_ledsup_0);
  fpga_command();
  spi_send(FPGACMD_DIMMER_PRE3);  

  fpga_command();
  spi_send(FPGACMD_WORK_BUF_0);  
  fpga_command();
  spi_send(FPGACMD_SHOW_BUF_0);  
}

void setpixel(unsigned char x, unsigned char y, unsigned char r, unsigned char g)
{
  unsigned short addr = x+y*X_SIZE;
  unsigned char addrlo = (unsigned char) addr;
  unsigned char addrhi = (unsigned char) (addr>>8);

  fpga_data(); 
  spi_send(addrlo);
  spi_send(addrhi);
  spi_send(r);
  spi_send(g);
}

int main (void)
{
  /*  Words to be transmitted */

  #ifdef DEBUG
  debug();
  #endif

  // Setup clocks
  RCCU_Div2Config(ENABLE);
  RCCU_MCLKConfig(RCCU_DEFAULT);
  RCCU_FCLKConfig(RCCU_DEFAULT);
  RCCU_PCLKConfig(RCCU_RCLK_8);
  RCCU_PLL1Config(RCCU_PLL1_Mul_16,RCCU_Div_2);
  RCCU_RCLKSourceConfig(RCCU_PLL1_Output);
  //  RCCU_RCLKSourceConfig(RCCU_CLOCK2_16);

  {
    unsigned char inc=0,x,y;
    while(1)
      {
	bsp_init_gfx();
	for (y=0;y<Y_SIZE;y++) {
	  for (x=0;x<X_SIZE;x++) {
	    setpixel(x,y,255-x*6,inc);
	  }
	}
	inc++;
      }
  }
}



















