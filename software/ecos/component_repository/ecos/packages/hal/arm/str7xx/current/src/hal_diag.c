/*=============================================================================
//
//      hal_diag.c
//
//      HAL diagnostic output code
//
//=============================================================================
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
//=============================================================================
//#####DESCRIPTIONBEGIN####
//
// Author(s):   mtrojer
// Date:        2007-03-08
// Purpose:     HAL diagnostic output
// Description: Implementations of HAL diagnostic output support.
//
//####DESCRIPTIONEND####
//
//===========================================================================*/

#include <pkgconf/hal.h>
#include CYGBLD_HAL_PLATFORM_H

#include <cyg/infra/cyg_type.h>         // base types

#include <cyg/hal/hal_arch.h>           // SAVE/RESTORE GP macros
#include <cyg/hal/hal_io.h>             // IO macros
#include <cyg/hal/hal_if.h>             // interface API
#include <cyg/hal/hal_intr.h>           // HAL_ENABLE/MASK/UNMASK_INTERRUPTS
#include <cyg/hal/hal_misc.h>           // Helper functions
#include <cyg/hal/drv_api.h>            // CYG_ISR_HANDLED

#include <cyg/hal/plf_io.h>             //

#define UART2_Rx_Pin (0x0001<<13)  /*  TQFP 64: pin N° 5  , TQFP 144 pin N° 9 */
#define UART2_Tx_Pin (0x0001<<14)  /*  TQFP 64: pin N° 6  , TQFP 144 pin N° 10 */
#define UART3_Rx_Pin (0x0001<<1)   /*  TQFP 64: pin N° 52 , TQFP 144 pin N° 123 */
#define UART3_Tx_Pin (0x0001<<0)   /*  TQFP 64: pin N° 53 , TQFP 144 pin N° 124 */

//-----------------------------------------------------------------------------
typedef struct {
    cyg_uint8 id;
    cyg_int32 msec_timeout;
    int isr_vector;
} channel_data_t;

//-----------------------------------------------------------------------------

static void
cyg_hal_plf_serial_init_channel(void* __ch_data)
{
  cyg_uint8 id = ((channel_data_t*)__ch_data)->id;

  if (id==0) {
    // Setup UART2
    GPIO_Config(GPIO0, UART2_Tx_Pin, GPIO_AF_PP);
    GPIO_Config(GPIO0, UART2_Rx_Pin, GPIO_IN_TRI_CMOS);
    UART_Init(UART2);
    UART_OnOffConfig(UART2, ENABLE); 
    UART_FifoConfig (UART2, DISABLE);
    UART_FifoReset  (UART2 , UART_RxFIFO);
    UART_FifoReset  (UART2 , UART_TxFIFO);
    UART_LoopBackConfig(UART2 , DISABLE);
//    UART_Config(UART2,CYGNUM_HAL_VIRTUAL_VECTOR_CHANNELS_DEFAULT_BAUD,UART_NO_PARITY,UART_1_StopBits,UARTM_8D);
    UART_Config(UART2,38400,UART_NO_PARITY,UART_1_StopBits,UARTM_8D);
    UART_RxConfig(UART2 ,ENABLE);
  }
  else if (id==1) {
    // Setup UART3 
    GPIO_Config(GPIO0, UART3_Tx_Pin, GPIO_AF_PP);
    GPIO_Config(GPIO0, UART3_Rx_Pin, GPIO_IN_TRI_CMOS);
    UART_Init(UART3);
    UART_OnOffConfig(UART3, ENABLE); 
    UART_FifoConfig (UART3, DISABLE);
    UART_FifoReset  (UART3 , UART_RxFIFO);
    UART_FifoReset  (UART3 , UART_TxFIFO);
    UART_LoopBackConfig(UART3 , DISABLE);
//    UART_Config(UART3,CYGNUM_HAL_VIRTUAL_VECTOR_CHANNELS_DEFAULT_BAUD,UART_NO_PARITY,UART_1_StopBits,UARTM_8D);
	UART_Config(UART3,38400,UART_NO_PARITY,UART_1_StopBits,UARTM_8D);
	UART_RxConfig(UART3 ,ENABLE);
  }
}

void
cyg_hal_plf_serial_putc(void *__ch_data, char c)
{
  cyg_uint8 id = ((channel_data_t*)__ch_data)->id;

  CYGARC_HAL_SAVE_GP();
  
  if (id==0)
    UART_ByteSend(UART2, &c);
  else if (id==1)
    UART_ByteSend(UART3, &c);
  
  CYGARC_HAL_RESTORE_GP();
}

