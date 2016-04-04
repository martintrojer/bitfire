#
# command.py, command module for high-level communication
#

from comm import *
import sys
from hex_import import *

def DeviceTable(l):
    print "/--------------------------------------------------------------------------\\"
    print "| State |  Equipment  |     PID    | NAdr |  CRC   |  Bootresponse  |  I/F |"
    print "|-------+-------------+------------+------+--------+----------------+------|"
    for i in l:
        r = '| '+i[0].ljust(6)+'| '
        r += i[1].ljust(12)+'| 0x'
        r += ((hex(i[2])[2:]).rjust(8,'0')).strip('L') + ' | 0x'
        r += ((hex(i[3])[2:]).rjust(2,'0'))+ ' | 0x'
        r += (hex(i[4])[2:]).rjust(4,'0') + ' | '
        r += i[5].ljust(15) + '| '
        r += i[7].ljust(4)+' |'
        print r

    print "\\--------------------------------------------------------------------------/"
    
# Discover, function for creating a list with information on alla devices
# connected to the MagicConfig network
def Discover():
    # List of lists that contain
    # 1. Exec State (Application/Boot)
    # 2. Equipment ID
    # 3. Product ID
    # 4. Network adress
    # 5. Firmware ID
    # 6. Bootcode
    # 7. Boot vString
    # 8. Boot interface
    l=[]

    # First examin bridge device
    if not com_checkopen(): return []

    com_local()

    if com_reveal():
        s=com_getstatus()
        v=com_getversion()
        l.append(makelrec(s,v))

        # If bridge device has a tunnel examin rest of the network accordingly
        # I2C bridge equipped
        if l[0][1] in ('Bitfire','FireFighter'):
            if l[0][0] == 'Appl':
                if com_openiic(0):
                    s=com_getstatus()
                    if len(s)>0:
                        v=com_getversion()
                        l.append(makelrec(s,v,'i2c'))
                    com_local()
            
    return l

def makelrec(s,v,type='uart'):
    # Equipment ID list
    eid = {1:'Bitfire',2:'Atmosfire'}
    bcode = {0:'Passed',1:'Bootbyte Lock',2:'Checksum Error',3:'Hardware Lock'}
    estate = {'A':'Appl','B':'Boot'}

    a=[]
        
    if type=='uart':
        # Exec state
        if estate.has_key(s[:1]):
            a.append(estate[s[:1]])
        else:
            a.append('Unkown State')

        # Equipment ID
        try:
            i = int(s[2:4],16)
        except:
            i = 0
            
        if eid.has_key(i):
            a.append(eid[i])
        else:
            a.append('Unknown')

        # Product ID
        try:
            a.append(int(s[5:13],16))
        except:
            a.append(0)

        # Network Adress
        a.append(2)

        # Firmware CRC
        try:
            a.append(int(s[17:21],16))
        except:
            a.append(0)

        # Bootcode
        i = int(s[22:24],16)
        if bcode.has_key(i):
            a.append(bcode[i])
        else:
            a.append('Code Unknown')

        a.append(v)
        a.append('UART')

    if type=='i2c':
        # Exec state
        if estate.has_key(s[3:4]):
            a.append(estate[s[3:4]])
        else:
            a.append('Unkown State')

        # Equipment ID
        i = int(s[5:7],16)
        if eid.has_key(i):
            a.append(eid[i])
        else:
            a.append('Unknown')

        # Product ID
        a.append(int(s[8:16],16))

        # Network Adress
        a.append(int(s[17:19],16))

        # Firmware ID
        a.append(int(s[20:24],16))

        # Bootcode
        i = int(s[25:27],16)
        if bcode.has_key(i):
            a.append(bcode[i])
        else:
            a.append('Code Unknown')

        a.append(v[3:])
        a.append('I2C')
        
    return a

