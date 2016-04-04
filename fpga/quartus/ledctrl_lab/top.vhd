-- LED CTRL Testbench
-- mtrojer@arrownordic.com
-- $Id$

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;

entity top is 

  port( clock : in std_logic;        

        outdata00,outclk00: out std_logic;
        outdata01,outclk01: out std_logic;
        outdata02,outclk02: out std_logic;
        outdata03,outclk03: out std_logic;
        outdata04,outclk04: out std_logic;

        outdata10,outclk10: out std_logic;
        outdata11,outclk11: out std_logic;
        outdata12,outclk12: out std_logic;
        outdata13,outclk13: out std_logic;
        outdata14,outclk14: out std_logic;

        shutmux, shutled : out std_logic;

        tpwm : out std_logic;

        pspi0_latchn, pspi0_enablen : out std_logic;
        pspi1_latchn, pspi1_enablen : out std_logic;
        pen_mux : out std_logic_vector(7 downto 0) );


--	   data    : IN STD_LOGIC_VECTOR (7 DOWNTO 0);
--         wren    : IN STD_LOGIC  := '1';
--         wraddress       : IN STD_LOGIC_VECTOR (10 DOWNTO 0));
end;

architecture top of top is

  component SIMPLEPWM is
    PORT
      (
        clk : in std_logic;
        duty : in std_logic_vector(7 downto 0);
        pwm : out std_logic
      );
  end component;

  component mypll
    PORT
      (
        inclk0 : IN STD_LOGIC  := '0';
        c0 : OUT STD_LOGIC ;
        c1 : OUT STD_LOGIC 
        );
  end component;

  component altmem
    PORT
      (
        data : IN STD_LOGIC_VECTOR (7 DOWNTO 0);
        wren : IN STD_LOGIC  := '1';
        wraddress : IN STD_LOGIC_VECTOR (10 DOWNTO 0);
        rdaddress : IN STD_LOGIC_VECTOR (6 DOWNTO 0);
        wrclock : IN STD_LOGIC ;
        rdclock : IN STD_LOGIC ;
        q : OUT STD_LOGIC_VECTOR (127 DOWNTO 0)
	);
  end component;

  component LEDCTRL
  port( clk, rst     : in std_logic;

        -- mem interface
        rdaddr       : out std_logic_vector(6 downto 0);
        q            : in std_logic_vector(127 downto 0);

        -- led control
        spi0_latchn  : out std_logic;
        spi0_enablen : out std_logic;
        spi1_latchn  : out std_logic;
        spi1_enablen : out std_logic;
        en_mux       : out std_logic_vector(7 downto 0);

        -- spi interfaces
        spi00_ready   : in std_logic;
        spi00_data    : out std_logic_vector(15 downto 0);
        spi00_datardy : out std_logic;
        spi01_ready   : in std_logic;
        spi01_data    : out std_logic_vector(15 downto 0);
        spi01_datardy : out std_logic;
        spi02_ready   : in std_logic;
        spi02_data    : out std_logic_vector(15 downto 0);
        spi02_datardy : out std_logic;
        spi03_ready   : in std_logic;
        spi03_data    : out std_logic_vector(15 downto 0);
        spi03_datardy : out std_logic;
        spi04_ready   : in std_logic;
        spi04_data    : out std_logic_vector(15 downto 0);
        spi04_datardy : out std_logic;

        spi10_ready   : in std_logic;
        spi10_data    : out std_logic_vector(15 downto 0);
        spi10_datardy : out std_logic;
        spi11_ready   : in std_logic;
        spi11_data    : out std_logic_vector(15 downto 0);
        spi11_datardy : out std_logic;
        spi12_ready   : in std_logic;
        spi12_data    : out std_logic_vector(15 downto 0);
        spi12_datardy : out std_logic;
        spi13_ready   : in std_logic;
        spi13_data    : out std_logic_vector(15 downto 0);
        spi13_datardy : out std_logic;
        spi14_ready   : in std_logic;
        spi14_data    : out std_logic_vector(15 downto 0);
        spi14_datardy : out std_logic);  
  end component;
  
  component OUTSPI_TOP
    port( clk, rst: in std_logic;        
          indata: in std_logic_vector(15 downto 0);
          inrdy: in std_logic;
          outdata,outclk: out std_logic;
          rdy : out std_logic);
  end component;

  component OUTSPI_BOT
    port( clk, rst: in std_logic;        
          indata: in std_logic_vector(15 downto 0);
          inrdy: in std_logic;
          outdata,outclk: out std_logic;
          rdy : out std_logic);
  end component;

  signal reset : std_logic;             -- Global reset signal
  signal duty : integer range 0 to 255;  -- PWM dutycycle

  -- WR port of RAM
  signal data      : STD_LOGIC_VECTOR (7 DOWNTO 0) := "00000000";
  signal wren      : STD_LOGIC  := '0';
  signal wraddress : STD_LOGIC_VECTOR (10 DOWNTO 0) := "00000000000";

  -- Clock signals
  signal PLL_OUT_20  : std_logic;
  signal PLL_OUT_100 : std_logic;

  signal MEM_IN_DATA   : std_logic_vector(7 downto 0) := "00000000";
  signal MEM_IN_WREN   : std_logic := '0';
  signal MEM_IN_WRADDR : std_logic_vector(10 downto 0) := "00000000000";
  signal MEM_IN_RDADDR : std_logic_vector(6 downto 0) := "0000000";
  signal MEM_OUT_Q     : std_logic_vector(127 downto 0);
  
  signal SPI00_IN_DATA : std_logic_vector(15 downto 0) := "0000000000000000";
  signal SPI00_IN_DATARDY : std_logic := '0';
  signal SPI00_OUT_DATA : std_logic := '0';
  signal SPI00_OUT_CLK : std_logic := '0';
  signal SPI00_OUT_RDY : std_logic := '0';
  signal SPI01_IN_DATA : std_logic_vector(15 downto 0) := "0000000000000000";
  signal SPI01_IN_DATARDY : std_logic := '0';
  signal SPI01_OUT_DATA : std_logic := '0';
  signal SPI01_OUT_CLK : std_logic := '0';
  signal SPI01_OUT_RDY : std_logic := '0';
  signal SPI02_IN_DATA : std_logic_vector(15 downto 0) := "0000000000000000";
  signal SPI02_IN_DATARDY : std_logic := '0';
  signal SPI02_OUT_DATA : std_logic := '0';
  signal SPI02_OUT_CLK : std_logic := '0';
  signal SPI02_OUT_RDY : std_logic := '0';
  signal SPI03_IN_DATA : std_logic_vector(15 downto 0) := "0000000000000000";
  signal SPI03_IN_DATARDY : std_logic := '0';
  signal SPI03_OUT_DATA : std_logic := '0';
  signal SPI03_OUT_CLK : std_logic := '0';
  signal SPI03_OUT_RDY : std_logic := '0';
  signal SPI04_IN_DATA : std_logic_vector(15 downto 0) := "0000000000000000";
  signal SPI04_IN_DATARDY : std_logic := '0';
  signal SPI04_OUT_DATA : std_logic := '0';
  signal SPI04_OUT_CLK : std_logic := '0';
  signal SPI04_OUT_RDY : std_logic := '0';

  signal SPI10_IN_DATA : std_logic_vector(15 downto 0) := "0000000000000000";
  signal SPI10_IN_DATARDY : std_logic := '0';
  signal SPI10_OUT_DATA : std_logic := '0';
  signal SPI10_OUT_CLK : std_logic := '0';
  signal SPI10_OUT_RDY : std_logic := '0';
  signal SPI11_IN_DATA : std_logic_vector(15 downto 0) := "0000000000000000";
  signal SPI11_IN_DATARDY : std_logic := '0';
  signal SPI11_OUT_DATA : std_logic := '0';
  signal SPI11_OUT_CLK : std_logic := '0';
  signal SPI11_OUT_RDY : std_logic := '0';
  signal SPI12_IN_DATA : std_logic_vector(15 downto 0) := "0000000000000000";
  signal SPI12_IN_DATARDY : std_logic := '0';
  signal SPI12_OUT_DATA : std_logic := '0';
  signal SPI12_OUT_CLK : std_logic := '0';
  signal SPI12_OUT_RDY : std_logic := '0';
  signal SPI13_IN_DATA : std_logic_vector(15 downto 0) := "0000000000000000";
  signal SPI13_IN_DATARDY : std_logic := '0';
  signal SPI13_OUT_DATA : std_logic := '0';
  signal SPI13_OUT_CLK : std_logic := '0';
  signal SPI13_OUT_RDY : std_logic := '0';
  signal SPI14_IN_DATA : std_logic_vector(15 downto 0) := "0000000000000000";
  signal SPI14_IN_DATARDY : std_logic := '0';
  signal SPI14_OUT_DATA : std_logic := '0';
  signal SPI14_OUT_CLK : std_logic := '0';
  signal SPI14_OUT_RDY : std_logic := '0';

