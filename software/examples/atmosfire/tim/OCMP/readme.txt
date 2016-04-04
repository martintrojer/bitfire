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

This Device is configured to Toggle the TIM3 Output compare A pin from low
level to high level after 0xF000 timer period. The GPIO pin is also toggled from
low level to high level and may be used as a reference to verify the delay of the
Output compare function.
 
 This example demonstrates the following features of the Timer :
  - Timer in Output Compare A
  - Timer Output Compare interrupt  

Directory contents
==================
 scat.scf    Scatter file
 71x_conf.h  Library Configuration file
 71x_it.c    Interrupt handlers
 main.c      Main program

Hardware implementation:
=======================
 This example works as standalone on the STR7-Verif board and
 extra hardware implementation is not required to run the example.
 Only you need a scope to visualize the TIM3 OCMPA signal.

How to use it
=============
 In order to make the program work,you must do the following :

  - Ensure that you have Flash memory mapped at address 0x0
  - Then compile and run it.

(C) COPYRIGHT 2003 STMicroelectronics
