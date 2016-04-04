CC = gcc
#CC = $(TOOLDIR)/bin/arm-elf-gcc

threads = threads
fs = fs
hello = hello
simple = simple

BUILDDIR = ../../build_tree/synth
#BUILDDIR = ../../build_tree/ecos

BITFIREDIR = ../../..

FLAGS = -g
INC = -I$(BUILDDIR)/install/include -I$(BUILDDIR)/install/include/lua
LD = -L$(BITFIREDIR)/gfxlib/lib -L$(BITFIREDIR)/bsp/atmosfire/lib -L$(BUILDDIR)/install/lib -Ttarget.ld -nostdlib -Xlinker --no-warn-mismatch
TLIB = $(BUILDDIR)/install/lib/libtarget.a $(BUILDDIR)/install/lib/libextras.a

all: $(threads) $(fs)

$(simple): simple.c $(TLIB)
	$(CC) simple.c -o $(simple) $(FLAGS) $(INC) $(LD)

$(threads): threads.c $(TLIB)
	$(CC) threads.c -o $(threads) $(FLAGS) $(INC) $(LD)

$(fs): fs.c $(TLIB)
	$(CC) fs.c -o $(fs) $(FLAGS) $(INC) $(LD)

$(hello): hello_fs.c $(TLIB)
	$(CC) hello_fs.c -o $(shell) $(FLAGS) $(INC) $(LD)

clean:
	rm -rf $(threads) $(fs) $(hello)
