#set remote hardware-breakpoint-limit 2

#tar rem 192.168.123.157:3333
tar rem localhost:3333

#mon reset
mon arm7_9 sw_bkpts enable
#mon arm7_9 force_hw_bkpts enable

#b flash_hwr_init
#b flash_erase_block
#b flash_read_buf
#b flash_hwr_map_error
#b flash_code_overlaps

#b main
#load

