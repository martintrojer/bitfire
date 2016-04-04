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
 This example demonstrates the PWM input feature of the Timer :
 Timer 3 is configured to measure the frequency and the duty-cycle of an
 input signal using the timer Input Capture A module.

 This example run at 32MHz with a 16MHz external oscilator.

Directory contents
==================
 scat.scf    Scatter file
 71x_conf.h  Library Configuration file
 71x_it.c    Interrupt handlers
 main.c      Main program

Hardware implementation:
=======================
 An external square signal must be injected at P1.1 pin.
 The signal frequency should belong to [500000..500]Hz


How to use it
=============
 In order to make the program work, you must do the following :
  - Compile and run the program.
  - The signal frequency and duty-cycle will be printed in debugger output
    window.

(C) COPYRIGHT 2003 STMicroelectronics
