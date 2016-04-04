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

 In this example BSPI0 and BSPI1 are connected together on the same bus. BSPI0 is
 configured as Master and BSPI1 as Slave, both with a word of 8 bits and Tx/Rx
 FIFO of 9 words. The BSPI clock frequency is fixed to APB1/6.

 The BSPI0 transmits a nine data buffer size to BSPI1.  Then the data received by
 the BSPI1 is stored in the "Receivetab" variable which can be seen on the debugger
 watch window.

 This example demonstrates the following features of the BSPI :

  - Master and Slave modes support,
  - Internal clock prescaler,
  - Programmable depth transmit FIFO,
  - Programmable depth receive FIFO,
  - Operate with 16 bit word length.

 It is made of 4 parts :

  1) Configure BSPI0 as a master,
  2) Configure BSPI1 as a slave,
  3) Transmission of five words from Master to Slave,
  4) Reception of nine words from Master.

Directory contents
==================
 scat.scf    Scatter file
 71x_conf.h  Library Configuration file
 71x_it.c    Interrupt handlers
 main.c      Main program

Hardware implementation:
=======================
 In order to make the program work, you must do the following implementation:

  - Connect BSPI0 MISO pin (P0.0)  to BSPI1 MISO pin (P0.4),
  - Connect BSPI0 MOSI pin (P0.1)  to BSPI1 MOSI pin (P0.5),
  - Connect BSPI0 SCLK pin (P0.2)  to BSPI1 SCLK pin (P0.6),
  - Connect BSPI1 SS   pin (P0.7)  to ground.

How to use it
=============
 In order to make the program work, you must do the following :

  - Compile and run it.
  - You can verify the "Receivetab" variable content using the watch window.


(C) COPYRIGHT 2003 STMicroelectronics
