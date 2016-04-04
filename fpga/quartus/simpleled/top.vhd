-- ledspi testbench
-- mtrojer@arrownordic.com
-- $Id$

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;

entity top is 
  port( tclk : in std_logic;        
		shutmux, shutled : out std_logic;
		toutclk : out std_logic;
		d1_spi0, d2_spi0, d3_spi0, d4_spi0: out std_logic;
        tpwm, en0_mux, nlatch_spi0, nenable_spi0, clk_spi0, do_spi0: out std_logic);
end;

architecture top of top is
  component OUTSPI
  port( clk, rst: in std_logic;        
        indata: in std_logic_vector(15 downto 0);
        inrdy: in std_logic;
        outdata,outclk: out std_logic;
        rdy: out std_logic);
  end component;

	component SIMPLEPWM is
	  port ( clk : in std_logic;
   	      duty : in std_logic_vector(7 downto 0);
   	      pwm : out std_logic );
	  end component;

	component mypll
		PORT
		(
			inclk0		: IN STD_LOGIC  := '0';
			c0		: OUT STD_LOGIC 
		);
	end component;

	signal duty : integer range 0 to 255;

	signal trst : std_logic;
	signal spirdy : std_logic;
	signal sendrdy : std_logic;
	signal ctr : integer range 0 to 50400;
	
	signal leds : integer range 0 to 255;
	signal fastclk : std_logic;
	  
begin
  U0: OUTSPI port map(clk=>fastclk,
                      rst=>trst,
                      indata=>conv_std_logic_vector(leds,16),
                      inrdy=>sendrdy,
                      outdata=>do_spi0,
                      outclk=>clk_spi0,
                      rdy=>spirdy);   

  U1: SIMPLEPWM port map(clk=>tclk,
                     	duty=>conv_std_logic_vector(duty,8),
                     pwm=>tpwm);


	U2: mypll port map(inclk0=>tclk, c0=>fastclk);

	shutmux <= '1';
	shutled <= '0';
	nenable_spi0 <= '0';

	d1_spi0 <= '0';
	d2_spi0 <= '0';
	d3_spi0 <= '0';
	d4_spi0 <= '0';
		
	duty <= 64;
	leds <= 170;
	
	trst <= '0';
	
	toutclk <= fastclk;

	process(tclk)							-- ctr
	begin
		if tclk'event and tclk='1' then
			if ctr=50400 then
				ctr <= 0;
			else
				ctr <= ctr+1;
			end if;
		end if;		
	end process;

	process(tclk)							-- en0_mux
	begin
		if tclk'event and tclk='1' then
			if ctr=0 then
				en0_mux <= '1';
			elsif ctr=7200 then
				en0_mux <= '0';
			end if;	
		end if;
	end process;

	process(tclk)							-- sendrdy
	begin
		if tclk'event and tclk='1' then
			if ctr<20 then
				if spirdy='1' then
					sendrdy<='1';
				else
					sendrdy<='0';
				end if;
			end if;
		end if;
	end process;

	process(tclk)
	begin
		if tclk'event and tclk='1' then
			if ctr=40 then
				nlatch_spi0 <= '1';
			elsif ctr=42 then
				nlatch_spi0 <= '0';
			end if;
		end if;
	end process;
end;
