#
# Communication functions for MagicConfig based bootloader and application
#

import serial
import string
from string import find
from time import sleep
from time import time

ser = serial.Serial()
IIC_on = 0
pagesize=64

# Check if comport is open
def com_checkopen():
    if ser.isOpen():
        return -1
    else:
        return 0

# Public com_setup function
def com_setup(p):
    try:
        _com_setup(p)
    except serial.SerialException:
        return 0
    return -1

# Setup MagicConfig on serial port
def _com_setup(p):
    global ser
    ser.baudrate = 38400
    ser.port = p
    ser.timeout = 1
    ser.open()

# Unlock the interface
def com_reveal():
    global ser,IIC_on
    ser.flushOutput()
    ser.flushInput()
    if not IIC_on:
        ser.write('\x1a#close\r')
        sleep(0.05)
        ser.write('\x1aquit\r')
        ser.write('\x1aGreetingZ_MrMega,ReveaL_YourselF%760913%#\r')

        sleep(0.05)
        ser.flushInput()

    ser.write('\x1aversion\r')
    r = ser.readline()

    if find(r,'MCF') == -1:
        return 0

    return -1

# com_wpid function
def com_wpid(pid):
    global ser,IIC_on
    ser.write('\x1awpid ' + string.zfill((hex(pid))[2:],8) + '\r')

    r = ser.readline()
    while r == '\n':
        r = ser.readline()

    if IIC_on: r=r[3:]

    if r == 'OK\r\n':
        return -1
    else:
        return 0

# com_close function
def com_close():
    global ser
    ser.close()

# com_quit fucntion
def com_quit():
    ser.write('\x1a#close\r')
    sleep(0.05)
    ser.write('\x1aquit\r')
    return -1

# com_local function, exits IIC tunnel
def com_local():
    global ser,IIC_on
    
    ser.write('\x1a#close\r')

    r = readline_stout(0.05)
    while r == '\n':
        r = readline_stout(0.05)

    if r == 'OK\r\n':
        IIC_on=0
        return -1
    else:
        return 0

# Write checksum
def com_writecrc(crc):
    global ser,IIC_on
    ser.write('\x1acrc ' + string.zfill((hex(crc))[2:],4) + '\r')

    r = ser.readline()
    while r == '\n':
        r = ser.readline()

    if IIC_on: r=r[3:]

    if r == 'OK\r\n':
        return -1
    else:
        return 0

# Open I2C tunnel
def com_openiic(adr):
    global ser,IIC_on
    ser.write('\x1a#open ' + string.zfill((hex(adr))[2:],2) + '\r')

    r = ser.readline()
    while r == '\n':
        r = ser.readline()
    
    if r == 'OK\r\n':
        IIC_on = -1
        return -1
    else:
        return 0

# com_bind, bind adress on network device
def com_bind(pid,adr):
    global ser,IIC_on
    ser.write('\x1abind ' + string.zfill((hex(pid))[2:],8) + ' ' + string.zfill((hex(adr))[2:],2) + '\r')

    r = ser.readline()
    while r == '\n':
        r = ser.readline()

    if IIC_on: r=r[3:]

    if r == 'OK\r\n':
        return -1
    else:
        return 0

# Unleash application
def com_unleash():
    global ser
    ser.write('\x1aunleash\r')
    return -1

# Freeze the application
def com_freeze():
    global ser
    ser.write('\x1afreeze\r')
    return -1

# Get status
def com_getstatus(strp=False):
    global ser,IIC_on
    ser.write('\x1astatus\r')

    r = readline_stout(0.05)
    while r == '\n':
        r = readline_stout(0.05)

    if IIC_on and strp: r=r[3:]

    if len(r) > 2: r = r[:len(r)-2]
    return r

# Get version
def com_getversion():
    global ser,IIC_on
    ser.write('\x1aversion\r')

    r = readline_stout(0.05)
    while r == '\n':
        r = readline_stout(0.05)

    if IIC_on: r=r[3:]

    if len(r) > 2: r = r[:len(r)-2]
    return r

# Format application sector in device
def com_format():
    global ser,IIC_on

    ser.write('\x1aformat\r')

    r = ser.readline()
    while r == '\n':
        r = ser.readline()

    if IIC_on: r=r[3:]

    if r == 'OK\r\n':
        return -1
    else:
        return 0

