/******************************************************************************

Project:           Portable command line ISP for Philips LPC2000 family
                   and Analog Devices ADUC70xx

Filename:          lpc21isp.c

Compiler:          Microsoft VC 6/7, GCC Cygwin, GCC Linux

Autor:             Martin Maurer (Martin.Maurer@clibb.de)

Copyright:         (c) Martin Maurer 2003, 2004, All rights reserved
Portions Copyright (c) by Aeolus Development 2004 http://www.aeolusdevelopment.com

Version:           1.22

Change-History:

 1.00  2004-01-08  Initial Version, tested for MSVC6/7 and GCC under Cygwin
 1.01  2004-01-10  Porting to Linux (at least compiling must work)
 1.02  2004-01-10  Implemented conversion intel hex format -> binary
 1.03  2004-01-25  Preparation to upload to public website
 1.04  2004-02-12  Merged in bugfixes by Soeren Gust
 1.05  2004-03-14  Implement printing of error codes as text / strings
 1.06  2004-03-09  Merged in bugfixes by Charles Manning:
                   The '?' sychronisation does not reliably respond to the first '?'.
                   I added some retries.
                   The LPC2106 sometimes responds to the '?' by echoing them back.
                   This sometimes causes an attempt to match "?Synchonized".
                   Added code to strip off any leading '?'s.
                   Timeouts were too long.
                   Change from RTS/CTS to no flow control.
                   Done because many/most people will use only 3-wire comms.
                   Added some progress tracing.
 1.07  2004-03-14  Implement handling of control lines for easier booting
 1.08  2004-04-01  Bugfix for upload problem
 1.09  2004-04-03  Redesign of upload routine
                   Now always 180 byte blocks are uploaded, to prevent
                   small junks in uuencoding
 1.10  2004-04-03  Clear buffers before sending commands to LPC21xx,
                   this prevents synchronizing errors when previously loaded
                   program does a lot of output, so FIFO of PC runs full
 1.11  2004-04-03  Small optimization for controlling reset line
                   otherwise termonly starts LPC twice, free PC buffers
 1.12  2004-04-04  Add switch to enable logging terminal output to lpc21isp.log
 1.13  2004-05-19  Merged in improvement by Charles Manning:
                   Instead of exiting the wrong hex file size is corrected
 1.14  2004-07-07  Merged in improvement by Alex Holden:
                   Remove little/big endian dependancy
 1.15  2004-09-27  Temporary improvement by Cyril Holweck:
                   Removed test (data echoed = data transmited) on the main
                   data transfert, since this was the biggest failure
                   reason and is covered by checksome anyway.
                   Added COMPILE_FOR_LPC21, to have target dump it's own
                   memory to stdout.
 1.16  2004-10-09  Merged in bugfix / improvement by Sinelnikov Evgeny
                   I found out that Linux and Windows serial port initialization
                   are different with pinouts states. My board don't get
                   reset signal at first cycle of DTR pinout moving.
                   And I add this moving to initalization cycle.
 1.17  2004-10-21  Changes by Cyril Holweck
                   Divide main, take out the real programming function, that can
                   also be used by a target to copy its own code to another.
 1.18  2004-10-26  Changes by Cyril Holweck
                   Added a "G 0 A\r\n" at end of programming to run code.
 1.19  2004-11-03  Changes by Robert Adsett
                   Add support for Analog Devices.
                   Separate file load from programming.
                   Change from a debug on/off flag to debug level
                   Remove if(debug) tests and replace with DebugPrintf
                   statements.
                   Change serial I/O and timing so that the system
                   dependancies are isolated to a few portability functions.
                   Add support for binary serial I/O.
                   Add doxygen support.
 1.20  2004-11-07  Preparation for multiport booting (factory support)
 1.21  2004-11-08  Bugfix from Robert Adsett
                   BinaryLength was not initialized
 1.22  2004-11-08  Changes from Cyril Holweck / Evgeny Sinelnikov
                   Forgotten IspEnvironment-> and bugfixes if COMPILE_FOR_LINUX
                   If COMPILE_FOR_LPC21, PhilipsDownload() 'acts as' main():
                   - it should not be static and should return int.
                   - no sub-function can use exit() but only return()
                   Use 'char' instead of 'byte' ;)

******************************************************************************/

#if defined(_WIN32) && !defined(__CYGWIN__)
    #define COMPILE_FOR_WINDOWS
    #define COMPILED_FOR ("Windows")
#elif defined(__CYGWIN__)
    #define COMPILE_FOR_CYGWIN
    #define COMPILED_FOR ("Cygwin")
#elif defined(__arm__) || defined(__thumb__)
    #define COMPILE_FOR_LPC21
    #define COMPILED_FOR ("ARM")
    #define printf iprintf
#else
    #define COMPILE_FOR_LINUX
    #define COMPILED_FOR ("Linux")
#endif

#if defined COMPILE_FOR_WINDOWS || defined COMPILE_FOR_CYGWIN
#include <windows.h>
#include <io.h>
#endif // defined COMPILE_FOR_WINDOWS || defined COMPILE_FOR_CYGWIN

#if defined COMPILE_FOR_WINDOWS
#include <conio.h>
#endif // defined COMPILE_FOR_WINDOWS

#if defined COMPILE_FOR_LINUX
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/ioctl.h>
#endif // defined COMPILE_FOR_LINUX

#if defined COMPILE_FOR_LINUX || defined COMPILE_FOR_CYGWIN
#include <termios.h>
#include <unistd.h>     // for read and return value of lseek
#include <sys/time.h>   // for select_time
#endif // defined COMPILE_FOR_LINUX || defined COMPILE_FOR_CYGWIN

#include <stdio.h>      // stdout
#include <stdarg.h>

#if defined COMPILE_FOR_LPC21
#include <stdlib.h>
#include <string.h>
#include "lpc_ioctl.h"
#endif

#if !defined COMPILE_FOR_LPC21
#include <fcntl.h>
#endif

#ifndef O_BINARY
#define O_BINARY 0
#endif // O_BINARY

#ifndef DWORD
#define DWORD unsigned long
#endif // DWORD

#if defined COMPILE_FOR_LINUX
#define stricmp strcasecmp
#endif // defined COMPILE_FOR_LINUX

typedef unsigned char BINARY;   /**< data type used for microcontroller
                                *  memory image.                        */

/*
debug levels
0 - very quiet          - Anything at this level gets printed
1 - quiet               - Only error messages should be printed
2 - indicate progress   - Add progress messages
3 - first level debug   - Major level tracing
4 - second level debug  - Add detailed deugging
5 - log comm's          - log serial I/O
*/

typedef enum
{
    PHILIPS_ARM,
    ANALOG_DEVICES_ARM
} TARGET;

typedef struct
{

#if !defined COMPILE_FOR_LPC21
    TARGET micro;                      /**< The type of micro that will be
                                         * programmed.                           */
    int debug_level;
    unsigned char TerminalAfterUpload;
    unsigned char TerminalOnly;
    unsigned char FormatHex;
    unsigned char ControlLines;
    unsigned char LogFile;
    char *input_file;                   /**< Name of the file to get input from. */
    char *serial_port;                  /**< Name of the serial port to use to
                                         * communicate with the microcontroller.
                                         * Read from the command line.          */
    char *baud_rate;                    /**< Baud rate to use on the serial
                                         * port communicating with the
                                         * microcontroller. Read from the
                                         * command line.                        */
#endif // !defined COMPILE_FOR_LPC21

    char *StringOscillator;             /**< Holds representation of oscillator
                                          * speed from the command line.         */
    BINARY *BinaryContent;              /**< Binary image of the                 */
                                        /* microcontroller's memory.             */
    unsigned long BinaryLength;

#if defined COMPILE_FOR_WINDOWS || defined COMPILE_FOR_CYGWIN
    HANDLE hCom;
#endif // defined COMPILE_FOR_WINDOWS || defined COMPILE_FOR_CYGWIN

#if defined COMPILE_FOR_LINUX || defined COMPILE_FOR_LPC21
    int fdCom;
#endif // defined COMPILE_FOR_LINUX || defined COMPILE_FOR_LPC21

#if defined COMPILE_FOR_LINUX
    struct termios oldtio, newtio;
#endif // defined COMPILE_FOR_LINUX

    unsigned serial_timeout_count;   /**< Local used to track
                                      * timeouts on serial port read. */

} ISP_ENVIRONMENT;

static int debug_level = 2;

static void DumpString(int level, const void *s, size_t size, const char *fmt, ...);
static void SendComPort(ISP_ENVIRONMENT *IspEnvironment, const char *s);
static void ReceiveComPort(ISP_ENVIRONMENT *IspEnvironment, void *Answer, unsigned long MaxSize, unsigned long *RealSize, unsigned long WantedNr0x0A,unsigned timeOutMilliseconds);
static void DebugPrintf( int level, const char *fmt, ...);
static void PhilipsOutputErrorMessage(unsigned char ErrorNumber);
static unsigned char GetErrorNumber(const char *Answer);
static void SerialTimeoutSet(ISP_ENVIRONMENT *IspEnvironment, unsigned timeout_milliseconds);
static void SerialTimeoutTick(ISP_ENVIRONMENT *IspEnvironment);
static int SerialTimeoutCheck(ISP_ENVIRONMENT *IspEnvironment);

#if !defined COMPILE_FOR_LPC21
static void ClearSerialPortBuffers(ISP_ENVIRONMENT *IspEnvironment);
static void ControlModemLines(ISP_ENVIRONMENT *IspEnvironment, unsigned char DTR, unsigned char RTS);
static unsigned int Ascii2Hex(char c);
#endif

/************* Portability layer. Serial and console I/O differences    */
/* are taken care of here.                                              */

