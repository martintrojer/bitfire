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
AF_LD_SCRIPT_RAM=../../../bsp/atmosfire/ldscripts/ram_gnu.ld
AF_LD_SCRIPT_ROM=../../../bsp/atmosfire/ldscripts/flash_gnu.ld

CC_FLAGS = -mcpu=arm7tdmi -g
CC_INC = -I../../../inc -I../../../gfxlib/inc -I../../../bsp/bitfire/inc

BF_LD_FLAGS = -L../../../bsp/bitfire/lib -L../../../gfxlib/lib -lgfx
AF_LD_FLAGS = -L../../../bsp/atmosfire/lib -L../../../gfxlib/lib -lgfx -latmosfirebsp
LD_FLAGS2 = -L$(TOOLDIR)/arm-elf/lib -L$(TOOLDIR)/lib/gcc/arm-elf/$(GCC_VER) -lc -lm -lgcc --no-warn-mismatch

BF_TARGET = gfxlib_effects_bitfire.elf
AF_TARGET = gfxlib_effects_atmosfire.elf

OBJS = gfxlib_effects.o

BF_STARTOBJ = bitfire_startup_gnu.o
AF_STARTOBJ = atmosfire_startup_gnu.o

BF_STARTSRC = ../../../bsp/bitfire/src/bitfire_startup_gnu.s
AF_STARTSRC = ../../../bsp/atmosfire/src/atmosfire_startup_gnu.s

all: bitfire-flash

bitfire-flash: $(BF_STARTOBJ) $(OBJS) 
	$(LD) -o gfxlib_effects_bitfire00.elf $(BF_STARTOBJ) $(OBJS) -T$(BF_LD_SCRIPT_ROM) $(BF_LD_FLAGS) -lbitfirebsp00 $(LD_FLAGS2)
	$(OC) -O ihex gfxlib_effects_bitfire00.elf gfxlib_effects_bitfire00.hex
	$(LD) -o gfxlib_effects_bitfire01.elf $(BF_STARTOBJ) $(OBJS) -T$(BF_LD_SCRIPT_ROM) $(BF_LD_FLAGS) -lbitfirebsp01 $(LD_FLAGS2)
	$(OC) -O ihex gfxlib_effects_bitfire01.elf gfxlib_effects_bitfire01.hex
	$(LD) -o gfxlib_effects_bitfire02.elf $(BF_STARTOBJ) $(OBJS) -T$(BF_LD_SCRIPT_ROM) $(BF_LD_FLAGS) -lbitfirebsp02 $(LD_FLAGS2)
	$(OC) -O ihex gfxlib_effects_bitfire02.elf gfxlib_effects_bitfire02.hex
	$(LD) -o gfxlib_effects_bitfire10.elf $(BF_STARTOBJ) $(OBJS) -T$(BF_LD_SCRIPT_ROM) $(BF_LD_FLAGS) -lbitfirebsp10 $(LD_FLAGS2)
	$(OC) -O ihex gfxlib_effects_bitfire10.elf gfxlib_effects_bitfire10.hex
	$(LD) -o gfxlib_effects_bitfire11.elf $(BF_STARTOBJ) $(OBJS) -T$(BF_LD_SCRIPT_ROM) $(BF_LD_FLAGS) -lbitfirebsp11 $(LD_FLAGS2)
	$(OC) -O ihex gfxlib_effects_bitfire11.elf gfxlib_effects_bitfire11.hex
	$(LD) -o gfxlib_effects_bitfire12.elf $(BF_STARTOBJ) $(OBJS) -T$(BF_LD_SCRIPT_ROM) $(BF_LD_FLAGS) -lbitfirebsp12 $(LD_FLAGS2)
	$(OC) -O ihex gfxlib_effects_bitfire12.elf gfxlib_effects_bitfire12.hex
	$(LD) -o gfxlib_effects_bitfire20.elf $(BF_STARTOBJ) $(OBJS) -T$(BF_LD_SCRIPT_ROM) $(BF_LD_FLAGS) -lbitfirebsp20 $(LD_FLAGS2)
	$(OC) -O ihex gfxlib_effects_bitfire20.elf gfxlib_effects_bitfire20.hex
	$(LD) -o gfxlib_effects_bitfire21.elf $(BF_STARTOBJ) $(OBJS) -T$(BF_LD_SCRIPT_ROM) $(BF_LD_FLAGS) -lbitfirebsp21 $(LD_FLAGS2)
	$(OC) -O ihex gfxlib_effects_bitfire21.elf gfxlib_effects_bitfire21.hex
	$(LD) -o gfxlib_effects_bitfire22.elf $(BF_STARTOBJ) $(OBJS) -T$(BF_LD_SCRIPT_ROM) $(BF_LD_FLAGS) -lbitfirebsp22 $(LD_FLAGS2)
	$(OC) -O ihex gfxlib_effects_bitfire22.elf gfxlib_effects_bitfire22.hex

bitfire-ram: $(BF_STARTOBJ) $(OBJS)
	$(LD) -o $(BF_TARGET) $(BF_STARTOBJ) $(OBJS) -T$(BF_LD_SCRIPT_RAM) $(BF_LD_FLAGS) $(LD_FLAGS2)
	$(OC) -O ihex $(BF_TARGET) $(BF_TARGET:.elf=.hex)
	$(OD) -d $(BF_TARGET) > $(BF_TARGET:.elf=.objdump)

atmosfire-flash: $(AF_STARTOBJ) $(OBJS) 
	$(LD) -o $(AF_TARGET) $(AF_STARTOBJ) $(OBJS) -T$(AF_LD_SCRIPT_ROM) $(AF_LD_FLAGS) $(LD_FLAGS2)
	$(OC) -O ihex $(AF_TARGET) $(AF_TARGET:.elf=.hex)
	$(OD) -d $(AF_TARGET) > $(AF_TARGET:.elf=.objdump)

atmosfire-ram: $(AF_STARTOBJ) $(OBJS)
	$(LD) -o $(AF_TARGET) $(AF_STARTOBJ) $(OBJS) -T$(AF_LD_SCRIPT_RAM) $(AF_LD_FLAGS) $(LD_FLAGS2)
	$(OC) -O ihex $(AF_TARGET) $(AF_TARGET:.elf=.hex)
	$(OD) -d $(AF_TARGET) > $(AF_TARGET:.elf=.objdump)

$(OBJS): %.o: %.c
	$(CC) -c $(CC_FLAGS) $(CC_INC) $< -o $@

$(BF_STARTOBJ): $(BF_STARTSRC)
	$(CC) -c $(CC_FLAGS) $(BF_CC_INC) ../../../bsp/bitfire/src/$*.s

$(AF_STARTOBJ): $(AF_STARTSRC)
	$(CC) -c $(CC_FLAGS) $(AF_CC_INC) ../../../bsp/atmosfire/src/$*.s

clean:
	rm -f $(BF_TARGET) $(AF_TARGET) $(OBJS) $(BF_STARTOBJ) $(AF_STARTOBJ) $(BF_TARGET:.elf=.hex) $(AF_TARGET:.elf=.hex) $(BF_TARGET:.elf=.objdump) $(AF_TARGET:.elf=.objdump) *.elf *.hex
