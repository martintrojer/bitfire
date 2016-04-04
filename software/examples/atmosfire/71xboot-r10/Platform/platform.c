#include "platform.h"
#include "71x_lib.h"
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

//Menu Constants
#define MENUEXIT 0x100       // Flag to cause menu loop to exit
#define MENUNUMBERMASK 0xFF // Test number field

/*
*******************************************************************************
*   DATA TYPES
*******************************************************************************
*/

//for parsing command line
enum ParseState {
	PS_WHITESPACE,
	PS_TOKEN,
	PS_STRING,
	PS_ESCAPE
};


/***********************************************************************
	Input/Output Functions
	
************************************************************************/

/*----------------------------------------------------------------------
 * Test to see if a key has been pressed on the input device.
 */
int SerialKeyPressed(char *key)
{
   	if (UARTX->SR & UART_RxBufFull)
   	{
   		*key = (char)UARTX->RBR;
   		return 1;
   	}
   	else 
   		return 0;
}

int KeyPressed(int *key)
{
	int ret=0;
	ret += SerialKeyPressed((char*)key);
	
	//if there is another input, we may check that here in this way:
	//ret += ButtonPressed(key);
	
	return ret; 
} 

/*----------------------------------------------------------------------
 * Get a key from the input device. 
 */
int GetKey(void)
{
    int key;

    //First clear Rx buffer
    UART_FifoReset(UARTX,UART_RxFIFO);    
	
	//then waiting for user input
    while (1)
    {
    	WaitMs(50);
    	if (SerialKeyPressed((char*)&key)) break;

		//
		//if there is another input, we may check that.
		//
    }
    
    return key;
}

/*----------------------------------------------------------------------
* Print a character  via the serial device
*/
void SerialPutChar(char c)
{
	UART_ByteSend(UARTX,(u8*)&c);
}

/*----------------------------------------------------------------------
 * Print a string via the serial device.
 */
void SerialPutString(char *s)
{
	
	while (*s != '\0')
	{
	   SerialPutChar(*s);
	   s ++;
	}
}

/*----------------------------------------------------------------------
 * Serial printf.
 */
void SerialPrintf (char * fmt, ...)
{
    char buffer[256];
    va_list ap;

    va_start(ap,fmt);
    //doPrint(buffer,fmt,ap);
    vsprintf(buffer, fmt, ap);
    SerialPutString(buffer);
}

/*----------------------------------------------------------------------
*  Get Input string from the console. 
*/
void GetInputString (char * buffP)
{
    int bytes_read = 0;
    char c;

    do {
    	c = GetKey();
    	if (c == '\r')   	break;
    	if (c == '\b')	//backspace
    	{
			if (bytes_read > 0)
			{
				SerialPutString("\b \b");
		    	bytes_read --;
    		}
    		continue;
    	}
    	if (bytes_read >= CMD_STRING_SIZE )
    	{
    		SerialPutString("Command string size overflow\r\n");
    		bytes_read = 0;
    		continue;
    	}
		
    	/*
    	if (c == UP_KEY || c==DOWN_KEY || c==LEFT_KEY ||c==RIGHT_KEY)
    	{
    		c = BEEP_KEY;
    		SerialPutChar(c);
    		continue;
    	}*/

    	if (c >= 0x20 && c<= 0x7E)
    	{
    		buffP[bytes_read++] = c;
    		SerialPutChar(c);
    	}	
    	
    }while (1);
    SerialPutString("\n\r");
    buffP[bytes_read] = '\0';

}

/*---------------------------------------------------------------------------
* This function replaces all the lower case charachters in the
*                    string to the upper case.
*/
#if 0
static
void ConvertToUpper (char * stringP)
{
    int i,j;
    char c;

    j = strlen(stringP);

    for (i = 0; i < j; ++i,++stringP)
    {
        c = *stringP;
        c = toupper(c);
        *stringP = c;
    }
}
#endif

/*
*******************************************************************************
*
*     This function is used to find a command structure in the
*     command list array. It converts all the charachters in the
*     input string to the upper case. It looks for the function
*     name of the command entry structure that matches the name
*     of the input string.
*
* INPUT PARAMETERS: cmdListP   is a pointer to the command list array.
*                   cmdStringP is a pointer to the command string.
*
* RETURNS:          a pointer  to command list entry structure in the command
*                              list array.
*                   NULL       if no match is found.
*
*******************************************************************************
*/
static
CommandListEntry_T * findCmd (CommandListEntry_T * command, char * cmdStringP)
{
    int i;

    for (i=0; command[i].name; i++)
    {
        if (strcmp(command[i].name, cmdStringP) == 0)
        {
            return &command[i];
        }
    }

    return NULL;
}
/*-----------------------------------------------------------------------
 * Parse user command line 
 */
