#
# hex_import.py, function for importing intel hex-files
#

import string

# Application sector given in number of pages
APP_BEGIN = 0x21
APP_END = 0x3F

# Import, load buffer and display code mapping
def LoadHEX(file,APP_BEGIN,APP_END):
    mem = ImportHEX(file)
    if mem=="Error":
        print "Error loading hexfile..."
        exit()
    map = CodeMapping(mem)

    print 'Code mapping in hexfile "' + file +'"'
    print "-----------------------------------------------------------------"
    print "   Begin        End     Size    (dec)"
    for i in map:
        print string.rjust(hex(i[0]),8) + ' - ' + string.rjust(hex(i[1]),8) + ' '+ string.rjust(hex(i[1]-i[0]+1),8 )+ ' '+string.rjust(str(i[1]-i[0]+1),8)
    print "-----------------------------------------------------------------"
    crc = CalcChecksum((APP_BEGIN*0x40),((APP_END*0x40)+63),mem)
    print 'Calculate checksum          ... 0x' + string.zfill(hex(crc)[2:],4)
    return mem,crc

# Build memory mapping list
def CodeMapping(mem):
    result = []
    sta = mem[0][0]
    rowstop = sta + len(mem[0])-1
    
    for i in mem[1:]:
        if i[0] <> rowstop:
            result.append([sta,rowstop-1])
            sta = i[0]
        rowstop = (i[0] + len(i) - 1)

    result.append([sta,rowstop-1])
    return result

#import hex file
def ImportHEX(file):
    f=open(file,'r')
    mem = []
    ext_adr = 0
    for line in f:
        l = line
        data = 0
        if l[0:1] == ':':
            length = string.atoi(l[1:3],16)
            adress = string.atoi(l[3:7],16)
            type = string.atoi(l[7:9],16)

            # Data record
            if type == 0:
                data = []
                data.append(adress + ext_adr)
                for i in range(length):
                    data.append(string.atoi(l[9+(i*2):11+(i*2)],16))
                crc = string.atoi(l[11+(i*2):13+(i*2)],16)
                ccrc = ((((length & 0xff)+ (type & 0xff) + (adress & 0xFF)+ ((adress>>8)&0xff) + (sum(data[1:]) & 0xFF)) ^ 0xFF)+1) & 0xff

                # CRC Check
                if crc <> ccrc:
                    return "Error"

                # Store datarecord in tulip
                mem.append(data)

            # End record
            elif type == 1:
                return mem
            # Extended segment adress record
            elif type == 2:
                usba = string.atoi(l[9:13],16)
                ccrc = ((((length & 0xff)+ (type & 0xff) + (adress & 0xFF)+ ((adress>>8)&0xff) + (usba & 0xff) + ((usba>>8)&0xff)) ^ 0xFF)+1) & 0xff
                crc = string.atoi(l[(len(l)-3):],16)

                if crc <> ccrc:
                    return "Error"

                ext_adr = (usba << 4)

            # Extended linear adress record
            elif type == 4:
                ulba = string.atoi(l[9:13],16)
                ccrc = ((((length & 0xff)+ (type & 0xff) + (adress & 0xFF)+ ((adress>>8)&0xff) + (ulba & 0xff) + ((ulba>>8)&0xff)) ^ 0xFF)+1) & 0xff
                crc = string.atoi(l[(len(l)-3):],16)

                if crc <> ccrc:
                    return "Error"

                ext_adr = (ulba << 16)
    return 0

# Calculate checksum
def CalcChecksum(app_beg,app_end,mem):
    crc = 0xffff
    sadr = app_beg
    for i in mem:
        if (i[0] >= app_beg) and (i[0] <= app_end):

            while(sadr < i[0]):
                sadr = sadr + 1
                crc = (crc - 0xff) & 0xffff
            sadr = i[0]

            for i2 in i[1:]:
                # Only calculate checksum on application space
                if (sadr >= app_beg) and (sadr <= app_end):
                    crc = (crc - i2) & 0xffff
                sadr = sadr + 1

    while(sadr <= app_end):
        sadr = sadr + 1
        crc = (crc - 0xff) & 0xffff

    return crc
