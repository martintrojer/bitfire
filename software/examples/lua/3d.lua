bitfire.init()
bitfire.vector_load(0)
o=0

while o<200 do
	bitfire.clrscr()
	bitfire.vector_render(512/18,100,100)
	bitfire.flipbuf()
--	sleep(1)
end
