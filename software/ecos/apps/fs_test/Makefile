#CC = gcc
CC = $(TOOLDIR)/bin/arm-elf-gcc
OC = $(TOOLDIR)/bin/arm-elf-objcopy

#BUILDDIR = ../../build_tree/synth_linux
BUILDDIR = ../../build_tree/ecos

BITFIREDIR = ../../..

FLAGS = -g
INC = -I$(BUILDDIR)/install/include -I$(BUILDDIR)/install/include/lua
LD = -L$(BITFIREDIR)/gfxlib/lib -L$(BITFIREDIR)/bsp/atmosfire/lib -L$(BUILDDIR)/install/lib -Ttarget.ld -nostdlib -Xlinker --no-warn-mismatch
TLIB = $(BUILDDIR)/install/lib/libtarget.a $(BUILDDIR)/install/lib/libextras.a

TARGET = fs_test
OBJS = fs_test.o

all: $(TARGET)

$(TARGET): $(OBJS) $(TLIB)
	$(CC) -o $(TARGET) $(OBJS) $(FLAGS) $(INC) $(LD)
	$(OC) -O binary $(shell) $(shell).bin

$(OBJS): %.o: %.c
	$(CC) -c $(FLAGS) $(INC) $< -o $@

clean:
	rm -rf $(TARGET) $(TARGET).bin $(OBJS)
