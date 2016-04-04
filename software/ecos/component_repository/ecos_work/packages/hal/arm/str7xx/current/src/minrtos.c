// STR7xx Minimal RTOS lib, drivers ripped from ST driver lib
// Martin Trojer (mtrojer@arrownordic.com)

#include <cyg/hal/plf_io.h> 

//=========================================================================================================
// Init Routines
//=========================================================================================================

void minrtos_init(void)
{
  EIC->ICR = 0x00000000;  /* Disable FIQ and IRQ interrupts */
  EIC->IER = 0x00000000;  /* Disable all channels interrupts */
  EIC->IPR = 0xFFFFFFFF;  /* Clear all pending bits */
  EIC->FIR = 0x0000000C;  /* Disable all FIQ channels interrupts and clear FIQ */
  EIC->CIPR = 0x00000000; /* channels pending bits, set the current priority level to zero */
  
  EIC_IRQConfig( ENABLE );
}

void minrtos_setup_clocks(void)
{
  RCCU_Div2Config(ENABLE);
  RCCU_MCLKConfig(RCCU_DEFAULT);
  RCCU_FCLKConfig(RCCU_DEFAULT);
  RCCU_PCLKConfig(RCCU_RCLK_2);
  RCCU_PLL1Config(RCCU_PLL1_Mul_16,RCCU_Div_2);
//  RCCU_PLL1Config(RCCU_PLL1_Mul_16,RCCU_Div_4);
  RCCU_RCLKSourceConfig(RCCU_PLL1_Output);
}

void minrtos_setup_tick_timer(int period)
{
  //Setup Timer1 (OS Tick 100ms)
/*  TIM_Init ( TIM1 );
  TIM_ClockSourceConfig(TIM1,TIM_INTERNAL);
  TIM_ITConfig(TIM1, TIM_TO_IT,ENABLE);
  TIM_PrescalerConfig ( TIM1, 5 );
  EIC_IRQChannelPriorityConfig( T1TIMI_IRQChannel, 1);
  TIM_CounterConfig(TIM1,TIM_START);*/
  
  TIM_Init(TIM1);
  TIM_ClockSourceConfig(TIM1,TIM_INTERNAL);
  TIM_PrescalerConfig (TIM1, 31);
  TIM_OCMPModeConfig(TIM1, TIM_CHANNEL_A, period-4, TIM_TIMING, TIM_HIGH);
  EIC_IRQChannelPriorityConfig(T1TIMI_IRQChannel, 1);
  TIM_ITConfig(TIM1, TIM_OCA_IT,ENABLE);
  
  EIC_IRQChannelConfig(T1TIMI_IRQChannel, ENABLE );
}

void minrtos_setup_uart(int id, int baud) // UART2 or 3
{
  if (id==0 || id == 2)  {
    // Setup UART2
    GPIO_Config(GPIO0, UART2_Tx_Pin, GPIO_AF_PP);
    GPIO_Config(GPIO0, UART2_Rx_Pin, GPIO_IN_TRI_CMOS);
    UART_OnOffConfig(UART2, ENABLE); 
    UART_FifoConfig (UART2, DISABLE);
    UART_FifoReset  (UART2 , UART_RxFIFO);
    UART_FifoReset  (UART2 , UART_TxFIFO);
    UART_LoopBackConfig(UART2 , DISABLE);
    UART_Config(UART3,baud,UART_NO_PARITY,UART_1_StopBits,UARTM_8D);
    EIC_IRQChannelPriorityConfig(UART2_IRQChannel, 1);
    UART_ItConfig(UART2,UART_RxBufFull, ENABLE);
    UART_RxConfig(UART2 ,ENABLE);  
    EIC_IRQChannelConfig(UART2_IRQChannel, ENABLE);
  }
  else {
    // Setup UART3 
    GPIO_Config(GPIO0, UART3_Tx_Pin, GPIO_AF_PP);
    GPIO_Config(GPIO0, UART3_Rx_Pin, GPIO_IN_TRI_CMOS);
    UART_OnOffConfig(UART3, ENABLE); 
    UART_FifoConfig (UART3, DISABLE);
    UART_FifoReset  (UART3 , UART_RxFIFO);
    UART_FifoReset  (UART3 , UART_TxFIFO);
    UART_LoopBackConfig(UART3 , DISABLE);
    UART_Config(UART3,baud,UART_NO_PARITY,UART_1_StopBits,UARTM_8D);
    EIC_IRQChannelPriorityConfig(UART3_IRQChannel, 1);
    UART_ItConfig(UART3,UART_RxBufFull, ENABLE);
    UART_RxConfig(UART3 ,ENABLE);  
    EIC_IRQChannelConfig(UART3_IRQChannel, ENABLE);
  }
}

