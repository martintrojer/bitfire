//	BITFire, basic board function tester, version 2 HW V1.12
//	dnilsson@arrownordic.com
//	Daniel Nilsson

#include "bitfire_types.h"
#include "lpc21xx.h"
#include "lpc2129_uart_polled.h"
#include "lpc2129_spi_polled.h"
#include "gfxlib.h"
#include "bitfire_fpga.h"

uart_control uart0;
uart_control uart1;
spi_control sc;

void init(void);
void putst(uart_control *uc, CHAR *st);
void print_help(void);
void busy_delay(UNS_32 dly);
void rs232_loopback(void);
void rs485_loopback(void);
void can_loopback(void);
UNS_8 can_pingpong(void);
UNS_8 check_loopback(void);

typedef struct {
  UNS_8 xpos;
  UNS_8 ypos;
  UNS_8 enable;
  UNS_8 orientation;
  UNS_8 sweepflag;
} marker_struct;

#define	vert	0
#define horiz	1

#define SWEEP_DLY_Y	300000
#define SWEEP_DLY_X	150000

#define RS485LB	1
#define	RS232LB	2
#define CANLB	4

#define	PP_TOUT_TIME	50000

#define RTS0	1<<15
#define	RTS1	1<<10
#define	CTS0	1<<14
#define CTS1	1<<11