# Load a flash page
def com_load(page,row,mem):
    global ser,pagesize

    adr = (pagesize*page)+(16*row)
    crc = 0xff
    count = 0
    r = ''
    for i in mem:
        if (i[0] <= (adr+15)) and ((i[0]+len(i)-2) >= adr):
            adj=0
            if (i[0] < adr):
                adj = (adr-i[0])
            if (i[0] > (adr+(len(r)/2))):
                s = (len(r)/2)
                r = string.ljust(r,(i[0]-adr)*2,'F')
                crc = (crc + (i[0]-adr-s)) & 0xff

            sadr = i[0]+adj
            for i2 in i[1+adj:]:
                if (sadr < adr+16) and (sadr >= adr):
                    r = r + string.zfill(hex(i2)[2:],2)
                    crc = (crc - i2) & 0xff
                sadr = sadr + 1

        if len(r) == 32: break

    if len(r)<32:
        crc = (crc + (16-(len(r)/2))) & 0xff
        r = string.ljust(r,32,'F')
    r = r + string.zfill(hex(crc)[2:],2)
    r = 'l' + string.zfill(hex(page)[2:],2) + str(row & 0x03) + r
    ser.write('\x1a' + r + '\r')
    w = r

    r = ser.readline()
    while r == '\n':
        r = ser.readline()

    if IIC_on: r=r[3:]

    if r == 'OK\r\n':
        return -1
    else:
        return 0

# Perform flash programming
def com_flash():
    global ser,IIC_on
    
    ser.write('\x1aflash\r')

    r = ser.readline()
    while r == '\n':
        r = ser.readline()

    if IIC_on: r=r[3:]

    if r == 'OK\r\n':
        return -1
    else:
        print r
        return 0

# Setmode on Bitfire
def com_setmode(mode='-'):
    if mode == 'mcu_wiggler':
        sch = '0'
    elif mode == 'mcu_flash':
        sch = '1'
    elif mode == 'mcu_extern':
        sch = '2'
    elif mode == 'fpga_jtag':
        sch = '3'
    elif mode == 'fpga_cfg':
        sch = '4'
    else:
        sch = '-'

    ser.write('\x1asetmode '+sch+'\r')

    r = ser.readline()
    while r == '\n':
        r = ser.readline()

    if r == 'OK\r\n':
        return -1
    else:
        return 0

def com_doreset(trgt):
    if trgt == 'cpu':
        sch = 'c'
    elif trgt == 'cpu_boot':
        sch = 'b'
    elif trgt == 'fpga':
        sch = 'f'
    else:
        return "Parameter error"

    ser.write('\x1areset '+sch+'\r')

    r = ser.readline()
    while r == '\n':
        r = ser.readline()

    if r == 'OK\r\n':
        return -1
    else:
        return 0

# Atmosfire command support
def com_reset():
    global ser,IIC_on
    ser.write('\x1areset\r')

    r = ser.readline()
    while r == '\n':
        r = ser.readline()

    if IIC_on: r=r[3:]

    if r == 'OK\r\n':
        return -1
    else:
        return 0


def com_getcfg():
    global ser,IIC_on
    ser.write('\x1agetcfg\r')

    r = ser.readline()
    while r == '\n':
        r = ser.readline()

    if IIC_on: r=r[3:]

    return r

def com_setcfg(mask):
    global ser,IIC_on
    ser.write('\x1agetcfg\r')

    r = ser.readline()
    while r == '\n':
        r = ser.readline()

    if IIC_on: r=r[3:]

    return r

def com_clrcfg(mask):
    global ser,IIC_on
    ser.write('\x1aclrcfg '+string.zfill((hex(mask))[2:],4)+'\r')

    r = ser.readline()
    while r == '\n':
        r = ser.readline()

    if IIC_on: r=r[3:]

    if r == 'OK\r\n':
        return -1
    else:
        return 0

def com_setcfg(mask):
    global ser,IIC_on
    ser.write('\x1asetcfg '+string.zfill((hex(mask))[2:],4)+'\r')

    r = ser.readline()
    while r == '\n':
        r = ser.readline()

    if IIC_on: r=r[3:]

    if r == 'OK\r\n':
        return -1
    else:
        return 0

def com_ping(t=-1):
    global ser,IIC_on

    if t==-1:
        ser.write('\x1aping\r')
    else:
        ser.write('\x1apingtime '+ string.zfill((hex(t))[2:],4)+'\r')

    r = ser.readline()
    while r == '\n':
        r = ser.readline()

    if IIC_on: r=r[3:]

    if r == 'OK\r\n':
        return -1
    else:
        return 0

def readline_stout(tout):
    tc = time() + tout
    while tc > time():
        if ser.inWaiting(): return ser.readline()
        pass
    return ''
