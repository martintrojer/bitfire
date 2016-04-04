# Makefile
# mtrojer@arrownordic.com
# $WCREV$

#TOOLDIR=/cygdrive/c/Bitfire/GNUARM
CC=$(TOOLDIR)/bin/arm-elf-gcc
LD=$(TOOLDIR)/bin/arm-elf-ld
AR=$(TOOLDIR)/bin/arm-elf-ar

CC_FLAGS = -mcpu=arm7tdmi -W -O2 -g
CC_INC = -I../inc -I../../../inc

TARGET = libbitfirebsp.a
OBJS = bitfire_fpga.o lpc2129_spi_polled.o lpc2129_uart_polled.o lpc2129_uart_int.o lpc2129_vic.o lpc2129_can_polled.o bitfire_gfx.o

$(TARGET): $(OBJS) $(GOBJS)
	$(AR) -r $(TARGET) $(OBJS)

MSRC = $(OBJS:.o=.c)
SRC = $(MSRC:%=../src/%)

$(OBJS): $(SRC)
	$(CC) -c $(CC_FLAGS) $(CC_INC) ../src/$*.c
clean:
	rm -f $(TARGET) $(OBJS) $(GOBJS) *.a
