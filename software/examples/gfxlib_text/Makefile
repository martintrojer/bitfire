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

BF_LD_SCRIPT_RAM=../../bsp/bitfire/ldscripts/ram_gnu.ld
BF_LD_SCRIPT_ROM=../../bsp/bitfire/ldscripts/flash_gnu.ld
AF_LD_SCRIPT_RAM=../../bsp/atmosfire/ldscripts/ram_gnu.ld
AF_LD_SCRIPT_ROM=../../bsp/atmosfire/ldscripts/flash_gnu.ld

CC_FLAGS = -mcpu=arm7tdmi -g
CC_INC = -I../../inc -I../../gfxlib/inc

BF_LD_FLAGS = -L../../bsp/bitfire/lib -L../../gfxlib/lib -lgfx -lbitfirebsp
AF_LD_FLAGS = -L../../bsp/atmosfire/lib -L../../gfxlib/lib -lgfx -latmosfirebsp
LD_FLAGS2 = -L$(TOOLDIR)/arm-elf/lib -L$(TOOLDIR)/lib/gcc/arm-elf/$(GCC_VER) -lc -lm -lgcc --no-warn-mismatch

BF_TARGET = gfxlib_text_bitfire.elf
AF_TARGET = gfxlib_text_atmosfire.elf

OBJS = gfxlib_text.o
AF_ITOBJ = 71x_it.o

BF_STARTOBJ = bitfire_startup_gnu.o
AF_VECTOBJ = 71x_vect_gnu.o
AF_INITOBJ = 71x_init_gnu.o

all: atmosfire-ram

bitfire-flash: $(BF_STARTOBJ) $(OBJS) 
	$(LD) -o $(BF_TARGET) $(BF_STARTOBJ) $(OBJS) -T$(BF_LD_SCRIPT_ROM) $(BF_LD_FLAGS) $(LD_FLAGS2)
	$(OC) -O ihex $(BF_TARGET) $(BF_TARGET:.elf=.hex)
	$(OD) -d $(BF_TARGET) > $(BF_TARGET:.elf=.objdump)

bitfire-ram: $(BF_STARTOBJ) $(OBJS)
	$(LD) -o $(BF_TARGET) $(BF_STARTOBJ) $(OBJS) -T$(BF_LD_SCRIPT_RAM) $(BF_LD_FLAGS) $(LD_FLAGS2)
	$(OC) -O ihex $(BF_TARGET) $(BF_TARGET:.elf=.hex)
	$(OD) -d $(BF_TARGET) > $(BF_TARGET:.elf=.objdump)

atmosfire-flash: $(AF_STARTOBJ) $(OBJS) 
	$(LD) -o $(AF_TARGET) $(AF_STARTOBJ) $(OBJS) -T$(AF_LD_SCRIPT_ROM) $(AF_LD_FLAGS) $(LD_FLAGS2)
	$(OC) -O ihex $(AF_TARGET) $(AF_TARGET:.elf=.hex)
	$(OD) -d $(AF_TARGET) > $(AF_TARGET:.elf=.objdump)

atmosfire-ram: $(AF_VECTOBJ) $(AF_INITOBJ) $(OBJS) $(AF_ITOBJ)
	$(LD) -o $(AF_TARGET) $(AF_VECTOBJ) $(AF_INITOBJ) $(OBJS) $(AF_ITOBJ) -T$(AF_LD_SCRIPT_RAM) $(AF_LD_FLAGS) $(LD_FLAGS2)
	$(OC) -O ihex $(AF_TARGET) $(AF_TARGET:.elf=.hex)
	$(OD) -d $(AF_TARGET) > $(AF_TARGET:.elf=.objdump)

$(OBJS): %.o: %.c
	$(CC) -c $(CC_FLAGS) $(CC_INC) $< -o $@

$(AF_ITOBJ): %.o: %.c
	$(CC) -c $(CC_FLAGS) $(CC_INC) $< -o $@

$(BF_STARTOBJ): $(BF_STARTSRC)
	$(CC) -c $(CC_FLAGS) $(BF_CC_INC) ../../bsp/bitfire/src/$*.s

$(AF_VECTOBJ): %.o: ../../bsp/atmosfire/src/%.s
	$(CC) -c $(CC_FLAGS) $(CC_INC) $< -o $@

$(AF_INITOBJ): %.o: ../../bsp/atmosfire/src/%.s
	$(CC) -c $(CC_FLAGS) $(CC_INC) $< -o $@

clean:
	rm -f $(BF_TARGET) $(AF_TARGET) $(OBJS) $(BF_STARTOBJ) $(AF_ITOBJ) $(AF_VECTOBJ) $(AF_INITOBJ) $(BF_TARGET:.elf=.hex) $(AF_TARGET:.elf=.hex) $(BF_TARGET:.elf=.objdump) $(AF_TARGET:.elf=.objdump)
