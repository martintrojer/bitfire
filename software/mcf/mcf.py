#
# MagicConfig utility
# By Daniel Nilsson (C) 2007, Arrow Engineering - TurnKey Solutions
#

import string
from hex_import import *
from comm import *
from sys import exit
from time import sleep
from helptext import htext
from command import *
from sys import argv
from opt import *

# Application sector given in number of pages
APP_BEGIN = 0x21
APP_END = 0x3F
mem = []

arg=argv[1:]

print """
 MCFG, MagicConfig Command Line Utility
 Daniel Nilsson (C) 2007 Arrow TurnKey Solutions, Version 1.00
"""


# Check if page-pause option exist and set ppause value
ppause=False
if findopt(arg,'-p'):
    ppause=True

# Print basic help if arg is empty or -help, -h, -? or --help options are given
if (len(arg) == 0 or findopt(arg,'-h') or findopt(arg,'-help') or findopt(arg,'--help') or findopt(arg,'-?')) or (len(arg)==1 and findopt(arg,'-p')):
    htext('basic',ppause)
    exit()

# Print help on a specific target if '-?:' is given.
if (findopt(arg,'-?:')):
    t=getoptv(arg,'-?:')
    if t.lower() in ('atmosfire','bitfire'):
        htext(t.lower(),ppause)
        exit()

# First locate the correct COM-port
if findopt(arg,'-c:'):
    if ord('0') <= ord(getoptv(arg,'-c:')[:1]) <= ord('9'):
        if not ComOpen(int(getoptv(arg,'-c:'))): exit()
    else:
        if not ComOpen(getoptv(arg,'-c:')): exit()
else:
    ComOpen()

print

# Load memory buffer if '-l' option exists.
if findopt(arg,'-l:'):
    mem,crc = LoadHEX(getoptv(arg,'-l:'),APP_BEGIN,APP_END)
    print

ulist = []

# List all units on network if '-list' option exist
if findopt(arg,'-list'):
    print "Discovering network ...",
    if not com_checkopen():
        print "Failed, comport not open."
        exit()

    ulist = Discover()
    if len(ulist) == 0:
        print "failed"
        exit()

    print "Done"
    DeviceTable(ulist)
    print
    
# Flash target if '-d:' option exists and buffer is loaded.
if findopt(arg,'-d:'):
    if len(mem) == 0:
        print "Trying to flash, but memorybuffer is empty!"
    else:
        print "-[ Flashing application sector ]-"
        if len(ulist) == 0:
            print "Discovering network configuration...",
            ulist=Discover()
            if len(ulist) == 0:
                print "Failed!"
            else:
                print "Done"

        r,rc = IdUnit(ulist,getoptv(arg,'-d:'))
        print rc,
        
        if len(r)>0:
            print "Done"
            print "Found EID:%(EID)s, PID:0x%(PID)08X, ADR:0x%(ADR)02X, Exec:%(EXE)s"%{'EID':r[1],'PID':r[2],'ADR':r[3],'EXE':r[0]}
            print "Setting target channel,",
            if r[3] == 0:
               print "Target adr 0x00 prohibited, please bind!"
            elif r[7] == 'I2C':
                print "opening I2C adress %02X..."%r[3],
                if com_openiic(r[3]):
                    if com_getstatus()[17:19] == '%02X'%r[3]:
                        print "OK"
                        FlashTarget(mem,APP_BEGIN,APP_END)
                    else:
                        print "Failed getstatus()!"
                else:
                    print "Failed openiic()!"
            elif r[7] == 'UART':
                print "closing tunnel...",
                if com_local():
                    if com_getstatus()[14:16]== 'XX':
                        print "OK"
                        FlashTarget(mem,APP_BEGIN,APP_END)
                    else:
                        print "Failed getstatus()!"
                else:
                    print "Failed local()"
            else:
                    print "Interface type not supported!"
                    
        else:
            print "Target not found!"
    print

