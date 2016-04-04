# helptext.py, module for displaying various texts
import platform

if platform.system()=="Windows":
	from msvcrt import kbhit
	from msvcrt import getch

def htext(stext,ppause):
	s=''
	if stext=='basic':
		s = """
 Usage: MCF [switches/options] [<target PID|EID|ADR> <command1> [commandN]]

 Basic commands

 [<PID|EID|ADR> <Command1 ... CommandN>]
 ------------------------------------------------------------------------------
 Sends a command to a given network member.
 
 Use '-?:<equipmentname>' to get a list of avalible commands and usage.

 PID is product ID in 8-digit hex format, EID is equipment ID as name or parts
 of the name, ADR is a two digit hex number for network adress.

 Several commands can be given at the same call

 Usage > MCF atmos cfg(logo=off,cke=on,ck=4mhz)
         MCF bit setmode(mcu_jtag)
         MCF at reset
         MCF -list at cfg
         MCF at ping
         MCF at ping(25)
         MCF bit reset
         MCF atmos status
         MCF 0x11 status
         MCF 0x02000010 cfg(logo=on,boot=cs0) reset

 Switches/options (always given before the basic command)

 
 -c:<port>
 ------------------------------------------------------------------------------
 COM-port selection

 Specify com-port, if swicth is omitted MCF will search for a valid MagicConfig
 loader on all avalible ports and stop its search when it finds the first
 avalible or exhaust on ports (scans upto port 100).

 Usage > MCF -c:com1 -list
         MCF -c:0 -list

 
 -l:<file.hex>
 ------------------------------------------------------------------------------
 Firmware load

 Loads "file.a90" into buffer and displays mapping and checksum information.

 Usage > MCF -l:test.a90

 
 -d:<PID|EID|ADR>
 ------------------------------------------------------------------------------
 Firmware upload

 Searches the bridge for a matching device and uploads the application from
 buffer. PID is product ID in 8-digit hex format, EID is equipment ID as name
 or parts of the name, ADR is a two digit hex number for network adress.
 Hexnumbers start with '0x'

 Used in conjuction with '-l' switch.

 Usage > MCF -l:test.a90 -d:atmosfire
         MCF -l:test.a90 -d:0x11
         MCF -l:test.a90 -d:0x020000a1

 
 -list
 ------------------------------------------------------------------------------
 List devices on network

 Lists all devices found on bridge, gives information on status, version etc.

 Usage > MCF -list
 ------------------------------------------------------------------------------


 -bind[:<PID>:<ADR>]
 ------------------------------------------------------------------------------
 Bind device(s) on I2C bridge to network adresse(s).

 If no parameters are used bind will automatically search the bus and bind all
 devices found on bridge to suitable adresses.

 Usage > MCF -bind:0x12345678:0x10
         MCF -bind


 -?:<target name>
 ------------------------------------------------------------------------------
 Displays help on a specific target, full target name must be given.
 Avalible targets are: bitfire, atmosfire

 Usage > MCF -?:bitfire


 -p
 ------------------------------------------------------------------------------
 Pause after each helpscreen (if -? / -h / -help is called)

 Usage > MCF -? -p
"""
	elif stext=='bitfire':
		s = """
 Bitfire Commands

 Usage > MCF bitfire <command> ... [commandN]

 Quicklist: setmode[(mode)]
            reset[(target)]
 ------------------------------------------------------------------------------

 Command: setmode[(mode)]
 
 Mode parameter         Will set onboard toolmode to
 ------------------------------------------------------------------------------
 "mcu_jtag" or "1"      Onboard Wiggler (MCU target)
 "mcu_flash" or "2"     Activates LPC2129 bootloader
 "mcu_ext" or "3"       Make sure JTAG port is activated (w.RTCK) ext JTAG mode
 "fpga_jtag" or "4"     FPGA ByteBlaster-II connected to FPGA J-TAG port.
 "fpga_cfg" or "5"      FPGA ByteBlaster-II connected to FPGA CFG device.
 Omitted                Idle, exit onboard toolmode.


 Command: reset[(target)]

 Target parameter       Will reset following target
 ------------------------------------------------------------------------------
 "mcu"                  Resets the MCU
 "boot"                 Resets the MCU with bootloader activated (LPC2129)
 "fpga"                 Resets the FPGA
 Omitted                Reset both MCU and FPGA
"""
	elif stext=='atmosfire':
		s = """
 Atmosfire Commands

 Usage > MCF atmosfire <command> ... [commandN]

 Quicklist: cfg[(settings)]
            reset
            ping[(time)]


 Command: cfg[(setting(s))]

 Several settings can passed, they must be seprated with a ',' e.g.:
 > MCF atmosfire cfg(logo=on,boot=CS0,cke=on)

 Settings
 ------------------------------------------------------------------------------
 Omitted          Current configuration is read and displayed.

 "sfreset="       "on" - Release \R pin on SPI Flash (low when reset)
                  "off" - Keep \R pin low on SPI Flash

 "wakeup="        "on" - Drive WAKEUP pin (P0.15) controlled by 'PING'
                  "off" - Release WAKEUP pin (P0.15) to Hi-Z (floating)

 "boot="          "flash" - Internal flash mapped at 0x00000000
                  "ram" - Internal SRAM mapped at 0x00000000
                  "cs0" - External memory on chip select 0

 "standby="       "on" - Standby pin driven low
                  "off" - Standby pin release (pulled up by 10K in not in STB)

 "logo="          "on" - Flashing logo turned on
                  "off" - Flashing logo turned off

 "sfsel="         "on" - \S pin on serial flash mapped to pin P1.8
                  "off" - \S pin unmapped (pulled high by 10K)

 "sfwrite="       "on" - \W pin on serial flash mapped to pin P2.13
                  "off" - \W pin unmapped (pulled high by 10K)

 "extbus="        "on" - External bus driver enabled
                  "off" - External bus driver disabled

 "cke="           "on" - CPLD is driving CK clock as a push-pull
                  "off" - CPLD is tristating CK clock for ext drive purpose

 "hclke="         "on" - CPLD is drining HCLK clock as a push-pull
                  "off" - CPLD is tristating HCLK clock for ext drive purpose.

 "hclk="          "on" - HCLK running 4MHz
                  "off" - HCLK stopped (GND)

 "ck="            "off" - CPLD stopped CK
                  "4MHZ" - CPLD driving 4MHz clock to CK
                  "8MHZ" - CPLD driving 8MHz clock to CK
                  "16MHZ" - CPLD driving 16MHz clock to CK

 "sram="          "CS0" - External SRAM visible on CS0
                  "CS1" - External SRAM visible on CS1
                  "off" - External SRAM invisible (disabled)


 Command: ping[(time)]

 Sends a pulse on the WAKEUP-pin (if drive is enabled, see settings) or sets
 a period time that automatically pings device at a given interval.

 If a period is set and ping is called without time parameter the period will
 sync and restart its count and a pulse will be generated. Calling ping without
 parameters and no period set forces a one-shot pulse on wakeup-pin.

 time - the time in seconds between each ping in 5.12ms resolution maximum time
 is 335.539 seconds and minimum 15.36ms.

 Calling ping(0) will disable the periodic ping generator.

 Command: reset

 Resets the Atmosfire.
 """
 
	if len(s)==0:
		print "*** Error htext() no text to display!"
		pass

	if platform.system()=="Windows":
		try:
			if ppause:
				c=0
				for i in s.splitlines():
					print i
				c+=1
				if c >= 24:
					while not kbhit():
						pass
					getch()
					c=0
			else:
				print s
		except:
			pass
	else:
		print s