# ComOpen, function for opening and verifying connection to a MagicConfig
# loader. If port is omitted it will search all avalible ports.
def ComOpen(port=''):
    if port == '':
        print "Looking for MagicConfig loader ...",
        for i in range(100):
            if com_setup(i):
                if com_reveal():
                    print "device discovered on %s."%ser.portstr
                    return -1
        print "Not found!"
    else:
        print "Opening connection on port %s ..."%port,
        if com_setup(port):
            if com_reveal():
                print "device discovered on %s."%ser.portstr
                return -1
        print "Failed!"
    return 0

# Flashning application code into device at the open channel
def FlashTarget(mem,APP_BEGIN,APP_END):
    print 'Requesting status string    ...',
    s = com_getstatus(True)
    if s<>'':
        print '"' + s + '"'
    else:
        print "Error!"
        return 0

    print 'Requesting version string   ... %s'%com_getversion()

    if s[:2] == 'A-':
        print 'Freezing from app-mode      ...',
        com_freeze()
        print "OK"
        sleep(1)

    print 'Contacting bootloader       ...',
    if com_reveal():
        print "OK"
    else:
        print "Error"
        return 0

    print 'Format application sector   ...',
    if com_format():
        print "OK"
    else:
        print "Error"
        return 0

    # Flash application
    print 'Flashing application        ...',
    for i in range(APP_BEGIN,APP_END+1):
        for i2 in range(0,4):
            if not com_load(i,i2,mem):
                print "Load error!"
                return 0
            sys.stdout.write('.')
        if not com_flash():
            print "Flash error!"
            return 0
    print "Done!"

    crc = CalcChecksum((APP_BEGIN*0x40),((APP_END*0x40)+63),mem)
    print "Calculating checksum          : 0x%04x"%crc

    print 'Writing checksum            ...',
    if com_writecrc(crc):
        print "OK"
    else:
        print "Error!"
        return 0

    print 'Unleash application         ...',
    if com_unleash():
        sleep(1)
        print "OK"
    else:
        print "Error!"
        return 0

    print 'Contacting bootloader       ...',
    if com_reveal():
        print "OK"
    else:
        print "Error"
        return 0

    print 'Requesting status string    ... %s'%com_getstatus(True)
    print "MagicConfig application download finished"

def IdUnit(ulist,param):
    r=[]
    rc='Identifying target by '
    if len(ulist)>0:
        # Adress / PID
        if param[0:2].upper()=='0X':
            # Adress (not UART)
            if len(param) == 4:
                rc += 'network adress... '
                try:
                    adr = int(param[2:],16)
                    for s in ulist:
                        if (s[7] != 'UART') and (s[3]==adr):
                            r=s
                            break
                except:
                    rc += "Parameter error!"

            # PID
            elif len(param) == 10:
                rc+='product ID (PID)... '
                try:
                    pid = int(param[2:],16)
                    for s in ulist:
                        if s[2]==pid:
                            r=s
                            break
                except:
                    rc += 'Parameter error! '

            else:
                rc += ' Unsupported hex format!'

        # EID name
        else:
            rc += 'equimpent ID name... '
            for s in ulist:
                if (s[1].upper()).find(param.upper()) != -1:
                    r = s
                    break

    return r,rc

# Bitfire support commands
def bitfire_setmode(arg):
    print "Setting toolmode '%s' ..."%arg,

    if arg=='(mcu_jtag)' or arg=='(1)':
        mode = 'mcu_wiggler'
    elif arg=='(mcu_flash)' or arg=='(2)':
        mode = 'mcu_flash'
    elif arg=='(mcu_ext)' or arg=='(3)':
        mode = 'mcu_extern'
    elif arg=='(fpga_jtag)' or arg=='(4)':
        mode = 'fpga_jtag'
    elif arg=='(fpga_cfg)' or arg=='(5)':
        mode = 'fpga_cfg'
    else:
        mode = ''

    if len(arg)!=0 and len(mode)==0:
        print "Illegal mode!"
    else:
        if com_setmode(mode):
            print "OK"
        else:
            print "Fail"