#if defined COMPILE_FOR_LINUX || defined COMPILE_FOR_CYGWIN
static int kbhit( void);
static int getch( void);
#endif // defined COMPILE_FOR_LINUX || defined COMPILE_FOR_CYGWIN

#if defined COMPILE_FOR_LINUX
static void Sleep(unsigned long MilliSeconds);
#endif // defined COMPILE_FOR_LINUX



#if !defined COMPILE_FOR_LPC21
static void OpenSerialPort(ISP_ENVIRONMENT *IspEnvironment)
{
    // Open COM-Port (different between Windows and Linux)

#if defined COMPILE_FOR_WINDOWS || defined COMPILE_FOR_CYGWIN
    DCB    dcb;
    COMMTIMEOUTS commtimeouts;

    IspEnvironment->hCom = CreateFile(IspEnvironment->serial_port, GENERIC_READ | GENERIC_WRITE,0,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);

    if(IspEnvironment->hCom == INVALID_HANDLE_VALUE)
    {
        DebugPrintf( 1, "Can't open COM-Port %s ! - Error: %ld\n", IspEnvironment->serial_port, GetLastError());
        exit(2);
    }

    DebugPrintf( 3, "COM-Port %s opened...\n", IspEnvironment->serial_port);

    GetCommState(IspEnvironment->hCom, &dcb);
    dcb.BaudRate = atol(IspEnvironment->baud_rate);
    dcb.ByteSize = 8;
    dcb.StopBits = ONESTOPBIT;
    dcb.Parity   = NOPARITY;
    if(SetCommState(IspEnvironment->hCom, &dcb) == 0)
    {
        DebugPrintf( 1, "Can't set baudrate %s ! - Error: %ld", IspEnvironment->baud_rate, GetLastError());
        exit(3);
    }

    SetCommMask(IspEnvironment->hCom,EV_RXCHAR | EV_TXEMPTY);

    commtimeouts.ReadIntervalTimeout         = MAXDWORD;
    commtimeouts.ReadTotalTimeoutMultiplier  =    0;
    commtimeouts.ReadTotalTimeoutConstant    =    1;
    commtimeouts.WriteTotalTimeoutMultiplier =    0;
    commtimeouts.WriteTotalTimeoutConstant   =    0;
    SetCommTimeouts(IspEnvironment->hCom, &commtimeouts);

#endif // defined COMPILE_FOR_WINDOWS || defined COMPILE_FOR_CYGWIN

#if defined COMPILE_FOR_LINUX

    IspEnvironment->fdCom = open(IspEnvironment->serial_port, O_RDWR | O_NOCTTY );

    if(IspEnvironment->fdCom < 0)
    {
        DebugPrintf( 1, "Can't open COM-Port %s !\n", IspEnvironment->serial_port);
        exit(2);
    }

    DebugPrintf( 3, "COM-Port %s opened...\n", IspEnvironment->serial_port);

    tcgetattr(IspEnvironment->fdCom, &IspEnvironment->oldtio); /* save current port settings */

    bzero(&IspEnvironment->newtio, sizeof(IspEnvironment->newtio));
    IspEnvironment->newtio.c_cflag = CS8 | CLOCAL | CREAD;

    switch(atol(IspEnvironment->baud_rate))
    {
        case 1152000: IspEnvironment->newtio.c_cflag |= B1152000; break;
        case  576000: IspEnvironment->newtio.c_cflag |=  B576000; break;
        case  230400: IspEnvironment->newtio.c_cflag |=  B230400; break;
        case  115200: IspEnvironment->newtio.c_cflag |=  B115200; break;
        case   57600: IspEnvironment->newtio.c_cflag |=   B57600; break;
        case   38400: IspEnvironment->newtio.c_cflag |=   B38400; break;
        case   19200: IspEnvironment->newtio.c_cflag |=   B19200; break;
        case    9600: IspEnvironment->newtio.c_cflag |=    B9600; break;
        default:
        {
            DebugPrintf( 1, "unknown baudrate %s\n", IspEnvironment->baud_rate);
            exit(3);
        }
    }

    IspEnvironment->newtio.c_iflag = IGNPAR;
    IspEnvironment->newtio.c_oflag = 0;

    /* set input mode (non-canonical, no echo,...) */
    IspEnvironment->newtio.c_lflag = 0;

    cfmakeraw(&IspEnvironment->newtio);
    IspEnvironment->newtio.c_cc[VTIME]    = 10;   /* inter-character timer used */
    IspEnvironment->newtio.c_cc[VMIN]     = 0;   /* blocking read until 0 chars received */

    tcflush(IspEnvironment->fdCom, TCIFLUSH);
    tcsetattr(IspEnvironment->fdCom, TCSANOW, &IspEnvironment->newtio);

#endif // defined COMPILE_FOR_LINUX
}

static void CloseSerialPort(ISP_ENVIRONMENT *IspEnvironment)
{
#if defined COMPILE_FOR_WINDOWS || defined COMPILE_FOR_CYGWIN

    CloseHandle(IspEnvironment->hCom);

#endif // defined COMPILE_FOR_WINDOWS || defined COMPILE_FOR_CYGWIN

#if defined COMPILE_FOR_LINUX

    tcsetattr(IspEnvironment->fdCom, TCSANOW, &IspEnvironment->oldtio);

    close(IspEnvironment->fdCom);

#endif // defined COMPILE_FOR_LINUX
}

#endif // !defined COMPILE_FOR_LPC21

/***************************** SendComPortBlock *************************/
/**  Sends a block of bytes out the opened com port.
\param [in] s block to send.
\param [in] n size of the block.
*/
static void SendComPortBlock(ISP_ENVIRONMENT *IspEnvironment, const void *s, size_t n)
{
#if defined COMPILE_FOR_WINDOWS || defined COMPILE_FOR_CYGWIN

    unsigned long realsize;

#endif // defined COMPILE_FOR_WINDOWS || defined COMPILE_FOR_CYGWIN

    DumpString( 4, s, n, "Sending ");

#if defined COMPILE_FOR_WINDOWS || defined COMPILE_FOR_CYGWIN

    WriteFile(IspEnvironment->hCom, s, n, &realsize, NULL);

#endif // defined COMPILE_FOR_WINDOWS || defined COMPILE_FOR_CYGWIN

#if defined COMPILE_FOR_LINUX || defined COMPILE_FOR_LPC21

    write(IspEnvironment->fdCom, s, n);

#endif // defined COMPILE_FOR_LINUX || defined COMPILE_FOR_LPC21
}

/***************************** SendComPort ******************************/
/**  Sends a string out the opened com port.
\param [in] s string to send.
*/
static void SendComPort(ISP_ENVIRONMENT *IspEnvironment, const char *s)
{
    SendComPortBlock( IspEnvironment, s, strlen(s));
}


/***************************** ReceiveComPortBlock **********************/
/**  Receives a buffer from the open com port. Returns all the characters
ready (waits for up to 'n' milliseconds before accepting that no more
characters are ready) or when the buffer is full. 'n' is system dependant,
see SerialTimeout routines.
\param [out] answer buffer to hold the bytes read from the serial port.
\param [in] max_size the size of buffer pointed to by answer.
\param [out] real_size pointer to a long that returns the amout of the
buffer that is actually used.
*/
static void ReceiveComPortBlock( ISP_ENVIRONMENT *IspEnvironment,
                                 void *answer, unsigned long max_size,
                                 unsigned long *real_size)
{

#if defined COMPILE_FOR_WINDOWS || defined COMPILE_FOR_CYGWIN

    ReadFile(IspEnvironment->hCom, answer, max_size, real_size, NULL);

#endif // defined COMPILE_FOR_WINDOWS || defined COMPILE_FOR_CYGWIN

#if defined COMPILE_FOR_LINUX || defined COMPILE_FOR_LPC21

    *real_size = read(IspEnvironment->fdCom, answer, max_size);

#endif // defined COMPILE_FOR_LINUX


    DumpString( 5, answer, (*real_size), "Read(Length=%ld): ", (*real_size));

    if( *real_size == 0)
    {
        SerialTimeoutTick( IspEnvironment );
    }
}


/***************************** SerialTimeoutSet *************************/
/**  Sets (or resets) the timeout to the timout period requested.  Starts
counting to this period.  This timeout support is a little odd in that the
timeout specifies the accumulated deadtime waiting to read not the total
time waiting to read. They should be close enought to the same for this
use. Used by the serial input routines, the actual counting takes place in
ReceiveComPortBlock.
\param [in] timeout_milliseconds the time in milliseconds to use for
timeout.  Note that just because it is set in milliseconds doesn't mean
that the granularity is that fine.  In many cases (particularly Linux) it
will be coarser.
*/
static void SerialTimeoutSet(ISP_ENVIRONMENT *IspEnvironment, unsigned timeout_milliseconds)
{

#if defined COMPILE_FOR_LINUX
    IspEnvironment->serial_timeout_count = timeout_milliseconds/100;
#elif defined COMPILE_FOR_LPC21
    IspEnvironment->serial_timeout_count = timeout_milliseconds*200;
#else
    IspEnvironment->serial_timeout_count = timeout_milliseconds;
#endif
}

/***************************** SerialTimeoutTick ************************/
/**  Performs a timer tick.  In this simple case all we do is count down
with protection against underflow and wrapping at the low end.
*/
static void SerialTimeoutTick(ISP_ENVIRONMENT *IspEnvironment)
{
    if( IspEnvironment->serial_timeout_count <= 1)
    {
        IspEnvironment->serial_timeout_count = 0;
    }
    else
    {
        IspEnvironment->serial_timeout_count--;
    }
}

/***************************** SerialTimeoutCheck ***********************/
/**  Check to see if the serial timeout timer has run down.
\retval 1 if timer has run out.
\retval 0 if timer still has time left.
*/
static int SerialTimeoutCheck(ISP_ENVIRONMENT *IspEnvironment)
{
    if( IspEnvironment->serial_timeout_count == 0)
    {
        return 1;
    }
    return 0;
}