int main(void)
{
  gl_col dcol = { 0, 0 };
  gl_point pt = { 0, 0 };
  UNS_16 j;
  marker_struct marker = { 0, 0, 0, vert, 0 };


  init();
  print_help();

  putst(&uart0,"Checking for loopback jumper...");
  switch(check_loopback()) {
  case 0:
    putst(&uart0,"not detected\n");
    break;
  case RS232LB:
    putst(&uart0,"RS232 loopback test active!\n");
    rs232_loopback();
    break;
  case RS485LB:
    putst(&uart0,"RS485 loopback test active!\n");
    rs485_loopback();
    break;
  case CANLB:
    putst(&uart0,"CAN loopback test active!\n");
    can_loopback();
    break;		
  }

  while(1) {
    // Draw display
    if(marker.enable) {
      gl_clrscr((gl_col *)&glcol_black);
      // Draw marker
      if(marker.orientation == vert) {
	pt.x = marker.xpos;
	for(j=0;j<GL_YSIZE;j++) {
	  pt.y = j;
	  gl_setpixel(&pt,&dcol);
	}	
      } else {
	pt.y = marker.ypos;
	for(j=0;j<GL_XSIZE;j++) {
	  pt.x = j;
	  gl_setpixel(&pt,&dcol);
	}	
      }
    } else {
      gl_clrscr(&dcol);
    }
    gl_flipbuf();

    UNS_8 ch = 0, ch2 = 0, u2state = 0, rtry = 0;
    switch(marker.sweepflag) {
    case 0:
		// Wait for command, inner loop survail UART1 for ping/pong command..
		while(!(U0LSR & 0x01)) {
        	// Loop the inverse of handshake signals on UART's
			if(IOPIN0 & CTS0) IOCLR0 |= RTS0; else IOSET0 |= RTS0;
			if(IOPIN0 & CTS1) IOCLR0 |= RTS1; else IOSET0 |= RTS1;

			// Check for 'ping\r' command on UART2, reply with 'pong\n' if detected
			if (U1LSR & 0x01) {
				ch2 = uart_polled_getchar(&uart1);
				rtry=1;
				while(rtry) {
					switch(u2state) {
						case 0:
							if(ch2 == 'p') {
								u2state=1;
							}
							rtry=0;
							break;
						case 1:
							if(ch2 == 'i') {
								u2state=2;
								rtry=0;
							} else u2state=0;
							break;
						case 2:
							if(ch2 == 'n') {
								u2state=3;
								rtry=0;
							} else u2state=0;
							break;
						case 3:
							if(ch2 == 'g') {
								u2state=4;
								rtry=0;
							} else u2state=0;
							break;
						case 4:
							if(ch2 == '\r') {
								u2state=0;
								rtry=0;
								putst(&uart1,"pong\n");
							} else u2state=0;
							break;
						default:
							u2state=0;
							break;
					}
				}
			}
		}
      	ch = uart_polled_getchar(&uart0);
      	uart_polled_putchar(&uart0,ch);
      	break;
    case 1:
		busy_delay(SWEEP_DLY_X);
		if(++marker.xpos == GL_XSIZE) {
			marker.xpos=0;
			marker.orientation=horiz;
			marker.sweepflag++;
			putst(&uart0,".");
		}
      	break;
    case 2:
		busy_delay(SWEEP_DLY_Y);
		if(++marker.ypos == GL_YSIZE) {
			marker.ypos=0;
			marker.orientation=vert;
			if(dcol.g == 0xff && dcol.r == 0) {
		  		dcol.r = 0xff;
		  		dcol.g = 0;
		  		marker.sweepflag=1;
		  		putst(&uart0,".");
			} else if(dcol.r == 0xff && dcol.g == 0) {
				dcol.g = 0xff;
		 		marker.sweepflag=1;
				putst(&uart0,".");
			} else {
				dcol.g = 0;
				dcol.r = 0;
				marker.sweepflag=0;
				marker.enable=0;
				putst(&uart0,". ready!\n");
			}
		}
		break;
    }

    // Do commnand
    switch(ch) {
		case 'q':
			putst(&uart0," -> Dimmer PWM level 0 set\n");
	    	fpga_command();
	    	spi_polled_putchar(&sc,FPGACMD_DIMMER_PRE0);
	    	break;
		case 'w':
			putst(&uart0," -> Dimmer PWM level 1 set\n");
			fpga_command();
      		spi_polled_putchar(&sc,FPGACMD_DIMMER_PRE1);
      		break;
		case 'e':
			putst(&uart0," -> Dimmer PWM level 2 set\n");
      		fpga_command();
      		spi_polled_putchar(&sc,FPGACMD_DIMMER_PRE2);
      		break;
		case 'r':
			putst(&uart0," -> Dimmer PWM level 3 set\n");
      		fpga_command();
      		spi_polled_putchar(&sc,FPGACMD_DIMMER_PRE3);
			break;
		case 't':
			putst(&uart0," -> Dimmer PWM level 4 set\n");
      		fpga_command();
      		spi_polled_putchar(&sc,FPGACMD_DIMMER_PRE4);
      		break;
    	case 'y':
      		putst(&uart0," -> Dimmer PWM level 5 set\n");
      		fpga_command();
      		spi_polled_putchar(&sc,FPGACMD_DIMMER_PRE5);
      		break;
    	case 'z':
      		putst(&uart0," -> Green: ON\n");
      		dcol.g = 0xff;
      		break;
    	case 'x':
      		putst(&uart0," -> Green: OFF\n");
      		dcol.g = 0;
      		break;
    	case 'a':
      		putst(&uart0," -> Red: ON\n");
      		dcol.r = 0xff;
      		break;
    	case 's':
      		putst(&uart0," -> Red: OFF\n");
      		dcol.r = 0;
      		break;
    	case '1':
      		putst(&uart0," -> LED Supply: ON\n");
      		fpga_command();
      		spi_polled_putchar(&sc,FPGACMD_shdn_ledsup_0);
      		break;
    	case '2':
      		putst(&uart0," -> LED Supply: OFF\n");
      		fpga_command();
      		spi_polled_putchar(&sc,FPGACMD_shdn_ledsup_1);
      		break;
    	case '4':
      		putst(&uart0," -> MUX supply: ON\n");
      		fpga_command();
      		spi_polled_putchar(&sc,FPGACMD_shdnn_muxsup_1);
      		break;
    	case '5':
      		putst(&uart0," -> MUX supply: OFF\n");
      		fpga_command();
      		spi_polled_putchar(&sc,FPGACMD_shdnn_muxsup_0);
      		break;
    	case 'v':
      		putst(&uart0," -> Marker: Vertical selected\n");
      		marker.orientation = vert;
      		marker.enable = 1;
      		break;
    	case 'h':
      		putst(&uart0," -> Marker: Horizontal selected\n");
      		marker.orientation = horiz;
      		marker.enable = 1;
      		break;
    	case '+':
      		if(marker.enable) {
				if(marker.orientation == horiz) {
	  				if(++marker.ypos >= GL_YSIZE) marker.ypos=0;
				} else {
	  				if(++marker.xpos >= GL_XSIZE) marker.xpos=0;
				}
			}
      		putst(&uart0," -> Marker: increment\n");
      		break;
		case '-':
      		if(marker.enable) {
				if(marker.orientation == horiz) {
	  				if(marker.ypos-- == 0) marker.ypos = (GL_YSIZE-1);
				} else {
	  				if(marker.xpos-- == 0) marker.xpos = (GL_XSIZE-1);
				}
			}
      		putst(&uart0," -> Marker: decrement\n");
      		break;
    	case 'o':
      		putst(&uart0," -> Marker: OFF\n");
      		marker.enable = 0;
      		break;
    	case 'c':
      		putst(&uart0," -> Marker: auto sweep...");
      		marker.sweepflag=1;
      		marker.orientation=vert;
      		marker.enable=1;
      		marker.ypos=0;
      		marker.xpos=0;
      		dcol.g=0xff;
      		dcol.r=0;
      		break;
      	case '.':
      		putst(&uart0," -> CAN Ping/Pong Test...");
			if(!can_pingpong()) {
				putst(&uart0," Success\n");
			} else {
				putst(&uart0," Fail\n");
			}
			break;
    	case '?':
      		print_help();
      	break;
	}
  }
}