//=========================================================================================================
// Interrupt Handlers
//=========================================================================================================
void Undefined_Handler(void) { }
void FIQ_Handler(void) { }
void SWI_Handler(void) { }
void Prefetch_Handler(void) { }
void Abort_Handler(void) { }
void T0TIMI_IRQHandler(void) { }
void FLASH_IRQHandler(void) { }
void RCCU_IRQHandler(void) { }
void RTC_IRQHandler(void) { }
void WDG_IRQHandler(void) { }
void XTI_IRQHandler(void) { }
void USBHP_IRQHandler(void) { }
void I2C0ITERR_IRQHandler(void) { }
void I2C1ITERR_IRQHandler(void) { }
void UART0_IRQHandler(void){ }
void UART1_IRQHandler(void) { }
void UART2_IRQHandler(void)
{
  u8 bRByte; 
  /*  Get the received byte, set the guard time to 0xFF */
  UART_ByteReceive(UART3, &bRByte, 0xFF);
  /*  Send the received byte */
  UART_ByteSend(UART3, &bRByte);
}
void UART3_IRQHandler(void)
{
  u8 bRByte; 
  /*  Get the received byte, set the guard time to 0xFF */
  UART_ByteReceive(UART3, &bRByte, 0xFF);
  /*  Send the received byte */
  UART_ByteSend(UART3, &bRByte);
}
void BSPI0_IRQHandler(void) { }
void BSPI1_IRQHandler(void) { }
void I2C0_IRQHandler(void) { }
void I2C1_IRQHandler(void) { }
void CAN_IRQHandler(void) { }
void ADC12_IRQHandler(void) { }
int status = 0;
void T1TIMI_IRQHandler(void)
{
  TIM_FlagClear(TIM1,TIM_OCFA);
  TIM_CounterConfig(TIM1,TIM_CLEAR);

  GPIO_Config(GPIO1, 1<<9, GPIO_OUT_PP);
  if (status)
    GPIO_BitWrite(GPIO1,9,1);
  else
    GPIO_BitWrite(GPIO1,9,0);
  
  status = ~status;
}
void T2TIMI_IRQHandler(void) { }
void T3TIMI_IRQHandler(void) { }
void HDLC_IRQHandler(void) { }
void USBLP_IRQHandler(void) { }
void T0TOI_IRQHandler(void) { }
void T0OC1_IRQHandler(void) { }
void T0OC2_IRQHandler(void) { }

