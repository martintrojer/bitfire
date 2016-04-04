-- ledspi.vhd
-- mtrojer@arrownordic.com
-- $WCREV$

library  ieee;
use ieee.std_logic_1164.all;

entity OUTSPI is
  port( clk, rst: in std_logic;        
        indata: in std_logic_vector(15 downto 0);
        inrdy: in std_logic;
        outdata,outclk: out std_logic;
        rdy : out std_logic);
end OUTSPI;

architecture outspi_behave of OUTSPI is 
  signal tmpdata: std_logic_vector(15 downto 0);
  signal bitctr : integer range 0 to 16;
  signal state : integer range 0 to 2;
begin 

  rdy <= '1' when state=0 else '0';
  
  process (rst,clk)
  begin
    if rst='1' then
      state <= 0;
    elsif clk'event and clk='1' then
      if state=0 then
          bitctr <= 0;
          outdata <= '0';
          outclk <= '0';
          if inrdy='1' then
            tmpdata <= indata;
            state <= 1;
          end if;
      elsif bitctr=16 then
        state <= 0;
      elsif state=1 then
        outdata <= tmpdata(bitctr);
        outclk <= '0';
        state <= 2;
      elsif state=2 then
        outclk <= '1';
        bitctr <= bitctr+1;
        state <= 1;
      end if;
    end if;
  end process;
end outspi_behave;