UNS_8 can_pingpong(void)
{
	UNS_8 loopcnt=0;
	UNS_32 tout=0;

	// Setup I/O's
	PINSEL1 &= ~(1<<14 | 1<<15 | 1<<16 | 1<<17 | 1<<18 | 1<<19);
	PINSEL1 |= (1<<14 | 1<<16 | 1<<18);

	// Setup CAN1 channel
	C1MOD |= 1<<0;		// Set RM
	C1MOD &= (1<<0);	// Set rest of the bits in CANMOD to 0
	AFMR |= (1<<1);		// Enable bypass

	C1BTR =	(C1BTR & ~(0x1ff)) + 3;			// Set divider for VPB clock (baudrate)
	C1TFI1 = (C1TFI1 & (~0xF0000))+(1<<16);	// Set DLC
	C1TDA1 = 0xAF;
	C1TID1 = 0x90;		// Identifier
	C1MOD &= ~(1<<0);	// Reset RM

	// Setup CAN2 channel
	C2MOD |= 1<<0;							// Set RM
	C2BTR =	(C2BTR & ~(0x1ff)) + 3;
	C2TFI1 = (C2TFI1 & (~0xF0000))+(1<<16);	// Set DLC
	C2TID1 = 0x22;		// Identifier
	C2TDA1 = 0x5F;		// Data
	C2MOD &= ~(1<<0);	// Reset RM

	// Ping/Pong 255 packets between CAN channels
	while(++loopcnt) {
		C2CMR |= 1<<2;		// Clear receive buffer for next reception on channel 2
		C1CMR |= 1<<0;		// Que transmission objects on channel 1

		// Wait for receive channel 2 with timeout
        tout=PP_TOUT_TIME;
	    while(!(C2GSR & (1<<0)))
			if(!tout--) return 1;

        // Check data on receiving channel 2
    	if((C2RDA & 0xff) != 0xAF) return 2;

	    C1CMR |= 1<<2;					// Clear receive buffer for next reception
    	C2CMR |= 1<<0;					// Que transmission object 2

		// Wait for receive channel 1 with timeout
        tout=PP_TOUT_TIME;
	    while(!(C1GSR & (1<<0)))
			if(!tout--) return 1;

        // Check data on receiving channel 1
    	if((C1RDA & 0xff) != 0x5F) return 3;
 	}

    // Success
	return 0;
}

