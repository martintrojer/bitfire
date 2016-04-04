#CC = gcc
#OC = objcopy
CC = $(TOOLDIR)/bin/arm-elf-gcc
OC = $(TOOLDIR)/bin/arm-elf-objcopy

shell = mtshell

#BUILDDIR = ../../build_tree/synth
BUILDDIR = ../../build_tree/ecos

BITFIREDIR = ../../..

FLAGS = -O2
INC = -I. -I$(BUILDDIR)/install/include -I$(BUILDDIR)/install/include/lua
LD = -L$(BITFIREDIR)/gfxlib/lib -L$(BITFIREDIR)/bsp/atmosfire/lib -L$(BUILDDIR)/install/lib -Ttarget.ld -nostdlib -Xlinker --no-warn-mismatch
TLIB = $(BUILDDIR)/install/lib/libtarget.a $(BUILDDIR)/install/lib/libextras.a

OBJS = mtshell.o thread_cleanup.o mtshell_thread.o ps_func.o base_funcs.o crc16.o xmodem.o lua_funcs.o 

all: $(shell)

$(shell): $(OBJS) $(TLIB)
	$(CC) -o $(shell) $(OBJS) $(FLAGS) $(INC) $(LD)
	$(OC) -O binary $(shell) $(shell).bin

$(OBJS): %.o: %.c
	$(CC) -c $(FLAGS) $(INC) $< -o $@

clean:
	rm -rf $(shell) $(shell).bin $(OBJS)
