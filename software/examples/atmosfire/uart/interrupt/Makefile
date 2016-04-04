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

BSPDIR=../../../../bsp/atmosfire

LD_SCRIPT_RAM=$(BSPDIR)/ldscripts/ram0_gnu.ld
LD_SCRIPT_EMI=$(BSPDIR)/ldscripts/extram_gnu.ld
LD_SCRIPT_ROM=$(BSPDIR)/ldscripts/flash_gnu.ld

CC_FLAGS = -mcpu=arm7tdmi -g
CC_INC = -I. -I$(BSPDIR)/inc

LD_FLAGS = -L$(BSPDIR)/lib -latmosfirebsp
LD_FLAGS2 = -L$(TOOLDIR)/arm-elf/lib -L$(TOOLDIR)/lib/gcc/arm-elf/$(GCC_VER) -lc -lm -lgcc --no-warn-mismatch

TARGET = uart_int.elf
OBJS = main.o 71x_it.o

VECTOBJ = 71x_vect_gnu.o
INITOBJ = 71x_init_svc_gnu.o
INITOBJEMI = 71x_init_emi_gnu.o

all: ram

flash: $(VECTOBJ) $(INITOBJ) $(OBJS)
	$(LD) -o $(TARGET) $(VECTOBJ) $(INITOBJ) $(OBJS) -T$(LD_SCRIPT_ROM) $(LD_FLAGS) $(LD_FLAGS2)
	$(OC) -O ihex $(TARGET) $(TARGET:.elf=.hex)
	$(OD) -d $(TARGET) > $(TARGET:.elf=.objdump)

ram: $(VECTOBJ) $(INITOBJ) $(OBJS)
	$(LD) -o $(TARGET) $(VECTOBJ) $(INITOBJ) $(OBJS) -T$(LD_SCRIPT_RAM) $(LD_FLAGS) $(LD_FLAGS2)
	$(OC) -O ihex $(TARGET) $(TARGET:.elf=.hex)
	$(OD) -d $(TARGET) > $(TARGET:.elf=.objdump)

extram: $(VECTOBJ) $(INITOBJEMI) $(OBJS)
	$(LD) -o $(TARGET) $(VECTOBJ) $(INITOBJEMI) $(OBJS) -T$(LD_SCRIPT_EMI) $(LD_FLAGS) $(LD_FLAGS2)
	$(OC) -O ihex $(TARGET) $(TARGET:.elf=.hex)
	$(OD) -d $(TARGET) > $(TARGET:.elf=.objdump)

$(OBJS): %.o: %.c
	$(CC) -c $(CC_FLAGS) $(CC_INC) $< -o $@

$(VECTOBJ): %.o: $(BSPDIR)/src/%.s
	$(CC) -c $(CC_FLAGS) $(CC_INC) $< -o $@

$(INITOBJ): %.o: $(BSPDIR)/src/%.s
	$(CC) -c $(CC_FLAGS) $(CC_INC) $< -o $@

$(INITOBJEMI): %.o: $(BSPDIR)/src/%.s
	$(CC) -c $(CC_FLAGS) $(CC_INC) $< -o $@

clean:
	rm -f $(TARGET) $(OBJS) $(VECTOBJ) $(INITOBJ) $(INITOBJEMI) $(TARGET:.elf=.hex) $(TARGET:.elf=.objdump)