static cyg_bool
cyg_hal_plf_serial_getc_nonblock(void* __ch_data, cyg_uint8* ch)
{
  cyg_uint8 id = ((channel_data_t*)__ch_data)->id;
  u16 UARTStatus;
  
  if (id==0) {
    if(!(UART_FlagStatus(UART2) & UART_RxBufFull))
      return false; 
    UARTStatus = UART_ByteReceive(UART2, ch, 0xFF);
  }
  else if (id==1) {
    if(!(UART_FlagStatus(UART3) & UART_RxBufFull))
      return false; 
    UARTStatus = UART_ByteReceive(UART3, ch, 0xFF);
  }
  return true;
}

cyg_uint8
cyg_hal_plf_serial_getc(void* __ch_data)
{
    cyg_uint8 ch;
    CYGARC_HAL_SAVE_GP();

    while(!cyg_hal_plf_serial_getc_nonblock(__ch_data, &ch));

    CYGARC_HAL_RESTORE_GP();
    return ch;
}

static void
cyg_hal_plf_serial_write(void* __ch_data, const cyg_uint8* __buf, 
                         cyg_uint32 __len)
{
    CYGARC_HAL_SAVE_GP();

    while(__len-- > 0)
        cyg_hal_plf_serial_putc(__ch_data, *__buf++);

    CYGARC_HAL_RESTORE_GP();
}

static void
cyg_hal_plf_serial_read(void* __ch_data, cyg_uint8* __buf, cyg_uint32 __len)
{
    CYGARC_HAL_SAVE_GP();

    while(__len-- > 0)
        *__buf++ = cyg_hal_plf_serial_getc(__ch_data);

    CYGARC_HAL_RESTORE_GP();
}

cyg_bool
cyg_hal_plf_serial_getc_timeout(void* __ch_data, cyg_uint8* ch)
{
    int delay_count;
    channel_data_t* chan = (channel_data_t*)__ch_data;
    cyg_bool res;
    CYGARC_HAL_SAVE_GP();

    delay_count = chan->msec_timeout * 10; // delay in .1 ms steps

    for(;;) {
        res = cyg_hal_plf_serial_getc_nonblock(__ch_data, ch);
        if (res || 0 == delay_count--)
            break;

        CYGACC_CALL_IF_DELAY_US(100);
    }

    CYGARC_HAL_RESTORE_GP();
    return res;
}

static int
cyg_hal_plf_serial_control(void *__ch_data, __comm_control_cmd_t __func, ...)
{
    static int irq_state = 0;
    channel_data_t* chan = (channel_data_t*)__ch_data;
    cyg_uint8 id = ((channel_data_t*)__ch_data)->id;
    int ret = 0;
    CYGARC_HAL_SAVE_GP();

    switch (__func) {
    case __COMMCTL_IRQ_ENABLE:
        irq_state = 1;
        HAL_INTERRUPT_ACKNOWLEDGE(chan->isr_vector);
        HAL_INTERRUPT_UNMASK(chan->isr_vector);

        if (id==0) {
          EIC_IRQChannelPriorityConfig(UART2_IRQChannel, 1);
          EIC_IRQChannelConfig(UART2_IRQChannel, ENABLE);
          EIC_IRQConfig(ENABLE);
          UART_ItConfig(UART2,UART_RxBufFull, ENABLE);
        }
        else if (id==1) {
          EIC_IRQChannelPriorityConfig(UART3_IRQChannel, 1);
          EIC_IRQChannelConfig(UART3_IRQChannel, ENABLE);
          EIC_IRQConfig(ENABLE);
          UART_ItConfig(UART3,UART_RxBufFull, ENABLE);
        }
        break;
    case __COMMCTL_IRQ_DISABLE:
        ret = irq_state;
        irq_state = 0;
        HAL_INTERRUPT_MASK(chan->isr_vector);
        
        if (id==0) {
          EIC_IRQChannelPriorityConfig(UART2_IRQChannel, 1);
          EIC_IRQChannelConfig(UART2_IRQChannel, DISABLE);
          EIC_IRQConfig(DISABLE);
          UART_ItConfig(UART2,UART_RxBufFull, DISABLE);
        }
        else if (id==1) {
          EIC_IRQChannelPriorityConfig(UART3_IRQChannel, 1);
          EIC_IRQChannelConfig(UART3_IRQChannel, DISABLE);
          EIC_IRQConfig(DISABLE);
          UART_ItConfig(UART3,UART_RxBufFull, DISABLE);
        }
        break;
    case __COMMCTL_DBG_ISR_VECTOR:
        ret = chan->isr_vector;
        break;
    case __COMMCTL_SET_TIMEOUT:
    {
        va_list ap;

        va_start(ap, __func);

        ret = chan->msec_timeout;
        chan->msec_timeout = va_arg(ap, cyg_uint32);

        va_end(ap);
    }        
    default:
        break;
    }
    CYGARC_HAL_RESTORE_GP();
    return ret;
}

