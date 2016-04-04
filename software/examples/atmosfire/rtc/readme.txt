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
 This example demonstrates how to configure the RTC in order to generate an IRQ every 
 second using the RTC second interrupt. The ISR will then clear the RTC interrupt and
 toggle the GPIO0 pins.

 This example demonstrates the following features of the RTC:
 
  - Configure the prescaler,
  - Enable the RTC second interrupt and global interrupt,
  - Toggle GPIO0 ports pins every sec in the IRQ handler.

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

 - Ensure that you have flash memory mapped at address 0x0,
 - Just compile and run it,

(C) COPYRIGHT 2003 STMicroelectronics
