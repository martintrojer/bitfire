#include "platform.h"

/***************************************************************
	GLOBAL VARIABLE
***************************************************************/
unsigned int gimagesize = 0;

/**********************************************************************
 Convert String to Integer. The valid string could be like:
 0x12345678
 23456789
 23K
 5M
  return : 	1, correct; 0, error
*/
int Str2Int(const char *inputstr,unsigned int *intnum)
{
	int i,res;
	unsigned int val = 0;

	if (inputstr[0] == '0' && (inputstr[1] == 'x'||inputstr[1] == 'X'))
	{	
		if (inputstr[2] == '\0')
		{
			return 0;
		}
		for (i=2;i<11;i++)
		{
			if (inputstr[i] == '\0')
			{
				*intnum = val;
				res = 1;//return 1;
				break;
			}
			if (ISVALIDHEX(inputstr[i]))
			{
				val = (val << 4) + CONVERTHEX(inputstr[i]);
			}	
			else 
			{
				//return 0;//inval input
				res = 0;
				break;
			}	
		}	
		
		if (i>=11) res = 0;	//over 8 digit hex --invalid
	}
	else //max 10-digit decimal input
	{
		for (i=0;i<11;i++)
		{
			if (inputstr[i] == '\0')
			{
				*intnum = val;
				//return 1;
				res =1;
				break;
			}
			else if ((inputstr[i] == 'k' ||inputstr[i] == 'K') && (i>0))
			{
				val = val << 10;
				*intnum = val;
				res = 1;
				break;
			}
			else if ((inputstr[i] == 'm' ||inputstr[i] == 'M') && (i>0))
			{	
				
				val = val << 20;
				*intnum = val;
				res = 1;
				break;
			}
			else if (ISVALIDDEC(inputstr[i]))
				val = val*10 + CONVERTDEC(inputstr[i]);
			else 
			{
				//return 0;//inval input
				res = 0;
				break;
			}	
		}
		if (i>=11) res = 0; //over 10 digit decimal --invalid
	}
	
	return res;
}

/************************************************************************************
 Get user input from console,and convert it to an Integer number
 return 1, correct
 return 0: input cancelled
***********************************************************************************/
int GetIntegerInput(unsigned int * num)
{
	char inputstr[16];

	while(1)
	{
		GetInputString(inputstr);
		if (inputstr[0] == '\0') continue;
		
		if ((inputstr[0] == 'a'||inputstr[0] == 'A')&& inputstr[1] == '\0')		
		{	
			PutString("User Cancelled \r\n");
			return 0;
		}	
		
		if (Str2Int(inputstr,num) ==0)	
			PutString("Error, Input again: \r\n");
		else return 1;	
	}
}