#if defined COMPILE_FOR_LINUX || defined COMPILE_FOR_CYGWIN
/***************************** getch ************************************/
/** Replacement for the common dos function of the same name. Reads a
single unbuffered character from the 'keyboard'.
\return The character read from the keyboard.
*/
static int getch(void)
{
    char ch;
    struct termios origtty, tty;

    /* store the current tty settings */
    tcgetattr(0, &origtty);

    /* start with the current settings */
    tty = origtty;
    /* make modifications to put it in raw mode, turn off echo */
    tty.c_lflag &= ~ICANON;
    tty.c_lflag &= ~ECHO;
    tty.c_lflag &= ~ISIG;
    tty.c_cc[VMIN] = 1;
    tty.c_cc[VTIME] = 0;

    /* put the settings into effect */
    tcsetattr(0, TCSADRAIN, &tty);

    /* Read in one character */
    read(0,&ch,1);

    /* reset the tty to its original settings */
    tcsetattr(0, TCSADRAIN, &origtty);

    return ch;
}
#endif // defined COMPILE_FOR_LINUX || defined COMPILE_FOR_CYGWIN

#if defined COMPILE_FOR_LINUX || defined COMPILE_FOR_CYGWIN
/***************************** kbhit ************************************/
/** Replacement for the common dos function of the same name. Indicates if
there are characters to be read from the console.
\retval 0 No characters ready.
\retval 1 Characters from the console ready to be read.
*/
static int kbhit(void)
{
    /* return 0 for no key pressed, 1 for key pressed */
    int return_value = 0;

    /* variables to store the current tty state, create a new one */
    struct termios origtty, tty;
    /* time struct for the select() function, to only wait a little while */
    struct timeval select_time;
    /* file descriptor variable for the select() call */
    fd_set readset;

    /* we're only interested in STDIN */
    FD_ZERO(&readset);
    FD_SET(STDIN_FILENO, &readset);

    /* store the current tty settings */
    tcgetattr(0, &origtty);

    /* start with the current settings */
    tty = origtty;
    /* make modifications to put it in raw mode, turn off echo */
    tty.c_lflag &= ~ICANON;
    tty.c_lflag &= ~ECHO;
    tty.c_lflag &= ~ISIG;
    tty.c_cc[VMIN] = 1;
    tty.c_cc[VTIME] = 0;

    /* put the settings into effect */
    tcsetattr(0, TCSADRAIN, &tty);

    /* how long to block for - this must be > 0.0, but could be changed
       to some other setting. 10-18msec seems to work well and only
       minimally load the system (0% CPU loading) */
    select_time.tv_sec = 0;
    select_time.tv_usec = 10;

    /* is there a keystroke there? */
    if (select(1, &readset, NULL, NULL, &select_time))
    {
        /* yes, remember it */
        return_value = 1;
    }

    /* reset the tty to its original settings */
    tcsetattr(0, TCSADRAIN, &origtty);

    /* return with what we found out */
    return return_value;
}
#endif // defined COMPILE_FOR_LINUX || defined COMPILE_FOR_CYGWIN


#if !defined COMPILE_FOR_LPC21
/***************************** ControlModemLines ************************/
/**  Controls the modem lines to place the microcontroller into various
states during the programming process.
error rather abruptly terminates the program.
\param [in] DTR the state to set the DTR line to.
\param [in] RTS the state to set the RTS line to.
*/
static void ControlModemLines(ISP_ENVIRONMENT *IspEnvironment, unsigned char DTR, unsigned char RTS)
{
#if defined COMPILE_FOR_LINUX
    int status;

    if(ioctl(IspEnvironment->fdCom, TIOCMGET, &status) == 0)
    {
        DebugPrintf( 1, "ioctl get ok, status = %X\n",status);
    }
    else
    {
        DebugPrintf( 1, "ioctl get failed\n");
    }

    if(DTR) status |=  TIOCM_DTR;
    else    status &= ~TIOCM_DTR;

    if(RTS) status |=  TIOCM_RTS;
    else    status &= ~TIOCM_RTS;

    if(ioctl(IspEnvironment->fdCom, TIOCMSET, &status) == 0)
    {
        DebugPrintf( 1, "ioctl set ok, status = %X\n",status);
    }
    else
    {
        DebugPrintf( 1, "ioctl set failed\n");
    }

    if(ioctl(IspEnvironment->fdCom, TIOCMGET, &status) == 0)
    {
        DebugPrintf( 1, "ioctl get ok, status = %X\n",status);
    }
    else
    {
        DebugPrintf( 1, "ioctl get failed\n");
    }

#endif // defined COMPILE_FOR_LINUX
#if defined COMPILE_FOR_WINDOWS || defined COMPILE_FOR_CYGWIN

    if(DTR) EscapeCommFunction(IspEnvironment->hCom, SETDTR);
    else    EscapeCommFunction(IspEnvironment->hCom, CLRDTR);

    if(RTS) EscapeCommFunction(IspEnvironment->hCom, SETRTS);
    else    EscapeCommFunction(IspEnvironment->hCom, CLRRTS);

#endif // defined COMPILE_FOR_WINDOWS || defined COMPILE_FOR_CYGWIN

    DebugPrintf( 3, "DTR (%d), RTS (%d)\n", DTR, RTS);
}


/***************************** ClearSerialPortBuffers********************/
/**  Empty the serial port buffers.  Cleans things to a known state.
*/
static void ClearSerialPortBuffers(ISP_ENVIRONMENT *IspEnvironment)
{
#if defined COMPILE_FOR_LINUX
    /* variables to store the current tty state, create a new one */
    struct termios origtty, tty;

    /* store the current tty settings */
    tcgetattr(IspEnvironment->fdCom, &origtty);

    // Flush input and output buffers
    tcsetattr(IspEnvironment->fdCom, TCSAFLUSH, &tty);

    /* reset the tty to its original settings */
    tcsetattr(IspEnvironment->fdCom, TCSADRAIN, &origtty);
#endif // defined COMPILE_FOR_LINUX
#if defined COMPILE_FOR_WINDOWS || defined COMPILE_FOR_CYGWIN
    PurgeComm(IspEnvironment->hCom, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);
#endif // defined COMPILE_FOR_WINDOWS || defined COMPILE_FOR_CYGWIN
}
#endif // !defined COMPILE_FOR_LPC21


#if defined COMPILE_FOR_LINUX
/***************************** Sleep ************************************/
/**  Provide linux replacement for windows function.
\param [in] Milliseconds the time to wait for in milliseconds.
*/
static void Sleep(unsigned long MilliSeconds)
{
    usleep(MilliSeconds*1000); //convert to microseconds
}
#endif // defined COMPILE_FOR_LINUX


/************* Applicationlayer.                                        */

/***************************** DebugPrintf ******************************/
/**  Prints a debug string depending the current debug level. The higher
the debug level the more detail that will be printed.  Each print
has an associated level, the higher the level the more detailed the
debugging information being sent.
\param [in] level the debug level of the print statement, if the level
 is less than or equal to the current debug level it will be printed.
\param [in] fmt a standard printf style format string.
\param [in] ... the usual printf parameters.
*/
static void DebugPrintf( int level, const char *fmt, ...)
{
    va_list ap;

    if( level <= debug_level)
    {
        va_start( ap, fmt);
        vprintf( fmt, ap);
        va_end( ap);
        fflush( stdout);
    }
}

/***************************** ReceiveComPort ***************************/
/**  Receives a buffer from the open com port. Returns when the buffer is
filled, the numer of requested linefeeds has been received or the timeout
period has passed
\param [in] ISPEnvironment.
\param [out] Answer buffer to hold the bytes read from the serial port.
\param [in] MaxSize the size of buffer pointed to by Answer.
\param [out] RealSize pointer to a long that returns the amout of the
buffer that is actually used.
\param [in] WantedNr0x0A the maximum number of linefeeds to accept before
returning.
\param [in] timeOutMilliseconds the maximum amount of time to wait before
reading with an incomplete buffer.
*/
static void ReceiveComPort( ISP_ENVIRONMENT *IspEnvironment,
                            void *Ans, unsigned long MaxSize,
                            unsigned long *RealSize, unsigned long WantedNr0x0A,
                            unsigned timeOutMilliseconds)
{
    unsigned long tmp_realsize;
    unsigned long nr_of_0x0A = 0;
    unsigned long p;
    char *Answer;

    Answer = Ans;

    SerialTimeoutSet( IspEnvironment, timeOutMilliseconds);

    (*RealSize) = 0;

    do
    {
        ReceiveComPortBlock( IspEnvironment, Answer + (*RealSize), MaxSize - 1 - (*RealSize), &tmp_realsize);

        if(tmp_realsize != 0)
        {
            for(p = (*RealSize); p < (*RealSize) + tmp_realsize; p++)
            {
                if(Answer[p] == 0x0a)
                {
                    nr_of_0x0A++;
                }
            }
        }

        (*RealSize) += tmp_realsize;

    } while(((*RealSize) < MaxSize) && (SerialTimeoutCheck(IspEnvironment) == 0) && (nr_of_0x0A < WantedNr0x0A));

    Answer[(*RealSize)] = 0;

    DumpString( 3, Answer, (*RealSize), "Answer(Length=%ld): ", (*RealSize));
}


#if !defined COMPILE_FOR_LPC21

