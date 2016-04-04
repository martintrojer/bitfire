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
 This example demonstrate one of the feature of the CAN.The program runs in loopback 
 mode combined with silent mode (i.e. self-test mode) to be independant from any true 
 CAN network.
 
 This example demonstrates the following features of the CAN :

  - Bus initialization (baudrate setting),
  - Loopback and silent test modes,
  - Configuration of message objects,
  - Transmit and receive of CAN messages,
  - Standard and extended identifiers,
  - Polling mode and interrupt mode.

 It is made of 2 parts :

  1) one message exchange in polling mode,
  2) one message exchange in interrupt mode.

Directory contents
==================
 scat.scf    Scatter file
 71x_conf.h  Library Configuration file
 71x_it.c    Interrupt handlers
 main.c      Main program

How to use it
=============
 In order to make the program work, you must do the following :
 
  - RAM should be mapped at address 0x0 using the board H/W boot pins
  - Compile and run the example.
  - You don't need to put any cap on the CAN bus because the loopback is internal.
    in any case, the CAN_TX pin is always active in this mode.


(C) COPYRIGHT 2003 STMicroelectronics