//=========================================================================================================
//=========================================================================================================
// rccu.c
//=========================================================================================================
//=========================================================================================================
/*******************************************************************************
* Function Name  : RCCU_FrequencyValue
* Description    : Calculates & Returns any internal RCCU clock frequency
*                  passed in parametres
* Input          : RCCU_Clocks ( RCCU_CLK2, RCCU_RCLK, RCCU_MCLK, RCCU_PCLK, RCCU_FCLK )
* Return         : u32
*******************************************************************************/
u32 RCCU_FrequencyValue ( RCCU_Clocks Internal_Clk )
{
  u32 Tmp;
  u8 Div = 0;
  u8 Mul = 0;
  RCCU_RCLK_Clocks CurrentRCLK;

  Tmp = ( RCCU_Div2Status() == SET )? RCCU_Main_Osc / 2 :  RCCU_Main_Osc;

  if ( Internal_Clk == RCCU_CLK2 )
  {
   Div = 1;
   Mul = 1;
  }
  else
  { CurrentRCLK = RCCU_RCLKClockSource ();
    switch ( CurrentRCLK ){
      case RCCU_CLOCK2_16 : Div = 16;
                            Mul = 1;
                            break;
      case RCCU_CLOCK2    : Div = 1;
                            Mul = 1;
                            break;
      case RCCU_PLL1_Output :{Mul=(RCCU->PLL1CR & RCCU_MX_Mask ) >> RCCU_MX_Index;
                              switch ( Mul )
                              {case 0: Mul = 20; break;
                               case 1: Mul = 12; break;
                               case 2: Mul = 24; break;
                               case 3: Mul = 16; break;
                              }
                              Div = ( RCCU->PLL1CR & RCCU_DX_Mask ) + 1;
                              break;}
     case RCCU_RTC_CLOCK :  Mul = 1;
                            Div = 1;
                            Tmp = RCCU_RTC_Osc;
                            break;}}

  switch ( Internal_Clk ){
      case RCCU_MCLK :{Div <<= PCU->MDIVR & RCCU_FACT_Mask;
                       break;}
      case RCCU_PCLK :{Div <<=(PCU->PDIVR & RCCU_FACT2_Mask ) >> RCCU_FACT2_Index;
                       break;}
      case RCCU_FCLK :{Div <<=  PCU->PDIVR & 0x3;
                       break;}}

  return (Tmp * Mul) / Div;
}
/*******************************************************************************
* Function Name  : RCCU_PLL1Config
* Description    : Configures the PLL1 div & mul factors.
* Input          : New_Mul ( RCCU_PLL1_Mul_12, RCCU_PLL1_Mul_16, RCCU_PLL1_Mul_20,
*                  RCCU_PLL1_Mul_24 )
*                : New_Div ( RCCU_Div_1, RCCU_Div_2, RCCU_Div_3, RCCU_Div_4,
*                  RCCU_Div_5, RCCU_Div_6, RCCU_Div_7)
* Return         : None
*******************************************************************************/
void RCCU_PLL1Config ( RCCU_PLL1_Mul New_Mul, RCCU_PLL_Div New_Div )
{
  u32 Tmp = 0;

  if (RCCU_FrequencyValue(RCCU_CLK2)>3000000)
    RCCU->PLL1CR|=RCCU_FREEN_Mask;
  else
    RCCU->PLL1CR&=~RCCU_FREEN_Mask;

  Tmp = ( RCCU->PLL1CR & ~RCCU_MX_Mask ) | ( New_Mul << RCCU_MX_Index );
  RCCU->PLL1CR = ( Tmp & ~RCCU_DX_Mask ) | New_Div | 0x40;
}
/*******************************************************************************
* Function Name  : RCCU_RCLKSourceConfig
* Description    : Selects the RCLK source clock
* Input          : New_Clock ( RCCU_PLL1_Output, RCCU_CLOCK2_16, RCCU_CLOCK2 )
* Return         : None
*******************************************************************************/
void RCCU_RCLKSourceConfig ( RCCU_RCLK_Clocks New_Clock )
{
  switch ( New_Clock )
  {
    case RCCU_CLOCK2    :{   /* Resets the CSU_Cksel bit in clk_flag */
                             RCCU->CFR &= ~RCCU_CSU_CKSEL_Mask;
                             /* Set the CK2_16 Bit in the CFR */
                             RCCU->CFR |= RCCU_CK2_16_Mask;
                             /* Deselect The CKAF */
                             RCCU->CCR   &= ~RCCU_CKAF_SEL_Mask;
                             /* switch off the PLL1 */
                             RCCU->PLL1CR=((RCCU->PLL1CR & ~RCCU_DX_Mask)\
                             |0x00000003) & ~RCCU_FREEN_Mask;
                              break;}
    case RCCU_CLOCK2_16  :{  /*  ReSet the CK2_16 Bit in the CFR */
                              RCCU->CFR &= ~RCCU_CK2_16_Mask;
                              /* Deselect The CKAF */
                              RCCU->CCR   &= ~RCCU_CKAF_SEL_Mask;
                             /*  switch off the PLL1 */
                              RCCU->PLL1CR=((RCCU->PLL1CR & ~RCCU_DX_Mask)\
                              |0x00000003) & ~RCCU_FREEN_Mask;
                              break;}
    case RCCU_PLL1_Output:{  /*  Set the CK2_16 Bit in the CFR */
                              RCCU->CFR = RCCU->CFR | RCCU_CK2_16_Mask;
                             /*  Waits the PLL1 to lock if DX bits are different from '111' */
                             /*  If all DX bit are set the PLL lock flag in meaningless */
                              if (( RCCU->PLL1CR & 0x0007 ) != 7)
                                while(!(RCCU->CFR & RCCU_LOCK_Mask));
                             /*  Deselect The CKAF */
                              RCCU->CCR  &= ~RCCU_CKAF_SEL_Mask;
                             /*  Select The CSU_CKSEL */
                              RCCU->CFR |= RCCU_CSU_CKSEL_Mask;
                              break;}
    case RCCU_RTC_CLOCK  :   {RCCU->CCR |= 0x04;
                              break;}
  }
}

