// UART polled
// mtrojer@arrownordic.com
// $WCREV$

#include "bitfire_types.h"
#include "bitfire.h"
#include "lpc21xx.h"
#include "lpc2129_vic.h"
#include "lpc2129_uart.h"
#include "lpc2129_uart_polled.h"

UNS_8 u0chrdy=0;
UNS_8 u1chrdy=0;
UNS_8 u0ch;
UNS_8 u1ch;

static void uart0_int_isr(void) __attribute__ ((interrupt ("IRQ")));
static void uart1_int_isr(void) __attribute__ ((interrupt ("IRQ")));

static void uart0_int_isr(void)
{
  UNS_8 ir = U0IIR;
  u0ch = U0RBR;  
  u0chrdy = 1;
  VICVectAddr = 0x0;
}

static void uart1_int_isr(void)
{  
  UNS_8 ir = U1IIR;
  u1ch = U1RBR;
  u1chrdy = 1;
  VICVectAddr = 0x0;
}

void uart_int_init(uart_control *uc)
{
  UNS_16 bauddivisor = VPB_CLOCK / (16*uc->baudrate);
  vic_control vc;
  
  if (uc->port==0) {
    PINSEL0 = PINSEL0 | 0x00000005;
    U0LCR = uc->LCR;
    U0DLL = bauddivisor & 0xFF;
    U0DLM = bauddivisor >> 8;
    U0LCR = uc->LCR & (~LCR_DLAB_1);
    vc.vecchannel = VIC_CH_UART0;
    vc.func = (void*)uart0_int_isr;
    U0IER = 0x1;
    U0FCR = 0x1;
  }

  if (uc->port==1) {
    PINSEL0 = PINSEL0 | 0x00050000;
    U1LCR = uc->LCR;
    U1DLL = bauddivisor & 0xFF;
    U1DLM = bauddivisor >> 8;
    U1LCR = uc->LCR & (~LCR_DLAB_1);
    vc.vecchannel = VIC_CH_UART1;
    vc.func = (void*)uart1_int_isr;
    U1IER = 0x1;
    U1FCR = 0x1;
  }

  vc.vecaddr = uc->vecaddr;  
  vic_install_isr(&vc);
  vic_enable_int(&vc);
 
  uc->status = UART_STATUS_OK;
}

UNS_8 uart_int_putchar(uart_control *uc, UNS_8 ch)
{
  if (uc->status!=UART_STATUS_OK) {
    return 0;
  }
  
  if (uc->port==0) {
    if (ch == '\n')  {
      while (!(U0LSR & 0x20));
      U0THR = CR;                          /* output CR */
    }
    while (!(U0LSR & 0x20));
    return (U0THR = ch);
  }

  if (uc->port==1) {
    if (ch == '\n')  {
      while (!(U1LSR & 0x20));
      U1THR = CR;                          /* output CR */
    }
    while (!(U1LSR & 0x20));
    return (U1THR = ch);
  }
}

UNS_8 uart_int_getchar(uart_control *uc, UNS_8 *err)
{
  if (uc->status!=UART_STATUS_OK) {
    *err = UART_ERR_NOT_INIT;
    return 0;
  }
    
  if (uc->port==0) {
    if (u0chrdy) {
      u0chrdy=0;
      *err = UART_ERR_NO_ERROR;
      return u0ch;
    }
    else
      *err = UART_ERR_NO_CHAR_READY;
  }

  if (uc->port==1) {
    if (u1chrdy) {
      u1chrdy=0;
      *err = UART_ERR_NO_ERROR;
      return u1ch;
    }
    else
      *err = UART_ERR_NO_CHAR_READY;
  }

  return 0;
}
