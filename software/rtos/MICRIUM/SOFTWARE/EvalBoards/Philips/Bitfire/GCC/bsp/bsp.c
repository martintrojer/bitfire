/*
*********************************************************************************************************
*                                               Philips LPC210x
*                                  LPC210x Kick Start Card Board Support Package
*
*                                    (c) Copyright 2004, Micrium, Weston, FL
*                                              All Rights Reserved
*
*
* File : BSP.C
* By   : Jean J. Labrosse
* Changed by:
* mtrojer@arrownordic.com
* $WCREV$
*********************************************************************************************************
*/

#include <includes.h>

/*
*********************************************************************************************************
*                                              CONSTANTS
*********************************************************************************************************
*/

#define  BSP_UNDEF_INSTRUCTION_VECTOR_ADDR   (*(INT32U *)0x00000004L)
#define  BSP_SWI_VECTOR_ADDR                 (*(INT32U *)0x00000008L)
#define  BSP_PREFETCH_ABORT_VECTOR_ADDR      (*(INT32U *)0x0000000CL)
#define  BSP_DATA_ABORT_VECTOR_ADDR          (*(INT32U *)0x00000010L)
#define  BSP_IRQ_VECTOR_ADDR                 (*(INT32U *)0x00000018L)
#define  BSP_FIQ_VECTOR_ADDR                 (*(INT32U *)0x0000001CL)

#define  BSP_IRQ_ISR_ADDR                    (*(INT32U *)0x00000038L)
#define  BSP_FIQ_ISR_ADDR                    (*(INT32U *)0x0000003CL)

#define VIC_WDT    0
#define VIC_TIMER0 4
#define VIC_TIMER1 5
#define VIC_UART0  6
#define VIC_UART1  7
#define VIC_PWM0   8
#define VIC_I2C    9
#define VIC_SPI    10
#define VIC_RTC    13
#define VIC_EINT0  14
#define VIC_EINT1  15
#define VIC_EINT2  16

/*
*********************************************************************************************************
*                                               DATA TYPES
*********************************************************************************************************
*/

typedef  void (*PFNCT)(void);


/*
*********************************************************************************************************
*                                              VARIABLES
*********************************************************************************************************
*/

INT32U  VIC_SpuriousInt;

/*
*********************************************************************************************************
*                                              PROTOTYPES
*********************************************************************************************************
*/

void  VIC_Init(void);
void  VIC_Dummy(void);
void  VIC_DummyWDT(void);
void  VIC_DummyTIMER0(void);
void  VIC_DummyTIMER1(void);
void  VIC_DummyUART0(void);
void  VIC_DummyUART1(void);
void  VIC_DummyPWM0(void);
void  VIC_DummyI2C(void);
void  VIC_DummySPI(void);
void  VIC_DummyRTC(void);
void  VIC_DummyEINT0(void);
void  VIC_DummyEINT1(void);
void  VIC_DummyEINT2(void);

/*
*********************************************************************************************************
*                                         BSP INITIALIZATION
*
* Description : This function should be called by your application code before you make use of any of the
*               functions found in this module.
*               
* Arguments   : none
*********************************************************************************************************
*/

void  BSP_Init (void)
{
    VIC_Init();                                   /* Initialize the Vectored Interrupt Controller      */

    Tmr_TickInit();
}

/*
*********************************************************************************************************
*                                           IRQ ISR HANDLER
*
* Description : This function is called by OS_CPU_IRQ_ISR() to determine the source of the interrupt
*               and process it accordingly.
*
* Arguments   : none
*********************************************************************************************************
*/

void  OS_CPU_IRQ_ISR_Handler (void)
{
    PFNCT  pfnct;

#if 1  
    pfnct = (PFNCT)VICVectAddr;                 /* Read the interrupt vector from the VIC               */
    if (pfnct != (PFNCT)0) {                    /* Make sure we don't have a NULL pointer               */
        (*pfnct)();                             /* Execute the ISR for the interrupting device          */
    }
#else    
    pfnct = (PFNCT)VICVectAddr;                 /* Read the interrupt vector from the VIC               */
    while (pfnct != (PFNCT)0) {                 /* Make sure we don't have a NULL pointer               */
      (*pfnct)();                               /* Execute the ISR for the interrupting device          */
        pfnct = (PFNCT)VICVectAddr;             /* Read the interrupt vector from the VIC               */
    }
#endif    
}


/*
*********************************************************************************************************
*                                           FIQ ISR HANDLER
*
* Description : This function is called by OS_CPU_FIQ_ISR() to determine the source of the interrupt
*               and process it accordingly.
*
* Arguments   : none
*********************************************************************************************************
*/

void  OS_CPU_FIQ_ISR_Handler (void)
{
    PFNCT  pfnct;
    
#if 1   
    pfnct = (PFNCT)VICVectAddr;                 /* Read the interrupt vector from the VIC               */
    if (pfnct != (PFNCT)0) {                    /* Make sure we don't have a NULL pointer               */
        (*pfnct)();                             /* Execute the ISR for the interrupting device          */
    }
#else    
    pfnct = (PFNCT)VICVectAddr;                 /* Read the interrupt vector from the VIC               */
    while (pfnct != (PFNCT)0) {                 /* Make sure we don't have a NULL pointer               */
      (*pfnct)();                               /* Execute the ISR for the interrupting device          */
        pfnct = (PFNCT)VICVectAddr;             /* Read the interrupt vector from the VIC               */
    }
#endif    
}


