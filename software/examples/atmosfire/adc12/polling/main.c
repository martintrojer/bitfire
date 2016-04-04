/******************** (C) COPYRIGHT 2003 STMicroelectronics ********************
* File Name          : main.c
* Author             : MCD Application Team
* Date First Issued  : 16/05/2003
* Description        : This example shows how to use the ADC12 in single channel 
*                      mode and calibrate the conversion result to get a linear
*                      conversion.
*********************************************************************************
* History:
* 13/01/06 : V3.1
* 24/05/05 : V3.0
* 30/11/04 : V2.0
* 16/05/03 : Created
*******************************************************************************
 THE PRESENT SOFTWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS WITH
 CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
 AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT, INDIRECT
 OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE CONTENT
 OF SUCH SOFTWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING INFORMATION
 CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/
#include "71x_lib.h"
#include <stdio.h>

  u16 ADC12_UnsigValue(u16 ADC_Reg);
  u16 ADC12_ConversionAverage( ADC12_Channels Channel, u16 N);

  u16 ADC_Min   = 0;     /*ADC register value at 0V*/
  u16 ADC_Max   = 0;     /*ADC register value at 2.5V*/
  u16 ADC_Unsig_Min = 0; /* ADC 12-bit unsigned value at 0V*/
  u16 ADC_Unsig_Max = 0; /* ADC 12-bit unsigned value at 2.5V */
  u16 SWEEP;
  u32 ADC_dV;
  u16 ADC_Reg, ADC_Unsig_Result;
  u32 Calib_result;

int main (void)
{

 #ifdef DEBUG
  debug();
 #endif /* DEBUG */

  /* GPIO peripheral configuration ------------------------------------------*/

  /*  Configure the Analog input AN3 to HI_AIN_TRI */
  GPIO_Config (GPIO1, (0x0001 <<3), GPIO_HI_AIN_TRI);

  /* Configure the P1.9 to input the push button signal */
  GPIO_Config( GPIO1, (0x0001 << 9), GPIO_IN_TRI_TTL);

  GPIO_WordWrite(GPIO0,0x000F);

  /* Configure the GPI0 port 0 pins to ouput push pull */
  GPIO_Config( GPIO0, 0x000F, GPIO_OUT_PP);

/* ADC12 peripheral configuration  ----------------------------------------*/

  /*  Initialize the conveter register. */
  ADC12_Init();

  /*  Configure the prescaler register using the configured PCLK with
   a sampling frequency=500Hz */
  ADC12_PrescalerConfig(500);

  /*  Select the single conversion mode */
  ADC12_ModeConfig (ADC12_SINGLE);

 /* Select the Channel 3 to be converted */
  ADC12_ChannelSelect(ADC12_CHANNEL3);

  /*  Start the Converter */
  ADC12_ConversionStart();

  /*Apply a 0V on the ADC channel 3 press the push button NXT */
  while( GPIO_BitRead(GPIO1, 9));

  /* Get the ADC conversion result for 0V voltage */
  ADC_Min =  ADC12_ConversionAverage( ADC12_CHANNEL3, 1000);

  /* switch off led on Port P0.0 */
  GPIO_BitWrite(GPIO0,0,0);
  printf(" ADC conversion result for 0V is %x\n", ADC_Min);

  /* Apply a 2.5V on the ADC channel 3 press the push button NXT */
  while( GPIO_BitRead(GPIO1, 9));
  ADC_Max =  ADC12_ConversionAverage( ADC12_CHANNEL3, 1000);

  /* switch on led on Port P0.1 */
  GPIO_BitWrite(GPIO0,1,0);

  printf(" ADC conversion result for 2.5V is %x\n", ADC_Max);

  /* Convert the ADC_Min into 12-bit unsigned value */
  ADC_Unsig_Min = ADC12_UnsigValue(ADC_Min);
  printf(" The 12-bit unsigned conversion result for 0V is %x\n", ADC_Unsig_Min);

  /*Convert the ADC_Max into 12-bit unsigned value */
  ADC_Unsig_Max = ADC12_UnsigValue(ADC_Max);
  printf(" The 12-bit unsigned conversion result for 2.5V is %x\n", ADC_Unsig_Max);

  /* Compute the ADC Gain  */
  SWEEP = ADC_Unsig_Max - ADC_Unsig_Min;

  /* Compute the ADC quantium in µV */
  if (SWEEP !=0)
    ADC_dV=(2500 * 1000)/SWEEP;
  printf( "The ADC computed quatium in µV is %d\n", ADC_dV);

  /* Wait for the push button to be pressed */
  while( GPIO_BitRead(GPIO1, 9));

  while(1)
  {
    /* Toggle the led on GPIO0.2 */
    GPIO_BitWrite(GPIO0, 2, ~(GPIO_BitRead( GPIO0, 2)));

    /*  Wait until the availabiliy of data in the specific flags */
    while (ADC12_FlagStatus (ADC12_DA3)==RESET);

    /*  Get the conversion result from the correponding Data register */
    ADC_Reg = ADC12_ConversionValue(ADC12_CHANNEL3);

    /* Convert the ADC Conversion result to 12 bit unsigned value.*/
    ADC_Unsig_Result = ADC12_UnsigValue(ADC_Reg);

    /* This variable is used to display the conversion result already
    calibrated on the watch windows */
    Calib_result = (ADC_Unsig_Result - ADC_Unsig_Min )*ADC_dV;

    /*  Display the conversion result after calibration */
    printf("Calib_result = %d microV \n",Calib_result);
  }

}

/*******************************************************************************
* Function Name  : ADC12_Unsig_Value
* Description    : This is used for ADC12 result calibration
* Input          : Conv_Res : Conversion result  12 bit two complement
* Output         : None
* Return         : Conversion result calibrated 12 bit unsigned value.
*******************************************************************************/
u16 ADC12_UnsigValue(u16 Conv_Res)
{
  if ((Conv_Res & 0x800) == 0x800)
    return (0x800 - (((~ Conv_Res)&0xFFF) +1));
  else
    return (Conv_Res + 0x800);
}

/*******************************************************************************
* Function Name  : ADC12_Conversion_Average
* Description    : This function is used to compute the average of an ADC 
*                  channel conversion    
* Input1         : ADC12_Channel: channel selected to be converted
* Input2         : u16 : number of time the channel is converted
* Output         : None
* Return         : Conversion average result 
*******************************************************************************/
u16 ADC12_ConversionAverage( ADC12_Channels Channel, u16 N)
{
  u16 i = 0;
  ADC12_Flags ChannelFlag;
  u32 Conv_Sum = 0;
  u16 Conv_Av =0;

  ChannelFlag = (ADC12_Flags)( 1 << (Channel >> 4) ) ;

  for( i = 0; i<N; i++)
  {
	  /* Wait until the availabilty of converted data */
    while( ADC12_FlagStatus( ChannelFlag) == RESET);

   	/* Get the conversion result and add it to Conv_Sum*/
    Conv_Sum += ADC12_ConversionValue( Channel);
   }
  
  Conv_Av = Conv_Sum/N;
 
  return( Conv_Av );
}

/******************* (C) COPYRIGHT 2003 STMicroelectronics *****END OF FILE****/
