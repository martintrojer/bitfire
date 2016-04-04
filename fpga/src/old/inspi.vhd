-- inspi.vhd
-- mtrojer@arrownordic.com
-- $WCREV$

library  ieee;
use ieee.std_logic_1164.all;

entity INSPI is
  port(clk, rst : in std_logic;
       indata   : in std_logic;
       outdata  : out std_logic_vector(7 downto 0);
       outrdy   : out std_logic);
end INSPI;

architecture inspi_behave of INSPI is 
  signal tmpres: std_logic_vector(7 downto 0);
  signal ctr: integer range 0 to 7;
  signal state : integer range 0 to 1;
begin 

  outdata <= tmpres;

  process (rst,clk)
  begin
    if rst='1' then
      state <= 0;
      outrdy <= '0';
    elsif clk'event and clk='1' then
      if state=0 then                   -- state 0: IDLE
        outrdy <= '0';
        tmpres(0) <= indata;
        ctr <= 1;
        state <= 1;
      elsif ctr=7 then
        tmpres(7) <= indata;
        state <= 0;
        outrdy <= '1';
      elsif state=1 then                -- state 1: running
        tmpres(ctr) <= indata;
        ctr <= ctr+1;
      end if;
    end if;
  end process;

end inspi_behave;
