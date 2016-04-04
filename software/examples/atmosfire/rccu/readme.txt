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
 This example shows how to use the RCCU peripheral in order to configure the system clock 
 signals. This is done by setting the clock source and using divider/multiplier ratios.

 This example demonstrates the following features of the RCCU :
 - Configuration of MCLK, FCLK, PCLK, RCLK source, PLL .

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

 - flash memory should be mapped at address 0x0 using the board H/W boot pins 
 - Just compile and run it.
 - You can probe the CKOUT signal (if available) to monitor the clock signal.
 - CKOUT signal should be equal to 12MHz, if a 16MHz external oscillator is used.

(C) COPYRIGHT 2003 STMicroelectronics