/*******************************************************************************
* Function Name  : RCCU_RCLKClockSource
* Description    : Returns the current RCLK source clock
* Input          : None
* Return         : RCCU_PLL1_Output, RCCU_CLOCK2_16, RCCU_CLOCK2, RCCU_RTC_CLOCK
*******************************************************************************/
RCCU_RCLK_Clocks RCCU_RCLKClockSource ( void )
{
  if ((RCCU->CCR & 0x04)==0x04)
    return RCCU_RTC_CLOCK;

  else if ((RCCU->CFR & RCCU_CK2_16_Mask)==0)
    return RCCU_CLOCK2_16;

  else if (RCCU->CFR & RCCU_CSU_CKSEL_Mask)
    return RCCU_PLL1_Output;

  else
    return RCCU_CLOCK2;
}


//=========================================================================================================
//=========================================================================================================
// tim.c
//=========================================================================================================
//=========================================================================================================
/*******************************************************************************
* Function Name  : TIM_Init
* Description    : This routine is used to Initialize the TIM peripheral 
*                  registers to their default values.
* Input          : TIM Timer to Initialize(TIM0, TIM1, TIM2, TIM3).
* Return         : None.
*******************************************************************************/
void TIM_Init( TIM_TypeDef *TIMx ) 
{
  TIMx->CR1 = 0x0000;
  TIMx->CR2 = 0x0000;
  TIMx->SR  = 0x0000;
}
/*******************************************************************************
* Function Name  : TIM_OCMPModeConfig
* Description    : This routine is used to configure the output compare feature.
* Input          : (1) TIM Timer(TIM0, TIM1, TIM2, TIM3).
*                : (2) OCMP Channel (TIM_CHANNEL_A, TIM_CHANNEL_B).
*                : (3) Pulse Length.
*                : (4) OC_Mode (TIM_TIMING,TIM_WAVE).
*                : (5) Level (TIM_HIGH,TIM_LOW).
* Return         : None.
*******************************************************************************/
void TIM_OCMPModeConfig (  TIM_TypeDef  *TIMx,
                           TIM_Channels Xchannel,
                           u16          XpulseLength,
                           TIM_OC_Modes     Xmode,
                           TIM_Logic_Levels Xlevel )
{
  u16 Tmp1 = 0x0000;
  u16 Tmp2 = TIMx->CR2;

  TIMx->CR2 = 0x0000;
  /* Start The TIM Counter */
  TIMx->CR1  = TIM_EN_Mask;
  /* Update the CR2 Register */
  TIMx->CR2  = Tmp2;
  switch ( Xmode )
  {
    case TIM_TIMING :
      /* Output Compare Used only for Internal Timing Operation */
      Tmp1 = Xchannel == TIM_CHANNEL_A ? Tmp1 & ~TIM_OCAE_Mask : Tmp1 & ~TIM_OCBE_Mask;
      break;
    case TIM_WAVE :
      /* Output Compare Used for external wave generation */
      Tmp1 = Xchannel == TIM_CHANNEL_A ? TIM_OCAE_Mask : TIM_OCBE_Mask;
      if ( Xlevel == TIM_HIGH )
        Tmp1 = Xchannel == TIM_CHANNEL_A ? Tmp1 | TIM_OLVLA_Mask  : Tmp1 | TIM_OLVLB_Mask;
      else
        Tmp1 = Xchannel == TIM_CHANNEL_A ? Tmp1 & ~TIM_OLVLA_Mask : Tmp1 & ~TIM_OLVLB_Mask;
      break;
  }
  if ( Xchannel == TIM_CHANNEL_A )
    TIMx->OCAR = (XpulseLength);
  else
    TIMx->OCBR = ( XpulseLength);
  TIMx->CNTR = 0x0000;
  TIMx->CR1 |= Tmp1;
}
/*******************************************************************************
* Function Name  : TIM_CounterConfig
* Description    : This routine is used to configure the Timer conuter. 
* Input          : (1) TIM Timer(TIM0, TIM1, TIM2, TIM3).
*                : (2) Xoperation(TIM_START, TIM_STOP, TIM_CLEAR).
* Return         : None.
*******************************************************************************/
void TIM_CounterConfig ( TIM_TypeDef  *TIMx, TIM_CounterOperations Xoperation)
{
  switch ( Xoperation )
  {
    case TIM_START :
      TIMx->CR1 |= TIM_EN_Mask;
      break;
    case TIM_STOP :
      TIMx->CR1 &= ~TIM_EN_Mask;
      break;
    case TIM_CLEAR :
      TIMx->CNTR = 0x1234;
      break;
  }
}
//=========================================================================================================
//=========================================================================================================
// gpio.c
//=========================================================================================================
//=========================================================================================================
/*******************************************************************************
* Function Name  : GPIO_Config
* Description    : Configure the GPIO port pins.
* Input 1        : GPIOx (x can be 0,1 or 2) the desired port.
* Input 2        : Port_Pins : pins placements.
* Input 3        : Pins Mode(GPIO_HI_AIN_TRI,GPIO_IN_TRI_TTL,GPIO_IN_TRI_CMOS,
*                   GPIO_IPUPD_WP,GPIO_OUT_OD,GPIO_OUT_PP,GPIO_AF_OD,GPIO_AF_PP).
* Output         : None
* Return         : None
*******************************************************************************/
void GPIO_Config (GPIO_TypeDef *GPIOx, u16 Port_Pins, GpioPinMode_TypeDef GPIO_Mode)
{
  switch (GPIO_Mode)
  {
    case GPIO_HI_AIN_TRI:
      GPIOx->PC0&=~Port_Pins;
      GPIOx->PC1&=~Port_Pins;
      GPIOx->PC2&=~Port_Pins;
      break;

    case GPIO_IN_TRI_TTL:
      GPIOx->PC0|=Port_Pins;
      GPIOx->PC1&=~Port_Pins;
      GPIOx->PC2&=~Port_Pins;
      break;

    case GPIO_IN_TRI_CMOS:
      GPIOx->PC0&=~Port_Pins;
      GPIOx->PC1|=Port_Pins;
      GPIOx->PC2&=~Port_Pins;
      break;

    case GPIO_IPUPD_WP:
      GPIOx->PC0|=Port_Pins;
      GPIOx->PC1|=Port_Pins;
      GPIOx->PC2&=~Port_Pins;
      break;

    case GPIO_OUT_OD:
      GPIOx->PC0&=~Port_Pins;
      GPIOx->PC1&=~Port_Pins;
      GPIOx->PC2|=Port_Pins;
      break;

    case GPIO_OUT_PP:
      GPIOx->PC0|=Port_Pins;
      GPIOx->PC1&=~Port_Pins;
      GPIOx->PC2|=Port_Pins;
      break;

    case GPIO_AF_OD:
      GPIOx->PC0&=~Port_Pins;
      GPIOx->PC1|=Port_Pins;
      GPIOx->PC2|=Port_Pins;
      break;

    case GPIO_AF_PP:
      GPIOx->PC0|=Port_Pins;
      GPIOx->PC1|=Port_Pins;
      GPIOx->PC2|=Port_Pins;
      break;
  }
}
/*******************************************************************************
* Function Name  : GPIO_BitWrite
* Description    : Set or reset the selected port pin.
* Input 1        : Selected GPIO port(GPIO1, GPIO1, GPIO2).
* Input 2        : Pin number.
* Input 3        : bit value.
* Output         : None
* Return         : None.
*******************************************************************************/
void GPIO_BitWrite(GPIO_TypeDef *GPIOx, u8 Port_Pin, u8 Port_Val)
{
  if (Port_Val&0x01) GPIOx->PD |= 1<<Port_Pin; else GPIOx->PD &= ~(1<<Port_Pin);
}
//=========================================================================================================
//=========================================================================================================
// uart.c
//=========================================================================================================
//=========================================================================================================
/*******************************************************************************
* Function Name  : UART_Init
* Description    : This function initializes the selected UART registers to 
*                  their reset values
* Input 1        : UARTx (x can be 0,1, 2 or 3) the selected UART
* Output         : None
* Return         : None
*******************************************************************************/
void UART_Init(UART_TypeDef *UARTx)
{ 
  UARTx->CR = 0x0000;
  UARTx->IER = 0x0000;
  (void)UARTx->RxBUFR;
  UARTx->RxRSTR = 0xFFFF;
  UARTx->TxRSTR = 0xFFFF;
  UARTx->TOR = 0x0000;
  UARTx->GTR = 0x0000;
}
/*******************************************************************************
* Function Name  : UART_BaudRateConfig
* Description    : This function configures the baud rate of the selected UART.
* Input 1        : UARTx (x can be 0,1, 2 or 3) the selected UART
* Input 2        : u32 The baudrate value
* Output         : None
* Return         : None
*******************************************************************************/
void UART_BaudRateConfig(UART_TypeDef *UARTx, u32 BaudRate)
{
  UARTx->BR = (u16)(RCCU_FrequencyValue(RCCU_FCLK)/(16*BaudRate));
}
/*******************************************************************************
* Function Name  : UART_Config
* Description    : This function configures the baudrate, the mode, the data
*                  parity and the number of stop bits of the selected UART.
* Input 1        : UARTx (x can be 0,1, 2 or 3) the selected UART
* Input 2        : u32 The baudrate value
* Input 3        : The parity type, it can be UART_EVEN_PARITY, UART_ODD_PARITY
*		   or UART_NO_PARITY
* Input 4        : The number of stop bits UART_0_5_StopBits, UART_1_StopBits, 
*                  UART_1_5_StopBits or UART_2_StopBits
* Input 5        : The UART mode, it can be
*                  UARTM_8D 	for 8-bit data format
*                  UARTM_7D_P 	for 7-bit data + parity format
*                  UART_9D 	for 9-bit data format
*                  UART_8D_W	for 8-bit data + wake-up bit format
*                  UART_8D_P	for 8-bit data + parity bit format   
* Output         : None
* Return         : None
*******************************************************************************/
void UART_Config(UART_TypeDef *UARTx, u32 BaudRate, UARTParity_TypeDef Parity,
                 UARTStopBits_TypeDef StopBits, UARTMode_TypeDef Mode)
{
  UART_ModeConfig(UARTx, Mode);
  UART_BaudRateConfig(UARTx, BaudRate);
  UART_ParityConfig(UARTx, Parity);
  UART_StopBitsConfig(UARTx, StopBits);
}
/*******************************************************************************
* Function Name  : UART_ItConfig
* Description    : This function enables or disables one or several interrupt
*                  sources of the selected UART.
* Input 1        : UARTx (x can be 0,1, 2 or 3) the selected UART
* Input 2        : The new interrupt flag or flags    
* Input 3        : ENABLE or DISABLE
* Output         : None
* Return         : None
* Note           : The UART interrupt flags are listed in the file uart.h
*******************************************************************************/
void UART_ItConfig(UART_TypeDef *UARTx, u16 UART_Flag, FunctionalState NewState)
{
  if (NewState==ENABLE) UARTx->IER|=UART_Flag; else UARTx->IER&=~UART_Flag;
}
/*******************************************************************************
* Function Name  : UART_FifoConfig
* Description    : This function enables or disables the Rx and Tx FIFOs of
*                  the selected UART.
* Input 1        : UARTx (x can be 0,1, 2 or 3) the selected UART
* Input 2        : ENABLE or DISABLE
* Output         : None
* Return         : None
*******************************************************************************/
void UART_FifoConfig(UART_TypeDef *UARTx, FunctionalState NewState)
{
  if (NewState==ENABLE) UARTx->CR|=0x0400; else UARTx->CR&=~0x0400;
}
/*******************************************************************************
* Function Name  : UART_FifoReset
* Description    : This function resets the Rx and the Tx FIFOs of the
*                  selected UART.
* Input 1        : UARTx (x can be 0,1, 2 or 3) the selected UART
* Input 2        : UART_RxFIFO or UART_TxFIFO
* Output         : None
* Return         : None
*******************************************************************************/
void UART_FifoReset(UART_TypeDef *UARTx, UARTFIFO_TypeDef FIFO)
{
  if (FIFO==UART_RxFIFO) UARTx->RxRSTR=0xFFFF; else UARTx->TxRSTR=0xFFFF;
}
/*******************************************************************************
* Function Name  : UART_LoopBackConfig
* Description    : This function enables or disables the loop back mode of
*                  the selected UART.
* Input 1        : UARTx (x can be 0,1, 2 or 3) the selected UART
* Input 2        : ENABLE or DISABLE
* Output         : None
* Return         : None
*******************************************************************************/
void UART_LoopBackConfig(UART_TypeDef *UARTx, FunctionalState NewState)
{
  if (NewState==ENABLE) UARTx->CR|=0x0040; else UARTx->CR&=~0x0040;
}
/*******************************************************************************
* Function Name  : UART_RxConfig
* Description    : This function enables or disables the UART data reception.
* Input 1        : UARTx (x can be 0,1, 2 or 3) the selected UART
* Input 2        : ENABLE or DISABLE
* Output         : None
* Return         : None
*******************************************************************************/
void UART_RxConfig(UART_TypeDef *UARTx, FunctionalState NewState)
{
  if (NewState==ENABLE) UARTx->CR|=0x0100; else UARTx->CR&=~0x0100;
}
/*******************************************************************************
* Function Name  : UART_OnOffConfig
* Description    : This function sets On/Off the selected UART.
* Input 1        : UARTx (x can be 0,1, 2 or 3) the selected UART
* Input 2        : ENABLE or DISABLE
* Output         : None
* Return         : None
*******************************************************************************/
void UART_OnOffConfig(UART_TypeDef *UARTx, FunctionalState NewState)
{
  if (NewState==ENABLE) UARTx->CR|=0x0080; else UARTx->CR&=~0x0080;
}
/*******************************************************************************
* Function Name  : UART_ByteSend
* Description    : This function sends a data byte to the selected UART.
* Input 1        : UARTx (x can be 0,1, 2 or 3) the selected UART
* Input 2        : A pointer on the data byte to send
* Output         : None
* Return         : None
*******************************************************************************/
void UART_ByteSend(UART_TypeDef *UARTx, u8 *Data)
{
  /* if FIFO ENABLED */  
  if (UARTx->CR & 0x0400) {
    /* Wait until the TxFIFO contains at least 1 free place */
    while((UARTx->SR & UART_TxFull));
  }
  /* if FIFO DISABLED */  
  else {
    /* Wait until the transmit shift register is empty */
    while (!(UARTx->SR & UART_TxEmpty));
  }
  UARTx->TxBUFR = *Data;
}
/*******************************************************************************
* Function Name  : UART_ByteReceive
* Description    : This function gets a data byte from the selected UART.
* Input 1        : UARTx (x can be 0,1, 2 or 3) the selected UART
* Input 2        : A pointer on the data where the data will be stored
* Input 3        : The time-out period
* Output         : The received 8-bit data
* Return         : The UARTx.SR register content
*******************************************************************************/
u16 UART_ByteReceive(UART_TypeDef *UARTx, u8 *Data, u8 TimeOut)
{
  u16 wStatus = 0;

  /* reload the Timeout counter */
  UARTx->TOR=TimeOut;

  /* while the UART_RxFIFO is empty and no Timeoutidle */
  while (!((wStatus=UARTx->SR) & (UART_TimeOutIdle|UART_RxHalfFull|UART_RxBufFull)));

  /* then read the Receive Buffer Register */
  *Data = (u8)UARTx->RxBUFR;

  return wStatus;
}
//=========================================================================================================
//=========================================================================================================
// bspi.c
//=========================================================================================================
//=========================================================================================================
/*******************************************************************************
* Function Name  : BSPI_Init
* Description    : Initializes BSPI peripheral control and registers to their default reset values.
* Input          : BSPIx where x can be 0 or 1 to select the BSPI peripheral.
* Output         : BSPI peripheral registers and BSPI global variables are initialized.
* Return         : None.
*******************************************************************************/
void BSPI_Init(BSPI_TypeDef *BSPIx)
{
  BSPIx->CSR2 = 0x41;
  BSPIx->CSR1 = 0x00;
  BSPIx->CLK  = 0x06;
}

