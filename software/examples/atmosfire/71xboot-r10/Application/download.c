#include "platform.h"
#include "common.h"
#include "ymodem.h"

extern char file_name[FILE_NAME_LENGTH];

void SerialDownload (int argc, const char **argv)
{
    int errno = 0;
    int i32Size;
	unsigned int u32DownloadAddr;

	if (argc == 1)
	{
		u32DownloadAddr = 0x62000000;
		/*
		PutString("Please input the destination address for download:\n\r");
		if (!GetIntegerInput(&u32DownloadAddr))
		{
    		PutString("Invalid Command Arguments!\n\r");
            return;
		}*/
    }
    else if (argc == 2)
    {
   		if (!Str2Int(argv[1], &u32DownloadAddr))
   		{
    		errno = 1;
        }
    }
    else
    {
  		errno = 1;
    }
    
    if (errno == 1)
	{
  		PutString("Invalid Command Arguments!\n\r");
  		return;
	}
	
    PutString("Waiting for the file to be sent ... (press 'a' to abort)\n\r");
    i32Size = ymodem_receive((char *)u32DownloadAddr);
    if (i32Size > 0)
    {
        Printf("\n\rTransfer Completed. %s Received, %d Bytes.\n\r", file_name, i32Size);
        gimagesize = i32Size;
    }
    else if (i32Size < 0)
    {
        PutString("\n\rAborted by user.\n\r");
    }
    else
    {
        PutString("\n\rFailed to receive the file!\n\r");
    }

    return;
}
