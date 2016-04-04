# opt.py, Option parser support

def getoptv(l,opt):
    for s in l:
        if (s.find(opt) == 0):
            if opt[len(opt)-1:len(opt)] != ':':
                if s[len(opt):len(opt)+1] in (' ',''):
                    return s[len(opt):]
            else:
                return s[len(opt):]
    return ''

def findopt(l,opt):
    for s in l:
        if (s.find(opt) == 0):
            if opt[len(opt)-1:len(opt)] != ':':
                if s[len(opt):len(opt)+1] in (' ',''):
                    return -1
            else:
                return -1
    return 0

def getoptlist(l,opt):
    for s in l:
        if (s.find(opt) == 0):
            if opt[len(opt)-1:len(opt)] != ':':
                if s[len(opt):len(opt)+1] in (' ',''):
                    a = s[len(opt):]
                    a = a.replace(':',' ')
                    return a.split()
            else:
                a = s[len(opt):]
                a = a.replace(':',' ')
                return a.split()
    return 0

# Returns new list without option/switched (searches for the first not-switch)
def stripopts(l):
    for i in range(len(l)):
        if l[i][:1]!='-':
            return l[i:]
    return []
