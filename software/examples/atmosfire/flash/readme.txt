*** This README.TXT file comes with the STR71x Software Library ***

*******************************************************************************
 THE PRESENT SOFTWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS WITH
 CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
 AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT, INDIRECT
 OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE CONTENT
 OF SUCH SOFTWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING INFORMATION
 CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************

Example description
===================
 This example shows how to use some of the flash software library routines.
 The program writes the value "0x12345678" to Bank 1 sector 0 address 0x400C0000
 of the flash memory.
 This example highlights the following features of the flash :

  - Disable temporarily FLASH_B1F0 write protection,
  - Erase Bank 1 sector 0,
  - Program "0x12345678" word at 0x400C0000 Flash address,
  - Read back the written value at address 0x400C0000 and display it on the 
    debugger output window.
  - Enable permanently FLASH_B1F0 write protection (line commented out in main.c)
  
  CAUTION: THE WRITE PROTECTION CAN NOT BE DISABLED ON A PERMANENT WAY!! ONCE 
  ENABLED, IT CAN BE DISABLED ONLY TEMPORARILY.

  In this example, write protection in main.c is intentionnaly commented 
  since this line will set the permanent flash write protection. Once activated, only 
  temporarily write unprotection (showed in this example) will allow to write in flash.
  Write protection will be reactivated automatically after reset.

Directory contents
==================
 scat.scf    Scatter file
 71x_conf.h  Library Configuration file
 71x_it.c    Interrupt handlers
 main.c      Main program

Hardware implementation:
=======================
 This example works as standalone on the STR7-Eval board (mb393) and
 extra hardware implementation is not required to run the example.

How to use it
=============
In order to make the program work, you must do the following :

  - Just compile and run it.
  - You can use the debugger memory windows to verify the good content of the flash at
    0x400C0000 address and see the value "0x12345678"
  - If you want to  enable the FLASH write protection feature, just uncomment the
    corresponding line in the main.c file.


(C) COPYRIGHT 2003 STMicroelectronics
