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
 The main program configures the UART0 to generate an interrupt after a byte
 reception. The received byte will be read in the UART0 interrupt service routine to be
 transmitted back to the Host PC.
 The communication is performed using the «Hyperterminal» application under Windows.

 It is made of 3 parts :

   1) Analog input configuration,

   2) UART configuration,

   2) EIC configuration.

   
Hardware implementation:
=======================
 Just connect the demo board using a serial link to a PC using COM A
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

  - Configure the device to have RAM mapped to address 0x0,
  - Load the program,
  - Open «Hyperterminal» application and configure the right PC COM port as following :

     * 9600 bps
     * 8 data bit
     * no parity
     * 1 Stop_bit
     * No flow control

  - Press any key and if the program works properly, you will see the same character
    in the «Hyperterminal» application window. If you stop the program execution,the
    characters won't be displayed.

(C) COPYRIGHT 2003 STMicroelectronics
