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
 This example demonstrate one of the EMI feature which is to copy part of the code
 from a memory to another one and execute from it.
 For instance, all the entire image is loaded in RAM, then "main.o" will be copied in
 external SRAM and executed from it.
 
 The user should see 5 LEDs(connected to GPIO0 port pins) toggling

 The example shows the following features :
  
  - how to copy and execute an object file from RAM to SRAM in the file "scat.scf"
  - how to configure the GPIO0 ports and toggle its pins in an infinite loop.

Hardware implementation:
=======================
 This example works as standalone on the STR7-Eval board (mb393) and
 extra hardware implementation is not required to run the example.

Directory contents
==================
 scat.scf    Scatter file
 71x_conf.h  Library Configuration file
 71x_it.c    Interrupt handlers
 main.c      Main program

How to use it
=============
 In order to make the program work, you must do the following :

  - Ensure that an external SRAM must be present in the EMI bank 1,
  - Compile and run the program,
  - Observe the GPIO0 pin state
  - Stop the program to see the Program Counter located around 
  address 0x62000000 (external SRAM)


(C) COPYRIGHT 2003 STMicroelectronics
