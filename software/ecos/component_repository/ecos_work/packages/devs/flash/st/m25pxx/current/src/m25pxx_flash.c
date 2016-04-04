//==========================================================================
//
//      m25pxx_flash.c
//
//==========================================================================
//####ECOSGPLCOPYRIGHTBEGIN####
// -------------------------------------------
// This file is part of eCos, the Embedded Configurable Operating System.
// Copyright (C) 1998, 1999, 2000, 2001, 2002 Red Hat, Inc.
// Copyright (C) 2006 eCosCentric Ltd
// Copyright (C) 2006 Andrew Lunn (andrew.lunn@ascom.ch)
//
// eCos is free software; you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free
// Software Foundation; either version 2 or (at your option) any later version.
//
// eCos is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
// for more details.
//
// You should have received a copy of the GNU General Public License along
// with eCos; if not, write to the Free Software Foundation, Inc.,
// 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA.
//
// As a special exception, if other files instantiate templates or use macros
// or inline functions from this file, or you compile this file and link it
// with other works to produce a work based on this file, this file does not
// by itself cause the resulting work to be covered by the GNU General Public
// License. However the source code for this file must still be made available
// in accordance with section (3) of the GNU General Public License.
//
// This exception does not invalidate any other reasons why a work based on
// this file might be covered by the GNU General Public License.
// -------------------------------------------
//####ECOSGPLCOPYRIGHTEND####
//==========================================================================
//#####DESCRIPTIONBEGIN####
//
// Author(s):    mtrojer
// Contributors: 
// Date:         2007-04-24
// Purpose:      
// Description:  
//              
//####DESCRIPTIONEND####
//
//==========================================================================

#include <pkgconf/hal.h>
#include <pkgconf/devs_flash_m25pxx.h>
#include <cyg/hal/plf_io.h>

#include <cyg/hal/hal_io.h>
#include <cyg/hal/hal_intr.h>
#include <cyg/infra/cyg_ass.h>

#define  _FLASH_PRIVATE_
#include <cyg/io/flash.h>

#include <string.h>

#include <m25pxx_flash.h>

