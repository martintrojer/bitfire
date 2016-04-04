/****** implementation File for support of STFL-I based Serial Flash Memory Driver *****

   Filename:    Serialize.c
   Description:  Support to c2076.c. This files is aimed at giving a basic
    example of the SPI serial interface used to communicate with STMicroelectronics
    serial Flash devices. The functions below are used in an environment where the
    master has an embedded SPI port (STMicroelectronics µPSD).

   Version:     1.0
   Date:        08-11-2004
   Authors:    Tan Zhi, STMicroelectronics, Shanghai (China)
   Copyright (c) 2004 STMicroelectronics.

   THE PRESENT SOFTWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS WITH
   CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME. AS A
   RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT, INDIRECT OR 
   CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE CONTENT OF SUCH
   SOFTWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING INFORMATION CONTAINED HEREIN
   IN CONNECTION WITH THEIR PRODUCTS.
********************************************************************************

   Version History.
   Ver.   Date      Comments
   
   1.0   08/11/2004   Initial release
   
*******************************************************************************/
#include "71x_lib.h"
#include "serialize.h"

/******************************************************************************* 
Function:     InitSPIMaster(void)
Arguments:    
Return Values:There is no return value for this function.
Description:  This function is a one-time configuration for the CPU to set some
              ports to work in SPI mode (when they have multiple functions. For
              example, in some CPUs, the ports can be GPIO pins or SPI pins if 
              properly configured).
              please refer to the specific CPU datasheet for proper 
              configurations.
*******************************************************************************/ 

void InitSPIMaster(void)
{
    // Setup clocks
	RCCU_Div2Config(ENABLE);
	RCCU_MCLKConfig(RCCU_DEFAULT);
	RCCU_FCLKConfig(RCCU_DEFAULT);
	RCCU_PCLKConfig(RCCU_RCLK_8);
	RCCU_PLL1Config(RCCU_PLL1_Mul_16,RCCU_Div_2);
	RCCU_RCLKSourceConfig(RCCU_PLL1_Output);
  //  RCCU_RCLKSourceConfig(RCCU_CLOCK2_16);
		
	GPIO_Config (GPIO0, 0x00FF, GPIO_AF_PP);
	BSPI_Enable ( BSPI1 , ENABLE );
	BSPI_Init ( BSPI1 ) ;
	BSPI_ClockDividerConfig ( BSPI1, 8);
	BSPI_Enable ( BSPI1, ENABLE );
	BSPI_MasterEnable ( BSPI1,ENABLE );
	BSPI_ClkActiveHigh(BSPI1,DISABLE);
	BSPI_ClkFEdge(BSPI1,DISABLE);
	BSPI_8bLEn(BSPI1,ENABLE);
	BSPI_TrFifoDepth(BSPI1,1);
	BSPI_RcFifoDepth(BSPI1,1);
	
	GPIO_Config(GPIO0, 1<<12, GPIO_OUT_PP);   // Sync signal to FPGA. When high FPGA ignores SPI traffic.
	GPIO_Config(GPIO0, 1<<7, GPIO_OUT_PP);    // SPI SSEL?
	GPIO_Config(GPIO1, 1<<8, GPIO_OUT_PP);    // CSEL (should be low when writing to flash)
	GPIO_Config(GPIO2, 1<<13, GPIO_OUT_PP);   // Flash write protect
	
	GPIO_BitWrite(GPIO2,13,1);                // Write protect off
}

/******************************************************************************* 
Function:     ConfigureSpiMaster(SpiMasterConfigOptions opt)
Arguments:    opt configuration options, all acceptable values are enumerated in
              SpiMasterConfigOptions, which is a typedefed enum.
Return Values:There is no return value for this function.
Description:  This function can be used to properly configure the SPI master 
              before and after the transfer/receive operation
Pseudo Code:
   Step 1  : perform or skip select/deselect slave
   Step 2  : perform or skip enable/disable transfer
   Step 3  : perform or skip enable/disable receive
*******************************************************************************/ 

void ConfigureSpiMaster(SpiMasterConfigOptions opt)
{
  // BSPI0 Always master
}