void can_loopback(void)
{
  gl_point pt = {0,0};
  UNS_8 loopcnt=0;

  // Setup I/O's
  PINSEL1 &= ~(1<<14 | 1<<15 | 1<<16 | 1<<17 | 1<<18 | 1<<19);
  PINSEL1 |= (1<<14 | 1<<16 | 1<<18);

  // Setup CAN1 channel
  C1MOD |= 1<<0;		// Set RM
  C1MOD &= (1<<0);		// Set rest of the bits in CANMOD to 0
  AFMR |= (1<<1);		// Enable bypass

  C1BTR =	(C1BTR & ~(0x1ff)) + 3;			// Set divider for VPB clock (baudrate)
  C1TFI1 = (C1TFI1 & (~0xF0000))+(1<<16);	// Set DLC
  C1TDA1 = 0xAF;
  C1TID1 = 0x90;	// Identifier
  C1MOD &= ~(1<<0);	// Reset RM

  // Setup CAN2 channel
  C2MOD |= 1<<0;		// Set RM
  C2BTR =	(C2BTR & ~(0x1ff)) + 3;
  C2TFI1 = (C2TFI1 & (~0xF0000))+(1<<16);	// Set DLC
  C2TID1 = 0x22;	// Identifier
  C2TDA1 = 0x5F;	// Data
  C2MOD &= ~(1<<0);	// Reset RM

  pt.x=0;
  pt.y=0;

  while(1) {
    if(loopcnt++==0xFF) {
      busy_delay(1000000);
      gl_clrscr((gl_col *)&glcol_black);
    }

    C2CMR |= 1<<2;					// Clear receive buffer for next reception
    C1CMR |= 1<<0;					// Que transmission objects


    pt.y = ((loopcnt&0xF0)>>4);
    pt.x = (loopcnt&0x0F);

    while(!(C2GSR & (1<<0)));		// Wait for receive channel 2
    if((C2RDA & 0xff) == 0xAF) {
      gl_setpixel(&pt,(gl_col *)&glcol_green);
    } else {
      gl_setpixel(&pt,(gl_col *)&glcol_red);
    }

    C1CMR |= 1<<2;					// Clear receive buffer for next reception
    C2CMR |= 1<<0;					// Que transmission object 2

    pt.x += 24;

    while(!(C1GSR & (1<<0)));		// Wait for receive channel 1
    if((C1RDA & 0xff) == 0x5F) {
      gl_setpixel(&pt,(gl_col *)&glcol_green);
    } else {
      gl_setpixel(&pt,(gl_col *)&glcol_red);
    }
  }
}

void rs232_loop_test(void);
void rs485_loop_test(void);
UNS_8 putchar_bin(uart_control *uc, UNS_8 ch);
UNS_16 check_char(uart_control *uc);

void rs232_loopback(void)
{
  // Init uart1
  uart1.port=1;
  uart1.LCR = LCR_WLS_8BIT | LCR_SBS_1BIT | LCR_PE_0 | LCR_PS_ODD | LCR_BK_0 | LCR_DLAB_1;
  uart1.baudrate = 115200;
  uart_polled_init(&uart1);

  // Setup handshake IO
  PINSEL0 &= ~(1<<29 | 1<<28 | 1<<31 | 1<<30 | 1<<21 | 1<<20 | 1<<23 | 1<<22);

  IODIR0 |= (RTS0 | RTS1);
  IODIR0 &= ~(CTS0 | CTS1);

  // Setup RSX pins on the FPGA
  fpga_command();
  spi_polled_putchar(&sc,FPGACMD_sel1_rsx_0);
  fpga_command();
  spi_polled_putchar(&sc,FPGACMD_sel2_rsx_0);

  // Perform the test
  rs232_loop_test();	
}

