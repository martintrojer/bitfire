#ifndef _platform_h
#define _platform_h

#ifndef NULL
#define NULL (void *)0
#endif


#define MAJOR_VER	1
#define MINOR_VER	0


		
//common routines
#define IS_AF(c)	((c >= 'A') && (c <= 'F'))
#define IS_af(c)	((c >= 'a') && (c <= 'f'))
#define IS_09(c)	((c >= '0') && (c <= '9'))
#define ISVALIDHEX(c)	IS_AF(c) || IS_af(c) || IS_09(c)
#define ISVALIDDEC(c)	IS_09(c)
#define CONVERTDEC(c)	(c - '0')

#define CONVERTHEX_alpha(c)	(IS_AF(c) ? (c - 'A'+10) : (c - 'a'+10))	
#define CONVERTHEX(c)   (IS_09(c) ? (c - '0') : CONVERTHEX_alpha(c))

// Constants used by Serial Command Line Mode
#define CMD_STRING_SIZE     128

//
#define	ENTER_KEY	0x0d
#define	BACK_KEY	0x08
#define BEEP_KEY	0x07
#define UP_KEY		0x41
#define DOWN_KEY	0x42
#define RIGHT_KEY	0x43
#define LEFT_KEY	0x44
#define HOME_KEY	0x48
#define END_KEY		0x4b
#define	ESC_KEY		0x1b

//Navigation Keys
#define KEY_ACTION	-1
#define KEY_BACK	-2
#define KEY_UP		-3
#define KEY_DOWN	-4
#define KEY_UNKNOWN	0
#define ISCONTROLKEY(key) (key >= KEY_DOWN && key<KEY_UNKNOWN)  

/*
************************************************************************************
*                            DATA TYPES 
************************************************************************************
*/

typedef void CommandFunc_T (int argc, const char ** );
typedef struct CommandListEntry_S
{
    char* name;             /* Command entry name */
    CommandFunc_T *func; /* Dispatch function */
    char *usagestr;
    char *helpstr;
} CommandListEntry_T;

typedef void MenuFunc_T (void* arg, char* string);

typedef struct MenuItem_S
{
	int selectionChar;
	char *displayedString;
	MenuFunc_T *func;
	void *arg;
	char *cmdlineString;
} MenuItem_T;

typedef struct Menu_S 
{
	char * banner;
	MenuItem_T *menuList;
    int parameter;                                 
} Menu_T;

/***********************************************************************
	GLOBAL VARIABLES
***********************************************************************/
extern CommandListEntry_T PlatformCmdList[];
extern Menu_T MainMenu;

/***********************************************************************
	Function Prototypes
***********************************************************************/
//Output functions
#define Printf	SerialPrintf
#define PutChar SerialPutChar
#define PutString SerialPutString

void SerialPrintf (char * fmt, ...);
void SerialPutChar(char c);
void SerialPutString(char *s);

// Menu functions
void BuildMenu(void * menuP, char * dummyP);
void TranslateMenuToCmd(void* arg, char * inputStringP);

// Command mode support.
void CommandMode(void *arg, char * dummyP);
void ExecCommandline (CommandListEntry_T * , char * );


// Input functions
int  KeyPressed(int *);
int  SerialKeyPressed(char *);
int  GetKey(void);
void GetInputString(char *);

// Timer services
void WaitMs(int msVal);


#endif