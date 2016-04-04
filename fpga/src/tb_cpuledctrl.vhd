-- LED CTRL Testbench
-- mtrojer@arrownordic.com
-- $WCREV$

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;

entity top is end;

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

  component CPUCTRL
    port ( clk, rst : in std_logic;
           spi_data : in std_logic_vector(7 downto 0);
           spi_rdy : in std_logic;
           cpu_cmd : in std_logic;

           shdn_ledsup : out std_logic;
           shdnn_muxsup : out std_logic;
           lbn_rsx : out std_logic;
           on_rsx : out std_logic;
           sel1_rsx : out std_logic;
           sel2_rsx : out std_logic;
           pwm_duty : out std_logic_vector(7 downto 0);
           show_buffer : out std_logic;
           work_buffer : out std_logic;

           data_to_mem : out std_logic_vector(15 downto 0);
           wr_to_mem : out std_logic;
           wraddr_to_mem: out std_logic_vector(9 downto 0)
           );
  end component;

  component INSPI_BUF
  port(clk, rst : in std_logic;
       spiclk   : in std_logic;
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

  component LEDCTRL
  port( clk, rst     : in std_logic;

        -- mem interface
        rdaddr       : out std_logic_vector(6 downto 0);
        q            : in std_logic_vector(127 downto 0);

        vertsync     : out std_logic;

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

  -- TOP ports
  signal clock : std_logic := '0';        
  signal tcsel, tsync : std_logic;
  signal tspiclk, tspidata : std_logic := '0';        
  signal outdata00,outclk00: std_logic := '0';
  signal outdata01,outclk01: std_logic := '0';
  signal outdata02,outclk02: std_logic := '0';
  signal outdata03,outclk03: std_logic := '0';
  signal outdata04,outclk04: std_logic := '0';
  signal outdata10,outclk10: std_logic := '0';
  signal outdata11,outclk11: std_logic := '0';
  signal outdata12,outclk12: std_logic := '0';
  signal outdata13,outclk13: std_logic := '0';
  signal outdata14,outclk14: std_logic := '0';
  signal shutmux, shutled : std_logic := '0';
  signal tpwm : std_logic := '0';
  signal pspi0_latchn, pspi0_enablen : std_logic := '0';
  signal pspi1_latchn, pspi1_enablen : std_logic := '0';
  signal pen_mux : std_logic_vector(7 downto 0) := "00000000";

  signal reset : std_logic;             -- Global reset signal
  signal spidata : std_logic_vector(7 downto 0) := "11000000";
  
  -- From INSPI
  signal result : std_logic_vector(7 downto 0) := "00000000";
  signal ready : std_logic := '0';
  
  -- Clock signals
  signal PLL_OUT_20  : std_logic := '0';
  signal PLL_OUT_100 : std_logic := '0';

  -- CPUCTRL signals
  signal ccout_shdn_ledsup : std_logic := '0';
  signal ccout_shdnn_muxsup : std_logic := '0';
  signal ccout_lbn_rsx : std_logic := '0';
  signal ccout_on_rsx : std_logic := '0';
  signal ccout_sel1_rsx : std_logic := '0';
  signal ccout_sel2_rsx : std_logic := '0';
  signal ccout_pwm_duty : std_logic_vector(7 downto 0) :="00000000";
  signal ccout_show_buffer : std_logic := '0';
  signal ccout_work_buffer : std_logic := '0';
  signal ccout_data_to_mem : std_logic_vector(15 downto 0) :="0000000000000000";
  signal ccout_wr_to_mem : std_logic := '0';
  signal ccout_wraddr_to_mem: std_logic_vector(9 downto 0) :="0000000000";

  -- register values (to fix vertsync/double buffer problem)
  signal reg_show_buffer : std_logic;
  signal reg_work_buffer : std_logic;

  -- MEM signals
  signal MEM0_IN_DATA   : std_logic_vector(15 downto 0);
  signal MEM0_IN_WREN   : std_logic;
  signal MEM0_IN_WRADDR : std_logic_vector(9 downto 0);
  signal MEM0_IN_RDADDR : std_logic_vector(6 downto 0);
  signal MEM0_OUT_Q     : std_logic_vector(127 downto 0);
  signal MEM1_IN_DATA   : std_logic_vector(15 downto 0);
  signal MEM1_IN_WREN   : std_logic;
  signal MEM1_IN_WRADDR : std_logic_vector(9 downto 0);
  signal MEM1_IN_RDADDR : std_logic_vector(6 downto 0);
  signal MEM1_OUT_Q     : std_logic_vector(127 downto 0);
  
  -- LC signals
  signal lcout_rdaddr : std_logic_vector(6 downto 0) := "0000000";
  signal lcin_q : std_logic_vector(127 downto 0);
  signal lcout_vertsync : std_logic;

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
--  PLL: mypll port map(inclk0=>clock,c0=>PLL_OUT_20,c1=>PLL_OUT_100);

  PWM: SIMPLEPWM port map(clk=>PLL_OUT_20,
                          duty=>ccout_pwm_duty,
                          pwm=>tpwm);
  
  CTRL: CPUCTRL port map(clk=>PLL_OUT_20,
                         rst=>tsync,
                         spi_data=>result,
                         spi_rdy=>ready,
                         cpu_cmd=>tcsel,
                         shdn_ledsup => ccout_shdn_ledsup,
                         shdnn_muxsup => ccout_shdnn_muxsup,
                         lbn_rsx => ccout_lbn_rsx,
                         on_rsx => ccout_on_rsx,
                         sel1_rsx => ccout_sel1_rsx,
                         sel2_rsx => ccout_sel2_rsx,
                         pwm_duty => ccout_pwm_duty,
                         show_buffer => ccout_show_buffer,
                         work_buffer => ccout_work_buffer,                       
                         data_to_mem=>ccout_data_to_mem,
                         wr_to_mem=>ccout_wr_to_mem,
                         wraddr_to_mem=>ccout_wraddr_to_mem);
  
  SPI: INSPI_BUF port map(clk=>PLL_OUT_20,
                      rst=>tsync,
                      spiclk=>tspiclk,
                      indata=>tspidata,
                      outdata=>result,
                      outrdy=>ready);

  MEM0: mymem port map(data=>MEM0_IN_DATA,
                       wren=>MEM0_IN_WREN,
                       wraddress=>MEM0_IN_WRADDR,
                       rdaddress=>MEM0_IN_RDADDR,
                       wrclock=>PLL_OUT_100,
                       rdclock=>PLL_OUT_100,
                       q=>MEM0_OUT_Q);

  MEM1: mymem port map(data=>MEM1_IN_DATA,
                       wren=>MEM1_IN_WREN,
                       wraddress=>MEM1_IN_WRADDR,
                       rdaddress=>MEM1_IN_RDADDR,
                       wrclock=>PLL_OUT_100,
                       rdclock=>PLL_OUT_100,
                       q=>MEM1_OUT_Q);

  LC : LEDCTRL port map(clk=>PLL_OUT_20,
                        rst=>reset,
                        rdaddr=>lcout_rdaddr,
                        q=>lcin_q,
                        vertsync=>lcout_vertsync,

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
  
  tcsel <= '1';
  reset <= '0';

  clock <= not(clock) after 27.13 ns;
  tspiclk <= not(tspiclk) after 100 ns;
  PLL_OUT_20 <= clock;
  PLL_OUT_100 <= clock;
    
  -- Double Buffer Stuff
  
  process (PLL_OUT_100)
  begin
    if reset='1' then
      reg_work_buffer <= '0';
      reg_show_buffer <= '0';
    elsif PLL_OUT_100'event and PLL_OUT_100='1' then
      if lcout_vertsync='1' then
        reg_work_buffer <= ccout_work_buffer;
        reg_show_buffer <= ccout_show_buffer;
      end if;
    end if;
  end process;
  
  MEM0_IN_data <= ccout_data_to_mem when reg_work_buffer='0' else (others=>'0');
  MEM1_IN_data <= ccout_data_to_mem when reg_work_buffer='1' else (others=>'0');
  MEM0_IN_WREN <= ccout_wr_to_mem when reg_work_buffer='0' else '0';
  MEM1_IN_WREN <= ccout_wr_to_mem when reg_work_buffer='1' else '0';
  MEM0_IN_WRADDR <= ccout_wraddr_to_mem when reg_work_buffer='0' else (others=>'0');
  MEM1_IN_WRADDR <= ccout_wraddr_to_mem when reg_work_buffer='1' else (others=>'0');
  MEM0_IN_RDADDR <= lcout_rdaddr when reg_show_buffer='0' else (others=>'0');
  MEM1_IN_RDADDR <= lcout_rdaddr when reg_show_buffer='1' else (others=>'0');  
  lcin_q <= MEM0_OUT_Q when reg_show_buffer='0' else MEM1_OUT_Q;      

  -- Stimuli
  
  verify: process
  begin
    for i in 0 to 512 loop
      if i=0 then
        tsync<='1';
      else
        tsync<='0';
      end if;
      wait until tspiclk='1';
      tspidata <= conv_std_logic_vector(i,1)(0);
--      if i>2 then
--        tspidata <= spidata(i-3);        
--      end if;
    end loop;
  end process;
end;
  
