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

BSPDIR=../../bsp/atmosfire
EXAMPLEDIR = ../../examples/atmosfire

LD_SCRIPT_RAM=$(BSPDIR)/ldscripts/ram_gnu.ld
LD_SCRIPT_EMI=$(BSPDIR)/ldscripts/extram_gnu.ld
LD_SCRIPT_ROM=$(BSPDIR)/ldscripts/flash_gnu.ld

CC_FLAGS = -mcpu=arm7tdmi -Os
CC_INC = -I. -I$(BSPDIR)/inc -I../src

LD_FLAGS = -L$(BSPDIR)/lib -latmosfirebsp -nostdlib
LD_FLAGS2 = -L$(TOOLDIR)/arm-elf/lib -L$(TOOLDIR)/lib/gcc/arm-elf/$(GCC_VER) -lc -lm -lgcc

OBJS = 71x_it.o
SYSCALL = syscalls.o

VECTOBJ = 71x_vect_gnu.o
INITOBJ = 71x_init_gnu.o
INITOBJEMI = 71x_init_emi_gnu.o

one: $(VECTOBJ) $(INITOBJEMI) $(SYSCALL) $(OBJS) all.o
	$(LD) -o one.elf $(VECTOBJ) $(INITOBJEMI) $(SYSCALL) $(OBJS) all.o -T$(LD_SCRIPT_EMI) $(LD_FLAGS) $(LD_FLAGS2)

min: $(VECTOBJ) $(INITOBJEMI) $(SYSCALL) $(OBJS) min.o
	$(LD) -o min.elf $(VECTOBJ) $(INITOBJEMI) $(SYSCALL) $(OBJS) min.o -T$(LD_SCRIPT_EMI) -L ../src -llua $(LD_FLAGS) $(LD_FLAGS2)

minc: $(VECTOBJ) $(INITOBJEMI) $(SYSCALL) $(OBJS) minc.o
	$(LD) -o minc.elf $(VECTOBJ) $(INITOBJEMI) $(SYSCALL) $(OBJS) minc.o -T$(LD_SCRIPT_EMI) -L ../src -llua $(LD_FLAGS) $(LD_FLAGS2)

noparser: $(VECTOBJ) $(INITOBJEMI) $(SYSCALL) $(OBJS) noparser.o
	$(LD) -o noparser.elf $(VECTOBJ) $(INITOBJEMI) $(SYSCALL) $(OBJS) noparser.o -T$(LD_SCRIPT_EMI)  -L ../src -llua $(LD_FLAGS) $(LD_FLAGS2)

minnp: $(VECTOBJ) $(INITOBJEMI) $(SYSCALL) $(OBJS) $(OBJS) min.o
	$(LD) -o minnp.elf $(VECTOBJ) $(INITOBJEMI) $(SYSCALL) $(OBJS) noparser.o min.o -T$(LD_SCRIPT_EMI)  -L ../src -llua $(LD_FLAGS) $(LD_FLAGS2)

$(OBJS): %.o: %.c
	$(CC) -c $(CC_FLAGS) $(CC_INC) $< -o $@

$(SYSCALL): %.o: $(EXAMPLEDIR)/syscalls/%.c
	$(CC) -c $(CC_FLAGS) $(CC_INC) $< -o $@

$(VECTOBJ): %.o: $(BSPDIR)/src/%.s
	$(CC) -c $(CC_FLAGS) $(CC_INC) $< -o $@

$(INITOBJ): %.o: $(BSPDIR)/src/%.s
	$(CC) -c $(CC_FLAGS) $(CC_INC) $< -o $@

$(INITOBJEMI): %.o: $(BSPDIR)/src/%.s
	$(CC) -c $(CC_FLAGS) $(CC_INC) $< -o $@

all.o: %.o: %.c
	$(CC) -c $(CC_FLAGS) $(CC_INC) $< -o $@

min.o: %.o: %.c
	$(CC) -c $(CC_FLAGS) $(CC_INC) $< -o $@

minc.o: %.o: %.c
	$(CC) -c $(CC_FLAGS) $(CC_INC) $< -o $@

noparser.o: %.o: %.c
	$(CC) -c $(CC_FLAGS) $(CC_INC) $< -o $@

clean:
	rm -f *.elf *.o *.hex