/***************************** ReceiveComPortBlockComplete **************/
/**  Receives a fixed block from the open com port. Returns when the
block is completely filled or the timeout period has passed
\param [out] block buffer to hold the bytes read from the serial port.
\param [in] size the size of the buffer pointed to by block.
\param [in] timeOut the maximum amount of time to wait before guvung up on
completing the read.
\return 0 if successful, non-zero otherwise.
*/
static int ReceiveComPortBlockComplete( ISP_ENVIRONMENT *IspEnvironment,
                                        void *block, size_t size, unsigned timeout)
{
    unsigned long realsize = 0, read;
    char *result;

    result = block;

    SerialTimeoutSet( IspEnvironment, timeout);

    do
    {
        ReceiveComPortBlock( IspEnvironment, result + realsize, size - realsize, &read);

        realsize += read;

    } while((realsize < size) && (SerialTimeoutCheck(IspEnvironment) == 0));

    DumpString( 3, result, realsize, "Answer(Length=%ld): ", realsize);

    if( realsize != size)
    {
        return 1;
    }
    return 0;
}

/***************************** ReadArguments ****************************/
/**  Reads the command line arguments and parses it for the various
options. Uses the same arguments as main.  Used to separate the command
line parsing from main and improve its readability.  This should also make
it easier to modify the command line parsing in the future.
\param [in] argc the number of arguments.
\param [in] argv an array of pointers to the arguments.
*/
static void ReadArguments(ISP_ENVIRONMENT *IspEnvironment, int argc, char *argv[])
{
    int i;

    if(argc >= 5)
    {
        for(i = 1; i < argc - 4; i++)
        {
            if(stricmp(argv[i], "-bin") == 0)
            {
                 IspEnvironment->FormatHex = 0;
                 DebugPrintf( 3, "Binary format file input.\n");
            }
            else if(stricmp(argv[i], "-hex") == 0)
            {
                 IspEnvironment->FormatHex = 1;
                 DebugPrintf( 3, "Hex format file input.\n");
            }
            else if(stricmp(argv[i], "-term") == 0)
            {
                 IspEnvironment->TerminalAfterUpload = 1;
                 DebugPrintf( 3, "Invoke terminal after upload.\n");
            }
            else if(stricmp(argv[i], "-termonly") == 0)
            {
                 IspEnvironment->TerminalOnly = 1;
                 DebugPrintf( 3, "Only provide terminal.\n");
            }
            else if(stricmp(argv[i], "-debug") == 0)
            {
                 debug_level = 4;
                 DebugPrintf( 3, "Turn on debug.\n");
            }
            else if(stricmp(argv[i], "-control") == 0)
            {
                 IspEnvironment->ControlLines = 1;
                 DebugPrintf( 3, "Use RTS/DTS to control target state.\n");
            }
            else if(stricmp(argv[i], "-logfile") == 0)
            {
                 IspEnvironment->LogFile = 1;
                 DebugPrintf( 3, "Log terminal output.\n");
            }
            else if( stricmp( argv[i], "-ADARM") == 0)
            {
                IspEnvironment->micro = ANALOG_DEVICES_ARM;
                DebugPrintf( 2, "Target: Analog Devices.\n");
            }
            else if( stricmp( argv[i], "-PHILIPSARM") == 0)
            {
                IspEnvironment->micro = PHILIPS_ARM;
                DebugPrintf( 2, "Target: Philips.\n");
            }
        }

        IspEnvironment->input_file = argv[argc - 4];
        IspEnvironment->StringOscillator = argv[argc - 1];
        IspEnvironment->serial_port = argv[argc - 3];
        IspEnvironment->baud_rate = argv[argc - 2];
    }

    if(argc < 5)
    {
        debug_level = (debug_level < 2) ? 2 : debug_level;
    }

    if(argc < 5)
    {
        DebugPrintf( 2, "\n");
        DebugPrintf( 2, "Portable command line ISP for Philips LPC2000 family and \n");
        DebugPrintf( 2, "Version 1.22                  Analog Devices ADUC 70xx\n");
        DebugPrintf( 2, "Compiled for %s: %s %s\n", COMPILED_FOR, __DATE__, __TIME__);
        DebugPrintf( 2, "Copyright (c) by Martin Maurer, 2003, 2004  Email: Martin.Maurer@clibb.de\n");
        DebugPrintf( 2, "Portions Copyright (c) by Aeolus Development 2004\n");
        DebugPrintf( 2, "                 http://www.aeolusdevelopment.com\n");
        DebugPrintf( 2, "\n");

        DebugPrintf( 1, "Syntax:  lpc21isp [Options] file comport baudrate Oscillator_in_kHz\n\n");
        DebugPrintf( 1, "Example: lpc21isp test.hex com1 115200 14746\n\n");
        DebugPrintf( 1, "Options: -bin           for uploading binary file\n");
        DebugPrintf( 1, "         -hex           for uploading file in intel hex format (default)\n");
        DebugPrintf( 1, "         -term          for starting terminal after upload\n");
        DebugPrintf( 1, "         -termonly      for starting terminal without an upload\n");
        DebugPrintf( 1, "         -debug         for creating a lot of debug infos\n");
        DebugPrintf( 1, "         -control       for controlling RS232 lines for easier booting\n");
        DebugPrintf( 1, "                        (Reset = DTR, EnableBootLoader = RTS)\n");
        DebugPrintf( 1, "         -logfile       for enabling logging of terminal output to lpc21isp.log\n");
        DebugPrintf( 1, "         -ADARM         for downloading to an Analog Devices\n");
        DebugPrintf( 1, "                        ARM microcontroller ADUC70xx\n");
        DebugPrintf( 1, "         -PHILIPSARM    for downloading to a microcontroller from\n");
        DebugPrintf( 1, "                        Philips LPC2000 family (default)\n");

        exit(1);
    }

    if(IspEnvironment->micro == PHILIPS_ARM)
    {
        if (strlen(IspEnvironment->StringOscillator) > 5)
        {
            printf("Invalid crystal frequency %s",argv[argc - 1]);
            exit(1);
        }
    }
}

typedef enum
{
    PROGRAM_MODE,
    RUN_MODE
} TARGET_MODE;

/***************************** ResetTarget ******************************/
/**  Resets the target leaving it in either download (program) mode or
run mode.
\param [in] mode the mode to leave the target in.
*/
static void ResetTarget( ISP_ENVIRONMENT *IspEnvironment, TARGET_MODE mode)
{

    if(IspEnvironment->ControlLines)
    {

        switch( mode)
        {
                /* Reset and jump to boot loader.                       */
            case PROGRAM_MODE:
                ControlModemLines(IspEnvironment, 1, 1);
                Sleep(100);
                ClearSerialPortBuffers(IspEnvironment);
                Sleep(100);
                ControlModemLines(IspEnvironment, 0, 1);
                Sleep(100);
                break;

                /* Reset and start uploaded program                     */
            case RUN_MODE:
                ControlModemLines(IspEnvironment, 1, 0);
                Sleep(100);
                ClearSerialPortBuffers(IspEnvironment);
                Sleep(100);
                ControlModemLines(IspEnvironment, 0, 0);
                Sleep(100);
                break;
        }
    }
}