void rs485_loopback(void)
{
  // Init uart1
  uart1.port=1;
  uart1.LCR = LCR_WLS_8BIT | LCR_SBS_1BIT | LCR_PE_0 | LCR_PS_ODD | LCR_BK_0 | LCR_DLAB_1;
  uart1.baudrate = 937500;
  uart_polled_init(&uart1);

  // Reinit uart0 in 937.5kbps
  uart0.port=0;
  uart0.LCR = LCR_WLS_8BIT | LCR_SBS_1BIT | LCR_PE_0 | LCR_PS_ODD | LCR_BK_0 | LCR_DLAB_1;
  uart0.baudrate = 937500;
  uart_polled_init(&uart0);

  // Setup handshake signals (RTS is used as driver enables
  PINSEL0 &= ~(1<<29 | 1<<28 | 1<<31 | 1<<30 | 1<<21 | 1<<20 | 1<<23 | 1<<22);
  IODIR0 |= (RTS0 | RTS1);	
  IOCLR0 = (RTS0 | RTS1);

  // Program FPGA to drive SEL-pins high for RS485 mode.
  fpga_command();
  spi_polled_putchar(&sc,FPGACMD_sel1_rsx_1);
  fpga_command();
  spi_polled_putchar(&sc,FPGACMD_sel2_rsx_1);

  // Perform the test
  rs485_loop_test();
}

void rs485_loop_test(void)
{
  UNS_8 loopcnt=0,loopcnt2=0xff,toggel=0;
  UNS_16 j,i;
	
  gl_col dcol = {0,0};
  gl_point pt = {0,0};
	
  while(1) {
    loopcnt2--;
    if(loopcnt++ == 0xFF) {
      busy_delay(1000000);
      gl_clrscr((gl_col *)&glcol_black);
      for(j=0;j<64;j++) {
	pt.x=(j&0x07)+16;
	pt.y=j>>3;
	if(j&0x08) {
	  dcol.r=0;
	  dcol.g=0xff;
	} else {
	  dcol.r=0xff;
	  dcol.g=0;
	}
	gl_setpixel(&pt,&dcol);
      }
    }
		
    pt.x=(loopcnt & 0x0F)+24;
    pt.y=((loopcnt & 0xF0)>>4);

    busy_delay(200);
    PINSEL0 &= ~(1<<2 | 1<<3);	// Disable RXD function on UART0
    IOSET0 = RTS0;				// Enable transmission from UART0
    busy_delay(200);

    U0FCR |= (1<<2);	// Flush TXD0 fifo
    U1FCR |= (1<<1);	// Flush RXD1 fifo

    putchar_bin(&uart0,loopcnt2);

    for(j=0;j<2000;j++) {
      if(check_char(&uart1)) {
	if(uart_polled_getchar(&uart1) == loopcnt2) {
	  gl_setpixel(&pt,(gl_col *)&glcol_green);
	} else {
	  gl_setpixel(&pt,(gl_col *)&glcol_red);
	}
	break;
      }
    }
    IOCLR0 = RTS0;		// Disable transmission from UART0
    PINSEL0 |= (1<<2);	// Enable RXD function on UART0
    U0FCR |= (1<<1);	// Flush RXD0 fifo

    pt.x -= 24;

    busy_delay(200);
    PINSEL0 &= ~(1<<18 | 1<<19);	// Disable RXD function on UART1
    IOSET0 = RTS1;					// Enable transmission from UART1
    busy_delay(200);

    U1FCR |= (1<<2);	// Flush TXD1 fifo
    U0FCR |= (1<<1);	// Flush RXD0 fifo
		
    putchar_bin(&uart1,loopcnt);
    for(j=0;j<2000;j++) {
      if(check_char(&uart0)) {
	if(uart_polled_getchar(&uart0) == loopcnt) {
	  gl_setpixel(&pt,(gl_col *)&glcol_green);
	} else {
	  gl_setpixel(&pt,(gl_col *)&glcol_red);
	}
	break;
      }
    }
    IOCLR0 = RTS1;		// Disable transmission from UART1
    PINSEL0 |= (1<<18);	// Enable RXD function on UART1
    U1FCR |= (1<<1);	// Flush RXD0 fifo
  }
}