begin
  PLL: mypll port map(inclk0=>clock,c0=>PLL_OUT_20,c1=>PLL_OUT_100);

  PWM: SIMPLEPWM port map(clk=>PLL_OUT_20,
                          duty=>conv_std_logic_vector(duty,8),
                          pwm=>tpwm);
  
  MEM: altmem port map(data=>data,
                       wren=>wren,
                       wraddress=>wraddress,
                       rdaddress=>MEM_IN_RDADDR,
                       wrclock=>PLL_OUT_20,
                       rdclock=>PLL_OUT_20,
                       q=>MEM_OUT_Q);

  LC : LEDCTRL port map(clk=>PLL_OUT_20,
                        rst=>reset,
                        rdaddr=>MEM_IN_RDADDR,
                        q=>MEM_OUT_Q,

                        spi0_latchn=>pspi0_latchn,
                        spi0_enablen=>pspi0_enablen,
                        spi1_latchn=>pspi1_latchn,
                        spi1_enablen=>pspi1_enablen,
                        en_mux=>pen_mux,
                                                
                        spi00_ready=>SPI00_OUT_RDY,
                        spi00_data=>SPI00_IN_DATA,
                        spi00_datardy=>SPI00_IN_DATARDY,
                        spi01_ready=>SPI01_OUT_RDY,
                        spi01_data=>SPI01_IN_DATA,
                        spi01_datardy=>SPI01_IN_DATARDY,
                        spi02_ready=>SPI02_OUT_RDY,
                        spi02_data=>SPI02_IN_DATA,
                        spi02_datardy=>SPI02_IN_DATARDY,
                        spi03_ready=>SPI03_OUT_RDY,
                        spi03_data=>SPI03_IN_DATA,
                        spi03_datardy=>SPI03_IN_DATARDY,
                        spi04_ready=>SPI04_OUT_RDY,
                        spi04_data=>SPI04_IN_DATA,
                        spi04_datardy=>SPI04_IN_DATARDY,

                        spi10_ready=>SPI10_OUT_RDY,
                        spi10_data=>SPI10_IN_DATA,
                        spi10_datardy=>SPI10_IN_DATARDY,
                        spi11_ready=>SPI11_OUT_RDY,
                        spi11_data=>SPI11_IN_DATA,
                        spi11_datardy=>SPI11_IN_DATARDY,
                        spi12_ready=>SPI12_OUT_RDY,
                        spi12_data=>SPI12_IN_DATA,
                        spi12_datardy=>SPI12_IN_DATARDY,
                        spi13_ready=>SPI13_OUT_RDY,
                        spi13_data=>SPI13_IN_DATA,
                        spi13_datardy=>SPI13_IN_DATARDY,
                        spi14_ready=>SPI14_OUT_RDY,
                        spi14_data=>SPI14_IN_DATA,
                        spi14_datardy=>SPI14_IN_DATARDY);
                        
  S0: OUTSPI_TOP port map(clk=>PLL_OUT_100,
                      rst=>reset,
                      indata=>SPI00_IN_DATA,
                      inrdy=>SPI00_IN_DATARDY,
                      outdata=>outdata00,
                      outclk=>outclk00,
                      rdy=>SPI00_OUT_RDY);

  S1: OUTSPI_TOP port map(clk=>PLL_OUT_100,
                      rst=>reset,
                      indata=>SPI01_IN_DATA,
                      inrdy=>SPI01_IN_DATARDY,
                      outdata=>outdata01,
                      outclk=>outclk01,
                      rdy=>SPI01_OUT_RDY);

  S2: OUTSPI_TOP port map(clk=>PLL_OUT_100,
                      rst=>reset,
                      indata=>SPI02_IN_DATA,
                      inrdy=>SPI02_IN_DATARDY,
                      outdata=>outdata02,
                      outclk=>outclk02,
                      rdy=>SPI02_OUT_RDY);

  S3: OUTSPI_TOP port map(clk=>PLL_OUT_100,
                      rst=>reset,
                      indata=>SPI03_IN_DATA,
                      inrdy=>SPI03_IN_DATARDY,
                      outdata=>outdata03,
                      outclk=>outclk03,
                      rdy=>SPI03_OUT_RDY);

  S4: OUTSPI_TOP port map(clk=>PLL_OUT_100,
                      rst=>reset,
                      indata=>SPI04_IN_DATA,
                      inrdy=>SPI04_IN_DATARDY,
                      outdata=>outdata04,
                      outclk=>outclk04,
                      rdy=>SPI04_OUT_RDY);

  S5: OUTSPI_BOT port map(clk=>PLL_OUT_100,
                      rst=>reset,
                      indata=>SPI10_IN_DATA,
                      inrdy=>SPI10_IN_DATARDY,
                      outdata=>outdata10,
                      outclk=>outclk10,
                      rdy=>SPI10_OUT_RDY);

  S6: OUTSPI_BOT port map(clk=>PLL_OUT_100,
                      rst=>reset,
                      indata=>SPI11_IN_DATA,
                      inrdy=>SPI11_IN_DATARDY,
                      outdata=>outdata11,
                      outclk=>outclk11,
                      rdy=>SPI11_OUT_RDY);

  S7: OUTSPI_BOT port map(clk=>PLL_OUT_100,
                      rst=>reset,
                      indata=>SPI12_IN_DATA,
                      inrdy=>SPI12_IN_DATARDY,
                      outdata=>outdata12,
                      outclk=>outclk12,
                      rdy=>SPI12_OUT_RDY);

  S8: OUTSPI_BOT port map(clk=>PLL_OUT_100,
                      rst=>reset,
                      indata=>SPI13_IN_DATA,
                      inrdy=>SPI13_IN_DATARDY,
                      outdata=>outdata13,
                      outclk=>outclk13,
                      rdy=>SPI13_OUT_RDY);

  S9: OUTSPI_BOT port map(clk=>PLL_OUT_100,
                      rst=>reset,
                      indata=>SPI14_IN_DATA,
                      inrdy=>SPI14_IN_DATARDY,
                      outdata=>outdata14,
                      outclk=>outclk14,
                      rdy=>SPI14_OUT_RDY);
  
	shutmux <= '1';
	shutled <= '0';

	data <= "00000000";
	wren <= '0';
	wraddress <= "00000000000";
	reset <= '0';
	duty <= 128;
end;
  
