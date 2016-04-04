# Makefile
# mtrojer@arrownordic.com
# $WCREV$

#TOOLDIR=/cygdrive/c/Bitfire/GNUARM
CC=$(TOOLDIR)/bin/arm-elf-gcc
LD=$(TOOLDIR)/bin/arm-elf-ld
AR=$(TOOLDIR)/bin/arm-elf-ar

CC_FLAGS = -mcpu=arm7tdmi -W -g -O2 -mthumb-interwork
CC_FLAGS2 = -mcpu=arm7tdmi -W -g -O0

CC_INC = -I../inc -I../../../inc -I.

TARGET = libatmosfirebsp.a
OBJS = 71x_lib.o adc12.o apb.o bspi.o can.o eic.o emi.o flash.o gpio.o i2c.o pcu.o rccu.o rtc.o tim.o uart.o wdg.o xti.o 
OBJS2 = atmosfire_fpga.o atmosfire_gfx.o

MSRC = $(OBJS:.o=.c)
SRC = $(MSRC:%=../src/%)
MSRC2 = $(OBJS2:.o=.c)
SRC2 = $(MSRC2:%=../src/%)

$(TARGET): $(OBJS) $(OBJS2)
	$(AR) -r $(TARGET) $(OBJS) $(OBJS2)

$(OBJS): $(SRC)
	$(CC) -c $(CC_FLAGS) $(CC_INC) ../src/$*.c

$(OBJS2): $(SRC2)
	$(CC) -c $(CC_FLAGS2) $(CC_INC) ../src/$*.c

clean:
	rm -f $(TARGET) $(OBJS) $(OBJS2) *.a