void rs232_loop_test(void)
{
  UNS_8 loopcnt=0,loopcnt2=0xff,toggel=0;
  UNS_16 j,i;
	
  gl_col dcol0 = {0,0};
  gl_col dcol1 = {0,0};
  gl_point pt = {0,0};
		
  while(1) {
    loopcnt2--;
    if(loopcnt++ == 0xFF) {
      busy_delay(1000000);
      gl_clrscr((gl_col *)&glcol_black);
			
      toggel^=0x01;
      if(toggel & 0x01) {
	IOSET0 = RTS1;
	IOCLR0 = RTS0;
      } else {
	IOSET0 = RTS0;
	IOCLR0 = RTS1;
      }

      busy_delay(5000);

      dcol0.r = 0;
      if(IOPIN0 & CTS0) dcol0.r=0xff;
      dcol1.r = 0;
      if(IOPIN0 & CTS1) dcol1.r=0xff;
			
      for(j=0;j<64;j++) {
	pt.y = j>>2;
	pt.x = (j&0x03)+16;
	gl_setpixel(&pt,&dcol0);
	pt.x += 4;
	gl_setpixel(&pt,&dcol1);
      }
    }
		
    pt.x=(loopcnt & 0x0F)+24;
    pt.y=((loopcnt & 0xF0)>>4);

    U0FCR |= (1<<2);	// Flush TXD0 fifo
    U1FCR |= (1<<1);	// Flush RXD1 fifo
    putchar_bin(&uart0,loopcnt);
    for(j=0;j<2000;j++) {
      if(check_char(&uart1)) {
	if(uart_polled_getchar(&uart1) == loopcnt) {
	  gl_setpixel(&pt,(gl_col *)&glcol_green);
	} else {
	  gl_setpixel(&pt,(gl_col *)&glcol_red);
	}
	break;
      }
    }

    pt.x -= 24;

    U1FCR |= (1<<2);	// Flush TXD1 fifo
    U0FCR |= (1<<1);	// Flush RXD0 fifo
    putchar_bin(&uart1,loopcnt2);
    for(j=0;j<2000;j++) {
      if(check_char(&uart0)) {
	if(uart_polled_getchar(&uart0) == loopcnt2) {
	  gl_setpixel(&pt,(gl_col *)&glcol_green);
	} else {
	  gl_setpixel(&pt,(gl_col *)&glcol_red);
	}
	break;
      }
    }
  }
}

UNS_8 putchar_bin(uart_control *uc, UNS_8 ch)
{
  if (uc->status!=UART_STATUS_OK)
    return 0;
  
  if (uc->port==0) {
    while (!(U0LSR & 0x20));
    return (U0THR = ch);
  }

  if (uc->port==1) {
    while (!(U1LSR & 0x20));
    return (U1THR = ch);
  }
}

UNS_16 check_char(uart_control *uc)
{
  if (uc->status!=UART_STATUS_OK)
    return 0;

  if (uc->port==0) {
    if(U0LSR & 0x01) return -1; else return 0;
  }

  if (uc->port==1) {
    if(U1LSR & 0x01) return -1; else return 0;
  }
}

void busy_delay(UNS_32 dly)
{
  UNS_32 j;
  for(j=dly;j>0;j--);
}

void print_help(void)
{
  putst(&uart0,"\n\n-----------------------------------------------------------\n");
  putst(&uart0,"  Testsuite V1.00, BITFire basic board function testing.\n");
  putst(&uart0,"  Daniel Nilsson (C) 2005 Arrow Engineering Sweden\n");
  putst(&uart0,"---------------------------------------------------------------\n");
  putst(&uart0," 'q' - Dimmer PWM level 0 set\n");
  putst(&uart0," 'w' - Dimmer PWM level 1 set\n");
  putst(&uart0," 'e' - Dimmer PWM level 2 set\n");
  putst(&uart0," 'r' - Dimmer PWM level 3 set\n");
  putst(&uart0," 't' - Dimmer PWM level 4 set\n");
  putst(&uart0," 'y' - Dimmer PWM level 5 set\n\n");
  putst(&uart0," 'z' - Green: ON                        'x' - Green: OFF\n");
  putst(&uart0," 'a' - Red: ON                          's' - Red: OFF\n");
  putst(&uart0," '1' - LED Supply: ON                   '2' - LED Supply: OFF\n");
  putst(&uart0," '4' - MUX supply: ON                   '5' - MUX supply: OFF\n");
  putst(&uart0," 'v' - Marker: Vertical selected        '+' - Marker: increment\n");
  putst(&uart0," 'h' - Marker: Horizontal selected      '-' - Marker: decrement\n");
  putst(&uart0," 'c' - Marker: auto sweep started       'o' - Marker: OFF\n");
  putst(&uart0," '.' - CAN Ping/Pong test\n");
  putst(&uart0," '?' - Print this help\n");
  putst(&uart0,"---------------------------------------------------------------\n");

}

