#ifndef _M25PXX_H
#define _M25PXX_H

#define DRIVER_VERSION_MAJOR 2
#define DRIVER_VERSION_MINOR 0

typedef unsigned char  ST_uint8; /* All HW dependent Basic Data Types */      
typedef          char  ST_sint8;  
typedef unsigned int   ST_uint16;      
typedef          int   ST_sint16;      
typedef unsigned long  ST_uint32;      
typedef          long  ST_sint32; 

/* With SYNCHRONOUS_IO defined, each function that sends an Instruction(e.g. PE) 
   shall not return until the Flash memory finishes executing the Instruction
   or a pre-set timeout limit is reached. the pre-set timeout value is in 
   accordance with the datasheet of each memory.
   
   To achieve Send-n-Forget feature, comment out this #define*/ 
#define SYNCHRONOUS_IO  

#define USE_M25PE16
/* Possible Values: USE_M25P05A
					USE_M25P10A
					USE_M25P20
					USE_M25P40
					USE_M25P80
					USE_M25P16
					USE_M25P32
					USE_M25P64
                    */

/*#define TIME_H_EXISTS*/  /* set this macro if C-library "time.h" is supported */
/* Possible Values: TIME_H_EXISTS 
                    - no define - TIME_H_EXISTS */
 
#ifndef TIME_H_EXISTS
#define COUNT_FOR_A_SECOND 0xFFFFFF   /* Timer Usage */
#endif

#define VERBOSE /* Activates additional Routines */
/* Currently the Error String Definition */

/********************** End of User Change Area *****************************/

/*******************************************************************************
Device Constants
*******************************************************************************/

#define MANUFACTURER_ST (0x20)    /* ST Manufacturer Identification is 0x20 */
#define MEMORYTYPE_M25Pxx (0x20)  /* JEDEC Memory Type for M25Pxx Identification is 0x20 */
#define ANY_ADDR (0x0)            /* Any address offset within the Flash Memory will do */

#ifdef USE_M25P05A                       /* The M25P05A device */
#define USE_JEDEC_STANDARD_TWO_BYTE_SIGNATURE /* undefine this macro to read One-Byte Signature*/
#ifdef  USE_JEDEC_STANDARD_TWO_BYTE_SIGNATURE
#define EXPECTED_DEVICE (0x2010) /* Preferred Device Identification: please refer to the datasheet */
#else
#define EXPECTED_DEVICE (0x05)   /* Device Identification: please refer to the datasheet */
#endif
#define FLASH_SIZE (0x010000)         /* Total device size in Bytes */
#define FLASH_PAGE_COUNT (0x0100)     /* Total device size in Pages */
#define FLASH_SECTOR_COUNT (0x02)     /* Total device size in Sectors */
#define FLASH_SMALLER_SECTOR_SIZE     /* Sector Size  = 256Kb*/
#define FLASH_WRITE_BUFFER_SIZE 0x100 /* Write Buffer = 256 bytes */ 
#define FLASH_MWA 1                   /* Minimum Write Access */ 
#define BE_TIMEOUT (0x6)              /* Timeout in seconds suggested for Bulk Erase Operation*/
#endif /* USE_M25P05A */

#ifdef USE_M25P10A                       /* The M25P10A device */
#define USE_JEDEC_STANDARD_TWO_BYTE_SIGNATURE /* undefine this macro to read One-Byte Signature*/
#ifdef  USE_JEDEC_STANDARD_TWO_BYTE_SIGNATURE
#define EXPECTED_DEVICE (0x2011) /* Preferred Device Identification: please refer to the datasheet */
#else
#define EXPECTED_DEVICE (0x10)   /* Device Identification: please refer to the datasheet */
#endif
#define FLASH_SIZE (0x020000)         /* Total device size in Bytes */
#define FLASH_PAGE_COUNT (0x0200)     /* Total device size in Pages */
#define FLASH_SECTOR_COUNT (0x04)     /* Total device size in Sectors */
#define FLASH_SMALLER_SECTOR_SIZE     /* Sector Size  = 256Kb*/
#define FLASH_WRITE_BUFFER_SIZE 0x100 /* Write Buffer = 256 bytes */ 
#define FLASH_MWA 1                   /* Minimum Write Access */ 
#define BE_TIMEOUT (0x6)              /* Timeout in seconds suggested for Bulk Erase Operation*/
#endif /* USE_M25P10A */

