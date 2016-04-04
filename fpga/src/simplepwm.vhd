-- cpuctrl.vhd
-- mtrojer@arrownordic.com
-- $WCREV$
--
-- Duty Cyclone one of 16 steps.
-- outclk period i 1/16 of inclk's.

library  ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;

entity SIMPLEPWM is
  port ( clk : in std_logic;
         duty : in std_logic_vector(7 downto 0);
         pwm : out std_logic );
end SIMPLEPWM;

architecture simplepwn_behave of SIMPLEPWM is
  signal ctr : integer range 0 to 255;
begin

  pwm <= '0' when ctr >= conv_integer(duty) else '1';

  process (clk)
  begin
    if clk'event and clk='1' then
      if ctr=255 then
        ctr <= 0;
      else
        ctr <= ctr + 1;
      end if;
    end if;
  end process;
  
end simplepwn_behave;
