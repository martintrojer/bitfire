#set remote hardware-breakpoint-limit 2

#tar rem 192.160.0.171:3333
tar rem localhost:3333

#mon reset
mon arm7_9 sw_bkpts enable
#mon arm7_9 force_hw_bkpts enable

#b main
#load

