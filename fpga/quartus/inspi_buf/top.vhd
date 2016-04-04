-- inspi testbech
-- mtrojer@arrownordic.com
-- $Id$

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;

entity top is 
  port( tinclk	: in std_logic;
        tclk, tcsel, tsync : in std_logic;
       tindata   : in std_logic;
       toutdata  : out std_logic_vector(7 downto 0);
       toutrdy   : out std_logic;
       toutclk  : out std_logic;
       toutio   : out std_logic);
end;

architecture top of top is

  component INSPI_BUF
  port(clk, rst : in std_logic;
       spiclk : in std_logic;
       indata   : in std_logic;
       outdata  : out std_logic_vector(7 downto 0);
       outrdy   : out std_logic);
  end component;

  component mypll
    PORT
      (
        inclk0	: IN STD_LOGIC  := '0';
        c0		: OUT STD_LOGIC 
        );
  end component;

begin
  U0: INSPI_BUF port map(clk=>tinclk,
                     rst=>tsync,
                     spiclk=>tclk,
                     indata=>tindata,
                     outdata=>toutdata,
                     outrdy=>toutrdy);

  PLL: mypll port map(inclk0=>tinclk,
                      c0=>toutclk);

	toutio <= tcsel;
end;
