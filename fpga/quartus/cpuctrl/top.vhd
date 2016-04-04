-- cpuctrl testbech
-- mtrojer@arrownordic.com
-- $Id: tb_cpuctrl.vhd,v 1.2 2005/02/09 23:35:41 MTrojer Exp $

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;

entity top is
  port
  (
    tinclk : in std_logic;
    tcsel, tsync : in std_logic;
    tspiclk, tspidata : in std_logic;

    tshdn_ledsup : out std_logic;
    tshdnn_muxsup : out std_logic;
    tlbn_rsx : out std_logic;
    ton_rsx : out std_logic;
    tsel1_rsx : out std_logic;
    tsel2_rsx : out std_logic;
    tpwm_duty : out std_logic_vector(7 downto 0);
    tshow_buffer : out std_logic;
    twork_buffer : out std_logic;

    tq : out std_logic_vector(15 downto 0)
  );
end;

architecture top of top is
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
          rdaddress		: IN STD_LOGIC_VECTOR (9 DOWNTO 0);
          wrclock		: IN STD_LOGIC ;
          rdclock		: IN STD_LOGIC ;
          q		: OUT STD_LOGIC_VECTOR (15 DOWNTO 0)
	);
end component;

  signal result : std_logic_vector(7 downto 0);
  signal ready : std_logic;
  
  signal ccout_shdn_ledsup : std_logic;
  signal ccout_shdnn_muxsup : std_logic;
  signal ccout_lbn_rsx : std_logic;
  signal ccout_on_rsx : std_logic;
  signal ccout_sel1_rsx : std_logic;
  signal ccout_sel2_rsx : std_logic;
  signal ccout_pwm_duty : std_logic_vector(7 downto 0);
  signal ccout_show_buffer : std_logic;
  signal ccout_work_buffer : std_logic;

signal data_mem   : std_logic_vector(15 downto 0);
  signal wr_mem     : std_logic;
  signal wraddr_mem : std_logic_vector(9 downto 0);
  signal rdaddr_mem : STD_LOGIC_VECTOR (9 DOWNTO 0);
  signal q_mem	    : STD_LOGIC_VECTOR (15 DOWNTO 0);
  
begin
  CTRL: CPUCTRL port map(clk=>tinclk,
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
                         data_to_mem=>data_mem,
                         wr_to_mem=>wr_mem,
                         wraddr_to_mem=>wraddr_mem);
  
  SPI: INSPI_BUF port map(clk=>tinclk,
                      rst=>tsync,
                      spiclk=>tspiclk,
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
                       

  rdaddr_mem <= wraddr_mem;
  tq <= q_mem;
  
  tshdn_ledsup <= not ccout_shdn_ledsup;
  tshdnn_muxsup <= ccout_shdnn_muxsup;
  tlbn_rsx <= not ccout_lbn_rsx;
  ton_rsx <= not ccout_on_rsx;
  tsel1_rsx <= ccout_sel1_rsx;
  tsel2_rsx <= ccout_sel2_rsx;
  tpwm_duty <= ccout_pwm_duty;        
  tshow_buffer <= ccout_show_buffer;
  twork_buffer <= ccout_work_buffer;

end;
