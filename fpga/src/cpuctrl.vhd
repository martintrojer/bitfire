-- cpuctrl.vhd
-- mtrojer@arrownordic.com
-- $WCREV$
--
-- When cpu_cmd=0 input should be addrlo,addrhi,datalo,datahi,addrlo and so on
--   this is writes to memory.
-- When cpu_cmd=1 input should be data
--   this is CPU to FPGA commands:
--
-- 0x00: shdn_ledsup = 0
-- 0x01: shdn_ledsup = 1
-- 0x02: shdnn_muxsup = 0
-- 0x03: shdnn_muxsup = 1
-- 0x04: lbn_rsx = 0
-- 0x05: lbn_rsx = 1
-- 0x06: on_rsx = 0
-- 0x07: on_rsx = 1
-- 0x08: sel1_rsx = 0
-- 0x09: sel1_rsx = 1
-- 0x0A: sel2_rsx = 0
-- 0x0B: sel2_rsx = 1
-- 0x10: DIMMERPWM preset 0 (0  duty)
-- 0x11: DIMMERPWM preset 1 (10 duty)
-- 0x12: DIMMERPWM preset 2 (20 duty)
-- 0x13: DIMMERPWM preset 3 (30 duty)
-- 0x14: DIMMERPWM preset 4 (40 duty)
-- 0x15: DIMMERPWM preset 5 (50 duty)
-- 0x20: SHOW BUFFER 0
-- 0x21: SHOW BUFFER 1
-- 0x22: WORK BUFFER 0
-- 0x23: WORK BUFFER 1

library  ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;

entity CPUCTRL is
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

end CPUCTRL;

architecture cpuctrl_behave of CPUCTRL is
  signal tmpaddr : std_logic_vector(9 downto 0);
  signal tmpdata : std_logic_vector(15 downto 0);
  signal state : integer range 0 to 7;
  signal cmdstate : integer range 0 to 1;
begin

  process (clk, rst)                            -- for cpu commands
  begin
    if rst='1' then
      cmdstate <= 0;
--      shdnn_muxsup <= '0';
--      shdn_ledsup <= '1';
--      on_rsx <= '1';
--      lbn_rsx <= '1';
--      sel1_rsx <= '0';
--      sel2_rsx <= '0';
--      pwm_duty <= conv_std_logic_vector(128,8);
--      show_buffer <= '0';
--      work_buffer <= '0';
    elsif clk'event and clk='1' then
      case cmdstate is
        when 0 =>
          if cpu_cmd='1' then
            if spi_rdy='1' then
              case spi_data is
                when X"00" =>
                  shdn_ledsup <= '0';
                when X"01" =>
                  shdn_ledsup <= '1';
                when X"02" =>
                  shdnn_muxsup <= '0';
                when X"03" =>
                  shdnn_muxsup <= '1';
                when X"04" =>
                  lbn_rsx <= '0';
                when X"05" =>
                  lbn_rsx <= '1';
                when X"06" =>
                  on_rsx <= '0';
                when X"07" =>
                  on_rsx <= '1';
                when X"08" =>
                  sel1_rsx <= '0';
                when X"09" =>
                  sel1_rsx <= '1';
                when X"0A" =>
                  sel2_rsx <= '0';
                when X"0B" =>
                  sel2_rsx <= '1';
                when X"10" =>
                  pwm_duty <= conv_std_logic_vector(0,8);
                when X"11" =>
                  pwm_duty <= conv_std_logic_vector(26,8);
                when X"12" =>
                  pwm_duty <= conv_std_logic_vector(51,8);
                when X"13" =>
                  pwm_duty <= conv_std_logic_vector(77,8);
                when X"14" =>
                  pwm_duty <= conv_std_logic_vector(102,8);
                when X"15" =>
                  pwm_duty <= conv_std_logic_vector(128,8);
                when X"20" =>
                  show_buffer <= '0';
                when X"21" =>
                  show_buffer <= '1';
                when X"22" =>
                  work_buffer <= '0';
                when X"23" =>
                  work_buffer <= '1';
                when others => null;
              end case;
              cmdstate <= 1;
            end if;
          end if;
        when 1 =>     
          if cpu_cmd='0' then
            cmdstate <= 0;
          end if;
      end case;
    end if;
  end process;
  
  main: process (clk, rst)
  begin  -- process main
    if rst = '1' then 
      state <= 0;
    elsif clk'event and clk = '1' then
      case state is
        when 0 =>                       -- reset state
          tmpaddr <= (others => '0');
          tmpdata <= (others => '0');
          wr_to_mem <= '0';
          if spi_rdy='1'then            -- low part of addr
            if cpu_cmd='0' then
              tmpaddr(7 downto 0) <= spi_data;
              state <= 1;
            end if;
          end if;
        when 1 =>                       -- spi toggle state
          if spi_rdy = '0' then
            state <= 2;
          end if;
        when 2 =>                       -- high part of addr
          if spi_rdy = '1' then         
            tmpaddr(9 downto 8) <= spi_data(1 downto 0);
            state <= 3;
          end if;
        when 3 =>                       -- spi toggle state
          if spi_rdy = '0' then         
            state <= 4;
          end if;
        when 4 =>                       -- low part of data
          if spi_rdy = '1' then
            tmpdata(7 downto 0) <= spi_data;
            state <= 5;
          end if;
        when 5 =>                       -- spi toggle state
          if spi_rdy = '0' then
            state <= 6;
          end if;
        when 6 =>                       -- high part of data_to_mem
          if spi_rdy = '1' then
            tmpdata(15 downto 8) <= spi_data;
            state <= 7;
          end if;
        when 7 =>                       -- send to memory
          wr_to_mem <= '1';
          wraddr_to_mem <= tmpaddr;
          data_to_mem <= tmpdata;
          if spi_rdy = '0' then
            state <= 0;
          end if;
      end case;
    end if;
  end process main;
  
end cpuctrl_behave;
