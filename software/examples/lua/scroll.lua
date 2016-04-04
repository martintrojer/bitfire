bitfire.init()

s=0
o=0
while s==0 do
	bitfire.clrscr()
	s,o = bitfire.scrolltext("lua",0,0,100,100,o,1)
	bitfire.flipbuf()
	sleep(1)
end
