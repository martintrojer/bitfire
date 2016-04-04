# Makefile
# mtrojer@arrownordic.com
# $WCREV$

#TOOLDIR=/cygdrive/c/Bitfire/GNUARM
CC=$(TOOLDIR)/bin/arm-elf-gcc
LD=$(TOOLDIR)/bin/arm-elf-ld
AR=$(TOOLDIR)/bin/arm-elf-ar
OC=$(TOOLDIR)/bin/arm-elf-objcopy
OD=$(TOOLDIR)/bin/arm-elf-objdump

UCOSDIR = ../../../../../uCOS-II

CC_FLAGS = -mcpu=arm7tdmi -O2
CC_INC = -I. -I../bsp -I$(UCOSDIR)/Source -I$(UCOSDIR)/Ports/ARM/Generic/GNU

TARGET = libucosii.a
OBJUCOS = os_core.o os_flag.o os_mbox.o os_mem.o os_mutex.o os_q.o os_sem.o os_task.o os_time.o
OBJUCOSPORT = os_cpu_c.o os_dbg.o
OBJUCOSPORTA = os_cpu_a.o 

SRCUCOS= $(UCOSDIR)/Source/os_core.c $(UCOSDIR)/Source/os_flag.c $(UCOSDIR)/Source/os_mbox.c $(UCOSDIR)/Source/os_mem.c $(UCOSDIR)/Source/os_mutex.c $(UCOSDIR)/Source/os_q.c $(UCOSDIR)/Source/os_sem.c $(UCOSDIR)/Source/os_task.c $(UCOSDIR)/Source/os_time.c
SRCUCOSPORT =  $(UCOSDIR)/Ports/ARM/Generic/GNU/os_cpu_c.c $(UCOSDIR)/Ports/ARM/Generic/GNU/os_dbg.c
SRCUCOSPORTA = $(UCOSDIR)/Ports/ARM/Generic/GNU/os_cpu_a.s

$(TARGET): $(OBJUCOS) $(OBJUCOSPORTA) $(OBJUCOSPORT) 
	$(AR) -r $(TARGET) $(OBJUCOS) $(OBJUCOSPORTA) $(OBJUCOSPORT)

$(OBJUCOS): $(SRCUCOS)
	$(CC) -c $(CC_FLAGS) $(CC_INC) $(UCOSDIR)/Source/$*.c

$(OBJUCOSPORT): $(SRCUCOSPORT)
	$(CC) -c $(CC_FLAGS) $(CC_INC) $(UCOSDIR)/Ports/ARM/Generic/GNU/$*.c

$(OBJUCOSPORTA): $(SRCUCOSPORTA)
	$(CC) -c $(CC_FLAGS) $(CC_INC) $(UCOSDIR)/Ports/ARM/Generic/GNU/$*.s

clean:
	rm -f $(TARGET) $(OBJUCOS) $(OBJUCOSPORTA) $(OBJUCOSPORT)