/***************************** LoadFile *********************************/
/**  Loads the requested file to download into memory.
\param [in] structure containing input filename
*/
static void LoadFile(ISP_ENVIRONMENT *IspEnvironment)
{
    int fd;
    int i;
    unsigned long  Pos;
    unsigned long  FileLength;
    BINARY        *FileContent;              /**< Used to store the content of a hex */
                                             /* file before converting to binary.    */
    unsigned long BinaryMemSize;

    fd = open(IspEnvironment->input_file, O_RDONLY | O_BINARY);
    if(fd == -1)
    {
        DebugPrintf( 1, "Can't open file %s\n", IspEnvironment->input_file);
        exit(1);
    }

    FileLength = lseek(fd, 0L, 2);      // Get file size

    if(FileLength == -1)
    {
        DebugPrintf( 1, "\nFileLength = -1 !?!\n");
        exit(1);
    }

    lseek(fd, 0L, 0);

    FileContent = malloc(FileLength);

    BinaryMemSize = FileLength * 2;

    IspEnvironment->BinaryLength = 0;	/* Increase length as needed.		*/

    IspEnvironment->BinaryContent = malloc(BinaryMemSize);

    read(fd, FileContent, FileLength);

    close(fd);

    DebugPrintf( 2, "File %s loaded...\n", IspEnvironment->input_file);

        // Intel-Hex -> Binary Conversion

    if(IspEnvironment->FormatHex != 0)
    {
        unsigned char  RecordLength;
        unsigned short RecordAddress;
        unsigned long  RealAddress = 0;
        unsigned char  RecordType;
        unsigned char  Hexvalue;

        DebugPrintf( 2, "Converting file %s to binary format...\n", IspEnvironment->input_file);

        Pos = 0;
        while(Pos < FileLength)
        {
            if(FileContent[Pos] == '\r')
            {
                Pos++;
                continue;
            }

            if(FileContent[Pos] == '\n')
            {
                Pos++;
                continue;
            }

            if(FileContent[Pos] != ':')
            {
                DebugPrintf( 1, "Missing start of record (':') wrong byte %c / %02X\n", FileContent[Pos], FileContent[Pos]);
                exit(1);
            }

            Pos++;

            RecordLength   = Ascii2Hex(FileContent[Pos++]);
            RecordLength <<= 4;
            RecordLength  |= Ascii2Hex(FileContent[Pos++]);

            DebugPrintf( 4, "RecordLength = %02X\n", RecordLength);

            RecordAddress   = Ascii2Hex(FileContent[Pos++]);
            RecordAddress <<= 4;
            RecordAddress  |= Ascii2Hex(FileContent[Pos++]);
            RecordAddress <<= 4;
            RecordAddress  |= Ascii2Hex(FileContent[Pos++]);
            RecordAddress <<= 4;
            RecordAddress  |= Ascii2Hex(FileContent[Pos++]);

            DebugPrintf( 4, "RecordAddress = %04X\n", RecordAddress);

            RealAddress = RealAddress - (RealAddress & 0xffff) + RecordAddress;

            DebugPrintf( 4, "RealAddress = %08lX\n", RealAddress);

            RecordType      = Ascii2Hex(FileContent[Pos++]);
            RecordType    <<= 4;
            RecordType     |= Ascii2Hex(FileContent[Pos++]);

            DebugPrintf( 4, "RecordType = %02X\n", RecordType);

            if(RecordType == 0x00)          // 00 - Data record
            {
                    // Memory for binary file big enough ?
                while(RealAddress + RecordLength > BinaryMemSize)
                {
                    BinaryMemSize <<= 1;
                    IspEnvironment->BinaryContent = realloc(IspEnvironment->BinaryContent, BinaryMemSize);
                }

                    // We need to know, what the highest address is,
                    // how many bytes / sectors we must flash
                if(RealAddress + RecordLength > IspEnvironment->BinaryLength)
                {
                    IspEnvironment->BinaryLength = RealAddress + RecordLength;
                    DebugPrintf( 3, "Image size now: %ld\n", IspEnvironment->BinaryLength);
                }

                for(i = 0; i < RecordLength; i++)
                {
                    Hexvalue        = Ascii2Hex(FileContent[Pos++]);
                    Hexvalue      <<= 4;
                    Hexvalue       |= Ascii2Hex(FileContent[Pos++]);
                    IspEnvironment->BinaryContent[RealAddress + i] = Hexvalue;
                }
            }
            else if(RecordType == 0x01)     // 01 - End of file record
            {
                break;
            }
            else if(RecordType == 0x02)     // 02 - Extended segment address record
            {
                for(i = 0; i < RecordLength * 2; i++)   // double amount of nibbles
                {
                    RealAddress <<= 4;
                    if(i == 0)
                    {
                        RealAddress  = Ascii2Hex(FileContent[Pos++]);
                    }
                    else
                    {
                        RealAddress |= Ascii2Hex(FileContent[Pos++]);
                    }
                }
                RealAddress <<= 4;
            }
            else if(RecordType == 0x03)     // 03 - Start segment address record
            {
                for(i = 0; i < RecordLength * 2; i++)   // double amount of nibbles
                {
                    RealAddress <<= 4;
                    if(i == 0)
                    {
                        RealAddress  = Ascii2Hex(FileContent[Pos++]);
                    }
                    else
                    {
                        RealAddress |= Ascii2Hex(FileContent[Pos++]);
                    }
                }
                RealAddress <<= 8;
            }
            else if(RecordType == 0x04)     // 04 - Extended linear address record, used by IAR
            {
                DebugPrintf( 1, "RecordType %02X not yet implemented - ignore ?\n", RecordType);
                // exit(1);
            }
            else if(RecordType == 0x05)     // 05 - Start linear address record
            {
                DebugPrintf( 1, "RecordType %02X not yet implemented\n", RecordType);
                exit(1);
            }

            while(FileContent[Pos++] != 0x0a)      // Search till line end
            {
            }
        }

        DebugPrintf( 2, "File %s converted to binary format...\n", IspEnvironment->input_file);

            // When debugging is switched on, output result of conversion to file debugout.bin
        if(debug_level >= 4)
        {
            int fdout;
            fdout = open("debugout.bin", O_RDWR | O_BINARY | O_CREAT | O_TRUNC, 0777);
            write(fdout, IspEnvironment->BinaryContent, IspEnvironment->BinaryLength);
            close(fdout);
        }
    }
    else
    {
        memcpy(IspEnvironment->BinaryContent, FileContent, FileLength);

        IspEnvironment->BinaryLength = FileLength;
    }

    DebugPrintf( 2, "Image size : %ld\n", IspEnvironment->BinaryLength);

        // check length to flash for correct alignment, can happen with broken ld-scripts
    if (IspEnvironment->BinaryLength % 4 != 0)
    {
        unsigned long NewBinaryLength = ((IspEnvironment->BinaryLength + 3)/4) * 4;

        DebugPrintf( 2, "Warning:  data not aligned to 32 bits, padded (length was %lX, now %lX)\n", IspEnvironment->BinaryLength, NewBinaryLength);

        IspEnvironment->BinaryLength = NewBinaryLength;
    }

}


#define ANALOG_DEVICES_SYNC_CHAR        ((BINARY)0x08)
#define ANALOG_DEVICES_SYNC_RESPONSE    ("ADuC")
#define ANALOG_DEVICES_SYNC_SIZE        (strlen( ANALOG_DEVICES_SYNC_RESPONSE))

typedef struct {
    BINARY product_id[15];
    BINARY version[3];
    BINARY reserved[4];
    BINARY terminator[2];
    } AD_SYNC_RESPONSE;

/***************************** AnalogDevicesSync ************************/
/**  Attempt to synchronize with an Analog Device ARM micro.  Sends a
backspace and reads back the microcontrollers response.  Performs
multiple retries. Exits the program on error, returns to caller in the
case of success.
*/
static void AnalogDevicesSync(ISP_ENVIRONMENT *IspEnvironment)
{
    BINARY sync;                        /* Holds sync command.          */
    AD_SYNC_RESPONSE response;          /* Response from micro.         */
    int sync_attempts;                  /* Number of retries.           */

        /*  Make sure we don't read garbage later instead of the        */
        /* response we expect from the micro.                           */
    ClearSerialPortBuffers(IspEnvironment);

    DebugPrintf( 2, "Synchronizing\n"); /* Progress report.             */

    sync = ANALOG_DEVICES_SYNC_CHAR;    /* Build up sync command.       */

        /*  Perform the actual sync attempt.  First send the sync       */
        /* character, the attempt to read back the response.  For the   */
        /* AD ARM micro this is a fixed length block.  If response is   */
        /* received attempt to validate it by comparing the first       */
        /* characters to those expected.  If the received block does    */
        /* not validate or is incomplete empty the serial buffer and    */
        /* retry.                                                       */
    for(sync_attempts = 0; sync_attempts < 5; sync_attempts++)
    {
        SendComPortBlock( IspEnvironment, &sync, 1);

        if( ReceiveComPortBlockComplete( IspEnvironment, &response, sizeof( response),
            500) == 0)
        {

            if( memcmp( response.product_id, ANALOG_DEVICES_SYNC_RESPONSE,
                ANALOG_DEVICES_SYNC_SIZE) == 0)
            {
                return;
            }
            else
            {
                DumpString( 3, &response, sizeof(response),
                    "Unexpected response to sync attempt ");
            }
        }
        else
        {
            DebugPrintf( 3, "No (or incomplete) answer on sync attempt\n");
        }

        ClearSerialPortBuffers(IspEnvironment);
    }

    DebugPrintf( 1, "No (or unacceptable) answer on sync attempt\n");
    exit(4);
}

typedef struct {
    char start1;
    char start2;
    BINARY bytes;
    char cmd;
    BINARY address_h;
    BINARY address_u;
    BINARY address_m;
    BINARY address_l;
    BINARY data[251];
    } AD_PACKET;

/***************************** AnalogDevicesFormPacket ******************/
/**  Create an Analog Devices communication packet from the constituent
elements.
\param [in] cmd The command being sent, one of 'E' for erase, 'W' for
write, 'V' for verify or 'R' for run..
\param [in] no_bytes the number of data bytes to send with the command in
the packet.
\param [in] address the address to apply the command to.
\param [in] data the data to send with the packet, may be null if no_bytes
is zero.
\param[out] packet that will be filled.
*/
static void AnalogDevicesFormPacket( ISP_ENVIRONMENT *IspEnvironment,
                                     char cmd, int no_bytes, unsigned int address,
                                     const void *data, AD_PACKET *packet)
{
    BINARY checksum;
    const BINARY *data_in;
    int i;

        /*  Some sanity checking on the arguments.  These should only   */
        /* fail if there is a bug in the caller.                        */
        /*  Check 1) that the number of data bytes is in an acceptable  */
        /* range, 2) that we have a non-null pointer if data is being   */
        /* put in the packet and 3) that we have a non-null pointer to  */
        /* the packet to be filled. We just exit with an error message  */
        /* if any of these tests fail.                                  */
    if( (no_bytes < 0) || (no_bytes > 250))
    {
        DebugPrintf( 1,
            "The number of bytes (%d) passed to FormPacket is invalid.\n",
            no_bytes);
        exit( -1);
    }
    if( (data == 0) && (no_bytes != 0))
    {
        DebugPrintf( 1,
            "A null pointer to data paased to FormPacket when data was expected.\n");
        exit( -1);
    }
    if( packet == 0)
    {
        DebugPrintf( 1,
            "A null packet pointer was passed to FormPacket.\n");
        exit( -1);
    }

    checksum = 0;               /*  Checksum starts at zero.            */

    data_in = data;             /*  Pointer pun so we can walk through  */
                                /* the data.                            */

    packet->start1 = 0x7;       /*  The start of the packet is constant.*/
    packet->start2 = 0xE;

        /*  Fill in the rest of the packet and calculate the checksum   */
        /* as we go.                                                    */

        /* The number of bytes is the number of data bytes + the        */
        /* address bytes + the command byte.                            */
    packet->bytes = no_bytes + 5;

    checksum += packet->bytes;

        /*  The command for the packet being sent.  No error checking   */
        /* done on this.                                                */
    packet->cmd = cmd;

    checksum += cmd;

        /*  Now break up the address and place in the proper packet     */
        /* locations.                                                   */
    packet->address_l = address & 0xFF;
    packet->address_m = (address >> 8) & 0xFF;
    packet->address_u = (address >> 16) & 0xFF;
    packet->address_h = (address >> 24) & 0xFF;

    checksum += packet->address_l;
    checksum += packet->address_m;
    checksum += packet->address_u;
    checksum += packet->address_h;

        /*  Copy the data bytes into the packet.  We could use memcpy   */
        /* but we have to calculate the checksum anyway.                */
    for( i = 0; i < no_bytes; i++)
    {
        packet->data[i] = data_in[i];
        checksum += data_in[i];
    }

        /*  Finally, add the checksum to the end of the packet.         */
    packet->data[i] = -checksum;
}

