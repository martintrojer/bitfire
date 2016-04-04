-- ledspi testbench
-- mtrojer@arrownordic.com
-- $WCREV$

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;

entity testbench is end;

architecture testbench of testbench is
  component OUTSPI
  port( clk, rst: in std_logic;        
        indata: in std_logic_vector(15 downto 0);
        inrdy: in std_logic;
        outdata,outclk: out std_logic;
        rdy: out std_logic);
  end component;

  signal clock, inrdy, odata, oclk, oready : std_logic := '0';
  signal reset : std_logic := '1';
  signal data : std_logic_vector(15 downto 0);
  
begin
  U0: OUTSPI port map(clk=>clock,
                      rst=>reset,
                      indata=>data,
                      inrdy=>inrdy,
                      outdata=>odata,
                      outclk=>oclk,
                      rdy=>oready);
  
  clock <= not(clock) after 10 ns;

  process
  begin
    wait until clock='1';
    reset<='0';
    data <= "1001100101010101";
    inrdy <= '1';
  end process;
  
end;
