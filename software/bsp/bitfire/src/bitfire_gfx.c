// Handle graphics (setpixel) with the Bitfire CPU
// mtrojer@arrownordic.com
// $WCREV$

#include "bitfire_types.h"
#include "bitfire_fpga.h"
#include "lpc2129_spi_polled.h"

#ifndef GL_XSIZE
#define GL_XSIZE 40
#endif
#ifndef GL_YSIZE
#define GL_YSIZE 16
#endif

#ifndef ME_X
#define ME_X 0
#endif

#ifndef ME_Y
#define ME_Y 0
#endif

typedef struct
{
  UNS_16 x;
  UNS_16 y;
} gl_point;

typedef struct
{
  UNS_8 r;
  UNS_8 g;
} gl_col;

static spi_control fpga_spi = {0, 0,
			       SPCR_MSTR_MASTER | SPCR_LSBF_MSB,
			       SPCCR_FASTEST,0}; 

static UNS_8 bsp_workbuf;

void bsp_init_gfx(void)
{
  fpga_init();
  spi_polled_init(&fpga_spi);

  fpga_command();
  spi_polled_putchar(&fpga_spi, FPGACMD_shdnn_muxsup_1);
  fpga_command();
  spi_polled_putchar(&fpga_spi, FPGACMD_shdn_ledsup_0);
  fpga_command();
  spi_polled_putchar(&fpga_spi, FPGACMD_DIMMER_PRE5);  

  bsp_workbuf = 0;
  fpga_command();
  spi_polled_putchar(&fpga_spi, FPGACMD_WORK_BUF_0);  
  fpga_command();
  spi_polled_putchar(&fpga_spi, FPGACMD_SHOW_BUF_0);  
}

void bsp_flipbuf(void)
{
  if (bsp_workbuf==0) {
    fpga_command();
    spi_polled_putchar(&fpga_spi, FPGACMD_WORK_BUF_1);  
    fpga_command();
    spi_polled_putchar(&fpga_spi, FPGACMD_SHOW_BUF_0);  
    bsp_workbuf=1;
  }
  else {
    fpga_command();
    spi_polled_putchar(&fpga_spi, FPGACMD_WORK_BUF_0);  
    fpga_command();
    spi_polled_putchar(&fpga_spi, FPGACMD_SHOW_BUF_1);  
    bsp_workbuf=0;
  }
}

void bsp_setpixelxy(UNS_16 x, UNS_16 y, gl_col *col)
{
  if (x>=ME_X*GL_XSIZE && x<(ME_X+1)*GL_XSIZE && y>=ME_Y*GL_YSIZE && y<(ME_Y+1)*GL_YSIZE) {
    UNS_16 addr = (x%GL_XSIZE)+(y%GL_YSIZE)*GL_XSIZE;
    UNS_8 addrlo = (UNS_8) addr;
    UNS_8 addrhi = (UNS_8) (addr>>8);

    fpga_data();
    spi_polled_putchar(&fpga_spi,addrlo);
    spi_polled_putchar(&fpga_spi,addrhi);
    spi_polled_putchar(&fpga_spi,col->r);
    spi_polled_putchar(&fpga_spi,col->g);
  }
}

void bsp_setpixel(gl_point *pos, gl_col *col)
{
  bsp_setpixelxy(pos->x,pos->y,col);
}

void bsp_getid(gl_point *idx)
{
  idx->x = ME_X;
  idx->y = ME_Y;
}
