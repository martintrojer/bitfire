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

BF_LD_SCRIPT_RAM=../../../bsp/bitfire/ldscripts/ram_gnu.ld
BF_LD_SCRIPT_ROM=../../../bsp/bitfire/ldscripts/flash_gnu.ld

CC_FLAGS = -mcpu=arm7tdmi -g -O
CC_INC = -I../../../inc -I../../../gfxlib/inc -I../../../bsp/bitfire/inc

BF_LD_FLAGS = -L../../../bsp/bitfire/lib -L../../../gfxlib/lib -lgfx
LD_FLAGS2 = -L$(TOOLDIR)/arm-elf/lib -L$(TOOLDIR)/lib/gcc/arm-elf/$(GCC_VER) -lc -lm -lgcc --no-warn-mismatch

BF_TARGET = muti_demo.elf

OBJS = multi_demo.o

BF_STARTOBJ = bitfire_startup_gnu.o

BF_STARTSRC = ../../../bsp/bitfire/src/bitfire_startup_gnu.s

all: bitfire-flash

bitfire-flash: $(BF_STARTOBJ) $(OBJS) 
	$(LD) -o multi_demo00.elf $(BF_STARTOBJ) $(OBJS) -T$(BF_LD_SCRIPT_ROM) $(BF_LD_FLAGS) -lbitfirebsp00 $(LD_FLAGS2)
	$(OC) -O ihex multi_demo00.elf multi_demo00.hex
	$(LD) -o multi_demo01.elf $(BF_STARTOBJ) $(OBJS) -T$(BF_LD_SCRIPT_ROM) $(BF_LD_FLAGS) -lbitfirebsp01 $(LD_FLAGS2)
	$(OC) -O ihex multi_demo01.elf multi_demo01.hex
	$(LD) -o multi_demo02.elf $(BF_STARTOBJ) $(OBJS) -T$(BF_LD_SCRIPT_ROM) $(BF_LD_FLAGS) -lbitfirebsp02 $(LD_FLAGS2)
	$(OC) -O ihex multi_demo02.elf multi_demo02.hex
	$(LD) -o multi_demo10.elf $(BF_STARTOBJ) $(OBJS) -T$(BF_LD_SCRIPT_ROM) $(BF_LD_FLAGS) -lbitfirebsp10 $(LD_FLAGS2)
	$(OC) -O ihex multi_demo10.elf multi_demo10.hex
	$(LD) -o multi_demo11.elf $(BF_STARTOBJ) $(OBJS) -T$(BF_LD_SCRIPT_ROM) $(BF_LD_FLAGS) -lbitfirebsp11 $(LD_FLAGS2)
	$(OC) -O ihex multi_demo11.elf multi_demo11.hex
	$(LD) -o multi_demo12.elf $(BF_STARTOBJ) $(OBJS) -T$(BF_LD_SCRIPT_ROM) $(BF_LD_FLAGS) -lbitfirebsp12 $(LD_FLAGS2)
	$(OC) -O ihex multi_demo12.elf multi_demo12.hex
	$(LD) -o multi_demo20.elf $(BF_STARTOBJ) $(OBJS) -T$(BF_LD_SCRIPT_ROM) $(BF_LD_FLAGS) -lbitfirebsp20 $(LD_FLAGS2)
	$(OC) -O ihex multi_demo20.elf multi_demo20.hex
	$(LD) -o multi_demo21.elf $(BF_STARTOBJ) $(OBJS) -T$(BF_LD_SCRIPT_ROM) $(BF_LD_FLAGS) -lbitfirebsp21 $(LD_FLAGS2)
	$(OC) -O ihex multi_demo21.elf multi_demo21.hex
	$(LD) -o multi_demo22.elf $(BF_STARTOBJ) $(OBJS) -T$(BF_LD_SCRIPT_ROM) $(BF_LD_FLAGS) -lbitfirebsp22 $(LD_FLAGS2)
	$(OC) -O ihex multi_demo22.elf multi_demo22.hex

$(OBJS): %.o: %.c
	$(CC) -c $(CC_FLAGS) $(CC_INC) $< -o $@

$(BF_STARTOBJ): $(BF_STARTSRC)
	$(CC) -c $(CC_FLAGS) $(BF_CC_INC) ../../../bsp/bitfire/src/$*.s

clean:
	rm -f $(BF_TARGET) $(OBJS) $(BF_STARTOBJ) $(BF_TARGET:.elf=.hex) $(BF_TARGET:.elf=.objdump) *.elf *.hex