#define ANALOG_DEVICES_ACK      0x6
#define ANALOG_DEVICES_NAK      0x7

/***************************** AnalogDevicesSendPacket ******************/
/**  Send a previously form Analog Devices communication.  Retry a
couple of times if needed but fail by exiting the program if no ACK is
forthcoming.
\param [in] packet the packet to send.
*/
static void AnalogDevicesSendPacket( ISP_ENVIRONMENT *IspEnvironment,
                                     const AD_PACKET * packet)
{
    BINARY response;
    int try = 0;

    do {
        try++;

                /*  Make sure we don't read garbage later instead of    */
                /* the response we expect from the micro.               */
        ClearSerialPortBuffers(IspEnvironment);

                /*  Send the packet, the size is the number of data     */
                /* bytes in the packet plus 3 bytes worth of header     */
                /* plus checksum.                                       */
        SendComPortBlock( IspEnvironment, packet, packet->bytes + 4);

                /*  Receive the response and check, return to caller    */
                /* if successful.                                       */
        if( ReceiveComPortBlockComplete( IspEnvironment, &response, 1, 500) == 0)
        {
            if( response == ANALOG_DEVICES_ACK)
            {
                DebugPrintf( 3, "Packet Sent\n");
                return;
            }
            if( response != ANALOG_DEVICES_NAK)
            {
                DebugPrintf( 3, "Unexpected response to packet (%x)\n", (int)response);
            }
            DebugPrintf( 2, "*");
        }
    } while( try < 3);

    DebugPrintf( 1, "Send packet failed\n");
    exit( -1);
}

/***************************** AnalogDevicesErase ***********************/
/**  Erase the Analog Devices micro.  We take the simple way out and
just erase the whole thing.
*/
static void AnalogDevicesErase(ISP_ENVIRONMENT *IspEnvironment)
{
    BINARY pages;
    AD_PACKET packet;

    pages = 0;
    DebugPrintf( 2, "Erasing .. ");
    AnalogDevicesFormPacket( IspEnvironment, 'E', 1, 0, &pages, &packet);
    AnalogDevicesSendPacket( IspEnvironment, &packet);
    DebugPrintf( 2, "Erased\n");
}

#define AD_PACKET_SIZE (250)

/***************************** AnalogDevicesWrite ***********************/
/**  Write the program.
\param [in] data the program to download to the micro.
\param [in] address where to start placing the program.
\param [in] bytes the size of the progrm to download.
*/
static void AnalogDevicesWrite( ISP_ENVIRONMENT *IspEnvironment,
                                const void *data, long address, size_t bytes)
{
    AD_PACKET packet;
    const BINARY *prog_data;

    DebugPrintf( 2, "Writing %d bytes ", bytes);
    prog_data = data;
    while( bytes > AD_PACKET_SIZE)
    {
        AnalogDevicesFormPacket( IspEnvironment, 'W', AD_PACKET_SIZE, address, prog_data, &packet);
        AnalogDevicesSendPacket( IspEnvironment, &packet);
        address += AD_PACKET_SIZE;
        prog_data += AD_PACKET_SIZE;
        bytes -= AD_PACKET_SIZE;
        DebugPrintf( 2, ".");
    }
    if( bytes > 0)
    {
        AnalogDevicesFormPacket( IspEnvironment, 'W', bytes, address, prog_data, &packet);
        AnalogDevicesSendPacket( IspEnvironment, &packet);
        DebugPrintf( 2, ".");
    }
}

/***************************** AnalogDevicesDownload ********************/
/**  Perform the download into an Analog Devices micro.  As a quick and
 * dirty hack against flash relocations at 0x80000
 * \return 0 if ok, error code else
 * \ToDo: possible to implement the return value instead of calling
 * exit() in sub-functions
 */
static int AnalogDevicesDownload(ISP_ENVIRONMENT *IspEnvironment)
{
    AnalogDevicesSync(IspEnvironment);
    AnalogDevicesErase(IspEnvironment);
    if( IspEnvironment->BinaryLength > 0x80000)
    {
        DebugPrintf( 2, "Note:  Flash remapped 0x80000 to 0.\n");
        AnalogDevicesWrite( IspEnvironment, IspEnvironment->BinaryContent + 0x80000, 0, IspEnvironment->BinaryLength-0x80000);
    }
    else
    {
        AnalogDevicesWrite( IspEnvironment, IspEnvironment->BinaryContent, 0, IspEnvironment->BinaryLength);
    }
    return(0);
}

/* note -- need to rationalize timeouts, use define?
similarly for the number of sync attempts
*/

#endif // !defined COMPILE_FOR_LPC21



/***************************** Download *********************************/
/**  Download the file from the internal memory image to the philips microcontroller.
 *   This function is visible from outside if COMPILE_FOR_LPC21
 */
