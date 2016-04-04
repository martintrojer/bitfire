-- simplepwm testbech
-- mtrojer@arrownordic.com
-- $WCREV$

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;

entity testbench is end;

architecture testbench of testbench is
  component SIMPLEPWM
  port ( clk : in std_logic;
         duty : in std_logic_vector(3 downto 0);
         pwm : out std_logic );
  end component;

  signal sclk : std_logic := '0';
  signal sduty : std_logic_vector(3 downto 0) := "0000";
  signal spwm : std_logic;
  
begin
  U0: SIMPLEPWM port map(clk=>sclk, duty=>sduty, pwm=>spwm);

  sclk <= not(sclk) after 10 ns;

  process
  begin
    for i in 0 to 2500 loop
      wait until sclk='1';
      if i=0 then
        sduty <= conv_std_logic_vector(0,4);
      elsif i=500 then
        sduty <= conv_std_logic_vector(4,4);        
      elsif i=1000 then
        sduty <= conv_std_logic_vector(8,4);        
      elsif i=1500 then
        sduty <= conv_std_logic_vector(12,4);        
      elsif i=2000 then
        sduty <= conv_std_logic_vector(15,4);        
      end if;
    end loop;
  end process;

end;
