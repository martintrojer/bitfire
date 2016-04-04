bitfire.init()

s=0
o=0
while s==0 do
	bitfire.clrscr()
	s,o = bitfire.pixsinscroll("lua",0,0,100,100,o,0)
	bitfire.flipbuf()
--	sleep(1)
end