#if !defined COMPILE_FOR_LPC21
static
#endif
int PhilipsDownload(ISP_ENVIRONMENT *IspEnvironment)
{
    unsigned long realsize;
    char Answer[128];
    char temp[128];
    char *strippedAnswer;
    int  strippedsize;
    int nQuestionMarks = 0;
    int found = 0;
    unsigned long Sector;
    unsigned long SectorLength;
    char tmpString[128];
    char uuencode_table[64];
    int Line;
    unsigned long tmpStringPos;
    unsigned long BlockOffset;
    unsigned long Block;
    unsigned long Pos;
    unsigned long CopyLength;
    int c,k=0,i;
    unsigned long ivt_CRC;          // CRC over interrupt vector table
    unsigned long block_CRC;

        // Build up uuencode table
    uuencode_table[0] = 0x60;           // 0x20 is translated to 0x60 !

    for(i = 1; i <= 64; i++)
    {
        uuencode_table[i] = (char)(0x20 + i);
    }

        // Patch 0x14, otherwise it is not running and jumps to boot mode

    ivt_CRC = 0;

        /* Clear the vector at 0x14 so it doesn't affect the checksum: */
    for(i = 0; i < 4; i++)
    {
        IspEnvironment->BinaryContent[i + 0x14] = 0;
    }
        /* Calculate a native checksum of the little endian vector table: */
    for(i = 0; i < (4 * 8);) {
        ivt_CRC += IspEnvironment->BinaryContent[i++];
        ivt_CRC += IspEnvironment->BinaryContent[i++] << 8;
        ivt_CRC += IspEnvironment->BinaryContent[i++] << 16;
        ivt_CRC += IspEnvironment->BinaryContent[i++] << 24;
    }

        /* Negate the result and place in the vector at 0x14 as little endian
         * again. The resulting vector table should checksum to 0. */
    ivt_CRC = -ivt_CRC;
    for(i = 0; i < 4; i++)
    {
        IspEnvironment->BinaryContent[i + 0x14] = (unsigned char)(ivt_CRC >> (8 * i));
    }

    DebugPrintf( 3, "Position 0x14 patched: ivt_CRC = 0x%08lX\n", ivt_CRC);


    DebugPrintf( 2, "Synchronizing\n");

    for(nQuestionMarks = found = 0; !found && nQuestionMarks < 100; nQuestionMarks++)
    {
        SendComPort( IspEnvironment, "?");

        memset(Answer,0,sizeof(Answer));
        ReceiveComPort( IspEnvironment, Answer, sizeof(Answer)-1, &realsize, 1,100);

        strippedAnswer = Answer;
        strippedsize = realsize;
        while((strippedsize > 0) && ((*strippedAnswer == '?') || (*strippedAnswer == 0)))
        {
            strippedAnswer++;
            strippedsize--;
        }

        DumpString( 3, strippedAnswer, strippedsize, "StrippedAnswer(Length=%ld): '", strippedsize);

        if(strcmp(strippedAnswer, "Synchronized\r\n") == 0)
        {
            found = 1;
        }
#if !defined COMPILE_FOR_LPC21
        else
        {
            ResetTarget(IspEnvironment, PROGRAM_MODE );
        }
#endif
    }

    if(!found)
    {
        DebugPrintf( 1, "No answer on '?'\n");
        return(4);
    }

    DebugPrintf( 3, "Synchronized 0\n");

    SendComPort( IspEnvironment, "Synchronized\r\n");

    ReceiveComPort( IspEnvironment, Answer, sizeof(Answer)-1, &realsize, 2,1000);

    if(strcmp(Answer, "Synchronized\r\nOK\r\n") != 0)
    {
        printf("No answer on 'Synchronized'\n");
        return(4);
    }

    DebugPrintf( 3, "Synchronized 1\n");

    DebugPrintf( 2, "Setting oscillator\n");

    sprintf( temp, "%s\r\n", IspEnvironment->StringOscillator);
    SendComPort( IspEnvironment, temp);

    ReceiveComPort( IspEnvironment, Answer, sizeof(Answer)-1, &realsize, 2,1000);

    sprintf( temp, "%s\r\nOK\r\n", IspEnvironment->StringOscillator);

    if(strcmp(Answer, temp) != 0)
    {
        DebugPrintf( 1, "No answer on Oscillator-Command\n");
        return(4);
    }

    DebugPrintf( 2, "Unlock\n");

    SendComPort( IspEnvironment, "U 23130\r\n");

    ReceiveComPort( IspEnvironment, Answer, sizeof(Answer)-1, &realsize, 2,5000);

    if(strcmp(Answer, "U 23130\r\n0\r\n") != 0)
    {
        DebugPrintf( 1, "Unlock-Command:\n");
        PhilipsOutputErrorMessage(GetErrorNumber(Answer));
        return(4);
    }

    DebugPrintf( 2, "Read bootcode version\n");

    SendComPort( IspEnvironment, "K\r\n");

    ReceiveComPort( IspEnvironment, Answer, sizeof(Answer)-1, &realsize, 4,5000);

    if(strncmp(Answer, "K\r\n", 3) != 0)
    {
        DebugPrintf( 1, "No answer on Read Boot Code Version\n");
        return(4);
    }

    DebugPrintf( 3, "\"%s\"\n",Answer);

    DebugPrintf( 2, "Read part ID\n");
    SendComPort( IspEnvironment, "J\r\n");

    ReceiveComPort( IspEnvironment, Answer, sizeof(Answer)-1, &realsize, 3,5000);

    if(strncmp(Answer, "J\r\n", 3) != 0)
    {
        DebugPrintf( 1, "No answer on Read Part Id\n");
        return(4);
    }

    DebugPrintf( 3, "\"%s\"\n",Answer);

    for(Sector = 0;;Sector++)
    {
        DebugPrintf( 3, "Writing Sector %ld: ", Sector);
        fflush(stdout);

        sprintf(tmpString, "P %ld %ld\r\n", Sector, Sector);
        SendComPort( IspEnvironment, tmpString);

        ReceiveComPort( IspEnvironment, Answer, sizeof(Answer)-1, &realsize, 2,5000);

        sprintf(tmpString, "P %ld %ld\r\n0\r\n", Sector, Sector);
        if(strcmp(Answer, tmpString) != 0)
        {
            DebugPrintf( 1, "Wrong answer on Prepare-Command (1) (Sector %ld)\n", Sector);
            PhilipsOutputErrorMessage(GetErrorNumber(Answer));
            return(4);
        }

        DebugPrintf( 2, ".");
        fflush(stdout);

        sprintf(tmpString, "E %ld %ld\r\n", Sector, Sector);
        SendComPort( IspEnvironment, tmpString);

        ReceiveComPort( IspEnvironment, Answer, sizeof(Answer)-1, &realsize, 2,5000);

        sprintf(tmpString, "E %ld %ld\r\n0\r\n", Sector, Sector);
        if(strcmp(Answer, tmpString) != 0)
        {
            DebugPrintf( 1, "Wrong answer on Erase-Command (Sector %ld)\n", Sector);
            PhilipsOutputErrorMessage(GetErrorNumber(Answer));
            return(4);
        }

        DebugPrintf( 2, ".");
        fflush(stdout);

        SectorLength = IspEnvironment->BinaryLength - Sector * 0x2000;
            // printf("\n\nDebug: BinaryLength=%ld, Sector=%ld, SectorLength=%ld\n\n", IspEnvironment->BinaryLength, Sector, SectorLength);

            // We can only write one sector, then we must prepare and erase a new sector
        if(SectorLength > 0x2000)
        {
            SectorLength = 0x2000;
        }

            // Write multiple of 45 * 4 Byte blocks to RAM, but copy maximum of on sector to Flash
            // In worst case we transfer up to 180 byte to much to RAM
            // but then we can always use full 45 byte blocks and length is multiple of 4
        if((SectorLength % (45 * 4)) != 0)
        {
            SectorLength += ((45 * 4) - (SectorLength % (45 * 4)));
        }
        sprintf(tmpString, "W %ld %ld\r\n", 0x40000200L, SectorLength);
        SendComPort( IspEnvironment, tmpString);

        ReceiveComPort( IspEnvironment, Answer, sizeof(Answer)-1, &realsize, 2,5000);

        sprintf(tmpString, "W %ld %ld\r\n0\r\n", 0x40000200L, SectorLength);
        if(strcmp(Answer, tmpString) != 0)
        {
            DebugPrintf( 1, "Wrong answer on Write-Command\n");
            PhilipsOutputErrorMessage(GetErrorNumber(Answer));
            return(4);
        }

        DebugPrintf( 2, ".");
        fflush(stdout);

        block_CRC = 0;
        Line = 0;

            // Transfer blocks of 45 * 4 bytes to RAM
        for(Pos = Sector * 0x2000; (Pos < (Sector + 1) * 0x2000) && (Pos < IspEnvironment->BinaryLength); Pos += (45 * 4))
        {
            for(Block = 0; Block < 4; Block++)  // Each block 45 bytes
            {
                DebugPrintf( 2, ".");
                fflush(stdout);

                    // Uuencode one 45 byte block
                tmpStringPos = 0;

                tmpString[tmpStringPos++] = (char)(' ' + 45);  // Encode Length of block

                for(BlockOffset = 0; BlockOffset < 45; BlockOffset++)
                {
                    c = IspEnvironment->BinaryContent[Pos + Block * 45 + BlockOffset];

                    block_CRC += c;

                    k = (k << 8) + (c & 255);

                    if((BlockOffset % 3) == 2)   // Collecting always 3 Bytes, then do processing in 4 Bytes
                    {
                        tmpString[tmpStringPos++] = uuencode_table[(k >> 18) & 63];
                        tmpString[tmpStringPos++] = uuencode_table[(k >> 12) & 63];
                        tmpString[tmpStringPos++] = uuencode_table[(k >>  6) & 63];
                        tmpString[tmpStringPos++] = uuencode_table[ k        & 63];
                    }
                }

                tmpString[tmpStringPos++] = '\r';
                tmpString[tmpStringPos++] = '\n';
                tmpString[tmpStringPos++] = 0;

                SendComPort( IspEnvironment, tmpString);

                ReceiveComPort( IspEnvironment, Answer, sizeof(Answer)-1, &realsize, 1,5000);

                if(strncmp(Answer, tmpString, tmpStringPos) != 0)
                {
                    DebugPrintf( 1, "Error on writing data (1)\n");
                    return(4);
                }

                Line++;

                DebugPrintf( 3, "Line = %d\n", Line);

                if(Line == 20)
                {
                        // printf("block_CRC = %ld\n", block_CRC);

                    sprintf(tmpString, "%ld\r\n", block_CRC);
                    SendComPort( IspEnvironment, tmpString);

                    ReceiveComPort( IspEnvironment, Answer, sizeof(Answer)-1, &realsize, 2,5000);

                    sprintf(tmpString, "%ld\r\nOK\r\n", block_CRC);
                    if(strcmp(Answer, tmpString) != 0)
                    {
                        DebugPrintf( 1, "Error on writing block_CRC (1)\n");
                        return(4);
                    }

                    Line = 0;
                    block_CRC = 0;
                }
            }
        }

        if(Line != 0)
        {
            sprintf(tmpString, "%ld\r\n", block_CRC);
            SendComPort( IspEnvironment, tmpString);

            ReceiveComPort( IspEnvironment, Answer, sizeof(Answer)-1, &realsize, 2,5000);

            sprintf(tmpString, "%ld\r\nOK\r\n", block_CRC);
            if(strcmp(Answer, tmpString) != 0)
            {
                DebugPrintf( 1, "Error on writing block_CRC (2)\n");
                return(4);
            }
        }

        sprintf(tmpString, "P %ld %ld\r\n", Sector, Sector);
        SendComPort( IspEnvironment, tmpString);

        ReceiveComPort( IspEnvironment, Answer, sizeof(Answer)-1, &realsize, 2,5000);

        sprintf(tmpString, "P %ld %ld\r\n0\r\n", Sector, Sector);
        if(strcmp(Answer, tmpString) != 0)
        {
            DebugPrintf( 1, "Wrong answer on Prepare-Command (2) (Sector %ld)\n", Sector);
            PhilipsOutputErrorMessage(GetErrorNumber(Answer));
            return(4);
        }

            // Calculate CopyLength, which is one of the following values: 512, 1024, 4096, 8192

        if(SectorLength <=  512)
        {
            CopyLength =  512;
        }
        else if(SectorLength <= 1024)
        {
            CopyLength = 1024;
        }
        else if(SectorLength <= 4096)
        {
            CopyLength = 4096;
        }
        else
        {
            CopyLength = 8192;
        }

        sprintf(tmpString, "C %ld %ld %ld\r\n", Sector * 0x2000, 0x40000200L, CopyLength);
        SendComPort( IspEnvironment, tmpString);

        ReceiveComPort( IspEnvironment, Answer, sizeof(Answer)-1, &realsize, 2,5000);

        sprintf(tmpString, "C %ld %ld %ld\r\n0\r\n", Sector * 0x2000, 0x40000200L, CopyLength);
        if(strcmp(Answer, tmpString) != 0)
        {
            DebugPrintf( 1, "Wrong answer on Write-Command\n");
            PhilipsOutputErrorMessage(GetErrorNumber(Answer));
            return(4);
        }

        DebugPrintf( 2, "\nSector %ld written\n", Sector);
        fflush(stdout);

        if(((Sector + 1) * 0x2000) >= IspEnvironment->BinaryLength)
        {
            break;
        }
    }

    DebugPrintf( 2, "Download Finished... now launching the brand new code\n" );
    fflush(stdout);

    SendComPort(IspEnvironment, "G 0 A\r\n"); //goto 0 : run this fresh new downloaded code code
    ReceiveComPort(IspEnvironment, Answer, sizeof(Answer)-1, &realsize, 2,5000);
    if(strcmp((const char *)Answer, "G 0 A\r\n0\r\n") != 0)
    {
        printf("Failed to run the new downloaded code\n");
        PhilipsOutputErrorMessage(GetErrorNumber(Answer));
        return(4);
    }

    fflush(stdout);
    return(0);
}


