set remote hardware-breakpoint-limit 2

#tar rem 192.168.123.157:3333
tar rem localhost:3333

#mon reset
mon arm7_9 sw_bkpts enable
#mon arm7_9 force_hw_bkpts enable

#b Reset_Handler
#b UndefinedHandler
#b SWIHandler
#b PrefetchAbortHandler
#b DataAbortHandler
#b IRQHandler
#b FIQHandler

#b T0TIMIIRQHandler
#b FLASHIRQHandler
#b RCCUIRQHandler
#b RTCIRQHandler
#b WDGIRQHandler
#b XTIIRQHandler
b USBHPIRQHandler
#b I2C0ITERRIRQHandler
#b I2C1ITERRIRQHandler
#b UART0IRQHandler
#b UART1IRQHandler
#b UART2IRQHandler
#b UART3IRQHandler
#b BSPI0IRQHandler
#b BSPI1IRQHandler
#b I2C0IRQHandler
#b I2C1IRQHandler
#b CANIRQHandler
#b ADC12IRQHandler
#b T1TIMIIRQHandler
#b T2TIMIIRQHandler
#b T3TIMIIRQHandler
#b HDLCIRQHandler
b USBLPIRQHandler
#b T0TOIIRQHandler
#b T0OC1IRQHandler
#b T0OC2IRQHandler

b USB_Istr


#b main
#load

