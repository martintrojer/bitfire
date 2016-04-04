// Atmosfire libc syscalls

#include <time.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <errno.h>
#undef errno
extern int errno;
extern int *__errno _PARAMS ((void))
{
}

#include "71x_lib.h"
#include "rccu.h"

#define UART0_Rx_Pin (0x0001<<8)   /*  TQFP 64: pin N° 63 , TQFP 144 pin N° 143 */
#define UART0_Tx_Pin (0x0001<<9)   /*  TQFP 64: pin N° 64 , TQFP 144 pin N° 144 */
#define UART1_Rx_Pin (0x0001<<10)  /*  TQFP 64: pin N° 1  , TQFP 144 pin N° 1 */
#define UART1_Tx_Pin (0x0001<<11)  /*  TQFP 64: pin N° 2  , TQFP 144 pin N° 3 */
#define UART2_Rx_Pin (0x0001<<13)  /*  TQFP 64: pin N° 5  , TQFP 144 pin N° 9 */
#define UART2_Tx_Pin (0x0001<<14)  /*  TQFP 64: pin N° 6  , TQFP 144 pin N° 10 */
#define UART3_Rx_Pin (0x0001<<1)   /*  TQFP 64: pin N° 52 , TQFP 144 pin N° 123 */
#define UART3_Tx_Pin (0x0001<<0)   /*  TQFP 64: pin N° 53 , TQFP 144 pin N° 124 */

//#define Use_UART0
//#define Use_UART1
//#define Use_UART2
#define Use_UART3

#ifdef Use_UART0
#define  UARTX  UART0
#define  UARTX_Rx_Pin  UART0_Rx_Pin
#define  UARTX_Tx_Pin  UART0_Tx_Pin
#define  UARTX_Periph  UART0_Periph
#endif /* Use_UART0 */

#ifdef Use_UART1
#define  UARTX  UART1
#define  UARTX_Rx_Pin  UART1_Rx_Pin
#define  UARTX_Tx_Pin  UART1_Tx_Pin
#define  UARTX_Periph  UART1_Periph
#endif /* Use_UART1 */

#ifdef Use_UART2
#define  UARTX  UART2
#define  UARTX_Rx_Pin  UART2_Rx_Pin
#define  UARTX_Tx_Pin  UART2_Tx_Pin
#define  UARTX_Periph  UART2_Periph
#endif /* Use_UART2 */

#ifdef Use_UART3
#define  UARTX  UART3
#define  UARTX_Rx_Pin  UART3_Rx_Pin
#define  UARTX_Tx_Pin  UART3_Tx_Pin
#define  UARTX_Periph  UART3_Periph
#endif /* Use_UART3 */

void init(void)
{
  #ifdef DEBUG
  debug();
  #endif

  RCCU_Div2Config(ENABLE);
  RCCU_MCLKConfig(RCCU_DEFAULT);
  RCCU_FCLKConfig(RCCU_RCLK_2);
  RCCU_PCLKConfig(RCCU_RCLK_2);
    
  RCCU_PLL1Config(RCCU_PLL1_Mul_16,RCCU_Div_2);
  RCCU_RCLKSourceConfig(RCCU_PLL1_Output);
  
   /* GPIO peripheral configuration -------------------------------------------*/

  /*  Configure the GPIO pins */
  GPIO_Config(GPIO0, UARTX_Tx_Pin, GPIO_AF_PP);
  GPIO_Config(GPIO0, UARTX_Rx_Pin, GPIO_IN_TRI_CMOS);
  
  /* UART peripheral configuration -------------------------------------------*/

  /*  Configure the UART X */
  /*  Turn UARTX on */
  UART_OnOffConfig(UARTX, ENABLE); 
  /*  Disable FIFOs */      
  UART_FifoConfig (UARTX, DISABLE); 
  /*  Reset the UART_RxFIFO */     
  UART_FifoReset  (UARTX , UART_RxFIFO); 
  /*  Reset the UART_TxFIFO */
  UART_FifoReset  (UARTX , UART_TxFIFO); 
  /*  Disable Loop Back */
  UART_LoopBackConfig(UARTX , DISABLE);                                           
 /* Configure the UARTX as following:
                             - Baudrate = 9600 Bps
                             - No parity
                             - 8 data bits
                             - 1 stop bit */
  UART_Config(UARTX,9600,UART_NO_PARITY,UART_1_StopBits,UARTM_8D);
 /*  Enable Rx */
  UART_RxConfig(UARTX ,ENABLE);          
}

