/*
*******************************************************************************
*   HEADER FILES
*******************************************************************************
*/

#include "platform.h"

/*
*******************************************************************************
*   GLOBAL DEFINITIONS
*******************************************************************************
*/
#if 0
MenuItem_T Menu0List[] =
{
    {1,  "FTP download",    TranslateMenuToCmd,    PlatformCmdList,     "FTPDOWN"},
    {2,	 "Serial UART download",TranslateMenuToCmd,PlatformCmdList,     "SERIALDOWN"},
    {3,	 "USB download",TranslateMenuToCmd,PlatformCmdList,     "USBDOWN"},
    {14, "Exit",            NULL,                   NULL,              NULL},
    {13,  NULL,             NULL,              		NULL,		       NULL}
};

Menu_T Menu0= {"==== Menu0 ==== ", Menu0List, 0};
#endif

/*-------------------- Diagnostic Manager Main Menu --------------------*/

MenuItem_T MainMenuList[] =
{
    {1,  "Serial Download Image",      	TranslateMenuToCmd,     PlatformCmdList,  "serialdown"},
    {2,  "Program External Flash",     	TranslateMenuToCmd,     PlatformCmdList,  "progextflash"},
    {3,	 "Boot uClinux",				TranslateMenuToCmd,		PlatformCmdList, "loadlinux"},
    {4,  "Command Mode", 				CommandMode,  			NULL,  		NULL},
    {13,  NULL,             NULL,              		NULL,		       NULL}
};

Menu_T MainMenu   = {"==== Main Menu ====", MainMenuList, 0};