#ifdef USE_M25P20                        /* The M25P20 device */
#define EXPECTED_DEVICE (0x11)        /* Device Identification for the M25P20 */
#define FLASH_SIZE (0x040000)         /* Total device size in Bytes */
#define FLASH_PAGE_COUNT (0x0400)     /* Total device size in Pages */
#define FLASH_SECTOR_COUNT (0x04)     /* Total device size in Sectors */
#define FLASH_WRITE_BUFFER_SIZE 0x100 /* Write Buffer = 256 bytes */ 
#define FLASH_MWA 1                   /* Minimum Write Access */ 
#define BE_TIMEOUT (0x6)              /* Timeout in seconds suggested for Bulk Erase Operation*/
#endif /* USE_M25P20 */

#ifdef USE_M25P40                        /* The M25P40 device */
#define EXPECTED_DEVICE (0x12)        /* Device Identification for the M25P40 */
#define FLASH_SIZE (0x080000)         /* Total device size in Bytes */
#define FLASH_PAGE_COUNT (0x0800)     /* Total device size in Pages */
#define FLASH_SECTOR_COUNT (0x08)     /* Total device size in Sectors */
#define FLASH_WRITE_BUFFER_SIZE 0x100 /* Write Buffer = 256 bytes */ 
#define FLASH_MWA 1                   /* Minimum Write Access */
#define BE_TIMEOUT (0x03)             /* Timeout in seconds suggested for Bulk Erase Operation*/
#endif /* USE_M25P40 */

#ifdef USE_M25P80                        /* The M25P80 device */
#define EXPECTED_DEVICE (0x13)        /* Device Identification for the M25P80 */
#define FLASH_SIZE (0x0100000)        /* Total device size in Bytes */
#define FLASH_PAGE_COUNT (0x01000)    /* Total device size in Pages */
#define FLASH_SECTOR_COUNT (0x10)     /* Total device size in Sectors */
#define FLASH_WRITE_BUFFER_SIZE 0x100 /* Write Buffer = 256 bytes */ 
#define FLASH_MWA 1                   /* Minimum Write Access */ 
#define BE_TIMEOUT (0x14)             /* Timeout in seconds suggested for Bulk Erase Operation*/
#endif /* USE_M25P80 */

#ifdef USE_M25P16                        /* The M25P16 device */
#define USE_JEDEC_STANDARD_TWO_BYTE_SIGNATURE /* undefine this macro to read One-Byte Signature*/
#ifdef  USE_JEDEC_STANDARD_TWO_BYTE_SIGNATURE
#define EXPECTED_DEVICE (0x2015) /* Device Identification for the USE_M25P16 */
#else
#define EXPECTED_DEVICE (0x14)   /* Device Identification for the USE_M25P16 */
#endif
#define FLASH_SIZE (0x0200000)        /* Total device size in Bytes */
#define FLASH_PAGE_COUNT (0x02000)    /* Total device size in Pages */
#define FLASH_SECTOR_COUNT (0x20)     /* Total device size in Sectors */
#define FLASH_WRITE_BUFFER_SIZE 0x100 /* Write Buffer = 256 bytes */ 
#define FLASH_MWA 1                   /* Minimum Write Access */ 
#define BE_TIMEOUT (0x46)             /* Timeout in seconds suggested for Bulk Erase Operation*/
#endif /* USE_M25P16 */ 

#ifdef USE_M25PE16                        /* The M25PE16 device */
#define USE_JEDEC_STANDARD_TWO_BYTE_SIGNATURE /* undefine this macro to read One-Byte Signature*/
#ifdef  USE_JEDEC_STANDARD_TWO_BYTE_SIGNATURE
#define EXPECTED_DEVICE (0x8015) /* Device Identification for the USE_M25PE16 */
#else
#define EXPECTED_DEVICE (0x15)   /* Device Identification for the USE_M25PE16 */
#endif
#define FLASH_SIZE (0x0200000)        /* Total device size in Bytes */
#define FLASH_PAGE_COUNT (0x02000)    /* Total device size in Pages */
#define FLASH_SECTOR_COUNT (0x200)     /* Total device size in Sectors */
#define FLASH_WRITE_BUFFER_SIZE 0x100 /* Write Buffer = 256 bytes */ 
#define FLASH_MWA 1                   /* Minimum Write Access */ 
#define BE_TIMEOUT (0x46)             /* Timeout in seconds suggested for Bulk Erase Operation*/
#endif /* USE_M25PE16 */ 

