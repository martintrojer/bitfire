-- cpuctrl testbech
-- mtrojer@arrownordic.com
-- $WCREV$

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;

entity testbench is end;

architecture testbench of testbench is
  component CPUCTRL
    port ( clk, rst : in std_logic;
           spi_data : in std_logic_vector(7 downto 0);
           spi_rdy : in std_logic;
           cpu_cmd : in std_logic;
           data_to_mem : out std_logic_vector(15 downto 0);
           wr_to_mem : out std_logic;
           wraddr_to_mem: out std_logic_vector(9 downto 0)
           );
  end component;

  component INSPI
  port(clk, rst : in std_logic;
       indata   : in std_logic;
       outdata  : out std_logic_vector(7 downto 0);
       outrdy   : out std_logic);
  end component;

  component mymem
    PORT
      (
      data		: IN STD_LOGIC_VECTOR (15 DOWNTO 0);
      wren		: IN STD_LOGIC  := '1';
      wraddress		: IN STD_LOGIC_VECTOR (9 DOWNTO 0);
      rdaddress		: IN STD_LOGIC_VECTOR (6 DOWNTO 0);
      wrclock		: IN STD_LOGIC ;
      rdclock		: IN STD_LOGIC ;
      q		: OUT STD_LOGIC_VECTOR (127 DOWNTO 0)
      );
  end component;

  signal tinclk : std_logic := '0';
  signal tcsel, tsync : std_logic := '0';
  signal tspiclk, tspidata : std_logic := '0';
  signal tq : std_logic;
  
  signal result : std_logic_vector(7 downto 0);
  signal ready : std_logic;
  
  signal data_mem   : std_logic_vector(15 downto 0);
  signal wr_mem     : std_logic;
  signal wraddr_mem : std_logic_vector(9 downto 0);
  signal rdaddr_mem : STD_LOGIC_VECTOR (6 DOWNTO 0);
  signal q_mem	    : STD_LOGIC_VECTOR (127 DOWNTO 0);
  
begin

    CTRL: CPUCTRL port map(clk=>tinclk,
                         rst=>tsync,
                         spi_data=>result,
                         spi_rdy=>ready,
                         cpu_cmd=>tcsel,
                         data_to_mem=>data_mem,
                         wr_to_mem=>wr_mem,
                         wraddr_to_mem=>wraddr_mem);
  
  SPI: INSPI port map(clk=>tspiclk,
                      rst=>tsync,
                      indata=>tspidata,
                      outdata=>result,
                      outrdy=>ready);

  MEM: mymem port map(data=>data_mem,
                      wren=>wr_mem,
                      wraddress=>wraddr_mem,
                      rdaddress=>rdaddr_mem,
                      wrclock=>tinclk,
                      rdclock=>tinclk,
                      q=>q_mem);
                       

  rdaddr_mem <= (others => '0');
  tq <= q_mem(12);

  tinclk <= not(tinclk) after 20 ns;
  tspiclk <= tinclk;
    
    
  verify: process
  begin
    for i in 0 to 15 loop
      wait until tinclk='1';
      tspidata <= conv_std_logic_vector(i,1)(0);
    end loop;
  end process;

end;
