// FreeRTOS aeb01 demo
// mtrojer@arrownordic.com
// $WCREV$

/* Standard includes. */
#include <stdio.h>

/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#include "bitfire_types.h"
#include "gfxlib.h"
#include "gfxtxt.h"
#include "font_rom8x8.h"

#include "lpc2129_uart_polled.h"

/*-----------------------------------------------------------*/

xSemaphoreHandle scroll1;
xSemaphoreHandle scroll2;

static void taskcode( void *pvParameters );

uart_control uart0;

int main( void )
{
  UNS_16 i;
  CHAR ch='0';
  CHAR buf[6]="TaskX";

  uart0.port=0;
  uart0.LCR = LCR_WLS_8BIT | LCR_SBS_1BIT | LCR_PE_0 | LCR_PS_ODD | LCR_BK_0 | LCR_DLAB_1;
  uart0.baudrate = 115200;
  uart_polled_init(&uart0);
  
  gl_init();
  gltxt_setfont(font_rom8x8_bits,8,8);

  for (i=0;i<10;i++) {
    buf[4]=ch;
    xTaskCreate(taskcode, (const signed char*)buf, configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY+i+1, NULL);
    ch++;
  }
    
  vSemaphoreCreateBinary(scroll1);
  vSemaphoreCreateBinary(scroll2);
  
  vTaskStartScheduler();

  // Should never reach here!
  return 0;
}

extern void vPortEnterCritical( void );
extern void vPortExitCritical( void );

// Since the pixels are written to the screen with a polled
// SPI peripheral, interrupts can't be enabled when setting pixels.
// If they are, an tick interrupt in the middle of a SPI pixelset
// can result in an pixel error.
void my_fillrect(gl_rect *rect, gl_col *col)
{
  vPortEnterCritical();
  gl_fillrect(rect, col);
  vPortExitCritical();
}

// See comment above
BOOL_8 my_scrolltext(CHAR *text, gl_point *pos, gl_col *col, UNS_32 *offset)
{
  BOOL_8 res;
  vPortEnterCritical();
  res = glfx_scrolltext(text, pos, col, offset, 0);
  vPortExitCritical();
  return res;
}

extern volatile CHAR *pxCurrentTCB; // Internal kernel TCB pointer

void taskcode(void *pvParameters)
{
  CHAR *buf=(CHAR*)(pxCurrentTCB+56); // Taskname is at this offset
  gl_col c = {gl_random()%256,gl_random()%256};
  while(1)
  {
    if (xSemaphoreTake(scroll1,(portTickType)0)==pdTRUE) {
      int i;
      for (i=0;i<5;i++)
        uart_polled_putchar(&uart0,buf[i]);
      uart_polled_putchar(&uart0,'\r');
      uart_polled_putchar(&uart0,'\n');
      {
        int offset=0;
        gl_point p = {0,0};
        gl_rect r = {0,0,GL_XSIZE,8};
        while(my_scrolltext(buf,&p,&c,&offset)==0) {
          vTaskDelay(5);
          my_fillrect(&r,&glcol_black);
        }
      }
      xSemaphoreGive(scroll1);
    }
    if (xSemaphoreTake(scroll2,(portTickType)0)==pdTRUE) {
      int i;
      for (i=0;i<5;i++)
        uart_polled_putchar(&uart0,buf[i]);
      uart_polled_putchar(&uart0,'\r');
      uart_polled_putchar(&uart0,'\n');
      {
        int offset=0;
        gl_point p = {0,8};
        gl_rect r = {0,8,GL_XSIZE,GL_YSIZE};
        while(my_scrolltext(buf,&p,&c,&offset)==0) {
          vTaskDelay(5);
          my_fillrect(&r,&glcol_black);
        }
      }
      xSemaphoreGive(scroll2);
    }
    vTaskDelay(1000+(gl_random()%1000));
  }
}