void init(void)
{
  // Setup UART0
  uart0.port=0;
  uart0.LCR = LCR_WLS_8BIT | LCR_SBS_1BIT | LCR_PE_0 | LCR_PS_ODD | LCR_BK_0 | LCR_DLAB_1;
  uart0.baudrate = 115200;
  uart_polled_init(&uart0);

  // Setup UART1
  uart1.port=1;
  uart1.LCR = LCR_WLS_8BIT | LCR_SBS_1BIT | LCR_PE_0 | LCR_PS_ODD | LCR_BK_0 | LCR_DLAB_1;
  uart1.baudrate = 115200;
  uart_polled_init(&uart1);

  	// Setup handshake IO
  	PINSEL0 &= ~(1<<29 | 1<<28 | 1<<31 | 1<<30 | 1<<21 | 1<<20 | 1<<23 | 1<<22);

	// Set handshake signals to normal I/O
	IODIR0 |= (RTS0 | RTS1);
	IODIR0 &= ~(CTS0 | CTS1);

  // Setup SPI0
  sc.port = 0;
  sc.status = 0;
  sc.SPCR = SPCR_MSTR_MASTER | SPCR_LSBF_LSB;
  sc.SPCCR = SPCCR_FASTEST;
  sc.vecaddr = 0;
  spi_polled_init(&sc);

  fpga_init();
  gl_init();
}

/*
 * Short on P0.18 and P0.19 gives RS232 loopback test (pin 1 & 2 in J12)
 * Short on P0.18 and P0.20 gives RS485 loopback test (pin 1 & 3 in J12)
 */
UNS_8 check_connect(UNS_32 ioout, UNS_32 ioin);

UNS_8 check_loopback(void)
{
  UNS_32 iod_save,ios_save;
  UNS_8 retval=0;

  iod_save = IODIR0;
  ios_save = PINSEL1;

  PINSEL1 &= ~(1<<4 | 1<<5 | 1<<6 | 1<<7 | 1<<8 | 1<<9);

  // Test for RS232 P0.18 & P0.20 outputs, P0.19 input)
  // Short J12.1 & J12.2
  IODIR0 |= (1<<18 | 1<<20);
  IODIR0 &= ~(1<<19);
  if(check_connect(1<<18,1<<19)) retval=RS232LB; else {

    // Test for RS485 P0.18 & P0.19 outputs, P0.20 input)
    // Short J12.1 & J12.3
    IODIR0 &= ~(1<<20);
    IODIR0 |= 1<<19;
    if(check_connect(1<<18,1<<20)) retval=RS485LB; else {

      // Test for CAN loopback P0.20 & P0.18 outputs, P0.19 input
      // Short J12.2 & J12.3
      IODIR0 &= ~(1<<19);
      IODIR0 |= 1<<20;
      if(check_connect(1<<20,1<<19)) retval=CANLB;
    }
  }	

  IODIR0 = iod_save;
  PINSEL1 = ios_save;
  return retval;
}

UNS_8 check_connect(UNS_32 ioout, UNS_32 ioin)
{
  UNS_16 j;
  UNS_8 flag=1;
	
  for(j=0;j<100;j++) {

    IOSET0 = ioout;
    busy_delay(500);
    if(!(IOPIN0 & ioin)) {
      flag=0;
      break;
    }

    IOCLR0 = ioout;
    busy_delay(500);
    if(IOPIN0 & ioin) {
      flag=0;
      break;
    }
  }
	
  return flag;
}


void putst(uart_control *uc, CHAR *st)
{
  do {
    uart_polled_putchar(uc,*st);
  } while(*st++ != '\0');
}
