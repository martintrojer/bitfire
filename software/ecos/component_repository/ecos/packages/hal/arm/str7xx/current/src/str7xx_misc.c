//==========================================================================
//
//      at91_misc.c
//
//      HAL misc board support code for Atmel AT91/EB40
//
//==========================================================================
//####ECOSGPLCOPYRIGHTBEGIN####
// -------------------------------------------
// This file is part of eCos, the Embedded Configurable Operating System.
// Copyright (C) 1998, 1999, 2000, 2001, 2002 Red Hat, Inc.
//
// eCos is free software; you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free
// Software Foundation; either version 2 or (at your option) any later version.
//
// eCos is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
// for more details.
//
// You should have received a copy of the GNU General Public License along
// with eCos; if not, write to the Free Software Foundation, Inc.,
// 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA.
//
// As a special exception, if other files instantiate templates or use macros
// or inline functions from this file, or you compile this file and link it
// with other works to produce a work based on this file, this file does not
// by itself cause the resulting work to be covered by the GNU General Public
// License. However the source code for this file must still be made available
// in accordance with section (3) of the GNU General Public License.
//
// This exception does not invalidate any other reasons why a work based on
// this file might be covered by the GNU General Public License.
//
// Alternative licenses for eCos may be arranged by contacting Red Hat, Inc.
// at http://sources.redhat.com/ecos/ecos-license/
// -------------------------------------------
//####ECOSGPLCOPYRIGHTEND####
//==========================================================================
//#####DESCRIPTIONBEGIN####
//
// Author(s):    mtrojer
// Date:         2007-03-08
// Purpose:      HAL board support
// Description:  Implementations of HAL board interfaces
//
//####DESCRIPTIONEND####
//
//========================================================================*/

#include <pkgconf/hal.h>

#include <cyg/infra/cyg_type.h>         // base types
#include <cyg/infra/cyg_trac.h>         // tracing macros
#include <cyg/infra/cyg_ass.h>          // assertion macros

#include <cyg/hal/hal_io.h>             // IO macros
#include <cyg/hal/hal_arch.h>           // Register state info
#include <cyg/hal/hal_diag.h>
#include <cyg/hal/hal_intr.h>           // necessary?
#include <cyg/hal/hal_cache.h>
#include <cyg/hal/hal_if.h>             // calling interface
#include <cyg/hal/hal_misc.h>           // helper functions
#ifdef CYGDBG_HAL_DEBUG_GDB_BREAK_SUPPORT
#include <cyg/hal/drv_api.h>            // HAL ISR support
#endif
#include <cyg/hal/plf_io.h>             // platform registers

static cyg_uint32 _period;

// Diagnostic LED
void set_led(int val)
{
  GPIO_Config(GPIO1, 1<<9, GPIO_OUT_PP);
  if (val)
    GPIO_BitWrite(GPIO1,9,1);
  else
    GPIO_BitWrite(GPIO1,9,0);
}

void hal_clock_initialize(cyg_uint32 period)
{
  CYG_ASSERT(period < 0x10000, "Invalid clock period");
  
  TIM_Init(TIM1);
  TIM_ClockSourceConfig(TIM1,TIM_INTERNAL);
  TIM_PrescalerConfig (TIM1, 31);
  TIM_OCMPModeConfig(TIM1, TIM_CHANNEL_A, period-4, TIM_TIMING, TIM_HIGH);
  EIC_IRQChannelPriorityConfig(T1TIMI_IRQChannel, 1);
  TIM_ITConfig(TIM1, TIM_OCA_IT,ENABLE);
  
  _period = period;
}

void hal_clock_reset(cyg_uint32 vector, cyg_uint32 period)
{
  CYG_ASSERT(period < 0x10000, "Invalid clock period");
  
  TIM_FlagClear(TIM1,TIM_OCFA);
  TIM_CounterConfig(TIM1,TIM_CLEAR);

//  HAL_INTERRUPT_ACKNOWLEDGE(CYGNUM_HAL_INTERRUPT_RTC);

    if (period != _period) {
        hal_clock_initialize(period);
    }
    _period = period;
}

void hal_clock_read(cyg_uint32 *pvalue)
{
    cyg_uint32 val;

    val = TIM_CounterValue(TIM1);
    
    *pvalue = val;
}