# Bind command if '-bind' option is found in argument.
if findopt(arg,'-bind') or findopt(arg,'-bind:'):
    print "-[ Adress binding operation ]-"

    # Discover network if not done before
    if len(ulist) == 0:
        print "Discovering network configuration...",
        ulist=Discover()
        if len(ulist) == 0:
            print "Failed!"
        else:
            print "Done"


    # Create address list
    l=[]
    for i in range(256):
        l.append(i)

    # Remove already binded devices
    for i in ulist:
        if i[3]!=0:
            print "Found binded member EID:%(EID)s, PID:0x%(PID)08X"%{'EID':i[1],'PID':i[2]}
            l[i[3]]=-1

    s = getoptv(arg,'-bind:')

    # Autobind members
    if s == '':
        c=1
        nadr=0
        nf=True
        for i in ulist:
            while l[c] == -1:
                c+=1
            nadr=l[c]
                
            if i[3]==0 and i[7]=='I2C':
                print "Binding member EID:%(EID)s, PID:0x%(PID)08X ..."%{'EID':i[1],'PID':i[2]},
                if com_openiic(0):
                    if com_bind(i[2],nadr):
                        print "OK"
                        nf=False
                    else:
                        print "bind(), bind command failed!"
                else:
                    print "bind(), failed to open tunnel!"
        if nf:
            print "No unbinded members found!"

    # Manual bind member
    else:
        i = s.find(':')
        nf=True
        if i == -1 or len(s)!=11:
            print "Bind target information syntax error!"
        else:
            try:
                pid=int(s[:i],16)
                adr=int(s[i+1:],16)
            except:
                pid=0
                adr=0

            # Find member
            for m in ulist:
                if m[2]==pid:
                    nf=False
                    print "Binding member EID:%(EID)s, PID:0x%(PID)08X ..."%{'EID':m[1],'PID':m[2]},

                    if com_openiic(0):
                        if com_bind(pid,adr):
                            if adr==0:
                                print "OK, unbinded"
                            else:
                                print "OK, binded to %02X"%adr
                        else:
                            print "bind(), bind command failed!"
                    else:
                        print "bind(), failed to open tunnel!"

                    com_local()
        if nf:
            print "No matching network members found!"

# Find the target and check if there is a command(s) to execute
cmd = stripopts(arg)
cexec=False
if len(cmd) > 0:
    print "-[ Executing list of commands ]-"

    # Discover network if not done before
    if len(ulist) == 0:
        print "Discovering network configuration...",
        ulist=Discover()
        if len(ulist) == 0:
            print "Failed!"
        else:
            print "Done"

    # Try identifying target
    r,rc = IdUnit(ulist,cmd[0])
    print rc,

    # Locate and open channel to device
    if len(r)>0:
        print "Done"
        print "Found EID:%(EID)s, PID:0x%(PID)08X, ADR:0x%(ADR)02X, Exec:%(EXE)s"%{'EID':r[1],'PID':r[2],'ADR':r[3],'EXE':r[0]}
        print "Setting target channel,",
        if r[3] == 0:
            print "Target adr 0x00 prohibited, please bind!"
        elif r[7] == 'I2C':
            print "opening I2C adress %02X..."%r[3],
            if com_openiic(r[3]):
                if com_getstatus()[17:19] == '%02X'%r[3]:
                    print "OK"
                    cexec=True
                else:
                    print "Failed getstatus()!"
            else:
                print "Failed openiic()!"
        elif r[7] == 'UART':
            print "closing tunnel...",
            if com_local():
                if com_getstatus()[14:16]== 'XX':
                    print "OK"
                    cexec=True
                else:
                    print "Failed getstatus()!"
            else:
                print "Failed local()"
        else:
            print "Interface type not supported!"
    else:
        print "Target not found!"

    # Execute command list if channel open was sucessful
    if cexec:
        print
        # Loop for all commands
        c=0
        for s in cmd[1:]:
            print "%02d:"%c,
            c+=1
            
            # Bitfire application commands
            if r[1]=='Bitfire':
                if (s.lower()).find('setmode')==0:
                    bitfire_setmode(s.lower()[7:])
                elif (s.lower()).find('reset')==0:
                    bitfire_reset(s.lower()[5:])

            # Atmosfire application commands
            elif r[1]=='Atmosfire':
                if (s.lower()).find('ping')==0:
                    atmosfire_ping(s.lower()[4:])
                elif (s.lower()).find('cfg')==0:
                    atmosfire_cfg(s.lower()[3:])
                elif s.lower()=='reset':
                    atmosfire_reset()

            # Target not supported
            else:
                print "Target not supported!"

if com_checkopen():
    com_quit()
    com_close()
print
print "Operations finished, leaving."
