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
 This example highlights two main features of the Watchdog Timer peripheral:
  
  - Timer interrupt,
  - Watchdog mode.

 The user has an example for each feature that can be activated from the main.c file.

 It is made of 2 parts :
1) Timer mode
   - Configure the GPIO,
   - Set the timer period value,
   - Configure the interrupt channel priority,
   - Enable the count-down timer,
   - Infinite loop (the interrupt handler toggles an I/O port).

2) Watchdog mode
   - Configure the GPIO,
   - Set the prescaler and the count reload value,
   - Enable the Watchdog (with no possibility to disable),
   - Refresh periodically the counter in a loop.
   _ Change the "#define Refresh_Time" value to change the Watchdog reload counter
     value. If the Refresh_Time < 1214, the Watchdog will generate a system Reset.

  
  NOTE: for the Watchdog example, the watchdog will generate a system reset which means the 
  debugger connexion to the target will be lost. Therefore to restart execution, the user will
  have to reconnect to the target.

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

  - RAM should be mapped at address 0x0 using the board H/W boot pins
  - Just compile and run it.


(C) COPYRIGHT 2003 STMicroelectronics
