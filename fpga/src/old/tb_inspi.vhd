-- inspi testbech
-- mtrojer@arrownordic.com
-- $WCREV$

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;

entity testbench is end;

architecture testbench of testbench is
  component INSPI
  port(clk, rst : in std_logic;
       indata   : in std_logic;
       outdata  : out std_logic_vector(7 downto 0);
       outrdy   : out std_logic);
  end component;

  signal clk, data : std_logic := '0';
  signal csel : std_logic := '1';
  signal result : std_logic_vector(7 downto 0);
  signal ready : std_logic;

begin
  U0: INSPI port map(clk=>clk,
                     rst=>csel,
                     indata=>data,
                     outdata=>result,
                     outrdy=>ready);

  clk <= not(clk) after 10 ns;

  verify: process
  begin
    for i in 0 to 15 loop
      wait until clk='1';
      data <= conv_std_logic_vector(i,1)(0);
    end loop;
  end process;

  chipsel: process
  begin
    for i in 0 to 32 loop
      wait for 100 ns;
      if i=15 then
        csel <='1';
      else
        csel <='0';
      end if;
    end loop;
  end process;

end;