void set_led(int status)
{
  GPIO_Config(GPIO1, 1<<9, GPIO_OUT_PP);
  if (status)
    GPIO_BitWrite(GPIO1,9,1);
  else
    GPIO_BitWrite(GPIO1,9,0);
}

void _exit(int status) {
}

int _gettimeofday (struct timeval * tp, struct timezone * tzp)
{
  if (tzp) {
      tzp->tz_minuteswest = 0;
      tzp->tz_dsttime = 0;
    }

  return 0;
}

int _times(struct tms *buf) {
  return -1;
}

int _link(char *old, char *new){
  errno=EMLINK;
  return -1;
}

int _unlink(char *name){
  errno=ENOENT;
  return -1; 
}

int _open(const char *name, int flags, int mode){
    return -1;
}

int _kill(int pid, int sig){
  errno=EINVAL;
  return(-1);
}

int _getpid (int n){
  return 1;
}

int _lseek(int file, int ptr, int dir){
  return 0;
}

int _fstat(int file, struct stat *st) {
  st->st_mode = S_IFCHR;
  return 0;
}


register char * stack_ptr asm ("sp");

caddr_t
_sbrk (int incr)
{
  extern char   end asm ("end");	/* Defined by the linker.  */
  static char * heap_end;
  char *        prev_heap_end;

  if (heap_end == NULL)
    heap_end = & end;
  
  prev_heap_end = heap_end;
  
  if (heap_end + incr > stack_ptr)
    {
      /* Some of the libstdc++-v3 tests rely upon detecting
	 out of memory errors, so do not abort here.  */
#if 0
      extern void abort (void);

      _write (1, "_sbrk: Heap and stack collision\n", 32);
      
      abort ();
#else
      errno = ENOMEM;
      return (caddr_t) -1;
#endif
    }
  
  heap_end += incr;

  return (caddr_t) prev_heap_end;
}

int isatty(int file){
  return 1;
}

int _close(int file){
  return -1;
}

int _write(int file, char *ptr, int len){
  int todo;
  for (todo = 0; todo < len; todo++) {
    // Ugly C terminal hack
    if (ptr[todo]=='\n') {
      char nl = '\r';
      UART_ByteSend(UARTX, (u8 *)&nl);
      /*  wait until the data transmission is finished */
      while(!((UART_FlagStatus(UARTX)) & UART_TxEmpty)); 
    }
    
    UART_ByteSend(UARTX, (u8 *)&ptr[todo]);
    /*  wait until the data transmission is finished */
    while(!((UART_FlagStatus(UARTX)) & UART_TxEmpty)); 
    
  }
  return len;
}

int _read(int file, char *ptr, int len){
  u16 UARTStatus;
  int todo;

  for (todo = 0; todo < len-1; todo++) {
    /*  wait until data is received */       
    while(!(UART_FlagStatus(UARTX) & UART_RxBufFull));  
    /*  Get the received data, set the guard time to 0xFF */
    UARTStatus = UART_ByteReceive(UARTX, &ptr[todo], 0xFF);
	/* TODO: Echo back to remove local echo requirement */
    if(ptr[todo]=='\r') {
      ptr[++todo]='\n';
      break;
    }
  }
  ptr[++todo] = 0;
  return todo;
}
