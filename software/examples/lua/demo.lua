-- Atmosfire/Bitfire demoreel in Lua
-- Martin Trojer 2007 (mtrojer@arrownordic.com)

scrolls = {	"Welcome to the Atmosfire development kit workshops 2007!              ",
			"Atmosfire Credits",
   			"Lead hardware design: Daniel Nilsson",
	  		"Lead sofrware design: Martin Trojer",
	 		"                                                                      ",
			"Let's atmosfire baby!                                                 "}

scroll_ctr = 1
scroll_offset = 0
effect_ctr = 0
vect_toggle = true
scroll_toggle = true
tmp_ctr = 0
		
bitfire.init()

while true do
-- Effects
--	if tmp_ctr == 0 then
--		bitfire.clrscr()
--		bitfire.flipbuf()
--		bitfire.clrscr()
--	end
	if effect_ctr == 0 then
		bitfire.plasma()
	elseif effect_ctr == 1 then
		if tmp_ctr == 0 then
			if vect_toggle then
				bitfire.vector_load(0)
			else
				bitfire.vector_load(1)
			end
			vect_toggle = not vect_toggle
		end
		bitfire.clrscr()
		if vect_toggle then
			bitfire.vector_render(512/8,50,50)
		else
			bitfire.vector_render(512/18,50,50)
		end
	elseif effect_ctr == 2 then
		bitfire.fire()
	elseif effect_ctr == 3 then
		bitfire.clrscr()
		bitfire.starfield(128)
	elseif effect_ctr == 4 then
		bitfire.fire2()
	end

	tmp_ctr = math.mod(tmp_ctr+1,500)
	if tmp_ctr==0 then
		effect_ctr = math.mod(effect_ctr+1,5)
--		print("******** PING!", effect_ctr)
	end
			
-- Scrolls
--	if scroll_offset==0 then
--		print(scrolls[scroll_ctr])
--	end

	if scroll_toggle then
		s,scroll_offset = bitfire.charsinscroll(scrolls[scroll_ctr],0,0,200,200,scroll_offset,0)
	else
		s,scroll_offset = bitfire.pixsinscroll(scrolls[scroll_ctr],0,0,200,200,scroll_offset,0)
	end

	bitfire.flipbuf()
	if s==1 then
		scroll_offset = 0
		scroll_toggle = not scroll_toggle
		scroll_ctr = 1 + math.mod(scroll_ctr,table.getn(scrolls))
	end

end
