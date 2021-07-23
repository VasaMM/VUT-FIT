library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.std_logic_arith.all;
use IEEE.std_logic_unsigned.all;

entity ledc8x8 is
	port (
		SMCLK: in std_logic;
		RESET: in std_logic;
		ROW:   out std_logic_vector (7 downto 0);
		LED:   out std_logic_vector (7 downto 0 )
	);
end ledc8x8;

architecture main of ledc8x8 is
	signal change_char:  std_logic := '0';
	signal clock_signal: std_logic := '0';
	
	signal counter_signal: std_logic_vector(22 downto 0);
	signal counter_row:    std_logic_vector(7  downto 0) := "10000000";
	signal charV:          std_logic_vector(7  downto 0) := "00000000";
	signal charM:          std_logic_vector(7  downto 0) := "00000000";
begin
	--Delicka signalu (clk / 256)
	delicka_signalu: process (RESET, SMCLK)
	begin
		if RESET = '1' then
			counter_signal <= "00000000000000000000000";
		elsif rising_edge(SMCLK) then
			if counter_signal(7 downto 0) = "11111111" then
				clock_signal <= '1';
			else
				clock_signal <= '0';
			end if;

			counter_signal <= counter_signal + 1;
			change_char    <= counter_signal(22);
		end if;
	end process;

	--Prepinani radku
	prepinani_radku: process (SMCLK, RESET, counter_row)
	begin
		if RESET = '1' then
			ROW <= "10000000";
			counter_row <= "10000000";
		elsif rising_edge(SMCLK) AND clock_signal = '1' then
			case counter_row is
				when "10000000" => counter_row <= "01000000";
				when "00000001" => counter_row <= "10000000";
				when "00000010" => counter_row <= "00000001";
				when "00000100" => counter_row <= "00000010";
				when "00001000" => counter_row <= "00000100";
				when "00010000" => counter_row <= "00001000";
				when "00100000" => counter_row <= "00010000";
				when "01000000" => counter_row <= "00100000";
				when others => null;
			end case;
			ROW <= counter_row;
		end if;
	end process;

	--Dekoder znaku V
	char_V_decoder: process (SMCLK, charV)
	begin
		if rising_edge(SMCLK) then		
			case counter_row is
				when "10000000" => charV <= "11011101";
				when "00000001" => charV <= "11011101";
				when "00000010" => charV <= "11011101";
				when "00000100" => charV <= "11011101";
				when "00001000" => charV <= "11011101";
				when "00010000" => charV <= "11101011";
				when "00100000" => charV <= "11101011";
				when "01000000" => charV <= "11110111";
				when others => null;
			end case;
		end if;
	end process;
	
	--Dekoder znaku M
	char_M_decoder: process (SMCLK, charM)
	begin
		if rising_edge(SMCLK) then
			case counter_row is
				when "00000001" => charM <= "11011101";
				when "00000010" => charM <= "11001001";
				when "00000100" => charM <= "11000001";
				when "00001000" => charM <= "11010101";
				when "00010000" => charM <= "11011101";
				when "00100000" => charM <= "11011101";
				when "01000000" => charM <= "11011101";
				when "10000000" => charM <= "11011101";
				when others => null;
			end case;
		end if;
	end process;	

	--Prepinani znaku, frekvence cca 1Hz -> tedy znak na 0.5s
	char_changer: process (SMCLK, RESET, charV, charM, change_char)
	begin
		if RESET = '1' then
			LED <= "11111111";
		elsif rising_edge(SMCLK) then
			if change_char = '0' then
				LED <= charV;
			else
				LED <= charM;
			end if;
		end if;
	end process;
end main;