def bitfire_reset(arg):
    if arg=='(mcu)':
        print "Resetting MCU ...",
        if com_doreset('cpu'):
            print "OK"
        else:
            print "Fail"
            
    elif arg=='(boot)':
        print "Resetting MCU and activating bootloader (LPC2129 only) ...",
        if com_doreset('cpu_boot'):
            print "OK"
        else:
            print "Fail"

    elif arg=='(fpga)':
        print "Resetting MCU and FPGA targets ...",
        if com_doreset('cpu') and com_doreset('fpga'):
            print "OK"
        else:
            print "Fail"

    else:
        print "Reset command: Illegal target!"

# Atmosfire support commands
def atmosfire_reset():
    print "Reset command ...",
    if com_reset():
        print "OK"
    else:
        print "Fail"

def atmosfire_ping(arg):
    if len(arg)==0:
        print "Sending one-shot ping command ...",
        if com_ping():
            print "OK"
        else:
            print "Fail"
    else:
        print "Setting pingtime,",
        s=arg[1:len(arg)-1]
        try:
            # Convert
            t=float(s)
            if t>335.539:
                t=335.539
            if t==0:
                t=0
            else:
                if t<0.01536:
                    t=0.01536

            # Adjust
            v=int(t/0.00512)
            t=v*0.00512
            
            print "time set %(1)01.03fs (0x%(2)04X) writing ..."%{'1':t,'2':v},

            if com_ping(v):
                print "OK"
            else:
                print "Fail"
            
        except:
            print "Value Error!"


