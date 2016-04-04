-- ledspi.vhd
-- mtrojer@arrownordic.com
-- $WCREV$

library  ieee;
use ieee.std_logic_1164.all;

entity OUTSPI_BOT is
  port( clk, rst: in std_logic;        
        indata: in std_logic_vector(15 downto 0);
        inrdy: in std_logic;
        outdata,outclk: out std_logic;
        rdy : out std_logic);
end OUTSPI_BOT;

architecture outspi_behave of OUTSPI_BOT is 
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
            tmpdata(15) <= indata(0);
            tmpdata(0) <= indata(1);
            tmpdata(14) <= indata(2);
            tmpdata(1) <= indata(3);
            tmpdata(13) <= indata(4);
            tmpdata(2) <= indata(5);
            tmpdata(12) <= indata(6);
            tmpdata(3) <= indata(7);
            tmpdata(11) <= indata(8);
            tmpdata(4) <= indata(9);
            tmpdata(10) <= indata(10);
            tmpdata(5) <= indata(11);
            tmpdata(9) <= indata(12);
            tmpdata(6) <= indata(13);
            tmpdata(8) <= indata(14);
            tmpdata(7) <= indata(15);
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