static int
cyg_hal_plf_serial_isr(void *__ch_data, int* __ctrlc, 
                       CYG_ADDRWORD __vector, CYG_ADDRWORD __data)
{
    int res = 0;
    channel_data_t* chan = (channel_data_t*)__ch_data;
    cyg_uint8 id = chan->id;
    cyg_uint8 ch;
    u16 UARTStatus;
    CYGARC_HAL_SAVE_GP();

    *__ctrlc = 0;
 
    if (id==0) {
      if((UART_FlagStatus(UART2) & UART_RxBufFull)) {
        UARTStatus = UART_ByteReceive(UART2, ch, 0xFF);
        if( cyg_hal_is_break( &ch , 1 ) )
          *__ctrlc = 1;
        res = CYG_ISR_HANDLED;
      }
    }
    else if (id==1) {
      if((UART_FlagStatus(UART3) & UART_RxBufFull))  {
        UARTStatus = UART_ByteReceive(UART3, ch, 0xFF);
        if( cyg_hal_is_break( &ch , 1 ) )
          *__ctrlc = 1;
        res = CYG_ISR_HANDLED;
      }
    }

    HAL_INTERRUPT_ACKNOWLEDGE(chan->isr_vector);
    
    CYGARC_HAL_RESTORE_GP();
    return res;
}

static channel_data_t str7xx_ser_channels[2] = {
  { 0, 1000, CYGNUM_HAL_INTERRUPT_UART2IRQ},
  { 1, 1000, CYGNUM_HAL_INTERRUPT_UART3IRQ}
};

static void
cyg_hal_plf_serial_init(void)
{
    hal_virtual_comm_table_t* comm;
    int cur;

    cur = CYGACC_CALL_IF_SET_CONSOLE_COMM(CYGNUM_CALL_IF_SET_COMM_ID_QUERY_CURRENT);

    // Init channels
    cyg_hal_plf_serial_init_channel(&str7xx_ser_channels[0]);
#if CYGNUM_HAL_VIRTUAL_VECTOR_COMM_CHANNELS > 1
    cyg_hal_plf_serial_init_channel(&str7xx_ser_channels[1]);
#endif

    // Setup procs in the vector table

    // Set channel 0
    CYGACC_CALL_IF_SET_CONSOLE_COMM(0);
    comm = CYGACC_CALL_IF_CONSOLE_PROCS();
    CYGACC_COMM_IF_CH_DATA_SET(*comm, &str7xx_ser_channels[0]);
    CYGACC_COMM_IF_WRITE_SET(*comm, cyg_hal_plf_serial_write);
    CYGACC_COMM_IF_READ_SET(*comm, cyg_hal_plf_serial_read);
    CYGACC_COMM_IF_PUTC_SET(*comm, cyg_hal_plf_serial_putc);
    CYGACC_COMM_IF_GETC_SET(*comm, cyg_hal_plf_serial_getc);
    CYGACC_COMM_IF_CONTROL_SET(*comm, cyg_hal_plf_serial_control);
    CYGACC_COMM_IF_DBG_ISR_SET(*comm, cyg_hal_plf_serial_isr);
    CYGACC_COMM_IF_GETC_TIMEOUT_SET(*comm, cyg_hal_plf_serial_getc_timeout);

#if CYGNUM_HAL_VIRTUAL_VECTOR_COMM_CHANNELS > 1
    // Set channel 1
    CYGACC_CALL_IF_SET_CONSOLE_COMM(1);
    comm = CYGACC_CALL_IF_CONSOLE_PROCS();
    CYGACC_COMM_IF_CH_DATA_SET(*comm, &str7xx_ser_channels[1]);
    CYGACC_COMM_IF_WRITE_SET(*comm, cyg_hal_plf_serial_write);
    CYGACC_COMM_IF_READ_SET(*comm, cyg_hal_plf_serial_read);
    CYGACC_COMM_IF_PUTC_SET(*comm, cyg_hal_plf_serial_putc);
    CYGACC_COMM_IF_GETC_SET(*comm, cyg_hal_plf_serial_getc);
    CYGACC_COMM_IF_CONTROL_SET(*comm, cyg_hal_plf_serial_control);
    CYGACC_COMM_IF_DBG_ISR_SET(*comm, cyg_hal_plf_serial_isr);
    CYGACC_COMM_IF_GETC_TIMEOUT_SET(*comm, cyg_hal_plf_serial_getc_timeout);
#endif

    // Restore original console
    CYGACC_CALL_IF_SET_CONSOLE_COMM(cur);
}

void
cyg_hal_plf_comms_init(void)
{
    static int initialized = 0;

    if (initialized)
        return;

    initialized = 1;

    cyg_hal_plf_serial_init();
}

//-----------------------------------------------------------------------------
// End of hal_diag.c