static void parseargs(char *argstr, int *argc_p, char **argv, char** resid)
{
	int argc = 0;
	char c;
	enum ParseState stackedState;
	enum ParseState lastState = PS_WHITESPACE;

	/* tokenize the argstr */
	while ((c = *argstr) != 0) {
		enum ParseState newState;

		if (c == ';' && lastState != PS_STRING && lastState != PS_ESCAPE)
			break;

		if (lastState == PS_ESCAPE) {
			newState = stackedState;
		} else if (lastState == PS_STRING) {
			if (c == '"') {
				newState = PS_WHITESPACE;
				*argstr = 0;
			} else {
				newState = PS_STRING;
			}
		} else if ((c == ' ') || (c == '\t')) {
			/* whitespace character */
			*argstr = 0;
			newState = PS_WHITESPACE;
		} else if (c == '"') {
			newState = PS_STRING;
			*argstr++ = 0;
			argv[argc++] = argstr;
		} else if (c == '\\') {
			stackedState = lastState;
			newState = PS_ESCAPE;
		} else {
			/* token */
			if (lastState == PS_WHITESPACE) {
				argv[argc++] = argstr;
			}
			newState = PS_TOKEN;
		}

		lastState = newState;
		argstr++;
	}

#if 0 /* for debugging */
	{
		int i;
		putLabeledWord("parseargs: argc=", argc);
		for (i = 0; i < argc; i++) {
			putstr("   ");
			putstr(argv[i]);
			putstr("\r\n");
		}
	}
#endif
	
	argv[argc] = NULL;
	if (argc_p != NULL)
		*argc_p = argc;

	if (*argstr == ';') {
		*argstr++ = '\0';
	}
	*resid = argstr;
}

/*
*******************************************************************************
*
* FUNCTION:         ExecCommandline
*
* DESCRIPTION:      This function is used to process an input string, split the
*                   input string into command and parameter strings, find a command
*                   structure in the command list array and execute the command.
*
* INPUT PARAMETERS: cmdListP   is a pointer to the command list array.
*                   cmdStringP is a pointer to the input string.
*
* RETURNS:          none.
*******************************************************************************
*/
void ExecCommandline (CommandListEntry_T * cmdListP, char * inputStringP)
{
    CommandListEntry_T * cmdListEntryP;
	int argc;
	char *argv[16];
	char *resid;

	while (*inputStringP) {
		memset(argv, 0, sizeof(argv));
		parseargs(inputStringP, &argc, argv, &resid);
		if (argc > 0)
		{
		    if ((cmdListEntryP=findCmd(cmdListP, argv[0])) == NULL)
		    {
		        SerialPutString("Command is not found\r\n");
		    }
		    else
		    {
			    cmdListEntryP->func(argc, (const char **)argv);
		    }
		}	
		inputStringP = resid;
	}

}

/*
*******************************************************************************
*
* FUNCTION:         CommandMode
*
* DESCRIPTION:      This function enters command mode via serial port.
					It is used to get an input string, find a command
*                   structure in the command list array and execute the command.
*					It can be treated as a Menu handler directly.
*
* RETURNS:          none.
*
*******************************************************************************
*/

void CommandMode(void * arg, char * dummyP)
{
    char inputString[CMD_STRING_SIZE];
    
    inputString[CMD_STRING_SIZE-1]= '\0';

	SerialPutString("Command line mode...:\r\n");
	while(1)
	{
		SerialPutChar('>');

        // Process a command string.
		GetInputString (inputString);
        if (strcmp(inputString,"EXIT")==0 || strcmp(inputString,"exit")==0)
        	return;
		ExecCommandline (PlatformCmdList, inputString);
	}
}
/*
*******************************************************************************
*
* FUNCTION:         TranslateMenuToCmd
*
* DESCRIPTION:      This function is used by the menu system. It is passed the
*                   string from menu entry structure. It splits the string into
*                   the command and parameter strings, finds a command in the
*                   command list array structure and executes the command.
*
* INPUT PARAMETERS: arg          is a pointer to the command list.
*                   inputStringP is a pointer to the the command string from menu
*                                entry structure.
*
* RETURNS:          none.
*
*******************************************************************************
*/

