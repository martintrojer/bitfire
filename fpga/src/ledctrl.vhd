-- LED Controller
-- mtrojer@arrownordic.com
-- $WCREV$
--
-- Updates an LED array of 80x16 LEDs (via 5 SPI channels, 16 leds per ch)
-- 16 lines of leds, each line consists of 5*8*2 leds (80)
-- Colordepth of each led is 8 bit
-- Memory needed 80*16*8 = 10240 bits.
-- 80*8 = 640 bits of data needs to be buffered outside the memory

library  ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;

entity LEDCTRL is
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
end LEDCTRL;

architecture ledctrl_behave of LEDCTRL is
  signal databuf00  : std_logic_vector(127 downto 0);
  signal databuf01  : std_logic_vector(127 downto 0);
  signal databuf02  : std_logic_vector(127 downto 0);
  signal databuf03  : std_logic_vector(127 downto 0);
  signal databuf04  : std_logic_vector(127 downto 0);

  signal databuf10  : std_logic_vector(127 downto 0);
  signal databuf11  : std_logic_vector(127 downto 0);
  signal databuf12  : std_logic_vector(127 downto 0);
  signal databuf13  : std_logic_vector(127 downto 0);
  signal databuf14  : std_logic_vector(127 downto 0);
  
  signal readctr   : integer range 0 to 10;
  signal addr0     : integer range 0 to 40;
  signal addr1     : integer range 40 to 80;
  signal state     : integer range 0 to 4;
  signal rsubstate : integer range 0 to 3;
  signal ssubstate : integer range 0 to 2;
  signal pwm_ctr   : integer range 0 to 256;

  signal spie_ctr : integer range 0 to 110;
  signal spie_flag : std_logic;
  
begin

  process (rst, clk)
  begin
    if rst='1' then
      spie_ctr <= 110;
    elsif clk'event and clk='1' then
      if spie_flag='1' then
        spie_ctr<=0;
        spi0_enablen <= '1';
        spi1_enablen <= '1';
      elsif spie_ctr=110 then
        spi0_enablen <= '0';
        spi1_enablen <= '0';
      else
        spie_ctr <= spie_ctr+1;
      end if;
    end if;
  end process;        
  
  main: process (rst, clk)
  begin
    if rst='1' then
      state <= 0;    
    elsif clk'event and clk='1' then
      case state is

        -----------------------------------------------------------------------
        -- STATE 0:
        -- reset everything
        when 0 =>
          rsubstate <= 0;
          ssubstate <= 0;
          readctr <= 0;
          addr0 <= 0;
          addr1 <= 40;

          vertsync <= '0';

          databuf00 <= (others => '0');
          databuf01 <= (others => '0');
          databuf02 <= (others => '0');
          databuf03 <= (others => '0');
          databuf04 <= (others => '0');
          databuf10 <= (others => '0');
          databuf11 <= (others => '0');
          databuf12 <= (others => '0');
          databuf13 <= (others => '0');
          databuf14 <= (others => '0');
          
          spi0_latchn <= '1';
--          spi0_enablen <= '1';
          spi1_latchn <= '1';
--          spi1_enablen <= '1';
          en_mux <= (others => '0');
          
          state <= 1;

        -----------------------------------------------------------------------
        -- STATE 1:
        -- fill the counter buffers
        when 1 =>                       
          case rsubstate is
            when 0 =>                   -- (sub) generate next addr to mem
              if readctr < 5 then
                rdaddr <= conv_std_logic_vector(addr0,7);
              else
                rdaddr <= conv_std_logic_vector(addr1,7);
              end if;
              rsubstate <= 1;
            when 1 =>                   -- (sub) wait for data to be ready
              rsubstate <= 2;
            when 2 =>                   -- (sub) put read word into buffer
              rsubstate <= 3;
              case readctr is
                when 0 =>
                  databuf00 <= q;
                  readctr <= readctr + 1;
                when 1 =>
                  databuf01 <= q;
                  readctr <= readctr + 1;
                when 2 =>
                  databuf02 <= q;
                  readctr <= readctr + 1;
                when 3 =>
                  databuf03 <= q;
                  readctr <= readctr + 1;
                when 4 =>
                  databuf04 <= q;
                  readctr <= readctr + 1;
                when 5 =>
                  databuf10 <= q;
                  readctr <= readctr + 1;
                when 6 =>
                  databuf11 <= q;
                  readctr <= readctr + 1;
                when 7 =>
                  databuf12 <= q;
                  readctr <= readctr + 1;
                when 8 =>
                  databuf13 <= q;
                  readctr <= readctr + 1;
                when 9 =>
                  databuf14 <= q;
                  readctr <= readctr + 1;
                when 10 => null;
              end case;
            when 3 =>
              if readctr < 6 then
                addr0 <= addr0 + 1;
              else
                addr1 <= addr1 + 1;
              end if;
              if readctr = 10 then
                state <= 2;
              else
                rsubstate <= 0;
              end if;
          end case;          

        -----------------------------------------------------------------------
        -- STATE 2:
        -- Start to update new line
        when 2 =>
          rsubstate <= 0;
          readctr <= 0;
          pwm_ctr <= 0;          
