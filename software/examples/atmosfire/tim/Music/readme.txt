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

 This example highlights one of the features of the TIMER which is to play a PWM
 wave generation.

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
  - If the program works properly you will have a wave generation which you can Stop
    using a Wakeup button(if implemented on the board).
  - If you want to repeat, just stop, reload and run the program.

(C) COPYRIGHT 2003 STMicroelectronics
