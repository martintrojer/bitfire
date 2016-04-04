// UART Interrupt test
// mtrojer@arrownordic.com
// $WCREV$

#include "bitfire_types.h"
#include "lpc2129_uart_int.h"
#include "lpc2129_vic.h"

int main(void)
{
  uart_control uart0;

  uart0.port=0;
  uart0.LCR = LCR_WLS_8BIT | LCR_SBS_1BIT | LCR_PE_0 | LCR_PS_ODD | LCR_BK_0 | LCR_DLAB_1;
  uart0.baudrate = 115200;
  uart0.vecaddr=2;

  uart_int_init(&uart0);
  
  uart_int_putchar(&uart0,'H');
  uart_int_putchar(&uart0,'e');
  uart_int_putchar(&uart0,'l');
  uart_int_putchar(&uart0,'l');
  uart_int_putchar(&uart0,'o');
  uart_int_putchar(&uart0,'!');
  uart_int_putchar(&uart0,' ');
  
   while(1) {
     UNS_8 err;
     UNS_8 ch = uart_int_getchar(&uart0, &err);
     if (err == UART_ERR_NO_ERROR)
       uart_int_putchar(&uart0,ch);
   }
}
