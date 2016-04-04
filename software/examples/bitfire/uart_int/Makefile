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

LD_SCRIPT_RAM=../../../bsp/bitfire/ldscripts/ram_gnu.ld
LD_SCRIPT_ROM=../../../bsp/bitfire/ldscripts/flash_gnu.ld

CC_FLAGS = -mcpu=arm7tdmi -g
CC_INC = -I../../../inc -I../../../bsp/bitfire/inc

LD_FLAGS = -L../../../bsp/bitfire/lib -lbitfirebsp
LD_FLAGS2 = -L$(TOOLDIR)/arm-elf/lib -L$(TOOLDIR)/lib/gcc/arm-elf/$(GCC_VER) -lc -lm -lgcc --no-warn-mismatch

TARGET = uart_int.elf
OBJS = uart_int.o

STARTOBJ = bitfire_startup_gnu.o
STARTSRC = ../../../bsp/bitfire/src/bitfire_startup_gnu.s

all: ram

flash: $(STARTOBJ) $(OBJS) 
	$(LD) -o $(TARGET) $(STARTOBJ) $(OBJS) -T$(LD_SCRIPT_ROM) $(LD_FLAGS) $(LD_FLAGS2)
	$(OC) -O ihex $(TARGET) $(TARGET:.elf=.hex)
	$(OD) -d $(TARGET) > $(TARGET:.elf=.objdump)

ram: $(STARTOBJ) $(OBJS)
	$(LD) -o $(TARGET) $(STARTOBJ) $(OBJS) -T$(LD_SCRIPT_RAM) $(LD_FLAGS) $(LD_FLAGS2)
	$(OC) -O ihex $(TARGET) $(TARGET:.elf=.hex)
	$(OD) -d $(TARGET) > $(TARGET:.elf=.objdump)

$(OBJS): %.o: %.c
	$(CC) -c $(CC_FLAGS) $(CC_INC) $< -o $@

$(STARTOBJ): $(STARTSRC)
	$(CC) -c $(CC_FLAGS) $(CC_INC) ../../../bsp/bitfire/src/$*.s

clean:
	rm -f $(TARGET) $(OBJS) $(STARTOBJ) $(TARGET:.elf=.hex) $(TARGET:.elf=.objdump)