#ifdef USE_M25P32                        /* The USE_M25P32 device */
#define USE_JEDEC_STANDARD_TWO_BYTE_SIGNATURE /* undefine this macro to read One-Byte Signature*/
#ifdef  USE_JEDEC_STANDARD_TWO_BYTE_SIGNATURE
#define EXPECTED_DEVICE (0x2016)    /* Device Identification for the USE_M25P32 */
#else
#define EXPECTED_DEVICE (0x15)      /* Device Identification for the USE_M25P32 */
#endif
#define FLASH_SIZE (0x0400000)           /* Total device size in Bytes */
#define FLASH_PAGE_COUNT (0x04000)       /* Total device size in Pages */
#define FLASH_SECTOR_COUNT (0x40)        /* Total device size in Sectors */
#define FLASH_WRITE_BUFFER_SIZE 0x100    /* Write Buffer = 256 bytes */ 
#define FLASH_MWA 1                      /* Minimum Write Access */ 
#define BE_TIMEOUT (0x80)                /* Timeout in seconds suggested for Bulk Erase Operation*/
#endif /* USE_M25P32 */


#ifdef USE_M25P64                        /* The USE_M25P64 device */
#define USE_JEDEC_STANDARD_TWO_BYTE_SIGNATURE /* undefine this macro to read One-Byte Signature*/
#ifdef  USE_JEDEC_STANDARD_TWO_BYTE_SIGNATURE
#define EXPECTED_DEVICE (0x2017)    /* Device Identification for the USE_M25P64 */
#else
#define EXPECTED_DEVICE (0x16)      /* Device Identification for the USE_M25P64 */
#endif
#define FLASH_SIZE (0x0800000)           /* Total device size in Bytes */
#define FLASH_PAGE_COUNT (0x08000)       /* Total device size in Pages */
#define FLASH_SECTOR_COUNT (0x80)        /* Total device size in Sectors */
#define FLASH_WRITE_BUFFER_SIZE 0x100    /* Write Buffer = 256 bytes */ 
#define FLASH_MWA 1                      /* Minimum Write Access */ 
#define BE_TIMEOUT (0x160)               /* Timeout in seconds suggested for Bulk Erase Operation*/
#define NO_DEEP_POWER_DOWN_SUPPORT       /* No support for Deep Power-down feature*/    
#endif /* USE_M25P64 */
/******************************************************************************* 
     DERIVED DATATYPES
*******************************************************************************/ 
/******** InstructionsCode ********/ 
#define SPI_FLASH_INS_DUMMY 0xAA            // dummy byte
enum
{
	//Instruction set
  SPI_FLASH_INS_WREN        = 0x06,		// write enable
  SPI_FLASH_INS_WRDI        = 0x04,		// write disable
  SPI_FLASH_INS_RDSR        = 0x05,		// read status register
  SPI_FLASH_INS_WRSR        = 0x01,		// write status register
  SPI_FLASH_INS_READ        = 0x03,		// read data bytes
  SPI_FLASH_INS_FAST_READ   = 0x0B,		// read data bytes at higher speed
  SPI_FLASH_INS_PP          = 0x02,		// page program
  SPI_FLASH_INS_SE          = 0xD8,		// sector erase
  SPI_FLASH_INS_SSE         = 0x20,		// Sub sector erase

#ifndef NO_DEEP_POWER_DOWN_SUPPORT
  SPI_FLASH_INS_RES         = 0xAB,		// release from deep power-down
  SPI_FLASH_INS_DP          = 0xB9,		// deep power-down
#endif

#ifdef USE_JEDEC_STANDARD_TWO_BYTE_SIGNATURE
  SPI_FLASH_INS_RDID        = 0x9F,		// read identification
#endif

  SPI_FLASH_INS_BE          = 0xC7		// bulk erase
};


/******** InstructionsType ********/ 

