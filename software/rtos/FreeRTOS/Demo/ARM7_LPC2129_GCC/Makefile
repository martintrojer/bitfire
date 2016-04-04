# Makefile
# mtrojer@arrownordic.com
# $WCREV$

#TOOLDIR=/opt/arm-elf
#GCC_VER=4.1.2
CC=$(TOOLDIR)/bin/arm-elf-gcc
LD=$(TOOLDIR)/bin/arm-elf-ld
AR=$(TOOLDIR)/bin/arm-elf-ar
OC=$(TOOLDIR)/bin/arm-elf-objcopy
OD=$(TOOLDIR)/bin/arm-elf-objdump

BFINC = ../../../../inc
BSPDIR = ../../../../bsp/bitfire
GFXDIR = ../../../../gfxlib

LD_SCRIPT_ROM=flash_gnu.ld

CC_FLAGS = -mcpu=arm7tdmi -g -D RUN_FROM_ROM -D GCC_ARM7
CC_INC = -I$(BFINC) -I$(BSPDIR)/inc -I$(GFXDIR)/inc -I. -I../../Source/include

LD_FLAGS = -L$(BSPDIR)/lib -L$(GFXDIR)/lib -lgfx -lbitfirebsp
LD_FLAGS2 = -L$(TOOLDIR)/arm-elf/lib -L$(TOOLDIR)/lib/gcc/arm-elf/$(GCC_VER) -lc -lm -lgcc --no-warn-mismatch

TARGET = rtosdemo.elf
OBJS = main.o
OBJSRC = list.o queue.o tasks.o
OBJPORT = port.o portISR.o
OBJMEM = heap_2.o

SRC = main.c
SRCSRC = ../../Source/list.c ../../Source/queue.c ../../Source/tasks.c
SRCPORT = ../../Source/portable/GCC/ARM7_LPC2000/port.c ../../Source/portable/GCC/ARM7_LPC2000/portISR.c
SRCMEM = ../../Source/portable/MemMang/heap_2.c

STARTOBJ = startup_gnu.o
STARTSRC = startup_gnu.s

default: $(STARTOBJ) $(OBJS) $(OBJSRC) $(OBJPORT) $(OBJMEM) 
	$(LD) -o $(TARGET) $(STARTOBJ) $(OBJS) $(OBJMEM) $(OBJSRC) $(OBJPORT) -T$(LD_SCRIPT_ROM) $(LD_FLAGS) $(LD_FLAGS2)
	$(OC) -O ihex $(TARGET) $(TARGET:.elf=.hex)
	$(OD) -d $(TARGET) > $(TARGET:.elf=.objdump)

ram: $(STARTOBJ) $(OBJS) $(OBJSRC) $(OBJPORT) $(OBJMEM)
	$(LD) -o $(TARGET) $(STARTOBJ) $(OBJS) $(OBJMEM) $(OBJSRC) $(OBJPORT) -T$(LD_SCRIPT_RAM) $(LD_FLAGS) $(LD_FLAGS2)
	$(OC) -O ihex $(TARGET) $(TARGET:.elf=.hex)
	$(OD) -d $(TARGET) > $(TARGET:.elf=.objdump)

$(OBJS): $(SRC)
	$(CC) -c $(CC_FLAGS) $(CC_INC) $*.c

$(OBJSRC): $(SRCSRC)
	$(CC) -c $(CC_FLAGS) $(CC_INC) ../../Source/$*.c

$(OBJPORT): $(SRCPORT)
	$(CC) -c $(CC_FLAGS) $(CC_INC) ../../Source/portable/GCC/ARM7_LPC2000/$*.c

$(OBJMEM): $(SRCMEM)
	$(CC) -c $(CC_FLAGS) $(CC_INC) ../../Source/portable/MemMang/$*.c

$(STARTOBJ): $(STARTSRC)
	$(CC) -c $(CC_FLAGS) $(CC_INC) $*.s

clean:
	rm -f $(TARGET) $(OBJS) $(OBJSRC) $(OBJPORT) $(OBJMEM) $(STARTOBJ) $(TARGET:.elf=.hex) $(TARGET:.elf=.objdump)
