-- inspi testbech
-- mtrojer@arrownordic.com
-- $Id$

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;

entity top is 
  port( tinclk	: in std_logic;
		t2, tout   : out std_logic);
end;

architecture top of top is

	component SIMPLEPWM is
	  port ( clk : in std_logic;
   	      duty : in std_logic_vector(7 downto 0);
   	      pwm : out std_logic );
	  end component;

	signal ctr : integer range 0 to 255;

begin
  U0: SIMPLEPWM port map(clk=>tinclk,
                     duty=>conv_std_logic_vector(ctr,8),
                     pwm=>tout);

	t2 <= tinclk;	

	ctr <= 64;
end;