typedef enum {
  WriteEnable,
  WriteDisable,
  ReadDeviceIdentification, 
#ifdef USE_JEDEC_STANDARD_TWO_BYTE_SIGNATURE
  ReadManufacturerIdentification, 
#endif
  ReadStatusRegister,
  WriteStatusRegister,
  Read,
  FastRead,
  PageProgram,
  SectorErase,
  SubSectorErase,
  BulkErase,
   
#ifndef NO_DEEP_POWER_DOWN_SUPPORT
  DeepPowerDown,
  ReleaseFromDeepPowerDown,
#endif
   
  Program
} InstructionType; 

/******** ReturnType ********/ 

typedef enum { 
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
  Flash_WrongType,
  Flash_Success
} ReturnType; 

/******** SectorType ********/ 

typedef ST_uint8 uSectorType; 
 
/******** PageType ********/ 

typedef ST_uint16 uPageType; 
 
/******** AddrType ********/ 

typedef ST_uint32 uAddrType; 
 
/******** ParameterType ********/ 
 
typedef union { 
 
  /**** WriteEnable has no parameters ****/ 
 
  /**** WriteDisable has no parameters ****/ 
 
  /**** ReadDeviceIdentification Parameters ****/ 
  struct { 
    ST_uint16 ucDeviceIdentification; 
  } ReadDeviceIdentification; 
 
  /**** ReadManufacturerIdentification Parameters ****/ 
  struct { 
    ST_uint8 ucManufacturerIdentification; 
  } ReadManufacturerIdentification; 

  /**** ReadStatusRegister Parameters ****/ 
  struct { 
    ST_uint8 ucStatusRegister; 
  } ReadStatusRegister;  

  /**** WriteStatusRegister Parameters ****/ 
  struct { 
    ST_uint8 ucStatusRegister; 
  } WriteStatusRegister;  

  /**** Read Parameters ****/ 
  struct { 
    uAddrType udAddr; 
    ST_uint32 udNrOfElementsToRead;         
    void *pArray; 
  } Read; 

  /**** FastRead Parameters ****/ 
  struct { 
    uAddrType udAddr; 
    ST_uint32 udNrOfElementsToRead;         
    void *pArray; 
  } FastRead; 

  /**** PageWrite Parameters ****/ 
  struct { 
    uAddrType udAddr; 
    ST_uint32 udNrOfElementsInArray;         
    void *pArray; 
  } PageWrite; 
    
  /**** PageProgram Parameters ****/ 
  struct { 
    uAddrType udAddr; 
    ST_uint32 udNrOfElementsInArray;         
    void *pArray; 
  } PageProgram; 

  /**** PageErase Parameters ****/ 
  struct { 
    uPageType upgPageNr; 
  } PageErase; 

  /**** SectorErase Parameters ****/ 
  struct { 
    uSectorType ustSectorNr; 
  } SectorErase; 

  /**** SubSectorErase Parameters ****/ 
  struct { 
	  unsigned int ustSectorNr; 
  } SubSectorErase; 
  
  /**** Write Parameters ****/ 
  struct { 
    uAddrType udAddr; 
    ST_uint32 udNrOfElementsInArray;         
    void *pArray; 
  } Write; 
    
  /**** Program Parameters ****/ 
  struct { 
    uAddrType udAddr; 
    ST_uint32 udNrOfElementsInArray;         
    void *pArray; 
  } Program; 

} ParameterType; 
 

/******************************************************************************
    Standard functions
*******************************************************************************/ 
  ReturnType  Flash( InstructionType insInstruction, ParameterType *fp );
  ReturnType  FlashWriteEnable( void );
  ReturnType  FlashWriteDisable( void );
  ReturnType  FlashReadDeviceIdentification( ST_uint16 *uwpDeviceIdentification);
#ifdef USE_JEDEC_STANDARD_TWO_BYTE_SIGNATURE
  ReturnType  FlashReadManufacturerIdentification( ST_uint8 *ucpManufacturerIdentification);
#endif
  ReturnType  FlashReadStatusRegister( ST_uint8 *ucpStatusRegister);
  ReturnType  FlashWriteStatusRegister( ST_uint8 ucStatusRegister);
  ReturnType  FlashRead( uAddrType udAddr, ST_uint8 *ucpElements, ST_uint32 udNrOfElementsToRead);
  ReturnType  FlashFastRead( uAddrType udAddr, ST_uint8 *ucpElements, ST_uint32 udNrOfElementsToRead);
  ReturnType  FlashPageProgram( uAddrType udAddr, ST_uint8 *pArray, ST_uint16 udNrOfElementsInArray );
  ReturnType  FlashSectorErase( uSectorType uscSectorNr );
  ReturnType  FlashSubSectorErase( unsigned int uscSectorNr );
  ReturnType  FlashBulkErase( void );