// -------------------------------------------------------------------------
//
// Delay for some number of micro-seconds
//
void hal_delay_us(cyg_int32 usecs)
{
  unsigned short v;
  TIM_Init ( TIM2 );
  TIM_ClockSourceConfig(TIM2,TIM_INTERNAL);
  TIM_PrescalerConfig ( TIM2, 31 );
  TIM_CounterConfig(TIM2,TIM_CLEAR);
  TIM_CounterConfig(TIM2,TIM_START);

  // Reset to 0xFFFC
  while(1) {
    v = TIM_CounterValue(TIM2);
    if (v==0xFFFC || v==0xFFFE || v==0xFFFF)
      continue;
    break;
  }
  TIM_FlagClear(TIM2,TIM_TOF);
    // Wait for the compare
  do {
    v = TIM_CounterValue(TIM2);
    if (TIM_FlagStatus(TIM2,TIM_TOF)==SET) {
        TIM_FlagClear(TIM2,TIM_TOF);
        usecs -= 65535;
    }
  } while (v<usecs);
}

// -------------------------------------------------------------------------
// Hardware init
void hal_hardware_init(void)
{
  EIC->ICR = 0x00000000;  /* Disable FIQ and IRQ interrupts */
  EIC->IER = 0x00000000;  /* Disable all channels interrupts */
  EIC->IPR = 0xFFFFFFFF;  /* Clear all pending bits */
  EIC->FIR = 0x0000000C;  /* Disable all FIQ channels interrupts and clear FIQ */
  EIC->CIPR = 0x00000000; /* channels pending bits, set the current priority level to zero */

  // Setup clocks
  RCCU_Div2Config(ENABLE);
  RCCU_MCLKConfig(RCCU_DEFAULT);
  RCCU_FCLKConfig(RCCU_DEFAULT);
  RCCU_PCLKConfig(RCCU_RCLK_2);
  RCCU_PLL1Config(RCCU_PLL1_Mul_16,RCCU_Div_2);
  RCCU_RCLKSourceConfig(RCCU_PLL1_Output);
    
  // Set up eCos/ROM interfaces
  hal_if_init();

  EIC_IRQConfig( ENABLE );
//  EIC_FIQConfig( ENABLE );
}

//
// This routine is called to respond to a hardware interrupt (IRQ).  It
// should interrogate the hardware and return the IRQ vector number.

int hal_IRQ_handler(void)
{
  unsigned int tmp, id;
  
  tmp = EIC->IVR;

  id = EIC_CurrentIRQChannelValue();
/*  if (id==T1TIMI_IRQChannel) {
    set_led(_toggle);
    _toggle = ~_toggle;
  }*/
  return id;
}

//
// Interrupt control
//

void hal_interrupt_mask(int vector)
{
  CYG_ASSERT(vector <= CYGNUM_HAL_ISR_MAX &&
      vector >= CYGNUM_HAL_ISR_MIN , "Invalid vector");

  EIC_IRQChannelConfig( vector, DISABLE );
}

void hal_interrupt_unmask(int vector)
{
  CYG_ASSERT(vector <= CYGNUM_HAL_ISR_MAX &&
      vector >= CYGNUM_HAL_ISR_MIN , "Invalid vector");

  EIC_IRQChannelConfig( vector, ENABLE );
}

void hal_interrupt_acknowledge(int vector)
{
  CYG_ASSERT(vector <= CYGNUM_HAL_ISR_MAX &&
      vector >= CYGNUM_HAL_ISR_MIN , "Invalid vector");

  EIC->IPR = 0x1<<vector;
}

void hal_interrupt_configure(int vector, int level, int up)
{
  CYG_ASSERT(vector <= CYGNUM_HAL_ISR_MAX &&
      vector >= CYGNUM_HAL_ISR_MIN , "Invalid vector");
}

void hal_interrupt_set_level(int vector, int level)
{
  CYG_ASSERT(vector <= CYGNUM_HAL_ISR_MAX &&
      vector >= CYGNUM_HAL_ISR_MIN , "Invalid vector");
  CYG_ASSERT(level >= 1 && level <= 7, "Invalid level");

  EIC_IRQChannelPriorityConfig( vector, level);
}

void hal_show_IRQ(int vector, int data, int handler)
{
  CYG_ASSERT(vector <= CYGNUM_HAL_ISR_MAX &&
      vector >= CYGNUM_HAL_ISR_MIN , "Invalid vector");
}

//--------------------------------------------------------------------------
// EOF hal_misc.c
