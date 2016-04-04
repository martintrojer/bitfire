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
GFXDIR=../../../../gfxlib

LD_SCRIPT_RAM=$(BSPDIR)/ldscripts/ram0_gnu.ld
LD_SCRIPT_EMI=$(BSPDIR)/ldscripts/extram_gnu.ld
LD_SCRIPT_ROM=$(BSPDIR)/ldscripts/flash_gnu.ld

CC_FLAGS = -mcpu=arm7tdmi -g -D RUN_FROM_ROM -D STR71X_GCC
CC_INC = -I../Common/include -I../../Source/include -I. -I$(BSPDIR)/inc -I$(GFXDIR)/inc -I../../../../inc

LD_FLAGS = -L$(GFXDIR)/lib -L$(BSPDIR)/lib -lgfx -latmosfirebsp 
LD_FLAGS2 = -L$(TOOLDIR)/arm-elf/lib -L$(TOOLDIR)/lib/gcc/arm-elf/$(GCC_VER) -lc -lm -lgcc --no-warn-mismatch

TARGET = freertos.elf
OBJS = main.o minrtos.o
OBJSRC = list.o queue.o tasks.o
OBJPORT = port.o portISR.o
OBJMEM = heap_2.o

VECTOBJ = 71x_vect_gnu.o
INITOBJ = 71x_init_svc_gnu.o
INITOBJEMI = 71x_init_emi_gnu.o

all: ram

ram: $(VECTOBJ) $(INITOBJ) $(OBJS) $(OBJSRC) $(OBJPORT) $(OBJMEM)
	$(LD) -o $(TARGET) $(VECTOBJ) $(INITOBJ) $(OBJS) $(OBJMEM) $(OBJSRC) $(OBJPORT) -T$(LD_SCRIPT_RAM) $(LD_FLAGS) $(LD_FLAGS2)
	$(OC) -O ihex $(TARGET) $(TARGET:.elf=.hex)
	$(OD) -d $(TARGET) > $(TARGET:.elf=.objdump)

$(OBJS): %.o: %.c
	$(CC) -c $(CC_FLAGS) $(CC_INC) $< -o $@

$(VECTOBJ): %.o: %.s
	$(CC) -c $(CC_FLAGS) $(CC_INC) $< -o $@

$(INITOBJ): %.o: $(BSPDIR)/src/%.s
	$(CC) -c $(CC_FLAGS) $(CC_INC) $< -o $@

$(OBJSRC): %.o: ../../Source/%.c
	$(CC) -c $(CC_FLAGS) $(CC_INC) $< -o $@

$(OBJPORT): %.o: ../../Source/portable/GCC/ARM7_STR71x/%.c
	$(CC) -c $(CC_FLAGS) $(CC_INC) $< -o $@

$(OBJMEM): %.o: ../../Source/portable/MemMang/%.c
	$(CC) -c $(CC_FLAGS) $(CC_INC) $< -o $@

clean:
	rm -f $(TARGET) $(OBJS) $(OBJSRC) $(OBJPORT) $(OBJMEM) $(VECTOBJ) $(INITOBJ) $(TARGET:.elf=.hex) $(TARGET:.elf=.objdump)