void TranslateMenuToCmd(void* arg, char * inputStringP)
{
    CommandListEntry_T * cmdListP = (CommandListEntry_T *) arg;
    char buff[CMD_STRING_SIZE], * buffP;

    buffP = buff;
    strcpy(buffP, inputStringP);
    buff[CMD_STRING_SIZE - 1] = '\0';

    ExecCommandline(cmdListP, buffP);
}


/*********************************************************************
	Menu functions
**********************************************************************/

/*----------------------------------------------------------------------
 * Translate a key to an index.
 */
static int translateKey(char key)
{
    if (ISVALIDHEX(key))
        return(CONVERTHEX(key));
    else return -1;
}

/*----------------------------------------------------------------------
 * draw a Menu including the title and menu items
 */

static 
void drawMenu(Menu_T * menuP,int selItemRow)
{
	int i;

    //Menu Title.
    SerialPrintf("\r\n%s\r\n",menuP->banner);

    for (i=0; i < 16; i++)
    {
        int num = menuP->menuList[i].selectionChar & MENUNUMBERMASK;
        if (menuP->menuList[i].displayedString == NULL)
            break;

        // Display the menu item.
		if (selItemRow == i)//selected menu item
		{
	        Printf("*%X=%-s\r\n",num,menuP->menuList[i].displayedString);
		}
		else {
	        Printf(" %X=%-s\r\n",num,menuP->menuList[i].displayedString);
		}
        
    }
}
/*----------------------------------------------------------------------
 * Find the menu entry that matches the number specified.
 */
static
int findMenu(int testNum, MenuItem_T * menuListP)
{
    int i = 0;

    do {
        if ((menuListP[i].selectionChar & MENUNUMBERMASK) == testNum)
            return i;
    } while (menuListP[i++].displayedString != 0) ;
    return -1;
}

/*---------------------------------------------------------------------
* Build a menu and Handle user selection
*/
void BuildMenu(void * menuP,char * dummyP)
{
	short int menuItemCount=0;
	int key,index;
    static int menulevel=0;
	MenuItem_T *menulistP = ((Menu_T *)menuP)->menuList;

    while(menulistP[menuItemCount].displayedString != 0)
        menuItemCount++;

	index = 0;
   	menulevel ++;
	do
    {
        if (index < 0 || index >= menuItemCount) index = 0;
        
		drawMenu((Menu_T *)menuP,index);

		//Wait for any key pressed
waitkey:
		key = GetKey();
		
		if (ISCONTROLKEY(key))
		{
			if (key == KEY_UP)
			{
				index --;
				if (index < 0)
				{
					index = menuItemCount -1;
				}
				continue;
			}
			else if (key == KEY_DOWN)
			{
				index ++;
				if (index >= menuItemCount) 
				{
					index = 0;
				}
				continue;
			}
			else if (key == KEY_ACTION)
			{
	            ;
			}
			else if (key == KEY_BACK)
			{
				//to upper level menu
	         if (menulevel > 1)
				break;
			 else goto waitkey;	
			}
			else if (key == KEY_UNKNOWN)
	      	{
	        	goto waitkey;
	      	}
		}
		else
		{
			//user activate the menu handler thru selection char(e.g. press number)
			index = findMenu(translateKey(key),menulistP);
			if (index == -1) goto waitkey;	//invalid selection, just do nothing
		}
		
    	if (menulistP[index].func)
        	menulistP[index].func(menulistP[index].arg,menulistP[index].cmdlineString);
    	else
    	{
            break;//exit the current menu
	    }

	}while(1);

   menulevel --;
}



/*******************************************************************************
*		Time Functions
********************************************************************************/

//NOTE: Now We set PCLK2 = 12MHz, so the max period is 65536*256/12M = 1398101us

void WaitMs(int msVal)
{
	WDG->CR = 0x00;
	while (msVal > 1024)
	{
		WDG_PeriodValueConfig(1024000);
		msVal -= 1024;

		WDG_CntOnOffConfig(ENABLE);	
		while(!(WDG->SR & 0x1));
		WDG->SR = 0x0000;
		WDG_CntOnOffConfig(DISABLE);
	}
	
	WDG_PeriodValueConfig(msVal*1000);
	WDG_CntOnOffConfig(ENABLE);	
	while(!(WDG->SR & 0x1));
	WDG->SR = 0x0000;
	WDG_CntOnOffConfig(DISABLE);

}