def atmosfire_cfg(arg):
    l=strip_keys(arg)
    smask=0
    cmask=0

    print "Setting configuration,",

    # Set mask and clear mask generation.
    for k in l:
        ival=False
        if k[0]=='sfreset':
            if k[1]=='on':
                smask |= 1<<15
            elif k[1]=='off':
                cmask |= 1<<15
            else:
                ival=True

        elif k[0]=='wakeup':
            if k[1]=='on':
                smask |= 1<<14
            elif k[1]=='off':
                cmask |= 1<<14
            else:
                ival=True

        elif k[0]=='boot':
            if k[1]=='flash':
                cmask |= (1<<13 | 1<<12)
            elif k[1]=='ram':
                smask |= 1<<13
                cmask |= 1<<12
            elif k[1]=='cs0':
                smask |= (1<<13 | 1<<12)
            else:
                ival=True

        elif k[0]=='standby':
            if k[1]=='on':
                cmask |= 1<<11
            elif k[1]=='off':
                smask |= 1<<11
            else:
                ival=True

        elif k[0]=='logo':
            if k[1]=='on':
                smask |= 1<<10
            elif k[1]=='off':
                cmask |= 1<<10
            else:
                ival=True

        elif k[0]=='sfsel':
            if k[1]=='on':
                smask |= 1<<9
            elif k[1]=='off':
                cmask |= 1<<9
            else:
                ival=True

        elif k[0]=='sfwrite':
            if k[1]=='on':
                smask |= 1<<8
            elif k[1]=='off':
                cmask |= 1<<8
            else:
                ival=True

        elif k[0]=='extbus':
            if k[1]=='on':
                smask |= 1<<7
            elif k[1]=='off':
                cmask |= 1<<7
            else:
                ival=True

        elif k[0]=='cke':
            if k[1]=='on':
                smask |= 1<<6
            elif k[1]=='off':
                cmask |= 1<<6
            else:
                ival=True

        elif k[0]=='hclke':
            if k[1]=='on':
                smask |= 1<<5
            elif k[1]=='off':
                cmask |= 1<<5
            else:
                ival=True

        elif k[0]=='hclk':
            if k[1]=='on':
                cmask |= 1<<4
            elif k[1]=='off':
                smask |= 1<<4
            else:
                ival=True

        elif k[0]=='ck':
            if k[1]=='off':
                cmask |= (1<<3 | 1<<2)
            elif k[1]=='4mhz':
                smask |= 1<<2
                cmask |= 1<<3
            elif k[1]=='8mhz':
                cmask |= 1<<2
                smask |= 1<<3
            elif k[1]=='16mhz':
                smask |= (1<<3 | 1<<2)
            else:
                ival=True

        elif k[0]=='sram':
            if k[1]=='off':
                cmask |= (1<<1 | 1<<0)
            elif k[1]=='cs0':
                smask |= (1<<1 | 1<<0)
            elif k[1]=='cs1':
                smask |= 1<<0
                cmask |= 1<<1
            else:
                ival=True

        if ival:
            print "Invalid key at setting '%s'!"%k[0]

    print "set mask %(smask)04X, clear mask %(cmask)04X."%{'smask':smask,'cmask':cmask}

    if smask!=0:
        print "Set config register ...",
        if com_setcfg(smask):
            print "OK"
        else:
            print "Fail"

    if cmask!=0:
        print "Clear config register ...",
        if com_clrcfg(cmask):
            print "OK"
        else:
            print "Fail"

    print "Reading current configuration,",
    r=com_getcfg()
    if len(r)==8:
        try:

            # Read config
            v=int(r[:6],16)
            print "config read %04X."%v

            # Display current configuration
            print
            print "Atmosfire Configuration"
            print "-"*78
            print "Serial flash reset pin :",
            if v & 0x8000:
                print "Controlled by system"
            else:
                print "Tied low"

            print "Wakeup pin             :",
            if v & 0x4000:
                print "Wakeup pin controlled by housekeeper"
            else:
                print "Wakeup pin floating (not controlled)"

            print "Boot mapping           :",
            if (v & 0x3000) == 0:
                print "Internal flash at 0x00000000"
            elif (v & 0x3000) == 0x1000:
                print "Illegal state!"
            elif (v & 0x3000) == 0x2000:
                print "Internal SRAM at 0x00000000"
            elif (v & 0x3000) == 0x3000:
                print "External memory on CS0 at 0x00000000"

            print "Standby pin            :",
            if (v & 0x800) == 0:
                print "Device in standby (low)."
            else:
                print "Device not in standby (high)."

            print "Logo enabled           :",
            if (v & 0x400):
                print "Logo enabled"
            else:
                print "Logo disabled"

            print "Serial flash \S pin    :",
            if (v & 0x200):
                print "Mapped to P1.8 on MCU"
            else:
                print "Unmapped (pulled high by 10K)"

            print "Serial flash \W pin    :",
            if (v & 0x100):
                print "Mapped to P2.13 on MCU"
            else:
                print "Unmapped (pulled high by 10K)"

            print "External bus driver    :",
            if (v & 0x80):
                print "Bus buffers enabled"
            else:
                print "Bus buffers disabled"

            print "CK drive enabled       :",
            if (v & 0x40):
                print "Clock driven from CPLD."
            else:
                print "Floating."

            print "HCLK drive enabled     :",
            if (v & 0x20):
                print "Clock driven from CPLD."
            else:
                print "Floating."

            print "HCLK stop select       :",
            if (v & 0x10):
                print "HCLK stopped."
            else:
                print "HCLK (4MHz) running."

            print "CK clock speed setting :",
            if (v & 0x0C) == 0:
                print "Tied to GND (stopped)."
            elif (v & 0x0C) == 0x04:
                print "4MHz (/4)"
            elif (v & 0x0C) == 0x08:
                print "8MHz (/2)"
            elif (v & 0x0C) == 0x0C:
                print "16MHz (/1)"

            print "SRAM map setting       :",
            if (v & 0x01) == 0:
                print "SRAM Disabled"
            elif (v & 0x03) == 0x01:
                print "SRAM mapped on CS1"
            elif (v & 0x03) == 0x03:
                print "SRAM mapped in CS0"

            print
        except:
            print "Failed cfg()!"
    else:
        print "Failed getcfg()!"

def strip_keys(arg):
    # Strip keywords with '=' assigns in a commalist and return a list collection
    s=arg.strip('()').lower()
    l=[]

    while s.find('=') != -1:
        pass
        st=s.find('=')
        if s.find(',')!=-1:
            stp=s.find(',')+1
            l.append([s[:st],s[st+1:s.find(',')]])
        else:
            l.append([s[:st],s[st+1:]])
            stp=len(s)
        s=s[stp:]
            
    return l

