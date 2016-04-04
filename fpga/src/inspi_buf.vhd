-- inspi.vhd
-- mtrojer@arrownordic.com
-- $WCREV$

library  ieee;
use ieee.std_logic_1164.all;

entity INSPI_BUF is
  port(clk, rst : in std_logic;
       spiclk : in std_logic;
       indata   : in std_logic;
       outdata  : out std_logic_vector(7 downto 0);
       outrdy   : out std_logic);
end INSPI_BUF;

architecture inspi_behave of INSPI_BUF is 
  signal tmpres: std_logic_vector(7 downto 0);
  signal ctr: integer range 0 to 8;
  signal state : integer range 0 to 1;
  signal outstate : integer range 0 to 3;
  
begin 

  process (rst,clk)                     -- to cross clock domains
  begin
    if rst='1' then
      outrdy <= '0';
      outdata <= (others=>'0');
      outstate <= 0;
    elsif clk'event and clk='1' then
      if ctr=8 then
        if outstate=0 then
          outdata <= tmpres;
          outstate <= 1;
        elsif outstate=3 then           -- 3 is a bit harsh, but we have the time
          outrdy  <= '1';               -- and it works.
        else
          outdata <= tmpres;
          outstate <= outstate + 1;
        end if;
      else
        outstate <= 0;
        outrdy <= '0';
        outdata <= (others=>'0');
      end if;
    end if;
  end process;
  
  process (rst,spiclk)
  begin
    if rst='1' then
      state <= 0;
      ctr <= 0;
      tmpres <= (others=>'0');
    elsif spiclk'event and spiclk='1' then
      if state=0 then                   -- state 0: IDLE
        tmpres(7) <= indata;
        ctr <= 1;
        state <= 1;
      elsif ctr=7 then
        tmpres(0) <= indata;
        state <= 0;
        ctr <= 8;
      elsif state=1 then                -- state 1: running
        tmpres(7-ctr) <= indata;
        ctr <= ctr+1;
      end if;
    end if;
  end process;

end inspi_behave;
