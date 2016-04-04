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
 This example demonstrates how to configure the XTI to generate IRQ interrupts.
 This example highlights one of the features of the XTI which is to generate an
 IRQ interrupts service routine and use the P0.15 pins (tied to the wakeup button 
 monted on the MB393 board)to execute the interrupt code.
 If the program work properly you will see the first led of the GPIO0 port pins
 change state every time when you press the wakeup button.

 This example demonstrates the following features of the XTI :

  - Initialization,
  - Line mode configuration,
  - Line configuration,
  - XTI mode configuration.

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

  - Ensure that RAM is mapped to address 0x00000000,
  - Compile and run it.
  - A square signal must be applied to the P0.15 pin (using wakeup button ).
  - Watch the P0.0 pin state.


(C) COPYRIGHT 2003 STMicroelectronics