/*******************************************************************************
* Function Name  : BSPI_TrItSrc.
* Description    : Configures the transmit interrupt source.
* Input 1        : BSPIx where x can be 0 or 1 to select the BSPI peripheral.
* Input 2        : TrItSrc: specifies the transmit interrupt source.
*		 : Refer to the section ‘Transmit Interrupt Sources’ for more details on the
*		 : allowed values of this parameter.
* Output         : TIE bit in BSPCSR2 register.
* Return         : None.
*******************************************************************************/
void BSPI_TrItSrc(BSPI_TypeDef *BSPIx, BSPI_TR_IT_SRCS TrItSrc)
{
  switch (TrItSrc)
  {
    case BSPI_TR_DIS:
      BSPIx->CSR2 &= ~0xC000;
      break;
    case BSPI_TR_FE:
      BSPIx->CSR2 &= ~0x8000;
      BSPIx->CSR2 |= 0x4000;
      break;
    case BSPI_TR_UFL:
      BSPIx->CSR2 &= ~0x4000;
      BSPIx->CSR2 |= 0x8000;
      break;
    case BSPI_TR_FF:
      BSPIx->CSR2 |= 0xC000;
      break;
  }
}

/*******************************************************************************
* Function Name  : BSPI_RcItSrc.
* Description    : Configures the receive interrupt source.
* Input 1        : BSPIx where x can be 0 or 1 to select the BSPI peripheral.
* Input 2        : RcItSrc: specifies the source for the receive interrupt.
*		 : Refer to the section ‘Receive Interrupt Sources’ for more details on the
*		 : allowed values of this parameter.
* Output         : RIE bit in BSPCSR1 is register.
* Return         : None.
*******************************************************************************/
void BSPI_RcItSrc(BSPI_TypeDef *BSPIx, BSPI_RC_IR_SRCS RcItSrc)
{
  switch (RcItSrc)
  {
    case BSPI_RC_DIS:
      BSPIx->CSR1 &= ~0x000C;
      break;
    case BSPI_RC_FNE:
      BSPIx->CSR1 &= ~0x0008;
      BSPIx->CSR1 |= 0x0004;
      break;
    case BSPI_RC_FF:
      BSPIx->CSR1 |= 0x000C;
      break;
  }
}

