*** This README.TXT file comes with the STR71x Software Library ***************

*******************************************************************************
 THE PRESENT SOFTWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS WITH
 CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
 AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT, INDIRECT
 OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE CONTENT
 OF SUCH SOFTWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING INFORMATION
 CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

Example description
===================
 This example shows how to use  the ADC12  in single channel conversion mode. 
 It aims to display on the terminal window the analog voltage in µV that had been 
 converted by the ADC cell. This calibration is performed by determining the ADC12 quatium
 in µV. This is done by computing the average of 1000 ADC conversion result of 0V and 2.5V
 and then calibrating the ADC conversion result. 
  
 This example demonstrates the following features of the ADC12 :

  - The use of the converter in the single channel conversion mode,
  - The ADC12 prescaler configuration.

  It is made of the following parts :

  1) GPIO peripheral configuration
    - Configure the used analog input to high impedance Analog input (channel 3 in this example)
    - Configure the GPIO0 port to output push-pull
    - Configure the pin 9 of GPIO1 to input tristate TLL
    - Switch on the four leds on GPIO0

  2) Converter configuration
    - Initialize the converter,
    - Configure the prescaler register,
    - Configure the conversion mode,
    - Select the channel to be converted,
    - Start the conversion.

  3) Get the conversion Data Average for 0V
    - Wait until the NXT push button is pressed
    - Compute the average of 1000 ADC conversion result for 0V. This is done using the 
      ADC12_ConversionAverage( ) function.  
    - The led on GPIO.0 is switched off    
    
 4) Get the conversion Data Average for 2.5V
    - Wait until the NXT push button is pressed
    - Compute the average of 1000 ADC conversion result for 2.5V. This is done using the 
      ADC12_ConversionAverage( ) function.
    - The led on GPIO.1 is switched off  

 6) Compute the ADC quatium in µV
   - Calibrate the Conversion Data Average for 0V to have a  12-bit unsigned value 
     using the function ADC12_UnsigValue() 
   - Calibrate the Conversion Data Average for 2.5V to have a  12-bit unsigned value
    using the function ADC12_UnsigValue()
   - Calculate the ADC quatium using these values.
   - Wait until NXT push button is pressed

 7) Compute the ADC analog input in µV
   - The led on GPIO0.2 toggles
   - Wait until the ADC conversion data on channel3 is available 
   - Calibrate this conversion result to have a 12-bit unsigned value using the function
    ADC12_UnsigValue()
   - Compute the ADC analog input in µV using the ADC quatium already and display it on the 
   terminal window  


Hardware implementation:
======================= 
This example requires et least 0V and 2.5V on the ADC analog channel 3. 
 - On the STR7-Eval board (mb393), the 0V can be applied by plugging the jumper CN6.
 - 2.5V is required on the ADC analog channel 3.
 - The analog signal should be input on ADC analog channel 3.
The user should first apply 0V and then 2.5V to compute the ADC quatium. 

Directory contents
==================
 scat.scf    Scatter file
 71x_conf.h  Library Configuration file
 71x_it.c    Interrupt handlers
 main.c      Main program

How to use it
=============
 In order to make the program work, you must do the following :

   - Just compile and run it.

(C) COPYRIGHT 2003 STMicroelectronics