#ifndef NO_DEEP_POWER_DOWN_SUPPORT
  ReturnType  FlashDeepPowerDown( void );
  ReturnType  FlashReleaseFromDeepPowerDown( void );
#endif
  ReturnType  FlashProgram( ST_uint32 udAddr, ST_uint8 *pArray , ST_uint32 udNrOfElementsInArray);

/******************************************************************************
  Utility functions
*******************************************************************************/ 
#ifdef VERBOSE
  ST_sint8 *FlashErrorStr( ReturnType rErrNum );
#endif

  ReturnType  FlashTimeOut( ST_uint32 udSeconds );
 
/******************************************************************************* 
  List of Errors and Return values, Explanations and Help. 
  ******************************************************************************** 

  Error Name:   Flash_AddressInvalid 
  Description:  The address given is out of the range of the Flash device. 
  Solution:     Check whether the address is in the valid range of the 
  Flash device. 
  ******************************************************************************** 
 
  Error Name:   Flash_PageEraseFailed 
  Description:  The Page erase Instruction did not complete successfully. 
  Solution:     Try to erase the Page again. If this fails once more, the device 
  may be faulty and need to be replaced. 
  ******************************************************************************** 
 
  Error Name:   Flash_PageNrInvalid 
  Note:         The Flash memory is not at fault.
  Description:  A Page has been selected (Parameter), which is not 
  within the valid range. Valid Page numbers are from 0 to
  FLASH_PAGE_COUNT - 1. 
  Solution:     Check that the Page number given is in the valid range. 
  ******************************************************************************** 
 
  Error Name:   Flash_SectorNrInvalid 
  Note:         The Flash memory is not at fault. 
  Description:  A Sector has been selected (Parameter), which is not 
  within the valid range. Valid Page numbers are from 0 to
  FLASH_SECTOR_COUNT - 1. 
  Solution:     Check that the Sector number given is in the valid range. 
  ******************************************************************************** 

  Return Name:  Flash_FunctionNotSupported 
  Description:  The user has attempted to make use of a functionality not 
  available on this Fash device (and thus not provided by the
  software drivers).   
  Solution:     This may happen after changing Flash SW Drivers in existing 
  environments. For example an application tries to use a
  functionality which is no longer provided with the new device. 
  ******************************************************************************** 
 
  Return Name:  Flash_NoInformationAvailable
  Description:  The system cannot give any additional information about the error.
  Solution:     None
  ******************************************************************************** 
 
  Error Name:   Flash_OperationOngoing 
  Description:  This message is one of two messages that are given by the TimeOut 
  subroutine. It means that the ongoing Flash operation is still within
  the defined time frame.  
  ******************************************************************************** 
 
  Error Name:   Flash_OperationTimeOut 
  Description:  The Program/Erase Controller algorithm could not finish an 
  operation successfully. It should have set bit 7 of the Status
  Register from 0 to 1, but that did not happen within a predetermined
  time. The program execution was therefore cancelled by a
  timeout. This may be because the device is damaged.  
  Solution:     Try the previous Instruction again. If it fails a second time then it 
  is likely that the device will need to be replaced. 
  ******************************************************************************** 
 
  Error Name:   Flash_ProgramFailed 
  Description:  The value that should be programmed has not been written correctly 
  to the Flash memory. 
  Solutions:    Make sure that the Page which is supposed to receive the value 
  was erased successfully before programming. Try to erase the Page and
  to program the value again. If it fails again then the device may
  be faulty. 
  ******************************************************************************** 

  Error Name:   Flash_WrongType 
  Description:  This message appears if the Manufacture and Device Identifications read from 
  the current Flash device do not match the expected identifier
  codes. This means that the source code is not explicitely written for
  the currently used Flash chip. It may work, but the operation cannot be
  guaranteed. 
  Solutions:    Use a different Flash chip with the target hardware or contact 
  STMicroelectronics for a different source code library. 
  ********************************************************************************

  Return Name:  Flash_Success 
  Description:  This value indicates that the Flash memory Instruction was executed 
  correctly. 
********************************************************************************/ 