/*******************************************************************************
* Function Name  : BSPI_TrFifoDepth
* Description    : Configures BSPI transmission FIFO number of words.
* Input 1        : BSPIx where x can be 0 or 1 to select the BSPI.
* Input 2        : TDepth:specifies the depth of the transmit FIFO.
* Output         : TFE bit in BSPCSR2 register.
* Return         : None.
*******************************************************************************/
void BSPI_TrFifoDepth(BSPI_TypeDef *BSPIx, u8 TDepth)
{
  if (TDepth > 0 && TDepth < 11)
  {
    TDepth--;
    BSPIx->CSR2 &= (TDepth*0x400) | ~0x3C00;
    BSPIx->CSR2 |= TDepth*0x400;
  }
  else BSPIx->CSR2 &= ~0x3C00;
}

/*******************************************************************************
* Function Name  : BSPI_RcFifoDepth
* Description    : Configures BSPI reception FIFO number of words.
* Input 1        : BSPIx where x can be 0 or 1 to select the BSPI peripheral.
* Input 2        : RDepth:specifies the depth of the receive FIFO.
* Output         : RFE bits [3:0] in BSPCSR1 register.
* Return         : None.
*******************************************************************************/
void BSPI_RcFifoDepth(BSPI_TypeDef *BSPIx, u8 RDepth)
{
  if (RDepth > 0 && RDepth < 11)
  {
    RDepth--;
    BSPIx->CSR1 &= (RDepth*0x1000) | ~0xF000;
    BSPIx->CSR1 |= RDepth*0x1000;
  }
  else BSPIx->CSR1 &= ~0xF000;
}

