set remote hardware-breakpoint-limit 2

#tar rem 192.168.123.157:3333
tar rem localhost:3333

#mon reset
mon arm7_9 sw_bkpts enable
#mon arm7_9 force_hw_bkpts enable

#b hal_clock_initialize
#b hal_clock_reset
#b hal_clock_read
#b hal_delay_us
#b hal_hardware_init
#b hal_IRQ_handler
#b hal_interrupt_mask
#b hal_interrupt_unmask
#b hal_interrupt_acknowledge
#b hal_interrupt_configure
#b hal_interrupt_set_level
#b hal_show_IRQ

#b cyg_hal_plf_serial_init_channel
#b cyg_hal_plf_serial_putc
#b cyg_hal_plf_serial_getc
#b cyg_hal_plf_serial_control
#b cyg_hal_plf_serial_isr
#b cyg_hal_plf_serial_init

#b str7xx_serial_config_port
#b str7xx_serial_init
#b str7xx_serial_putc
#b str7xx_serial_getc

#b flash_hwr_init
#b flash_erase_block
#b flash_program_buf

#b main
#load

