# Makefile
# mtrojer@arrownordic.com
# $WCREV$

#TOOLDIR=/cygdrive/c/Bitfire/GNUARM
CC=$(TOOLDIR)/bin/arm-elf-gcc
LD=$(TOOLDIR)/bin/arm-elf-ld
AR=$(TOOLDIR)/bin/arm-elf-ar

CC_FLAGS = -mcpu=arm7tdmi -W -O2 -g
CC_INC = -I../inc -I../../inc

TARGET = libgfx.a

OBJS = font_rom8x16.o font_rom8x8.o gfxlib.o gfxvec.o gl_circle.o gl_cliprect.o gl_clrscr.o gl_ellipse.o gl_fillrect.o gl_iswithin.o gl_line.o gl_pblit.o gl_printhexdot.o gl_random.o glfx_3dstar.o glfx_charsinscroll.o glfx_fire.o glfx_fire2.o glfx_init_greenfire_palette.o glfx_init_radient_palette.o glfx_init_redfire_palette.o glfx_init_yellowfire_palette.o glfx_julia.o glfx_mandel.o glfx_parallax.o glfx_pattern1.o glfx_pattern2.o glfx_pattern3.o glfx_pattern4.o glfx_pixsinscroll.o glfx_plasma.o glfx_scrolltext.o gltxt_printhex.o gltxt_printtext.o gltxt_renderchar.o gltxt_scroller.o myfixed.o 

MSRC = $(OBJS:.o=.c)
SRC = $(MSRC:%=../src/%)

$(TARGET): $(OBJS)
	$(AR) -r $(TARGET) $(OBJS)

$(OBJS): $(SRC)
	$(CC) -c $(CC_FLAGS) $(CC_INC) ../src/$*.c

clean:
	rm -f $(TARGET) $(OBJS)