/******************************************************************************* 
Function:     Serialize(const CharStream* char_stream_send, 
              CharStream* char_stream_recv, 
              SpiMasterConfigOptions optBefore,
              SpiMasterConfigOptions optAfter
              ) 
Arguments:    char_stream_send, the char stream to be sent from the SPI master to
              the Flash memory, usually contains instruction, address, and data to be 
              programmed.
              char_stream_recv, the char stream to be received from the Flash memory
              to the SPI master, usually contains data to be read from the memory.
              optBefore, configurations of the SPI master before any transfer/receive
              optAfter, configurations of the SPI after any transfer/receive
Return Values:TRUE
Description:  This function can be used to encapsulate a complete transfer/receive 
              operation
Pseudo Code:
   Step 1  : perform pre-transfer configuration
   Step 2  : perform transfer/ receive 
    Step 2-1: transfer ...
        (a typical process, it may vary with the specific CPU)
        Step 2-1-1:  check until the SPI master is available
        Step 2-1-2:  send the byte stream cycle after cycle. it usually involves: 
                     a) checking until the transfer-data-register is ready
                     b) filling the register with a new byte
    Step 2-2: receive ...
        (a typical process, it may vary with the specific CPU)
        Step 2-2-1:  Execute ONE pre-read cycle to clear the receive-data-register.
        Step 2-2-2:  receive the byte stream cycle after cycle. it usually involves: 
                     a) triggering a dummy cycle
                     b) checking until the transfer-data-register is ready(full)
                     c) reading the transfer-data-register 
   Step 3  : perform post-transfer configuration
*******************************************************************************/ 

static Bool do_Serialize(const CharStream* char_stream_send, 
               CharStream* char_stream_recv, 
               SpiMasterConfigOptions optBefore,
               SpiMasterConfigOptions optAfter
               ) 
{
  ST_uint32 i;
  ST_uint32 length;
  unsigned char* pChar;
  u8 rt[1];

  // Step 1  : perform pre-transfer configuration
  ConfigureSpiMaster(optBefore);

  // Step 2  : perform transfer / receive 
  // Step 2-1: transfer ...
  length = char_stream_send->length;
  pChar  = char_stream_send->pChar;
  // 2-1-1 Wait until SPI is available
  while (BSPI_FlagStatus(BSPI1,BSPI_TFF)==SET)
    ;
  // 2-1-2 send the byte stream cycle after cycle 
  for(i = 0; i < length; ++i)
    BSPI_WordSend(BSPI1,*(pChar++));

  // Step 2-2: receive ...
  // Step 2-2-1:  execute ONE pre-read cycle to clear the receive-data-register.
//  BSPI_BufferReceive(BSPI1,rt,1);

  // Step 2-2-2:  send the byte stream cycle after cycle.
  if(ptrNull != (int)char_stream_recv)                  // skip if no reception needed
  {
    length = char_stream_recv->length;
    pChar  = char_stream_recv->pChar;
    for(i = 0; i < length+1; ++i)
    {
		BSPI_WordSend(BSPI1,SPI_FLASH_INS_DUMMY);
		BSPI_BufferReceive(BSPI1,rt,1);
		if (i!=0)
			*(pChar++) = rt[0];
    }
  }

  // Step 3  : perform post-transfer configuration
  ConfigureSpiMaster(optAfter);

  return TRUE;
}

void enable_cs(void)
{
	GPIO_BitWrite(GPIO1,8,0);
	GPIO_BitWrite(GPIO0,12,1);
}

void disable_cs(void)
{
	GPIO_BitWrite(GPIO0,12,0);
	GPIO_BitWrite(GPIO1,8,1);
}

Bool Serialize(const CharStream* char_stream_send, 
               CharStream* char_stream_recv, 
               SpiMasterConfigOptions optBefore,
               SpiMasterConfigOptions optAfter
               ) 
{
	
	enable_cs();
	do_Serialize(char_stream_send,
		     char_stream_recv,
	       		optBefore,
			optAfter);
	disable_cs();

	return TRUE;
}

Bool Serialize_nf(const CharStream* char_stream_send, 
               CharStream* char_stream_recv, 
               SpiMasterConfigOptions optBefore,
               SpiMasterConfigOptions optAfter
               ) 
{
	do_Serialize(char_stream_send,
		     char_stream_recv,
	       		optBefore,
			optAfter);

	return TRUE;
}