/*******************************************************************************
* Function Name  : BSPI_BufferSend.
* Description    : Transmits data from a buffer.
* Input 1        : BSPIx where x can be 0 or 1 to select the BSPI peripheral.
* Input 2        : PtrToBuffer is an ‘u8’ pointer to the first word of the buffer to be transmitted.
* Input 3        : NbOfWords parameter indicates the number of words saved in the buffer to be sent.
* Output         : None.
* Return         : None.
*******************************************************************************/
void  BSPI_BufferSend(BSPI_TypeDef *BSPIx, u8 *PtrToBuffer, u8 NbOfWords)
{
  vu8 SendWord = 0;
  while (SendWord < NbOfWords)
  {
    BSPI_WordSend(BSPIx, *(PtrToBuffer+SendWord));
    SendWord++;
  }
}

/*******************************************************************************
* Function Name  : BSPI_BufferReceive.
* Description    : Receives number of data words and stores them in user defined area.
* Input 1        : BSPIx where x can be 0 or 1 to select the BSPI peripheral.
* Input 2        : PtrToBuffer is an ‘u8’ pointer to the first word of the defined area to save the received buffer.
* Input 3        : NbOfWords parameter indicates the number of words to be received in the buffer.
* Output         : None.
* Return         : None.
*******************************************************************************/
void BSPI_BufferReceive(BSPI_TypeDef *BSPIx, u8 *PtrToBuffer, u8 NbOfWords)
{
  vu16 ReceiveWord = 0;
  while (ReceiveWord < NbOfWords)
  {
    *(PtrToBuffer+ReceiveWord) = BSPI_WordReceive(BSPIx);
    ReceiveWord++;
  }
}
