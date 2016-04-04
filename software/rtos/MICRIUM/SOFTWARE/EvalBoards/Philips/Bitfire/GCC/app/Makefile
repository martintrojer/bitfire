# Makefile
# mtrojer@arrownordic.com
# $WCREV$

#TOOLDIR=/cygdrive/c/Bitfire/GNUARM
#GCC_VER=4.1.2
CC=$(TOOLDIR)/bin/arm-elf-gcc
LD=$(TOOLDIR)/bin/arm-elf-ld
AR=$(TOOLDIR)/bin/arm-elf-ar
OC=$(TOOLDIR)/bin/arm-elf-objcopy
OD=$(TOOLDIR)/bin/arm-elf-objdump

UCOSDIR = ../../../../../uCOS-II
ROOTDIR = ../../../../../../../..

LD_SCRIPT_RAM=$(ROOTDIR)/src/ram_gnu.ld
LD_SCRIPT_ROM=../bsp/flash_gnu.ld

CC_FLAGS = -mcpu=arm7tdmi -g
CC_INC = -I. -I../bsp -I../lib -I$(UCOSDIR)/Source -I$(UCOSDIR)/Ports/ARM/Generic/GNU -I$(ROOTDIR)/inc -I$(ROOTDIR)/bsp/bitfire/inc -I$(ROOTDIR)/gfxlib/inc

LD_FLAGS = -L$(ROOTDIR)/bsp/bitfire/lib -L$(ROOTDIR)/gfxlib/lib -L../lib -lucosii -lgfx -lbitfirebsp
LD_FLAGS2 = -L$(TOOLDIR)/arm-elf/lib -L$(TOOLDIR)/lib/gcc/arm-elf/$(GCC_VER) -lc -lm -lgcc --no-warn-mismatch

TARGET = rtosdemo.elf
OBJS = app.o
OBJBSP = bsp.o

SRC = app.c
SRCBSP = ../bsp/bsp.c

STARTOBJ = startup_gnu.o
STARTSRC = ../bsp/startup_gnu.s

default: $(STARTOBJ) $(OBJS) $(OBJBSP)
	$(LD) -o $(TARGET) $(STARTOBJ) $(OBJS) $(OBJBSP) -T$(LD_SCRIPT_ROM) $(LD_FLAGS) $(LD_FLAGS2)
	$(OC) -O ihex $(TARGET) $(TARGET:.elf=.hex)
	$(OD) -d $(TARGET) > $(TARGET:.elf=.objdump)

ram: $(STARTOBJ) $(OBJS) $(OBJBSP)
	$(LD) -o $(TARGET) $(STARTOBJ) $(OBJS) $(OBJBSP) -T$(LD_SCRIPT_RAM) $(LD_FLAGS) $(LD_FLAGS2)
	$(OC) -O ihex $(TARGET) $(TARGET:.elf=.hex)
	$(OD) -d $(TARGET) > $(TARGET:.elf=.objdump)

$(OBJS): $(SRC)
	$(CC) -c $(CC_FLAGS) $(CC_INC) $*.c

$(OBJBSP): $(SRCBSP)
	$(CC) -c $(CC_FLAGS) $(CC_INC) ../bsp/$*.c

$(STARTOBJ): $(STARTSRC)
	$(CC) -c $(CC_FLAGS) $(CC_INC) ../bsp/$*.s

clean:
	rm -f $(TARGET) $(STARTOBJ) $(OBJS) $(OBJBSP) $(TARGET:.elf=.hex) $(TARGET:.elf=.objdump)