#if !defined COMPILE_FOR_LPC21
/***************************** Terminal *********************************/
/**  Acts as a simple dumb terminal. Press 'ESC' to exit.
*/
static void Terminal( ISP_ENVIRONMENT *IspEnvironment)
{
    int           ch = 0;
    char buffer[128];
    int           fdlogfile;
    unsigned long realsize;

        // When logging is switched on, output terminal output to lpc21isp.log
    if(IspEnvironment->LogFile)
    {
        fdlogfile = open("lpc21isp.log", O_RDWR | O_BINARY | O_CREAT | O_TRUNC, 0777);
    }

    printf("Terminal started (press Escape to abort)\n\n");
    fflush(stdout);

    do
    {
            // Check for received characters

        ReceiveComPort( IspEnvironment, buffer, sizeof(buffer) - 1, &realsize, 0,200);

        if( realsize )
        {
            write(1, buffer, realsize);
            fflush(stdout);
            if(IspEnvironment->LogFile)     // When logging is turned on, then copy output to logfile
            {
                write(fdlogfile, buffer, realsize);
            }
        }

            // check for keypress, and write any out the port.
        if ( kbhit() )
        {
            ch = getch();
            if(ch == 0x1b)
            {
                break;
            }

            buffer[0] = (unsigned char)ch;
            buffer[1] = 0;

            SendComPort( IspEnvironment, buffer);
        }
    } while(ch != 0x1b);

    printf("\n\nTerminal stopped\n\n");
    fflush(stdout);

    if(IspEnvironment->LogFile)
    {
        close(fdlogfile);
    }

}

#endif // !defined COMPILE_FOR_LPC21

/***************************** main *************************************/
/**  main. Everything starts from here.
\param [in] argc the number of arguments.
\param [in] argv an array of pointers to the arguments.
*/

#if !defined COMPILE_FOR_LPC21

int main(int argc, char *argv[])
{
    ISP_ENVIRONMENT IspEnvironment;
    int downloadResult;

        // Initialize debug level
    debug_level = 2;

        // Initialize ISP Environment
    IspEnvironment.micro = PHILIPS_ARM;          /**< The type of micro that will be */
                                                 /* programmed.                      */
    IspEnvironment.TerminalAfterUpload = 0;
    IspEnvironment.TerminalOnly = 0;
    IspEnvironment.FormatHex = 1;
    IspEnvironment.ControlLines = 0;
    IspEnvironment.LogFile = 0;

        /* Read and parse the command line.                             */
    ReadArguments( &IspEnvironment, argc, argv);

        /* Download requested, read in the input file.                  */
    if(!IspEnvironment.TerminalOnly)
    {
        LoadFile(&IspEnvironment);
    }

    OpenSerialPort(&IspEnvironment);   /* Open the serial port to the microcontroller. */

    ResetTarget(&IspEnvironment, PROGRAM_MODE);

    ClearSerialPortBuffers(&IspEnvironment);


        /* Perform the requested download.                              */
    if(!IspEnvironment.TerminalOnly)
    {
        switch( IspEnvironment.micro)
        {
            case PHILIPS_ARM:
                downloadResult = PhilipsDownload(&IspEnvironment);
                break;

            case ANALOG_DEVICES_ARM:
                downloadResult = AnalogDevicesDownload(&IspEnvironment);
                break;
        }
        if( downloadResult != 0 )
        {
            CloseSerialPort(&IspEnvironment);
            exit(downloadResult);
        }
    }

    ResetTarget(&IspEnvironment, RUN_MODE);

    IspEnvironment.debug_level = 1;    /* From now on there is no more debug output !! */
                                       /* Therefore switch it off...                   */

        /* User asked for terminal emulation, provide a really dumb     */
        /* terminal.                                                    */
    if(IspEnvironment.TerminalAfterUpload || IspEnvironment.TerminalOnly)
    {
        Terminal(&IspEnvironment);
    }

    CloseSerialPort(&IspEnvironment);  /*  All done, close the serial port to the      */
    exit(0);                           /* microcontroller and exit.                    */
}

#endif // !defined COMPILE_FOR_LPC21

/***************************** DumpString ******************************/
/**  Prints an area of memory to stdout. Converts non-printables to hex.
\param [in] level the debug level of the block to be dumped.  If this is
less than or equal to the current debug level than the dump will happen
otherwise this just returns.
\param [in] b pointer to an area of memory.
\param [in] size the length of the memory block to print.
\param [in] fmt a printf style format string used to produce a prefix to
the block of dumped memory.
\param [in] ... the arguments referenced by the fmt string.
*/
static void DumpString(int level, const void *b, size_t size, const char *fmt, ...)
{
    size_t i;
    const char *s;
    va_list ap;

    if( level <= debug_level)
    {
        va_start( ap, fmt);
        vprintf( fmt, ap);
        va_end( ap);

        s = b;

        printf("'");
        for( i = 0; i < size; i++)
            {
            if(s[i] >= 0x20 && s[i] <= 0x7e) /*isprint?*/
            {
                putchar(s[i]);
            }
            else
            {
                printf("(%02X)", s[i]);
            }
        }
        printf("'\n");
    }

}


#if !defined COMPILE_FOR_LPC21
/***************************** Ascii2Hex ********************************/
/**  Converts a hex character to its equivalent number value. In case of an
error rather abruptly terminates the program.
\param [in] c the hex digit to convert.
\return the value of the hex digit.
*/
static unsigned int Ascii2Hex(char c)
{
    if(c >= '0' && c <= '9')
    {
        return(c - '0');
    }
    if(c >= 'A' && c <= 'F')
    {
        return(c - 'A' + 0x0au);
    }
    if(c >= 'a' && c <= 'f')
    {
        return(c - 'a' + 0x0au);
    }
    printf("Wrong Hex-Nibble %c (%02X)\n", c, c);
    exit(1);
}
#endif // !defined COMPILE_FOR_LPC21


/***************************** PhilipsOutputErrorMessage ***********************/
/**  Given an error number find and print the appropriate error message.
\param [in] ErrorNumber The number of the error.
*/
static void PhilipsOutputErrorMessage(unsigned char ErrorNumber)
{
//#if !defined COMPILE_FOR_LPC21
    switch(ErrorNumber)
    {
        case   0:
            DebugPrintf( 1, "CMD_SUCCESS");
            break;

        case   1:
            DebugPrintf( 1, "INVALID_COMMAND");
            break;

        case   2:
            DebugPrintf( 1, "SRC_ADDR_ERROR: Source address is not on word boundary.");
            break;
        case   3:
            DebugPrintf( 1, "DST_ADDR_ERROR: Destination address is not on a correct boundary.");
            break;

        case   4:
            DebugPrintf( 1, "SRC_ADDR_NOT_MAPPED: Source address is not mapped in the memory map.");
            DebugPrintf( 1, "                     Count value is taken in to consideration where applicable.");
            break;

        case   5:
            DebugPrintf( 1, "DST_ADDR_NOT_MAPPED: Destination address is not mapped in the memory map.");
            DebugPrintf( 1, "                     Count value is taken in to consideration where applicable.");
            break;

        case   6:
            DebugPrintf( 1, "COUNT_ERROR: Byte count is not multiple of 4 or is not a permitted value.");
            break;

        case   7:
            DebugPrintf( 1, "INVALID_SECTOR: Sector number is invalid or end sector number is");
            DebugPrintf( 1, "                greater than start sector number.");
            break;

        case   8:
            DebugPrintf( 1, "SECTOR_NOT_BLANK");
            break;

        case   9:
            DebugPrintf( 1, "SECTOR_NOT_PREPARED_FOR_WRITE_OPERATION:");
            DebugPrintf( 1, "Command to prepare sector for write operation was not executed.");
            break;

        case  10:
            DebugPrintf( 1, "COMPARE_ERROR: Source and destination data not equal.");
            break;

        case  11:
            DebugPrintf( 1, "BUSY: Flash programming hardware interface is busy.");
            break;

        case  12:
            DebugPrintf( 1, "PARAM_ERROR: Insufficient number of parameters or invalid parameter.");
            break;

        case  13:
            DebugPrintf( 1, "ADDR_ERROR: Address is not on word boundary.");
            break;

        case  14:
            DebugPrintf( 1, "ADDR_NOT_MAPPED: Address is not mapped in the memory map. ");
            DebugPrintf( 1, "                 Count value is taken in to consideration where applicable.");
            break;

        case  15:
            DebugPrintf( 1, "CMD_LOCKED");
            break;

        case  16:
            DebugPrintf( 1, "INVALID_CODE: Unlock code is invalid.");
            break;

        case  17:
            DebugPrintf( 1, "INVALID_BAUD_RATE: Invalid baud rate setting.");
            break;

        case  18:
            DebugPrintf( 1, "INVALID_STOP_BIT: Invalid stop bit setting.");
            break;

        case 255:                                                                                            break;
        default:
            DebugPrintf( 1, "unknown error %u", ErrorNumber);
            break;
    }
//#else // !defined COMPILE_FOR_LPC21
//    printf("error %u", ErrorNumber);
//#endif // !defined COMPILE_FOR_LPC21
}


/***************************** GetErrorNumber ***************************/
/**  Find error number in string.  This will normally be the string
returned from the microcontroller.
\param [in] Answer the buffer to search for the error number.
\return the error number found, if no linefeed found before the end of the
string an error value of 255 is returned. If a non-numeric value is found
then it is printed to stdout and an error value of 255 is returned.
*/
static unsigned char GetErrorNumber(const char *Answer)
{
    unsigned int i = 0;

    while(1)
    {
        if(Answer[i] == 0x00) return(255);
        if(Answer[i] == 0x0a) break;
        i++;
    }

    i++;

    if(Answer[i] < '0' || Answer[i] > '9')
    {
        DebugPrintf( 1, "ErrorString: %s", &Answer[i]);
        return(255);
    }

    return(atoi(&Answer[i]));
}


