// UCOSII aeb01 demo
// mtrojer@arrownordic.com
// $WCREV$

#include <includes.h>

#define  TASK_STK_SIZE      128
#define  TASK_START_PRIO    0

#define MAX_SCROLLTASK 8

OS_STK        AppStartTaskStk[TASK_STK_SIZE];
OS_STK        AppTaskStks[MAX_SCROLLTASK][TASK_STK_SIZE];

OS_EVENT *scroll1sem;
OS_EVENT *scroll2sem;

/*-----------------------------------------------------------*/

// Since the pixels are written to the screen with a polled
// SPI peripheral, interrupts can't be enabled when setting pixels.
// If they are, an tick interrupt in the middle of a SPI pixelset
// can result in an pixel error.
void my_fillrect(gl_rect *rect, gl_col *col)
{
  OS_CPU_SR  cpu_sr = 0;
  OS_ENTER_CRITICAL();
  gl_fillrect(rect, col);
  OS_EXIT_CRITICAL();
}

// See comment above
BOOL_8 my_scrolltext(CHAR *text, gl_point *pos, gl_col *col, UNS_32 *offset)
{
  BOOL_8 res;
  OS_CPU_SR  cpu_sr = 0;
  OS_ENTER_CRITICAL();
  res = glfx_scrolltext(text, pos, col, offset,0);
  OS_EXIT_CRITICAL();
  return res;
}

void my_setpixel(gl_point *pos, gl_col *col)
{
  OS_CPU_SR  cpu_sr = 0;
  OS_ENTER_CRITICAL();
  gl_setpixel(pos,col);
  OS_EXIT_CRITICAL();
}

static void AppStartTask (void *p_arg);
static void taskcode (void *p_arg);

/*-----------------------------------------------------------*/

int main (void)
{
  INT8U  err;
  UNS_32 i;
  CHAR ch='0';
  CHAR buf[6]="TaskX";

  VICIntEnClr = 0xFFFFFFFF;  // Off with the timer interrupt
    
  OSInit();                               /* Initialize "uC/OS-II, The Real-Time Kernel"             */

  OSTaskCreateExt(AppStartTask, 
		  (void *)0, 
		  (OS_STK *)&AppStartTaskStk[TASK_STK_SIZE - 1], 
		  TASK_START_PRIO, 
		  TASK_START_PRIO,
		  (OS_STK *)&AppStartTaskStk[0],
		  TASK_STK_SIZE,
		  (void *)0,
		  OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);

  for (i=0;i<MAX_SCROLLTASK;i++) {
    OSTaskCreateExt(taskcode,
		    (void*)0,
		    (OS_STK *) &AppTaskStks[i][TASK_STK_SIZE-1],
		    i+1,
		    i+1,
		    (OS_STK *) &AppTaskStks[i][0],
		    TASK_STK_SIZE,
		    (void*)0,
		    OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);
    buf[4]=ch;
    OSTaskNameSet(i+1,(INT8U*) buf, &err);
    ch++;
  }
  
#if OS_TASK_NAME_SIZE > 11                    
    OSTaskNameSet(TASK_START_PRIO, (INT8U*) "Start Task", &err);
#endif

#if OS_TASK_NAME_SIZE > 14                    
    OSTaskNameSet(OS_IDLE_PRIO, (INT8U*) "uC/OS-II Idle", &err);
#endif

#if (OS_TASK_NAME_SIZE > 14) && (OS_TASK_STAT_EN > 0)                    
    OSTaskNameSet(OS_STAT_PRIO, (INT8U*) "uC/OS-II Stat", &err);
#endif

    scroll1sem = OSSemCreate(1);
    scroll2sem = OSSemCreate(1);
    
    OSStart();                              /* Start multitasking (i.e. give control to uC/OS-II)      */
}

static void AppStartTask (void *p_arg)
{
  int inc=0;
  gl_init();
  gltxt_setfont(font_rom8x8_bits,8,8);
  gl_clrscr((gl_col*)&glcol_black);

  BSP_Init();
    
#if OS_TASK_STAT_EN > 0
    OSStatInit();
#endif

    while (TRUE) {
      //      gl_point p = {inc,0};
      //      my_setpixel(&p,(gl_col*)&glcol_yellow);
      OSTimeDly((INT16U)1000000);
      inc++;
    }
}

static void taskcode (void *p_arg)
{
  UNS_32 so=0;
  INT8U  err;
  gl_col c = {gl_random()%256,gl_random()%256};
  char taskname[OS_TASK_NAME_SIZE];
  
  OSTaskNameGet(OS_PRIO_SELF,taskname,&err);

  while(1) {
    OSSemPend(scroll1sem,1,&err);
    if (err==OS_NO_ERR) {
      gl_point p = {0,0};
      gl_rect r = {0,0,GL_XSIZE,8};
      while(my_scrolltext(taskname,&p,&c,&so)==0) {
	OSTimeDly(10);
	my_fillrect(&r,&glcol_black);
      }
      so=0;
      OSSemPost(scroll1sem);
    }
    OSSemPend(scroll2sem,1,&err);
    if (err==OS_NO_ERR) {
      gl_point p = {0,8};
      gl_rect r = {0,8,GL_XSIZE,GL_YSIZE};
      while(my_scrolltext(taskname,&p,&c,&so)==0) {
	OSTimeDly(10);
	my_fillrect(&r,&glcol_black);
      }
      so=0;
      OSSemPost(scroll2sem);
    }

    OSTimeDly(500+gl_random()%500);
  }  
}
