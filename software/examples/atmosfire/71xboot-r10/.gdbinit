#set remote hardware-breakpoint-limit 2
#tar rem 192.168.123.157:3333
tar rem 127.0.0.1:3333

#mon arm7_9 sw_bkpts enable
mon arm7_9 force_hw_bkpts enable

#set $pc=0x40000000
#hbreak main