//----------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------

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
  BSPI_BufferReceive(BSPI1,rt,1);

  // Step 2-2-2:  send the byte stream cycle after cycle.
  if(ptrNull != (int)char_stream_recv)                  // skip if no reception needed
  {
    length = char_stream_recv->length;
    pChar  = char_stream_recv->pChar;
    for(i = 0; i < length; ++i)
    {
      BSPI_WordSend(BSPI1,SPI_FLASH_INS_DUMMY);
      BSPI_BufferReceive(BSPI1,rt,1);
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

//----------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------

/**************** STFL-I based Serial Flash Memory Driver **********************

   Filename:    c2082.c
   Description: Library routines for the M25P05A, M25P10A, M25P20, M25P40, M25P80
                M25P16, M25P32, M25P64 Serial Flash Memories

   Version:     V2.0
   Date:        20/07/2005
   Authors:     Martin Trojer, Arrow Sweden (mtrojer@arrownordic.com)
				Tan Zhi, STMicroelectronics, Shanghai (China)
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
   
   1.0   16/11/2004   Initial release
   2.0   20/07/2005   Add support for M25P32, M25P64
                      Add JEDEC ID support for M25P05A, M25P10A
      
********************************************************************************

   This source file provides library C code for M25P05A, M25P10A, M25P20,
   M25P40, M25P80, M25P16, M25P32, M25P64 serial flash devices.

   The following functions are available in this library(some memories may only support
   a subset of the list, refer to the specific product datasheet for details):

      Flash(WriteEnable, 0)                         to disable Write Protect in the Flash memory
      Flash(WriteDisable, 0)                        to enable Write Protect in the Flash memory
      Flash(ReadDeviceIdentification, ParameterType)  to get the Device Identification from the device 
      Flash(ReadManufacturerIdentification, ParameterType)(if available in the memory)    to get the manufacturer Identification from the device 
      Flash(ReadStatusRegister, ParameterType)      to get the value in the Status Register from the device 
      Flash(WriteStatusRegister, ParameterType)      to set the value in the Status Register from the device 
      Flash(Read, ParameterType)                    to read from the Flash device
      Flash(FastRead, ParameterType)                to read from the Flash device in a faster way
      Flash(PageProgram, ParameterType)             to write an array of elements within one page
      Flash(SectorErase, ParameterType)             to erase a whole sector
      Flash(SubSectorErase, ParameterType)          to erase a sub sector
      Flash(BulkErase, ParameterType)               to erase the whole memory
      Flash(DeepPowerDown, 0)                       to set the memory into the low power consumption mode
      Flash(ReleaseFromDeepPowerDown, 0)            to wake up the memory from the low power consumption mode
      Flash(Program, ParameterType)                 to program an array of elements
      FlashErrorStr()                               to return an error description (define VERBOSE)
      
   Note that data Bytes will be referred to as elements throughout the document unless otherwise specified.

   For further information consult the related Datasheets and Application Note.
   The Application Note gives information about how to modify this code for
   a specific application.

   The hardware specific functions which may need to be modified by the user are:

      FlashWrite() used to write an element (uCPUBusType) to the Flash memory
      FlashRead()  used to read an element (uCPUBusType) from the Flash memory
      FlashTimeOut() to return after the function has timed out 

   A list of the error conditions can be found at the end of the header file.

*******************************************************************************/

#ifdef TIME_H_EXISTS
  #include <time.h>
#endif 

#ifdef SYNCHRONOUS_IO    
#define WAIT_TILL_Instruction_EXECUTION_COMPLETE(x) FlashTimeOut(0); while(IsFlashBusy()) \
    { \
        if(Flash_OperationTimeOut == FlashTimeOut(x)) return  Flash_OperationTimeOut; \
    };
 
#else  
    // do nothing
#endif


/******************************************************************************
    Global variables: none
*******************************************************************************/ 

/*******************************************************************************
Function:     ReturnType Flash( InstructionType insInstruction, ParameterType *fp )
Arguments:    insInstruction is an enum which contains all the available Instructions 
    of the SW driver.
              fp is a (union) parameter struct for all Flash Instruction parameters
Return Value: The function returns the following conditions: 

   Flash_AddressInvalid, 
   Flash_MemoryOverflow, 
   Flash_PageEraseFailed, 
   Flash_PageNrInvalid, 
   Flash_SectorNrInvalid, 
   Flash_FunctionNotSupported,
   Flash_NoInformationAvailable,
   Flash_OperationOngoing, 
   Flash_OperationTimeOut, 
   Flash_ProgramFailed, 
   Flash_SpecificError,
   Flash_Success, 
   Flash_WrongType

Description:  This function is used to access all functions provided with the
   current Flash device.

Pseudo Code:
   Step 1: Select the right action using the insInstruction parameter
   Step 2: Execute the Flash memory Function
   Step 3: Return the Error Code
*******************************************************************************/
ReturnType Flash( InstructionType insInstruction, ParameterType *fp ) { 
    ReturnType  rRetVal;
	ST_uint8  ucStatusRegister;
    ST_uint16 ucDeviceIdentification;
#ifdef USE_JEDEC_STANDARD_TWO_BYTE_SIGNATURE
    ST_uint8  ucManufacturerIdentification;
#endif 
   switch (insInstruction) {
      case WriteEnable: 
           rRetVal = FlashWriteEnable( ); 
           break; 

      case WriteDisable: 
           rRetVal = FlashWriteDisable( );
           break; 
            
      case ReadDeviceIdentification: 
           rRetVal = FlashReadDeviceIdentification(&ucDeviceIdentification);
           (*fp).ReadDeviceIdentification.ucDeviceIdentification = ucDeviceIdentification;
           break; 

#ifdef USE_JEDEC_STANDARD_TWO_BYTE_SIGNATURE
      case ReadManufacturerIdentification: 
           rRetVal = FlashReadManufacturerIdentification(&ucManufacturerIdentification);
           (*fp).ReadManufacturerIdentification.ucManufacturerIdentification = ucManufacturerIdentification;
           break; 
#endif

      case ReadStatusRegister:
            rRetVal = FlashReadStatusRegister(&ucStatusRegister);
           (*fp).ReadStatusRegister.ucStatusRegister = ucStatusRegister;
           break;

      case WriteStatusRegister:
           ucStatusRegister = (*fp).WriteStatusRegister.ucStatusRegister;
            rRetVal = FlashWriteStatusRegister(ucStatusRegister);
           break;

      case Read: 
           rRetVal = FlashRead( (*fp).Read.udAddr,
                                (*fp).Read.pArray,
                                (*fp).Read.udNrOfElementsToRead
                              ); 
           break; 

      case FastRead:
           rRetVal = FlashFastRead( (*fp).Read.udAddr,
                                    (*fp).Read.pArray,
                                    (*fp).Read.udNrOfElementsToRead
                                  ); 
           break;
 
      case PageProgram:
           rRetVal = FlashProgram( (*fp).PageProgram.udAddr,
                                 (*fp).PageProgram.pArray,
                                 (*fp).PageProgram.udNrOfElementsInArray
                               );  
           break;
           
      case SectorErase: 
           rRetVal = FlashSectorErase((*fp).SectorErase.ustSectorNr ); 
           break; 
           
      case SubSectorErase: 
           rRetVal = FlashSubSectorErase((*fp).SubSectorErase.ustSectorNr ); 
           break; 
		   
      case BulkErase: 
           rRetVal = FlashBulkErase( ); 
           break; 


 #ifndef NO_DEEP_POWER_DOWN_SUPPORT
      case DeepPowerDown:
           rRetVal = FlashDeepPowerDown( );                 
           break;
	  
      case ReleaseFromDeepPowerDown:
           rRetVal = FlashReleaseFromDeepPowerDown( );                 
           break;
#endif

      case Program:
           rRetVal = FlashProgram( (*fp).Program.udAddr,
                                   (*fp).Program.pArray,
                                   (*fp).Program.udNrOfElementsInArray);                 
           break;
           
       default:
           rRetVal = Flash_FunctionNotSupported;
           break;

   } /* EndSwitch */
   return rRetVal;
} /* EndFunction Flash */

/*******************************************************************************
Function:     FlashWriteEnable( void )
Arguments:    void

Return Value: 
   Flash_Success 

Description:  This function sets the Write Enable Latch(WEL)
              by sending a WREN Instruction.

Pseudo Code:
   Step 1: Initialize the data (i.e. Instruction) packet to be sent serially
   Step 2: Send the packet serially
*******************************************************************************/
ReturnType  FlashWriteEnable( void )
{
    CharStream char_stream_send;
    ST_uint8  cWREN = SPI_FLASH_INS_WREN; 

    // Step 1: Initialize the data (i.e. Instruction) packet to be sent serially
    char_stream_send.length = 1;
    char_stream_send.pChar  = &cWREN;

    // Step 2: Send the packet serially
    Serialize(&char_stream_send, 
              ptrNull,
              enumEnableTransOnly_SelectSlave,
              enumDisableTransOnly_DeSelectSlave
              );
    return Flash_Success;
}

/*******************************************************************************
Function:     FlashWriteDisable( void )
Arguments:    void

Return Value: 
   Flash_Success 

Description:  This function resets the Write Enable Latch(WEL)
              by sending a WRDI Instruction.

Pseudo Code:
   Step 1: Initialize the data (i.e. Instruction) packet to be sent serially
   Step 2: Send the packet serially
*******************************************************************************/
ReturnType  FlashWriteDisable( void )
{
    CharStream char_stream_send;
    ST_uint8  cWRDI = SPI_FLASH_INS_WRDI; 

    // Step 1: Initialize the data (i.e. Instruction) packet to be sent serially
    char_stream_send.length = 1;
    char_stream_send.pChar  = &cWRDI;

    // Step 2: Send the packet serially
    Serialize(&char_stream_send, 
              ptrNull,
              enumEnableTransOnly_SelectSlave,
              enumDisableTransOnly_DeSelectSlave
              );
    return Flash_Success;
}

/*******************************************************************************
Function:     FlashReadDeviceIdentification( ST_uint16 *uwpDeviceIdentification)
Arguments:    uwpDeviceIdentificaiton, 16-bit buffer to hold the DeviceIdentification read from the 
              memory, with memory type residing in the higher 8 bits, and
              memory capacity in the lower ones.

Return Value: 
   Flash_Success
   Flash_WrongType(if USE_JEDEC_STANDARD_TWO_BYTE_SIGNATURE defined)

Description:  This function returns the Device Identification (memory type + memory capacity)
              by sending an SPI_FLASH_INS_RDID Instruction.
              After retrieving the Device Identificaiton, the routine checks if the device is
              an expected device(defined by EXPECTED_DEVICE). If not, 
              Flash_WrongType is returned.

              If USE_JEDEC_STANDARD_TWO_BYTE_SIGNATURE is defined, the returned 16-bit
              word comprises memory type(higher 8 bits) and memory capacity
              (lower 8 bits).
              If USE_JEDEC_STANDARD_TWO_BYTE_SIGNATURE is NOT defined, only the lower
              8-bit byte of the returned 16-bit word is valid information,i.e. the 
              Device Identification.
              For memories that have a capacity of more than 16Mb(inclusive), 
              USE_JEDEC_STANDARD_TWO_BYTE_SIGNATURE is defined by default.

Pseudo Code:
   Step 1: Initialize the data (i.e. Instruction) packet to be sent serially
   Step 2: Send the packet serially
   Step 3: Device Identification is returned
*******************************************************************************/
ReturnType  FlashReadDeviceIdentification( ST_uint16 *uwpDeviceIdentification)
{
#ifdef USE_JEDEC_STANDARD_TWO_BYTE_SIGNATURE
    CharStream char_stream_send;
    CharStream char_stream_recv;
    ST_uint8  cRDID = SPI_FLASH_INS_RDID; 
    ST_uint8  pIdentification[3]; 

    // Step 1: Initialize the data (i.e. Instruction) packet to be sent serially
    char_stream_send.length  = 1;
    char_stream_send.pChar   = &cRDID;

    char_stream_recv.length  = 3;
    char_stream_recv.pChar   = &pIdentification[0];

    // Step 2: Send the packet serially
    Serialize(&char_stream_send, 
              &char_stream_recv,
              enumEnableTansRecv_SelectSlave,
              enumDisableTansRecv_DeSelectSlave
              );

    // Step 3: Device Identification is returned ( memory type + memory capacity )
    *uwpDeviceIdentification = char_stream_recv.pChar[1];
    *uwpDeviceIdentification <<= 8;
    *uwpDeviceIdentification |= char_stream_recv.pChar[2];

    if(EXPECTED_DEVICE == *uwpDeviceIdentification)
        return Flash_Success;
    else
        return Flash_WrongType;

#else   // USE_JEDEC_STANDARD_TWO_BYTE_SIGNATURE not defined
    CharStream char_stream_send;
    CharStream char_stream_recv;
    ST_uint8  pIns[4]; 
    ST_uint8  cRDID = SPI_FLASH_INS_RES; 
    ST_uint8  pIdentification; 

    // Step 1: Initialize the data (i.e. Instruction) packet to be sent serially
    char_stream_send.length  = 4;
    char_stream_send.pChar   = &pIns[0];
    pIns[0] = SPI_FLASH_INS_RES;
    pIns[1] = SPI_FLASH_INS_DUMMY;
    pIns[2] = SPI_FLASH_INS_DUMMY;
    pIns[3] = SPI_FLASH_INS_DUMMY;

    char_stream_recv.length  = 1;
    char_stream_recv.pChar   = &pIdentification;

    // Step 2: Send the packet serially
    Serialize(&char_stream_send, 
              &char_stream_recv,
              enumEnableTansRecv_SelectSlave,
              enumDisableTansRecv_DeSelectSlave
              );

    // Step 3: Get the returned device Identification
    *uwpDeviceIdentification = *char_stream_recv.pChar;

    if(EXPECTED_DEVICE == *uwpDeviceIdentification)
        return Flash_Success;
    else
        return Flash_WrongType;
#endif
}

#ifdef USE_JEDEC_STANDARD_TWO_BYTE_SIGNATURE
/*******************************************************************************
Function:     FlashReadManufacturerIdentification( ST_uint8 *ucpManufactureIdentification)
Arguments:    ucpManufacturerIdentification, 8-bit buffer to hold the manufacturer identification
              being read from the memory

Return Value: 
   Flash_WrongType: if any value other than MANUFACTURER_ST(0x20) is returned
   Flash_Success : if MANUFACTURER_ST(0x20) is correctly returned

Description:  This function returns the Manufacturer Identification(0x20) by sending an 
              SPI_FLASH_INS_RDID Instruction.
              After retrieving the Manufacturer Identification, the routine checks if the device is
              an ST memory product. If not, Flash_WrongType is returned.

Note: The availability of this function should be checked in the appropriate datasheet
	for each memory.

Pseudo Code:
   Step 1: Initialize the data (i.e. Instruction) packet to be sent serially
   Step 2: Send the packet serially
   Step 3: get the Manufacturer Identification
*******************************************************************************/
ReturnType  FlashReadManufacturerIdentification( ST_uint8 *ucpManufacturerIdentification)
{
    CharStream char_stream_send;
    CharStream char_stream_recv;
    ST_uint8  cRDID = SPI_FLASH_INS_RDID; 
    ST_uint8  pIdentification[3]; 

    // Step 1: Initialize the data (i.e. Instruction) packet to be sent serially
    char_stream_send.length  = 1;
    char_stream_send.pChar   = &cRDID;
    char_stream_recv.length  = 3;
    char_stream_recv.pChar   = &pIdentification[0];

    // Step 2: Send the packet serially
    Serialize(&char_stream_send, 
              &char_stream_recv,
              enumEnableTansRecv_SelectSlave,
              enumDisableTansRecv_DeSelectSlave
              );

    // Step 3: get the Manufacturer Identification
    *ucpManufacturerIdentification = pIdentification[0];
    if(MANUFACTURER_ST == *ucpManufacturerIdentification)
    {
        return Flash_Success;
    }
    else
    {
        return Flash_WrongType;
    }
}
#endif // end of #ifdef USE_JEDEC_STANDARD_TWO_BYTE_SIGNATURE

/*******************************************************************************
Function:     FlashReadStatusRegister( ST_uint8 *ucpStatusRegister)
Arguments:    ucpStatusRegister, 8-bit buffer to hold the Status Register value read
              from the memory

Return Value: 
   Flash_Success

Description:  This function reads the Status Register by sending an
               SPI_FLASH_INS_RDSR Instruction.

Pseudo Code:
   Step 1: Initialize the data (i.e. Instruction) packet to be sent serially
   Step 2: Send the packet serially, get the Status Register content

*******************************************************************************/
ReturnType  FlashReadStatusRegister( ST_uint8 *ucpStatusRegister)
{
    CharStream char_stream_send;
    CharStream char_stream_recv;
    ST_uint8  cRDSR = SPI_FLASH_INS_RDSR; 


    // Step 1: Initialize the data (i.e. Instruction) packet to be sent serially
    char_stream_send.length  = 1;
    char_stream_send.pChar   = &cRDSR;
    char_stream_recv.length  = 1;
    char_stream_recv.pChar   = ucpStatusRegister;

    // Step 2: Send the packet serially, get the Status Register content
    Serialize(&char_stream_send, 
              &char_stream_recv,
              enumEnableTansRecv_SelectSlave,
              enumDisableTansRecv_DeSelectSlave
              );

    return Flash_Success;
}


/*******************************************************************************
Function:     FlashWriteStatusRegister( ST_uint8 ucStatusRegister)
Arguments:    ucStatusRegister, an 8-bit new value to be written to the Status Register

Return Value: 
   Flash_Success

Description:  This function modifies the Status Register by sending an
              SPI_FLASH_INS_WRSR Instruction.
              The Write Status Register (WRSR) Instruction has no effect
              on b6, b5, b1(WEL) and b0(WIP) of the Status Register.b6 and b5 are 
              always read as 0.

Pseudo Code:
   Step 1: Disable Write protection
   Step 2: Initialize the data (i.e. Instruction & value) packet to be sent serially
   Step 3: Send the packet serially
   Step 4: Wait until the operation completes or a timeout occurs.
*******************************************************************************/
ReturnType  FlashWriteStatusRegister( ST_uint8 ucStatusRegister)
{
    CharStream char_stream_send;
    ST_uint8  pIns_Val[2];

    // Step 1: Disable Write protection
    FlashWriteEnable();

    // Step 2: Initialize the data (i.e. Instruction & value) packet to be sent serially
    char_stream_send.length = 2;
    char_stream_send.pChar  = pIns_Val;
    pIns_Val[0] = SPI_FLASH_INS_WRSR; 
    pIns_Val[1] = ucStatusRegister; 

    // Step 3: Send the packet serially
    Serialize(&char_stream_send, 
              ptrNull,
              enumEnableTransOnly_SelectSlave,
              enumDisableTransOnly_DeSelectSlave
              );
    // Step 4: Wait until the operation completes or a timeout occurs.
    WAIT_TILL_Instruction_EXECUTION_COMPLETE(1)
    return Flash_Success;
}

/*******************************************************************************
Function:     FlashRead( ST_uint32 udAddr, ST_uint8 *ucpElements, ST_uint32 udNrOfElementsToRead)
Arguments:    udAddr, start address to read from
              ucpElements, buffer to hold the elements to be returned
              udNrOfElementsToRead, number of elements to be returned, counted in bytes.

Return Value:
   Flash_AddressInvalid
   Flash_Success

Description:  This function reads the Flash memory by sending an
              SPI_FLASH_INS_READ Instruction.
              by design, the whole Flash memory space can be read with one READ Instruction
              by incrementing the start address and rolling to 0x0 automatically,
              that is, this function is across pages and sectors.

Pseudo Code:
   Step 1: Validate address input
   Step 2: Initialize the data (i.e. Instruction) packet to be sent serially
   Step 3: Send the packet serially, and fill the buffer with the data being returned
*******************************************************************************/
ReturnType  FlashRead( uAddrType udAddr, ST_uint8 *ucpElements, ST_uint32 udNrOfElementsToRead)
{
    CharStream char_stream_send;
    CharStream char_stream_recv;
    ST_uint8  pIns_Addr[4]; 

    // Step 1: Validate address input
    if(!(udAddr <  FLASH_SIZE)) return Flash_AddressInvalid;

    // Step 2: Initialize the data (i.e. Instruction) packet to be sent serially
    char_stream_send.length   = 4;
    char_stream_send.pChar    = pIns_Addr;
    pIns_Addr[0]              = SPI_FLASH_INS_READ;
    pIns_Addr[1]              = udAddr>>16;
    pIns_Addr[2]              = udAddr>>8;
    pIns_Addr[3]              = udAddr;

    char_stream_recv.length   = udNrOfElementsToRead;
    char_stream_recv.pChar    = ucpElements;

    // Step 3: Send the packet serially, and fill the buffer with the data being returned
    Serialize(&char_stream_send, 
              &char_stream_recv,
              enumEnableTansRecv_SelectSlave,
              enumDisableTansRecv_DeSelectSlave
              );

    return Flash_Success;
}

/*******************************************************************************
Function:     FlashFastRead( ST_uint32 udAddr, ST_uint8 *ucpElements, ST_uint32 udNrOfElementsToRead)
Arguments:    udAddr, start address to read from
              ucpElements, buffer to hold the elements to be returned
              udNrOfElementsToRead, number of elements to be returned, counted in bytes.

Return Value:
   Flash_AddressInvalid
   Flash_Success

Description:  This function reads the Flash memory by sending an
              SPI_FLASH_INS_FAST_READ Instruction.
              by design, the whole Flash memory space can be read with one FAST_READ Instruction
              by incrementing the start address and rolling to 0x0 automatically,
              that is, this function is across pages and sectors.

Pseudo Code:
   Step 1: Validate address input
   Step 2: Initialize the data (i.e. Instruction) packet to be sent serially
   Step 3: Send the packet serially, and fill the buffer with the data being returned
*******************************************************************************/
ReturnType  FlashFastRead( uAddrType udAddr, ST_uint8 *ucpElements, ST_uint32 udNrOfElementsToRead)
{
    CharStream char_stream_send;
    CharStream char_stream_recv;
    ST_uint8  pIns_Addr[5]; 

    // Step 1: Validate address input
    if(!(udAddr <  FLASH_SIZE)) return Flash_AddressInvalid;

    // Step 2: Initialize the data (i.e. Instruction) packet to be sent serially
    char_stream_send.length   = 5;
    char_stream_send.pChar    = pIns_Addr;
    pIns_Addr[0]              = SPI_FLASH_INS_FAST_READ;
    pIns_Addr[1]              = udAddr>>16;
    pIns_Addr[2]              = udAddr>>8;
    pIns_Addr[3]              = udAddr;
    pIns_Addr[4]              = SPI_FLASH_INS_DUMMY;

    char_stream_recv.length   = udNrOfElementsToRead;
    char_stream_recv.pChar    = ucpElements;

    // Step 3: Send the packet serially, and fill the buffer with the data being returned
    Serialize(&char_stream_send, 
              &char_stream_recv,
              enumEnableTansRecv_SelectSlave,
              enumDisableTansRecv_DeSelectSlave
              );

    return Flash_Success;
}

/*******************************************************************************
Function:     FlashPageProgram( ST_uint32 udAddr, ST_uint8 *pArray, ST_uint32 udNrOfElementsInArray)
Arguments:    udAddr, start address to write to
              pArray, buffer to hold the elements to be programmed
              udNrOfElementsInArray, number of elements to be programmed, counted in bytes

Return Value:
   Flash_AddressInvalid
   Flash_OperationOngoing
   Flash_OperationTimeOut
   Flash_Success

Description:  This function writes a maximum of 256 bytes of data into the memory by sending an
              SPI_FLASH_INS_PP Instruction.
              by design, the PP Instruction is effective WITHIN ONE page,i.e. 0xXX00 - 0xXXff.
              when 0xXXff is reached, the address rolls over to 0xXX00 automatically.
Note:
              This function does not check whether the target memory area is in a Software
              Protection Mode(SPM) or Hardware Protection Mode(HPM), in which case the PP
              Instruction will be ignored.
              The function assumes that the target memory area has previously been unprotected at both
              the hardware and software levels.
              To unprotect the memory, please call FlashWriteStatusRegister(ST_uint8 ucStatusRegister), 
              and refer to the datasheet for the setup of a proper ucStatusRegister value.
Pseudo Code:
   Step 1: Validate address input
   Step 2: Check whether any previous Write, Program or Erase cycle is on going
   Step 3: Disable Write protection (the Flash memory will automatically enable it again after 
           the execution of the Instruction)
   Step 4: Initialize the data (Instruction & address only) packet to be sent serially
   Step 5: Send the packet (Instruction & address only) serially
   Step 6: Initialize the data (data to be programmed) packet to be sent serially
   Step 7: Send the packet (data to be programmed) serially
   Step 8: Wait until the operation completes or a timeout occurs.
*******************************************************************************/
ReturnType  FlashPageProgram( uAddrType udAddr, ST_uint8 *pArray , ST_uint16 udNrOfElementsInArray)
{
    CharStream char_stream_send;
    ST_uint8  pIns_Addr[4]; 

    // Step 1: Validate address input
    if(!(udAddr <  FLASH_SIZE)) return Flash_AddressInvalid;

    // Step 2: Check whether any previous Write, Program or Erase cycle is on-going
    if(IsFlashBusy()) return Flash_OperationOngoing;

    // Step 3: Disable Write protection
    FlashWriteEnable();

    // Step 4: Initialize the data (Instruction & address only) packet to be sent serially
    char_stream_send.length   = 4;
    char_stream_send.pChar    = pIns_Addr;
    pIns_Addr[0]              = SPI_FLASH_INS_PP;
    pIns_Addr[1]              = udAddr>>16;
    pIns_Addr[2]              = udAddr>>8;
    pIns_Addr[3]              = udAddr;

    // Step 5: Send the packet (Instruction & address only) serially
	enable_cs();
    Serialize_nf(&char_stream_send, 
              ptrNull,
              enumEnableTransOnly_SelectSlave,
              enumNull
              );

    // Step 6: Initialize the data (data to be programmed) packet to be sent serially
    char_stream_send.length   = udNrOfElementsInArray;
    char_stream_send.pChar    = pArray;

    // Step 7: Send the packet (data to be programmed) serially
    Serialize_nf(&char_stream_send, 
              ptrNull,
              enumNull,
              enumDisableTransOnly_DeSelectSlave
              );
	disable_cs();

    // Step 8: Wait until the operation completes or a timeout occurs.
    WAIT_TILL_Instruction_EXECUTION_COMPLETE(1)

    return Flash_Success;
}

/*******************************************************************************
Function:     ReturnType FlashSectorErase( uSectorType uscSectorNr )
Arguments:    uSectorType is the number of the Sector to be erased.

Return Values:
   Flash_SectorNrInvalid
   Flash_OperationOngoing
   Flash_OperationTimeOut
   Flash_Success           

Description:  This function erases the Sector specified in uscSectorNr by sending an 
              SPI_FLASH_INS_SE Instruction.
              The function checks that the sector number is within the valid range 
              before issuing the erase Instruction. Once erase has completed the status
              Flash_Success is returned.
Note:
              This function does not check whether the target memory area is in a Software
              Protection Mode(SPM) or Hardware Protection Mode(HPM), in which case the PP
              Instruction will be ignored.
              The function assumes that the target memory area has previously been unprotected at both
              the hardware and software levels.
              To unprotect the memory, please call FlashWriteStatusRegister(ST_uint8 ucStatusRegister), 
              and refer to the datasheet to set a proper ucStatusRegister value.

Pseudo Code:
   Step 1: Validate the sector number input
   Step 2: Check whether any previous Write, Program or Erase cycle is on going
   Step 3: Disable Write protection (the Flash memory will automatically enable it 
           again after the execution of the Instruction)
   Step 4: Initialize the data (Instruction & address) packet to be sent serially
   Step 5: Send the packet (Instruction & address) serially
   Step 6: Wait until the operation completes or a timeout occurs.
*******************************************************************************/
ReturnType  FlashSectorErase( uSectorType uscSectorNr )
{
    CharStream char_stream_send;
    ST_uint8  pIns_Addr[4]; 

    // Step 1: Validate the sector number input
    if(!(uscSectorNr < FLASH_SECTOR_COUNT)) return Flash_SectorNrInvalid;

    // Step 2: Check whether any previous Write, Program or Erase cycle is on going
    if(IsFlashBusy()) return Flash_OperationOngoing;

    // Step 3: Disable Write protection
    FlashWriteEnable();

    // Step 4: Initialize the data (Instruction & address) packet to be sent serially
    char_stream_send.length   = 4;
    char_stream_send.pChar    = &pIns_Addr[0];
    pIns_Addr[0]              = SPI_FLASH_INS_SE;
    #ifdef FLASH_SMALLER_SECTOR_SIZE
    pIns_Addr[1]              = uscSectorNr>>1;
    pIns_Addr[2]              = uscSectorNr<<7;
    #else
    pIns_Addr[1]              = uscSectorNr;
    pIns_Addr[2]              = 0;
    #endif
    pIns_Addr[3]              = 0;

    // Step 5: Send the packet (Instruction & address) serially
    Serialize(&char_stream_send, 
              ptrNull,
              enumEnableTransOnly_SelectSlave,
              enumDisableTansRecv_DeSelectSlave
              );

    // Step 6: Wait until the operation completes or a timeout occurs.
    WAIT_TILL_Instruction_EXECUTION_COMPLETE(3)

    return Flash_Success;
}
/*******************************************************************************
Function:     ReturnType FlashSubSectorErase( unsgined int uscSectorNr )
Arguments:    uSectorType is the number of the Sector to be erased.

Return Values:
   Flash_SectorNrInvalid
   Flash_OperationOngoing
   Flash_OperationTimeOut
   Flash_Success           

Description:  This function erases the Sector specified in uscSectorNr by sending an 
              SPI_FLASH_INS_SE Instruction.
              The function checks that the sector number is within the valid range 
              before issuing the erase Instruction. Once erase has completed the status
              Flash_Success is returned.
Note:
              This function does not check whether the target memory area is in a Software
              Protection Mode(SPM) or Hardware Protection Mode(HPM), in which case the PP
              Instruction will be ignored.
              The function assumes that the target memory area has previously been unprotected at both
              the hardware and software levels.
              To unprotect the memory, please call FlashWriteStatusRegister(ST_uint8 ucStatusRegister), 
              and refer to the datasheet to set a proper ucStatusRegister value.

Pseudo Code:
   Step 1: Validate the sector number input
   Step 2: Check whether any previous Write, Program or Erase cycle is on going
   Step 3: Disable Write protection (the Flash memory will automatically enable it 
           again after the execution of the Instruction)
   Step 4: Initialize the data (Instruction & address) packet to be sent serially
   Step 5: Send the packet (Instruction & address) serially
   Step 6: Wait until the operation completes or a timeout occurs.
*******************************************************************************/
ReturnType  FlashSubSectorErase( unsigned int uscSectorNr )
{
	CharStream char_stream_send;
	ST_uint8  pIns_Addr[4]; 

    // Step 1: Validate the sector number input
	if(!(uscSectorNr < FLASH_SECTOR_COUNT)) return Flash_SectorNrInvalid;
	
	uscSectorNr = uscSectorNr * 0x1000;

    // Step 2: Check whether any previous Write, Program or Erase cycle is on going
	if(IsFlashBusy()) return Flash_OperationOngoing;

    // Step 3: Disable Write protection
	FlashWriteEnable();

    // Step 4: Initialize the data (Instruction & address) packet to be sent serially
	char_stream_send.length   = 4;
	char_stream_send.pChar    = &pIns_Addr[0];
	pIns_Addr[0]              = SPI_FLASH_INS_SSE;
#ifdef FLASH_SMALLER_SECTOR_SIZE
	pIns_Addr[1]              = uscSectorNr>>1;
	pIns_Addr[2]              = uscSectorNr<<7;
	pIns_Addr[3]              = 0;
#else
	pIns_Addr[1]              = (uscSectorNr&0x00FF0000)>>16;
	pIns_Addr[2]              = (uscSectorNr&0x0000FF00)>>8;
	pIns_Addr[3]              = (uscSectorNr&0x000000FF);
#endif

    // Step 5: Send the packet (Instruction & address) serially
	Serialize(&char_stream_send, 
			   ptrNull,
	  enumEnableTransOnly_SelectSlave,
   enumDisableTansRecv_DeSelectSlave
			 );

    // Step 6: Wait until the operation completes or a timeout occurs.
	WAIT_TILL_Instruction_EXECUTION_COMPLETE(3)

			return Flash_Success;
}
/*******************************************************************************
Function:     ReturnType FlashBulkErase( void )
Arguments:    none

Return Values:
   Flash_OperationOngoing
   Flash_OperationTimeOut
   Flash_Success           

Description:  This function erases the whole Flash memory by sending an 
              SPI_FLASH_INS_BE Instruction.
Note:
              This function does not check whether the target memory area (or part of it) 
			  is in a Software Protection Mode(SPM) or Hardware Protection Mode(HPM),
			  in which case the PP Instruction will be ignored.
              The function assumes that the target memory area has previously been unprotected at both
              the hardware and software levels.
              To unprotect the memory, please call FlashWriteStatusRegister(ST_uint8 ucStatusRegister), 
              and refer to the datasheet to set a proper ucStatusRegister value.

Pseudo Code:
   Step 1: Check whether any previous Write, Program or Erase cycle is on going
   Step 2: Disable the Write protection (the Flash memory will automatically enable it 
           again after the execution of the Instruction)
   Step 3: Initialize the data (Instruction & address) packet to be sent serially
   Step 4: Send the packet (Instruction & address) serially
   Step 5: Wait until the operation completes or a timeout occurs.
*******************************************************************************/
ReturnType  FlashBulkErase( void )
{
    CharStream char_stream_send;
    ST_uint8  cBE = SPI_FLASH_INS_BE; 

    // Step 1: Check whether any previous Write, Program or Erase cycle is on going
    if(IsFlashBusy()) return Flash_OperationOngoing;

    // Step 2: Disable Write protection
    FlashWriteEnable();

    // Step 3: Initialize the data(Instruction & address) packet to be sent serially
    char_stream_send.length   = 1;
    char_stream_send.pChar    = &cBE;

    // Step 4: Send the packet(Instruction & address) serially
    Serialize(&char_stream_send, 
              ptrNull,
              enumEnableTransOnly_SelectSlave,
              enumDisableTansRecv_DeSelectSlave
              );

    // Step 5: Wait until the operation completes or a timeout occurs.
    WAIT_TILL_Instruction_EXECUTION_COMPLETE(BE_TIMEOUT)

    return Flash_Success;
}

#ifndef NO_DEEP_POWER_DOWN_SUPPORT
/*******************************************************************************
Function:     FlashDeepPowerDown( void )
Arguments:    void

Return Value:
   Flash_OperationOngoing
   Flash_Success 

Description:  This function puts the device in the lowest consumption
              mode (the Deep Power-down mode) by sending an SPI_FLASH_INS_DP.
              After calling this routine, the Flash memory will not respond to any 
              subsequent Instruction except for the RDP Instruction.

Pseudo Code:
   Step 1: Initialize the data (i.e. Instruction) packet to be sent serially
   Step 2: Check whether any previous Write, Program or Erase cycle is on going
   Step 3: Send the packet serially
*******************************************************************************/
ReturnType  FlashDeepPowerDown( void )
{
    CharStream char_stream_send;
    ST_uint8  cDP = SPI_FLASH_INS_DP; 

    // Step 1: Initialize the data (i.e. Instruction) packet to be sent serially
    char_stream_send.length = 1;
    char_stream_send.pChar  = &cDP;

    // Step 2: Check whether any previous Write, Program or Erase cycle is on going
    if(IsFlashBusy()) return Flash_OperationOngoing;

    // Step 3: Send the packet serially
    Serialize(&char_stream_send, 
              ptrNull,
              enumEnableTransOnly_SelectSlave,
              enumDisableTransOnly_DeSelectSlave
              );

    return Flash_Success;
}

/*******************************************************************************
Function:     FlashReleaseFromDeepPowerDown( void )
Arguments:    void

Return Value:
   Flash_Success 

Description:  This function takes the device out of the Deep Power-down 
              mode by sending an SPI_FLASH_INS_RES.

Pseudo Code:
   Step 1: Initialize the data (i.e. Instruction) packet to be sent serially
   Step 2: Send the packet serially
*******************************************************************************/
ReturnType  FlashReleaseFromDeepPowerDown( void )
{
    CharStream char_stream_send;
    ST_uint8  cRES = SPI_FLASH_INS_RES; 

    // Step 1: Initialize the data (i.e. Instruction) packet to be sent serially
    char_stream_send.length = 1;
    char_stream_send.pChar  = &cRES;

    // Step 2: Send the packet serially
    Serialize(&char_stream_send, 
              ptrNull,
              enumEnableTransOnly_SelectSlave,
              enumDisableTransOnly_DeSelectSlave
              );
    return Flash_Success;
}
#endif //   end of #ifndef NO_DEEP_POWER_DOWN_SUPPORT

/*******************************************************************************
Function:     FlashProgram( ST_uint32 udAddr, ST_uint8 *pArray, ST_uint32 udNrOfElementsInArray )
Arguments:    udAddr, start address to program
              pArray, address of the  buffer that holds the elements to be programmed
              udNrOfElementsInArray, number of elements to be programmed, counted in bytes

Return Value:
   Flash_AddressInvalid
   Flash_MemoryOverflow
   Flash_OperationTimeOut
   Flash_Success

Description:  This function programs a chunk of data into the memory at one go.
              If the start address and the available space are checked successfully, 
              this function programs data from the buffer(pArray) to the memory sequentially by
              invoking FlashPageProgram(). This function automatically handles page boundary
              crossing, if any.
              Like FlashPageProgram(), this function assumes that the memory to be programmed 
              has been previously erased or that bits are only changed from 1 to 0.
Note:
              This function does not check whether the target memory area is in a Software
              Protection Mode(SPM) or Hardware Protection Mode(HPM), in which case the PP
              Instruction will be ignored.
              The function assumes that the target memory area has previously been unprotected at both
              the hardware and software levels.
              To unprotect the memory, please call FlashWriteStatusRegister(ST_uint8 ucStatusRegister), 
              and refer to the datasheet for a proper ucStatusRegister value.

Pseudo Code:
   Step 1: Validate address input
   Step 2: Check memory space available on the whole memory
   Step 3: calculte memory space available within the page containing the start address(udAddr)
   Step 3-1: if the page boundary is crossed, invoke FlashPageProgram() repeatedly
   Step 3-2: if the page boundary is not crossed, invoke FlashPageProgram() once only
*******************************************************************************/
ReturnType  FlashProgram( ST_uint32 udAddr, ST_uint8 *pArray, ST_uint32 udNrOfElementsInArray )
{
    ST_uint16 ucMargin;
    ST_uint16 ucPageCount, ucRemainder;
    ReturnType typeReturn;

    // Step 1: Validate address input
    if(!(udAddr <  FLASH_SIZE)) return Flash_AddressInvalid;

    // Step 2: Check memory space available on the whole memory
    if(udAddr + udNrOfElementsInArray > FLASH_SIZE) return Flash_MemoryOverflow;
    
    // Step 3: calculte memory space available within the page containing the start address(udAddr)
    ucMargin = (ST_uint8)(~udAddr) + 1;

    // Step 3-1: if the page boundary is crossed, invoke FlashPageWrite() repeatedly
    if(udNrOfElementsInArray > ucMargin)
    {
        typeReturn = FlashPageProgram(udAddr, pArray, ucMargin);
        if(Flash_Success != typeReturn) return typeReturn;              // return immediately if Not successful
        
        udNrOfElementsInArray -= ucMargin;                              // re-calculate the number of elements
        pArray += ucMargin;                                             // modify the pointer to the buffer
        udAddr += ucMargin;                                             // modify the start address in the memory
        ucPageCount = udNrOfElementsInArray / FLASH_WRITE_BUFFER_SIZE;  // calculate the number of pages to be programmed
        ucRemainder = udNrOfElementsInArray % FLASH_WRITE_BUFFER_SIZE;   // calculate the remainder after filling up one or more whole pages
        while(ucPageCount--)
        {
            typeReturn = FlashPageProgram(udAddr, pArray, FLASH_WRITE_BUFFER_SIZE);
            if(Flash_Success != typeReturn) return typeReturn;          // return immediately if Not successful
            pArray += FLASH_WRITE_BUFFER_SIZE;
            udAddr += FLASH_WRITE_BUFFER_SIZE;
        };
        return FlashPageProgram(udAddr, pArray, ucRemainder);
    }
    // Step 3-2: if the page boundary is not crossed, invoke FlashPageWrite() once only
    else
    {
        return FlashPageProgram(udAddr, pArray, udNrOfElementsInArray);
    }
}

/*******************************************************************************
Function:     IsFlashBusy( )
Arguments:    none

Return Value:
   TRUE
   FALSE

Description:  This function checks the Write In Progress (WIP) bit to determine whether
              the Flash memory is busy with a Write, Program or Erase cycle.

Pseudo Code:
   Step 1: Read the Status Register.
   Step 2: Check the WIP bit.
*******************************************************************************/
BOOL IsFlashBusy()
{
    ST_uint8 ucSR;

    // Step 1: Read the Status Register.
    FlashReadStatusRegister(&ucSR);

    // Step 2: Check the WIP bit.
    if(ucSR & SPI_FLASH_WIP)
        return TRUE;
    else
        return FALSE;
}

#ifdef VERBOSE
/******************************************************************************* 
Function:     FlashErrorStr( ReturnType rErrNum ); 
Arguments:    rErrNum is the error number returned from other Flash memory Routines 

Return Value: A pointer to a string with the error message 

Description:  This function is used to generate a text string describing the 
   error from the Flash memory. Call with the return value from other Flash memory routines. 
 
Pseudo Code: 
   Step 1: Return the correct string. 
*******************************************************************************/ 
ST_sint8 *FlashErrorStr( ReturnType rErrNum ) 
{   
   switch(rErrNum)
   {
   case Flash_AddressInvalid: 
      return "Flash - Address is out of Range"; 
   case Flash_MemoryOverflow: 
      return "Flash - Memory Overflows"; 
   case Flash_PageEraseFailed: 
      return "Flash - Page Erase failed"; 
   case Flash_PageNrInvalid: 
      return "Flash - Page Number is out of Range"; 
   case Flash_SectorNrInvalid: 
      return "Flash - Sector Number is out of Range"; 
  case Flash_FunctionNotSupported: 
      return "Flash - Function not supported"; 
   case Flash_NoInformationAvailable: 
      return "Flash - No Additional Information Available"; 
   case Flash_OperationOngoing: 
      return "Flash - Operation ongoing"; 
   case Flash_OperationTimeOut: 
      return "Flash - Operation TimeOut"; 
   case Flash_ProgramFailed: 
      return "Flash - Program failed";
   case Flash_Success: 
      return "Flash - Success"; 
   case Flash_WrongType: 
      return "Flash - Wrong Type"; 
   default: 
      return "Flash - Undefined Error Value"; 
   } /* EndSwitch */ 
} /* EndFunction FlashErrorString */
#endif /* VERBOSE Definition */

  
/*******************************************************************************
Function:     FlashTimeOut(ST_uint32 udSeconds)
Arguments:    udSeconds holds the number of seconds before TimeOut occurs

Return Value:
   Flash_OperationTimeOut   
   Flash_OperationOngoing 

Example:   FlashTimeOut(0)  // Initializes the Timer

           While(1) {
              ...
              If (FlashTimeOut(5) == Flash_OperationTimeOut) break;
              // The loop is executed for 5 Seconds before the operation is aborted
           } EndWhile

*******************************************************************************/
#ifdef TIME_H_EXISTS
/*-----------------------------------------------------------------------------
Description:   This function provides a timeout for Flash polling actions or
   other operations which would otherwise never return.
   The Routine uses the function clock() inside ANSI C library "time.h". 
-----------------------------------------------------------------------------*/
ReturnType FlashTimeOut(ST_uint32 udSeconds){
   static clock_t clkReset,clkCount;
     
   if (udSeconds == 0) { /* Set Timeout to 0 */
      clkReset=clock();
   } /* EndIf */

   clkCount = clock() - clkReset;

   if (clkCount<(CLOCKS_PER_SEC*(clock_t)udSeconds))
      return Flash_OperationOngoing;
   else
	  return Flash_OperationTimeOut;
}/* EndFunction FlashTimeOut */

#else 
/*-----------------------------------------------------------------------------
Description:   This function provides a timeout for Flash polling actions or
   other operations which would otherwise never return.
   The Routine uses COUNT_FOR_A_SECOND which is considered to be a loop that 
   counts for one second. It needs to be adapted to the target Hardware. 
-----------------------------------------------------------------------------*/
ReturnType FlashTimeOut(ST_uint32 udSeconds) {

   static ST_uint32 udCounter = 0;
   if (udSeconds == 0) { /* Set Timeout to 0 */
     udCounter = 0;
   } /* EndIf */

   if (udCounter == (udSeconds * COUNT_FOR_A_SECOND)) {
      udCounter = 0;
      return Flash_OperationTimeOut;
   } else {
      udCounter++;
      return Flash_OperationOngoing;
   } /* Endif */

} /* EndFunction FlashTimeOut */
#endif /* TIME_H_EXISTS */

//----------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------

int flash_query(void *data) 
{
  ParameterType fp;         /* Contains all Flash memory Parameters */
  Flash(ReadManufacturerIdentification, &fp);
  Flash(ReadDeviceIdentification, &fp);

  if (fp.ReadManufacturerIdentification.ucManufacturerIdentification == MANUFACTURER_ST ||
      fp.ReadDeviceIdentification.ucDeviceIdentification == EXPECTED_DEVICE)
    return FLASH_ERR_OK;
  else
    FLASH_ERR_HWR;
}

int flash_hwr_init(void)
{
  InitSPIMaster();
  
  if (flash_query(0) != FLASH_ERR_OK)
    return FLASH_ERR_HWR;
  
  // Have redboot use upper half of the flash
  flash_info.block_size = 0x10000;
  flash_info.blocks = 15;
  flash_info.buffer_size = 0;
  flash_info.start = (void *) 0x100000;
  flash_info.end =   (void *) 0x1F0000;

  return FLASH_ERR_OK;
}

int flash_erase_block (volatile unsigned long block) 
{
  ParameterType fp;
  ReturnType rRetVal;
      
  fp.SectorErase.ustSectorNr = block / 0x10000; 
  rRetVal = Flash(SectorErase, &fp);
  
  if (rRetVal==Flash_Success)
    return FLASH_ERR_OK;
  else
    return FLASH_ERR_ERASE;
}

int flash_program_buf (volatile unsigned long addr, unsigned long *data, int len)
{
  ParameterType fp;
  ReturnType rRetVal;
  
  fp.Program.udAddr = addr;
  fp.Program.pArray = data;
  fp.Program.udNrOfElementsInArray = len;
  rRetVal = Flash(Program,&fp);

  if (rRetVal==Flash_Success)
    return FLASH_ERR_OK;
  else
    return FLASH_ERR_PROGRAM;
}

int flash_read_buf(volatile unsigned long* addr, unsigned long* data, int len)
{
  ParameterType fp;
  ReturnType rRetVal;
  
  fp.Read.udAddr = addr;
  fp.Read.pArray = data;
  fp.Read.udNrOfElementsToRead = len;
  rRetVal = Flash(Read,&fp);

  if (rRetVal==Flash_Success)
    return FLASH_ERR_OK;
  else
    return FLASH_ERR_PROTOCOL;
}

int flash_program_buf_byte(volatile unsigned char *addr, unsigned char *data, int len)
{
	ParameterType fp;
	ReturnType rRetVal;
  
	fp.Program.udAddr = addr;
	fp.Program.pArray = data;
	fp.Program.udNrOfElementsInArray = len;
	rRetVal = Flash(Program,&fp);

	if (rRetVal==Flash_Success)
		return FLASH_ERR_OK;
	else
		return FLASH_ERR_PROGRAM;
}

int flash_read_buf_byte(volatile unsigned char* addr, unsigned char* data, int len)
{
	ParameterType fp;
	ReturnType rRetVal;
  
	fp.Read.udAddr = addr;
	fp.Read.pArray = data;
	fp.Read.udNrOfElementsToRead = len;
	rRetVal = Flash(Read,&fp);

	if (rRetVal==Flash_Success)
		return FLASH_ERR_OK;
	else
		return FLASH_ERR_PROTOCOL;
}

int flash_erase_block_sub (volatile unsigned long block) 
{
	ParameterType fp;
	ReturnType rRetVal;
      
	fp.SubSectorErase.ustSectorNr = block; 
//	printf("m25pxx_flash_suberase: %d\n",fp.SubSectorErase.ustSectorNr);
	rRetVal = Flash(SubSectorErase, &fp);
  
	if (rRetVal==Flash_Success)
		return FLASH_ERR_OK;
	else
		return FLASH_ERR_ERASE;
}

// Map a hardware status to a package error. NOP since the errors are
// already mapped.
int flash_hwr_map_error(int err){

 return err;
}

// See if a range of FLASH addresses overlaps currently running code
bool flash_code_overlaps(void *start, void *end)
{

    extern char _stext[], _etext[];

    return ((((unsigned long)&_stext >= (unsigned long)start) &&
             ((unsigned long)&_stext < (unsigned long)end)) ||
            (((unsigned long)&_etext >= (unsigned long)start) &&
             ((unsigned long)&_etext < (unsigned long)end)));
}
