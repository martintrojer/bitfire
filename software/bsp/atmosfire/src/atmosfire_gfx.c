// Handle graphics (setpixel) with the Atmosfire CPU
// mtrojer@arrownordic.com
// $WCREV$

#include "71x_lib.h"
#include "bitfire_types.h"
#include "atmosfire_fpga.h"

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


static void spi_send(UNS_16 x)
{
  while (BSPI_FlagStatus(BSPI1,BSPI_TFF)==SET)
    ;
  BSPI_WordSend(BSPI1, x);
}

static void spi_init(void)
{
  BSPI_Enable ( BSPI1 , ENABLE );
  BSPI_Init   ( BSPI1 ) ;
  BSPI_ClockDividerConfig ( BSPI1, 8);
  BSPI_Enable ( BSPI1 , ENABLE );
  BSPI_MasterEnable ( BSPI1,ENABLE);
  BSPI_ClkActiveHigh(BSPI1,DISABLE);
  BSPI_ClkFEdge(BSPI1,DISABLE);
  BSPI_8bLEn(BSPI1,ENABLE);
  BSPI_TrFifoDepth(BSPI1,1);
  BSPI_RcFifoDepth(BSPI1,1);
}

static UNS_8 bsp_workbuf;

void bsp_init_gfx(void)
{
  // Setup clocks
  RCCU_Div2Config(ENABLE);
  RCCU_MCLKConfig(RCCU_DEFAULT);
  RCCU_FCLKConfig(RCCU_DEFAULT);
  RCCU_PCLKConfig(RCCU_RCLK_2);
  RCCU_PLL1Config(RCCU_PLL1_Mul_16,RCCU_Div_2);
  RCCU_RCLKSourceConfig(RCCU_PLL1_Output);
  //  RCCU_RCLKSourceConfig(RCCU_CLOCK2_16);

  fpga_init();
  spi_init();
  
  fpga_command();
  spi_send(FPGACMD_shdnn_muxsup_1);
  fpga_command();
  spi_send(FPGACMD_shdn_ledsup_0);
  fpga_command();
  spi_send(FPGACMD_DIMMER_PRE3);  

  bsp_workbuf = 0;
  fpga_command();
  spi_send(FPGACMD_WORK_BUF_0);  
  fpga_command();
  spi_send(FPGACMD_SHOW_BUF_0);  
}

void bsp_flipbuf(void)
{
  if (bsp_workbuf==0) {
    fpga_command();
    spi_send(FPGACMD_WORK_BUF_1);  
    fpga_command();
    spi_send(FPGACMD_SHOW_BUF_0);  
    bsp_workbuf=1;
  }
  else {
    fpga_command();
    spi_send(FPGACMD_WORK_BUF_0);  
    fpga_command();
    spi_send(FPGACMD_SHOW_BUF_1);  
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
    spi_send(addrlo);
    spi_send(addrhi);
    spi_send(col->r);
    spi_send(col->g);
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