/*
*********************************************************************************************************
*                                       TICKER INITIALIZATION
*
* Description : This function is called to initialize uC/OS-II's tick source (typically a timer generating
*               interrupts every 1 to 100 mS).
*               
* Arguments   : none
*********************************************************************************************************
*/

void  Tmr_TickInit (void)
{
                                                 /* VIC TIMER #0 Initialization                        */
    VICIntSelect &= ~(1 << VIC_TIMER0);          /* Enable interrupts                                  */
    VICVectAddr2  = (INT32U)Tmr_TickISR_Handler; /* Set the vector address                             */
    VICVectCntl2  = 0x20 | VIC_TIMER0;           /* Enable vectored interrupts                         */
    VICIntEnable  =  (1 << VIC_TIMER0);          /* Enable Interrupts                                  */

    T0TCR         = 0;                           /* Disable timer 0.                                   */
    T0PC          = 0;                           /* Prescaler is set to no division.                   */
    T0MR0         = CPU_PERIPHERAL_CLK_FREQ / OS_TICKS_PER_SEC;  /* Count up to this value.            */
    T0MCR         = 3;                           /* Reset and interrupt on MR0 (match register 0).     */
    T0CCR         = 0;                           /* Capture is disabled.                               */
    T0EMR         = 0;                           /* No external match output.                          */
    T0TCR         = 1;                           /* Enable timer 0                                     */
}


/*
*********************************************************************************************************
*                                         TIMER #0 IRQ HANDLER
*
* Description : This function handles the timer interrupt that is used to generate TICKs for uC/OS-II.
*               
* Arguments   : none
*********************************************************************************************************
*/

void  Tmr_TickISR_Handler (void)
{
  T0IR        = 0xFF;                 /* Clear timer #0 interrupt                                    */
  OSTimeTick();                       /* If the interrupt is from the tick source, call OSTimeTick() */
  VICVectAddr = 0;
}


/*
*********************************************************************************************************
*                                        Vectored Interrupt Controller
*********************************************************************************************************
*/

void  VIC_Init (void)
{
    VICIntEnClr = 0xFFFFFFFF;                  /* Disable ALL interrupts                             */
    VICProtection = 0;                           /* Setup interrupt controller                         */

    VICVectAddr1  = (INT32U)VIC_DummyWDT;        /* Set the vector address                             */
    VICVectAddr2  = (INT32U)VIC_DummyTIMER0; 
    VICVectAddr3  = (INT32U)VIC_DummyTIMER1;
    VICVectAddr4  = (INT32U)VIC_DummyUART0;
    VICVectAddr5  = (INT32U)VIC_DummyUART1;
    VICVectAddr6  = (INT32U)VIC_DummyPWM0;
    VICVectAddr7  = (INT32U)VIC_DummyI2C;
    VICVectAddr8  = (INT32U)VIC_DummySPI;
    VICVectAddr9  = (INT32U)VIC_DummyRTC;
    VICVectAddr10 = (INT32U)VIC_DummyEINT0;
    VICVectAddr11 = (INT32U)VIC_DummyEINT1;
    VICVectAddr12 = (INT32U)VIC_DummyEINT2;
}


void  VIC_Dummy (void)
{
    while (1) {
        (void)VIC_SpuriousInt;
    }
}


void  VIC_DummyWDT (void)
{
    VIC_SpuriousInt = VIC_WDT;
    VIC_Dummy();
}


void  VIC_DummyTIMER0 (void)
{
    VIC_SpuriousInt = VIC_TIMER0;
    VIC_Dummy();
}


void  VIC_DummyTIMER1 (void)
{
    VIC_SpuriousInt = VIC_TIMER1;
    VIC_Dummy();
}


void  VIC_DummyUART0 (void)
{
    VIC_SpuriousInt = VIC_UART0;
    VIC_Dummy();
}


void  VIC_DummyUART1 (void)
{
    VIC_SpuriousInt = VIC_UART1;
    VIC_Dummy();
}


void  VIC_DummyPWM0 (void)
{
    VIC_SpuriousInt = VIC_UART1;
    VIC_Dummy();
}


void  VIC_DummyI2C (void)
{
    VIC_SpuriousInt = VIC_I2C;
    VIC_Dummy();
}


void  VIC_DummySPI (void)
{
    VIC_SpuriousInt = VIC_SPI;
    VIC_Dummy();
}


void  VIC_DummyRTC (void)
{
    VIC_SpuriousInt = VIC_RTC;
    VIC_Dummy();
}


void  VIC_DummyEINT0 (void)
{
    VIC_SpuriousInt = VIC_EINT0;
    VIC_Dummy();
}


void  VIC_DummyEINT1 (void)
{
    VIC_SpuriousInt = VIC_EINT1;
    VIC_Dummy();
}


void  VIC_DummyEINT2 (void)
{
    VIC_SpuriousInt = VIC_EINT2;
    VIC_Dummy();
}
