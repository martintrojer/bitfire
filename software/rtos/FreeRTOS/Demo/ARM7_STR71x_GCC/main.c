
/* Library includes. */
#include "minrtos.h"

/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#include "gfxlib.h"
#include "gfxtxt.h"
#include "font_rom8x8.h"

xSemaphoreHandle scroll1;
xSemaphoreHandle scroll2;

static void taskcode( void *pvParameters );
static void prvSetupHardware( void );

void main( void )
{
  int i;
  char ch='0';
  char buf[6]="TaskX";
  
  gl_init();
  gltxt_setfont(font_rom8x8_bits,8,8);
	
  /* Setup any hardware that has not already been configured by the low
	level init routines. */
	prvSetupHardware();

        for (i=0;i<10;i++) {
          buf[4]=ch;
          xTaskCreate(taskcode, (const signed char*)buf, configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY+i+1, NULL);
          ch++;
        }
        
		vSemaphoreCreateBinary(scroll1);
		vSemaphoreCreateBinary(scroll2);
        
		vTaskStartScheduler();

	/* We should never get here as control is now taken by the scheduler. */
	return;
}

static void prvSetupHardware( void )
{
  minrtos_init();
  minrtos_setup_clocks();
  
  minrtos_setup_uart(3,9600);
}

extern volatile char *pxCurrentTCB; // Internal kernel TCB pointer
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

void print(char *str)
{
  while(*str!='\0')
    minrtos_uart_putch(3,str++);
}

void taskcode(void *pvParameters)
{
  int i=0;
  gl_col c = {gl_random()%256,gl_random()%256};
  char *buf=(char*)(pxCurrentTCB+56); // Taskname is at this offset
  
  while(1){
    i++;
    if (xSemaphoreTake(scroll1,(portTickType)0)==pdTRUE) {
		print(buf);
		print("\r\n");
		{
			int offset=0;
			gl_point p = {0,0};
			gl_rect r = {0,0,GL_XSIZE,8};
			while(my_scrolltext(buf,&p,&c,&offset)==0) {
				vTaskDelay(2);
				my_fillrect(&r,&glcol_black);
			}
		}
	xSemaphoreGive(scroll1);
    }
	
	if (xSemaphoreTake(scroll2,(portTickType)0)==pdTRUE) {
		print(buf);
		print("\r\n");
		{
			int offset=0;
			gl_point p = {0,8};
			gl_rect r = {0,8,GL_XSIZE,GL_YSIZE};
			while(my_scrolltext(buf,&p,&c,&offset)==0) {
				vTaskDelay(2);
				my_fillrect(&r,&glcol_black);
			}
		}
		xSemaphoreGive(scroll2);
	}
	vTaskDelay(100+(rand()%500));
  }
}