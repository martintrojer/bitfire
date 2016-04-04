-- ledspi testbench
-- mtrojer@arrownordic.com
-- $Id$

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;

entity top is 
  port( tclk, trst: in std_logic;        
        tindata: in std_logic_vector(7 downto 0);
        tinrdy: in std_logic;
        toutdata,toutclk: out std_logic;
        trdy: out std_logic);
end;

architecture top of top is
  component OUTSPI
  port( clk, rst: in std_logic;        
        indata: in std_logic_vector(7 downto 0);
        inrdy: in std_logic;
        outdata,outclk: out std_logic;
        rdy: out std_logic);
  end component;
  
begin
  U0: OUTSPI port map(clk=>tclk,
                      rst=>trst,
                      indata=>tindata,
                      inrdy=>tinrdy,
                      outdata=>toutdata,
                      outclk=>toutclk,
                      rdy=>trdy);   
end;
