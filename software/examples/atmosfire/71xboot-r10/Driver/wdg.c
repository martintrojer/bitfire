/******************** (C) COPYRIGHT 2003 STMicroelectronics ********************
* File Name          : WDG.c
* Author             : MCD Application Team
* Date First Issued  : 24/10/2003
* Description        : This file provides all the WDG software functions
********************************************************************************
* History:
*  25/08/03 : First Version.
*******************************************************************************/

#include "wdg.h"

#ifndef abs
	#define abs(x) ((x)>0 ? (x) : -(x))
#endif

/*******************************************************************************
* Function Name  : FindFactors
* Description    : Search for the best (a,b) values that fit n = a*b
*                  with the following constraints: 1<=a<=256, 1<=b<=65536
* Input 1        : n: the number to decompose
* Input/Output 2 : a: a pointer to the first factor
* Input/Output 3 : b: a pointer to the second factor
* Return         : None
*******************************************************************************/
static void FindFactors(unsigned long n, unsigned int *a, unsigned long *b)
{
	unsigned long b0;
	unsigned int a0;
	long err, err_min=n;

	*a = a0 = ((n-1)/65536ul) + 1;
	*b = b0 = n / *a;

	for (; *a <= 256; (*a)++)
	{
		*b = n / *a;
		err = (long)*a * (long)*b - (long)n;
		if (abs(err) > (*a / 2))
		{
			(*b)++;
			err = (long)*a * (long)*b - (long)n;
		}
		if (abs(err) < abs(err_min))
		{
			err_min = err;
			a0 = *a;
			b0 = *b;
			if (err == 0) break;
		}
	}

	*a = a0;
	*b = b0;
}

/*******************************************************************************
* Function Name  : WDG_PeriodValueConfig
* Description    : Set the prescaler and counter reload value
* Input          : Amount of time (us) needed
* Return         : None
*******************************************************************************/
void WDG_PeriodValueConfig ( u32 Time )
{
	unsigned int a;
	unsigned long n, b;

	n = Time * (RCCU_FrequencyValue(RCCU_PCLK) / 1000000);
	FindFactors(n, &a, &b);
    WDG->PR = a - 1;
    WDG->VR = b - 1;
}

/******************* (C) COPYRIGHT 2003 STMicroelectronics *****END OF FILE****/
