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

LD_SCRIPT_RAM=ldscripts/ram_gnu.ld
LD_SCRIPT_EMI=ldscripts/extram_gnu.ld
LD_SCRIPT_ROM=ldscripts/flash_gnu.ld

CC_FLAGS = -mcpu=arm7tdmi -g -Os
CC_INC = -I. -IApplication -IDriver -IPlatform

LD_FLAGS2 = -L$(TOOLDIR)/arm-elf/lib -L$(TOOLDIR)/lib/gcc/arm-elf/$(GCC_VER) -lc -lm -lgcc --no-warn-mismatch

TARGET = 71xxboot.elf

INITOBJS = Init/71x_vect_gnu.o Init/71x_init_gnu.o
INITOBJSEMI = Init/71x_vect_gnu.o Init/71x_init_emi_gnu.o

DRIVOBJS = Driver/71x_it.o Driver/71x_lib.o Driver/eic.o Driver/emi.o Driver/gpio.o Driver/rccu.o Driver/uart.o Driver/wdg.o

#APPOBJS = Application/common.o Application/download.o Application/loadlinux.o Application/m28w320b.o Application/program.o Application/ymodem.o 
APPOBJS = Application/common.o Application/download.o Application/loadlinux.o Application/ymodem.o 
APPSOBJS = Application/calllinux_gnu.o

PLATOBJS = Platform/commandlist.o Platform/main.o Platform/menu.o Platform/platform.o

all: ram

flash: $(INITOBJS) $(DRIVOBJS) $(APPOBJS) $(APPSOBJS) $(PLATOBJS)
	$(LD) -o $(TARGET) $(INITOBJS) $(DRIVOBJS) $(APPOBJS) $(APPSOBJS) $(PLATOBJS) -T$(LD_SCRIPT_ROM) $(LD_FLAGS) $(LD_FLAGS2)
	$(OC) -O ihex $(TARGET) $(TARGET:.elf=.hex)
	$(OD) -d $(TARGET) > $(TARGET:.elf=.objdump)

ram: $(INITOBJS) $(DRIVOBJS) $(APPOBJS) $(APPSOBJS) $(PLATOBJS)
	$(LD) -o $(TARGET) $(INITOBJS) $(DRIVOBJS) $(APPOBJS) $(APPSOBJS) $(PLATOBJS) -T$(LD_SCRIPT_RAM) $(LD_FLAGS) $(LD_FLAGS2)
	$(OC) -O ihex $(TARGET) $(TARGET:.elf=.hex)
	$(OD) -d $(TARGET) > $(TARGET:.elf=.objdump)

extram: $(INITOBJSEMI) $(DRIVOBJS) $(APPOBJS) $(APPSOBJS) $(PLATOBJS)
	$(LD) -o $(TARGET) $(INITOBJSEMI) $(DRIVOBJS) $(APPOBJS) $(APPSOBJS) $(PLATOBJS) -T$(LD_SCRIPT_EMI) $(LD_FLAGS) $(LD_FLAGS2)
	$(OC) -O ihex $(TARGET) $(TARGET:.elf=.hex)
	$(OD) -d $(TARGET) > $(TARGET:.elf=.objdump)

$(INITOBJS): Init/%.o: Init/%.s
	$(CC) -c $(CC_FLAGS) $(CC_INC) $< -o $@

$(INITOBJSEMI): Init/%.o: Init/%.s
	$(CC) -c $(CC_FLAGS) $(CC_INC) $< -o $@

$(DRIVOBJS): Driver/%.o: Driver/%.c
	$(CC) -c $(CC_FLAGS) $(CC_INC) $< -o $@

$(APPOBJS): Application/%.o: Application/%.c
	$(CC) -c $(CC_FLAGS) $(CC_INC) $< -o $@

$(APPSOBJS): Application/%.o: Application/%.s
	$(CC) -c $(CC_FLAGS) $(CC_INC) $< -o $@

$(PLATOBJS): Platform/%.o: Platform/%.c
	$(CC) -c $(CC_FLAGS) $(CC_INC) $< -o $@

clean:
	rm -f $(TARGET) $(INITOBJS) $(INITOBJSEMI) $(DRIVOBJS) $(APPOBJS) $(APPSOBJS) $(PLATOBJS) $(TARGET:.elf=.hex) $(TARGET:.elf=.objdump)