/******************************************************************************
  External variable declaration
*******************************************************************************/ 

// none in this version of the release

/*******************************************************************************
  Flash Status Register Definitions (see Datasheet)
*******************************************************************************/
  enum
{
  SPI_FLASH_SRWD	= 0x80,				// Status Register Write Protect
  SPI_FLASH_BP2	= 0x10,				// Block Protect Bit2
  SPI_FLASH_BP1	= 0x08,				// Block Protect Bit1
  SPI_FLASH_BP0	= 0x04,				// Block Protect Bit0
  SPI_FLASH_WEL	= 0x02,				// write enable latch
  SPI_FLASH_WIP	= 0x01				// write/program/erase in progress indicator
};

/*******************************************************************************
Specific Function Prototypes
*******************************************************************************/
typedef unsigned char BOOL;

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

BOOL IsFlashBusy();

/*******************************************************************************
List of Specific Errors and Return values, Explanations and Help.
*******************************************************************************

// none in this version of the release
********************************************************************************/

#define ptrNull 0   // a null pointer

#define True  1
#define False 0
typedef unsigned char Bool;

// mask bit definitions for SPI master side configuration
enum
{
  MaskBit_Trans                          = 0x01,  // mask bit for Transfer enable/disable
  MaskBit_Recv                           = 0x02,  // mask bit for Receive enable/disable
  MaskBit_Trans_Relevant                 = 0x04,  // check whether MaskBit_Trans is necessary
  MaskBit_Recv_Relevant                  = 0x08,  // check whether MaskBit_Recv is necessary

  MaskBit_SlaveSelect                    = 0x10,  // mask bit for Slave Select/Deselect
  MaskBit_SelectSlave_Relevant           = 0x20,  // check whether MaskBit_SelectSlave is necessary

};

// Acceptable values for SPI master side configuration
typedef enum _SpiMasterConfigOptions
{
  enumNull                               = 0,     // do nothing
  enumEnableTransOnly                    = 0x05,  // enable transfer
  enumEnableRecvOnly                     = 0x0A,  // enable receive
  enumEnableTansRecv                     = 0x0F,  // enable transfer & receive

  enumEnableTransOnly_SelectSlave        = 0x35,  // enable transfer and select slave
  enumEnableRecvOnly_SelectSlave         = 0x3A,  // enable receive and select slave
  enumEnableTansRecv_SelectSlave         = 0x3F,  // enable transfer & receive and select slave

  enumDisableTransOnly                   = 0x04,  // disable transfer and deselect slave
  enumDisableRecvOnly                    = 0x08,  // disable receive
  enumDisableTransRecv                   = 0x0C,  // disable transfer & receive

  enumDisableTransOnly_DeSelectSlave     = 0x24,  // disable transfer and deselect slave
  enumDisableRecvOnly_DeSelectSlave      = 0x28,  // disable receive and deselect slave
  enumDisableTansRecv_DeSelectSlave      = 0x2C   // disable transfer & receive and deselect slave

}SpiMasterConfigOptions;

// char stream definition for 
typedef struct _structCharStream
{
  ST_uint8* pChar;                                // buffer address that holds the streams
  ST_uint32 length;                               // length of the stream in bytes
}CharStream;

void InitSPIMaster();                               // one-time setting to work in SPI mode
void ConfigureSpiMaster(
                        SpiMasterConfigOptions opt  // configuration options
                       ); 
Bool Serialize(
               const CharStream* char_stream_send,   // char stream to be sent to the memory(incl. instruction, address etc)
               CharStream* char_stream_recv,         // char stream to be received from the memory   
               SpiMasterConfigOptions optBefore,     // Pre-Configurations on the SPI master side
               SpiMasterConfigOptions optAfter       // Post-Configurations on the SPI master side
              );

Bool Serialize_nf(
                  const CharStream* char_stream_send,   // char stream to be sent to the memory(incl. instruction, address etc)
                  CharStream* char_stream_recv,         // char stream to be received from the memory   
                  SpiMasterConfigOptions optBefore,     // Pre-Configurations on the SPI master side
                  SpiMasterConfigOptions optAfter       // Post-Configurations on the SPI master side
                 );

void enable_cs(void);
void disable_cs(void);

#endif