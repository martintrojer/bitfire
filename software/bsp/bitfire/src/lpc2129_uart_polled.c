// UART polled
// mtrojer@arrownordic.com
// $WCREV$

#include "bitfire_types.h"
#include "bitfire.h"
#include "lpc21xx.h"
#include "lpc2129_uart.h"
#include "lpc2129_uart_polled.h"

void uart_polled_init(uart_control *uc)
{
  UNS_16 bauddivisor = VPB_CLOCK / (16*uc->baudrate);

  if (uc->port==0) {
    PINSEL0 = PINSEL0 | 0x00000005;
    U0LCR = uc->LCR;
    U0DLL = bauddivisor & 0xFF;
    U0DLM = bauddivisor >> 8;
    U0LCR = uc->LCR & (~LCR_DLAB_1);
  }

  if (uc->port==1) {
    PINSEL0 = PINSEL0 | 0x00050000;
    U1LCR = uc->LCR;
    U1DLL = bauddivisor & 0xFF;
    U1DLM = bauddivisor >> 8;
    U1LCR = uc->LCR & (~LCR_DLAB_1);
  }

  uc->status = UART_STATUS_OK;
}

UNS_8 uart_polled_putchar(uart_control *uc, UNS_8 ch)
{
  if (uc->status!=UART_STATUS_OK)
    return 0;
  
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

UNS_8 uart_polled_getchar(uart_control *uc)
{
  if (uc->status!=UART_STATUS_OK)
    return 0;

  if (uc->port==0) {
    while (!(U0LSR & 0x01));
    return (U0RBR);
  }

  if (uc->port==1) {
    while (!(U1LSR & 0x01));
    return (U1RBR);
  }
}
