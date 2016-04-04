*** This README.TXT file comes with the STARM71x Software Library ***

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
 This example provides a basic example showing how to use the I2C software library
 to communicate with an I2C EEPROM mounted on the MB393B board.

 The associated software performs a write operation to the I2C EEPROM
 followed by a read operation of the data already stored in the EEPROM.

 The example is made of 4 main parts :
  1)  I2C peripheral configuration.
  2)  Write operation to the I2C EEPROM.
  3)  Wait until the I2C EEPROM write operation is completed.
  4)  Read and display, in the debugger output window, the data already stored
      in the I2C EEPROM.

Hardware implementation:
=======================
 This example works on the STR7-Eval board (MB393) and no extra hardware
 implementation is required to run the example.

How to use it
=============
 In order to make the program work, you must do the following :

  - Ensure that the switch (SW6) mounted on the MB393 demo board is at
    (1-2) position,
  - Just compile and run it.


(C) COPYRIGHT 2004 STMicroelectronics
