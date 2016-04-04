/*
*******************************************************************************
*   HEADER FILES
*******************************************************************************
*/
#include "platform.h"
#include "string.h"
//#include "71x_lib.h"
#include "common.h"
static void GetVersionNum(int argc, const char **argv)
{
	Printf("Version: %d.%d\r\n",MAJOR_VER,MINOR_VER);
}
static void DefaultHandler(int argc, const char **argv)
{
/*
	if (argc >1)
	Printf("You select: %s\r\n",argv[1]);
    Printf("Waiting 5S...\r\n");
    WaitMs(5000);
    Printf("End Waiting.\r\n");
    Printf("Hit any key to contine\r\n");
    GetKey();
*/

	unsigned int num;
	while(1)
	{
		Printf("Please Input an Integer:\r\n");
		if (!GetIntegerInput(&num)) break;
		Printf("You have input: 0x%08x\r\n",num);
	}	
	
}

//
void ShowCommandHelp(int argc, const char **argv)
{
	int i;
	if (argc == 1)
	{
		Printf("All available commands:\r\n");
		for (i=0;PlatformCmdList[i].name;i++)
			Printf("%s\r\n",PlatformCmdList[i].usagestr);
	}
	else if (argc == 2)
	{
		for (i=0;PlatformCmdList[i].name;i++)
		if (strcmp(PlatformCmdList[i].name,argv[1])==0)
		{
			Printf("Usage:%s \r\n",PlatformCmdList[i].usagestr);
			Printf("Description:%s\r\n",PlatformCmdList[i].helpstr);
			return;
		}	
		Printf("Command not found!\r\n");
	}
}

extern void JumpTo(int argc, const char **argv);

/*
*******************************************************************************
*   GLOBAL DEFINITIONS
*******************************************************************************
*/
CommandListEntry_T PlatformCmdList[] =
{
	{"defhandle",	DefaultHandler, "defhandle","--default handler.Just for test."},
	
	{"getversion",	GetVersionNum, "getversion","--Query software version number."},

//    {"progextflash",	ProgramExtFlash, "progextflash [destflashaddr srcramaddr imagesize]","--Program image stored in RAM into flash."},

	{"serialdown",	SerialDownload, "serialdown [destramaddr]","--Download image into RAM via serial port."},

// 	{"usbdown",	USBDownload,"usbdown [ramaddr]","--Download image into RAM via USB."},

	{"loadlinux",	LoadLinux, "loadlinux [srcflashaddr]","--Load uclinux kernel."},
	
	{"jmpto",	JumpTo, "jmpto jmpaddr","--Jump to an address."},

	{"help", ShowCommandHelp, "help [command]","--Show command's help info."},
	
	{"exit", NULL, "exit","--Exit command mode and return to menu mode"},
    0
};