--          spi0_enablen <= '1';			
--          spi1_enablen <= '1';
          case addr1 is
            when 45 =>
              en_mux <= "00000001";
            when 50 =>
              en_mux <= "00000010";
            when 55 =>
              en_mux <= "00000100";
            when 60 =>
              en_mux <= "00001000";
            when 65 =>
              en_mux <= "00010000";
            when 70 =>
              en_mux <= "00100000";
            when 75 =>
              en_mux <= "01000000";
            when 80 =>
              en_mux <= "10000000";
            when others =>
              en_mux <= "00000000";
          end case;
          
          if spie_ctr=110 then
            spie_flag <= '1';
          elsif spie_ctr=109 then
            state <= 3;
          elsif spie_ctr=0 then
            spie_flag<='0';
          end if;

        -----------------------------------------------------------------------
        -- STATE 3:
        -- run the 5 SPIs
        when 3 =>
          case ssubstate is
            when 0 =>                   -- (sub) wait for all SPIs to be ready
              spi00_datardy <= '0';
              spi01_datardy <= '0';
              spi02_datardy <= '0';
              spi03_datardy <= '0';
              spi04_datardy <= '0';
              spi10_datardy <= '0';
              spi11_datardy <= '0';
              spi12_datardy <= '0';
              spi13_datardy <= '0';
              spi14_datardy <= '0';
              if pwm_ctr=256 then
                state <= 4;
              elsif (spi00_ready='1' and spi01_ready='1' and spi02_ready='1' and spi03_ready='1' and spi04_ready='1' and
                     spi10_ready='1' and spi11_ready='1' and spi12_ready='1' and spi13_ready='1' and spi14_ready='1') then
                ssubstate <= 1;
                spi0_latchn <= '1';
                spi1_latchn <= '1';
              end if;
            when 1 =>                   -- (sub) write out data on the SPIs              
              for i in 0 to 15 loop
                if pwm_ctr >= conv_integer(databuf00(7+8*i downto 8*i)) then
                  spi00_data(i) <= '0';
                else
                  spi00_data(i) <= '1';                                    
                end if;
                if pwm_ctr >= conv_integer(databuf01(7+8*i downto 8*i)) then
                  spi01_data(i) <= '0';
                else
                  spi01_data(i) <= '1';                                    
                end if;
                if pwm_ctr >= conv_integer(databuf02(7+8*i downto 8*i)) then
                  spi02_data(i) <= '0';
                else
                  spi02_data(i) <= '1';                                    
                end if;
                if pwm_ctr >= conv_integer(databuf03(7+8*i downto 8*i)) then
                  spi03_data(i) <= '0';
                else
                  spi03_data(i) <= '1';                                    
                end if;
                if pwm_ctr >= conv_integer(databuf04(7+8*i downto 8*i)) then
                  spi04_data(i) <= '0';
                else
                  spi04_data(i) <= '1';                                    
                end if;

                if pwm_ctr >= conv_integer(databuf10(7+8*i downto 8*i)) then
                  spi10_data(i) <= '0';
                else
                  spi10_data(i) <= '1';                                    
                end if;
                if pwm_ctr >= conv_integer(databuf11(7+8*i downto 8*i)) then
                  spi11_data(i) <= '0';
                else
                  spi11_data(i) <= '1';                                    
                end if;
                if pwm_ctr >= conv_integer(databuf12(7+8*i downto 8*i)) then
                  spi12_data(i) <= '0';
                else
                  spi12_data(i) <= '1';                                    
                end if;
                if pwm_ctr >= conv_integer(databuf13(7+8*i downto 8*i)) then
                  spi13_data(i) <= '0';
                else
                  spi13_data(i) <= '1';                                    
                end if;
                if pwm_ctr >= conv_integer(databuf14(7+8*i downto 8*i)) then
                  spi14_data(i) <= '0';
                else
                  spi14_data(i) <= '1';                                    
                end if;
              end loop;  -- i                  
              
              spi0_latchn <= '0';
              spi1_latchn <= '0';
              spi00_datardy <= '1';
              spi01_datardy <= '1';
              spi02_datardy <= '1';
              spi03_datardy <= '1';
              spi04_datardy <= '1';
              spi10_datardy <= '1';
              spi11_datardy <= '1';
              spi12_datardy <= '1';
              spi13_datardy <= '1';
              spi14_datardy <= '1';
              spie_flag <= '0';
--              spi0_enablen <= '0';
--              spi1_enablen <= '0';

              ssubstate <= 2;

            when 2 =>                   -- (sub) increment buffer pointer
              ssubstate <= 0;
              pwm_ctr <= pwm_ctr + 1;
          end case;

        -----------------------------------------------------------------------
        -- STATE 4:
        -- Line is over, start new line or screen
        when 4 =>
          if addr1=80 then
            state <= 0;
            vertsync <= '1';
          else
            ssubstate <= 0;
            state <= 1;
          end if;
      end case;
    end if;    
  end process main;
end;
