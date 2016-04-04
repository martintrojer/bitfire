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
 This example shows how to use the UART in order to communicate between the 
 STR71x and a PC using RS232 protocol.
 The main program waits for the user to send 4 ASCII characters from the PC, and
 the UART will echo the same characters back to the PC.
 The communication is performed using the «Hyperterminal» application under Windows.

 It is made of 2 parts :

   1) Analog input configuration,

   2) UART configuration.

Hardware implementation
======================= 
 Just connect the demo board using a serial link to a PC using COM A port
 on the STR7-Eval board (mb393)
 
Directory contents
==================
 scat.scf    Scatter file
 71x_conf.h  Library Configuration file
 71x_it.c    Interrupt handlers
 main.c      Main program

How to use it
=============
 In order to make the program work, you must do the following :
  - Open «Hyperterminal» application and configure the right COM port as following :

     * 9600 bps,
     * 8 data bit,
     * no parity,
     * 1 Stop_bit
     * no flow control.

  - Then compile and run it.

  - When you execute the code an "STR7" will be displayed on the PC «Hyperterminal» window.
 and then you will see after each 4 keys pressed the same 4 characters displayed on the hyperterminal.

(C) COPYRIGHT 2003 STMicroelectronics
