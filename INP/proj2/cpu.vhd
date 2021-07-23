-- cpu.vhd: Simple 8-bit CPU (BrainLove interpreter)
-- Copyright (C) 2016 Brno University of Technology,
--                    Faculty of Information Technology
-- Author(s): xmarti76
--

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;

-- ----------------------------------------------------------------------------
--                        Entity declaration
-- ----------------------------------------------------------------------------
entity cpu is
	port (
		CLK:   in std_logic;  -- hodinovy signal
		RESET: in std_logic;  -- asynchronni reset procesoru
		EN:    in std_logic;  -- povoleni cinnosti procesoru
	 
		-- synchronni pamet ROM
		CODE_ADDR: out std_logic_vector(11 downto 0);  -- adresa do pameti
		CODE_DATA: in  std_logic_vector(7 downto 0);   -- CODE_DATA <- rom[CODE_ADDR] pokud CODE_EN='1'
		CODE_EN:   out std_logic;                      -- povoleni cinnosti
		
		-- synchronni pamet RAM
		DATA_ADDR:  out std_logic_vector(9 downto 0);  -- adresa do pameti
		DATA_WDATA: out std_logic_vector(7 downto 0);  -- mem[DATA_ADDR] <- DATA_WDATA pokud DATA_EN='1'
		DATA_RDATA: in  std_logic_vector(7 downto 0);  -- DATA_RDATA <- ram[DATA_ADDR] pokud DATA_EN='1'
		DATA_RDWR:  out std_logic;                     -- cteni (1) / zapis (0)
		DATA_EN:    out std_logic;                     -- povoleni cinnosti
		
		-- vstupni port
		IN_DATA: in  std_logic_vector(7 downto 0);   -- IN_DATA <- stav klavesnice pokud IN_VLD='1' a IN_REQ='1'
		IN_VLD:  in  std_logic;                      -- data platna
		IN_REQ:  out std_logic;                      -- pozadavek na vstup data
		
		-- vystupni port
		OUT_DATA: out std_logic_vector(7 downto 0);    -- zapisovana data
		OUT_BUSY: in  std_logic;                       -- LCD je zaneprazdnen (1), nelze zapisovat
		OUT_WE:   out std_logic                        -- LCD <- OUT_DATA pokud OUT_WE='1' a OUT_BUSY='0'
	);
end cpu;


-- ----------------------------------------------------------------------------
--                      Architecture declaration
-- ----------------------------------------------------------------------------
architecture behavioral of cpu is

-- Seznam stavu FSM (konecneho automatu)
type fsm_state is (
	INIT,              -- Zacatek programu
	FETCH,             -- Cteni programu
	PTRINC,            -- Prikaz '>'
	PTRDEC,            -- Prikaz '<'
	DATAREAD,          -- Priprava pro prikazy '+' a '-'
	DATAINC,           -- Prikaz '+'
	DATADEC,           -- Prikaz '-'
	WHILE_START,       -- Prikaz '['
	WHILE_IGNORE,      -- Ignrovani cyklu
	WHILE_END,         -- Prikaz ']'
	WHILE_END_RETURN,  -- Presouvam se na zacatek cyklu
	WHILE_START_WAIT,  -- Jsem na konci cyklu - musim 1 takt pockat
	PUTCHAR,           -- Prikaz '.'
	GETCHAR,           -- Prikaz ','
	TMP_PUT,           -- Prikaz '$'
	TMP_GET,           -- Prikaz '!'
	RUN_ERROR,         -- Nastala nejaka chyba (napr. neodpovidajici pocet '[' a ']')
	PROGRAM_END,       -- Konec programu
	ERROR1, ERROR2, ERROR3, ERROR4, ERROR5, ERROR6
);

signal next_state: fsm_state;  -- Pristi stav
signal prev_state: fsm_state;  -- Aktualni stav

-- pc = program counter
signal pc_mem: std_logic_vector(11 downto 0);      -- Ukazatel do pameti ROM
signal pc_inc: std_logic;                          -- nacti dalsi data
signal pc_dec: std_logic;                          -- nacti predchozi data

signal ptr_mem: std_logic_vector(9 downto 0);      -- Ukazatel do pameti RAM
signal ptr_inc: std_logic;                         -- nacti dalsi data
signal ptr_dec: std_logic;                         -- nacti predchozi data

-- cyc = cycle  =>  citac cyklu
signal cyc_mem:  std_logic_vector(7 downto 0);     -- Pocitadlo cyklu
signal cyc_ignr: std_logic_vector(7 downto 0);     -- Hodnota pro ulozeni pozice zanoreneho cyklu
signal cyc_set:  std_logic;                        -- Zarazka pro preskoceni zanorenych cyklu
signal cyc_inc:  std_logic;                        -- zvys pocet
signal cyc_dec:  std_logic;                        -- sniz pocet

signal tmp_var:    std_logic_vector(7 downto 0);   -- 1B registr pro prikazy '$' a '!'
signal tmp_ld:     std_logic;                      -- Nacti 1B z RAM do tmp_var
signal lcd_wr:     std_logic;                      -- Zapis znaku na LCD
signal kb_read:    std_logic;                      -- Cteni znaku z klavesnice
signal tmp_byte:   std_logic_vector(7 downto 0);   -- Registr pro 1 pracovní byte
signal data_plus:  std_logic;                      -- Zapis na tmp_byte DATA_RDATA + 1
signal data_minus: std_logic;                      -- Zapis na tmp_byte DATA_RDATA - 1


begin
----------------------------------------
-- Program counter register (ukazatel do ROM)
----------------------------------------
pc_reg: process (RESET, CLK) begin
	-- vynuluj pri RESET
	if (RESET = '1') then
		pc_mem <= (others => '0');
	-- Nastupni hrana na CLK
	elsif rising_edge(CLK) then
		if (pc_inc = '1') then
			pc_mem <= pc_mem + 1;
		elsif (pc_dec = '1') then
			pc_mem <= pc_mem - 1;
		end if;
	end if;
end process pc_reg;
-- Zapisu ukazatel do pameti ROM
CODE_ADDR <= pc_mem;


----------------------------------------
-- Pointer register (ukazatel do RAM)
----------------------------------------
ptr_reg: process (RESET, CLK) begin
	-- vynuluj pri RESET
	if (RESET = '1') then
		ptr_mem <= (others => '0');
	-- Nastupni hrana na CLK
	elsif rising_edge(CLK) then
		if (ptr_inc = '1') then
			ptr_mem <= ptr_mem + 1;
		elsif (ptr_dec = '1') then
			ptr_mem <= ptr_mem - 1;
		end if;
	end if;
end process ptr_reg;
-- Zapisu ukazatel do pameti RAM
DATA_ADDR <= ptr_mem;


----------------------------------------
-- Cycle register  =>  citac cyklu
----------------------------------------
cyc_reg: process (RESET, CLK) begin
	-- vynuluj pri RESET
	if (RESET = '1') then
		cyc_mem  <= (others => '0');
		cyc_ignr <= (others => '0');
	-- Nastupni hrana na CLK
	elsif rising_edge(CLK) then
		if (cyc_inc = '1') then
			cyc_mem <= cyc_mem + 1;
		elsif (cyc_dec = '1') then
			cyc_mem <= cyc_mem - 1;
		end if;
		if (cyc_set = '1') then
			cyc_ignr <= cyc_mem;
		end if;
	end if;
end process cyc_reg;


----------------------------------------
-- MUX register - obsluhuje TMP 1B registr pro prikazy '$' a '!' a ovladani LCD
----------------------------------------
mux_reg: process (RESET, CLK) begin
	-- vynuluj pri RESET
	if (RESET = '1') then
		tmp_var  <= (others => '0');
		tmp_byte <= (others => '0');
	-- Nastupni hrana na CLK
	elsif rising_edge(CLK) then
		if (tmp_ld = '1') then
			tmp_var <= DATA_RDATA;
		elsif (data_plus = '1')  then
			tmp_byte <= DATA_RDATA + 1;
		elsif (data_minus = '1') then
			tmp_byte <= DATA_RDATA - 1;
		elsif (lcd_wr = '1') then
			tmp_byte <= DATA_RDATA;
		elsif (kb_read = '1') then
			tmp_byte <= IN_DATA;
		end if;
	end if;
end process mux_reg;


----------------------------------------
-- FSM - finite state machine
----------------------------------------

-- Prepnu nasledujici stav na aktualni
fsm_prev_state: process (RESET, CLK, EN) begin
	-- Pri RESET v FETCH
	if (RESET = '1') then
		prev_state <= INIT;
	-- Nastupni hrana na CLK
	elsif rising_edge(CLK) and (EN = '1') then
		prev_state <= next_state;
	end if;
end process fsm_prev_state;

fsm_next_state: process (prev_state, CODE_DATA, DATA_RDATA, tmp_byte, cyc_mem, cyc_ignr, OUT_BUSY, IN_VLD, tmp_var) begin
	-- nuluji promenne
	CODE_EN    <= '1';
	DATA_RDWR  <= '1';
	DATA_EN    <= '0';
	DATA_WDATA <= (others => '0');
	pc_inc     <= '0';
	ptr_inc    <= '0';
	cyc_inc    <= '0';
	pc_dec     <= '0';
	ptr_dec    <= '0';
	cyc_dec    <= '0';
	tmp_ld     <= '0';
	data_plus  <= '0';
	data_minus <= '0';
	lcd_wr     <= '0';
	cyc_set    <= '0';
	OUT_WE     <= '0';
	OUT_DATA   <= (others => '0');
	IN_REQ     <= '0';


	-- Stavovy automat
	case prev_state is
		-- Inicializace
		when INIT =>
			next_state <= FETCH;
		-- Ctu prikaz
		when FETCH =>
			pc_inc <= '1';  -- Zvys ukazatel do ROM
			case CODE_DATA is
				when X"3E" =>  -- '>'
					ptr_inc    <= '1';  -- Zvys ukazatel do RAM
					next_state <= PTRINC;
				when X"3C" =>  -- '<'
					ptr_dec    <= '1';  -- Sniz ukazatel do RAM
					next_state <= PTRDEC;
				when X"2B" =>  -- '+'
					DATA_EN    <= '1';  -- Zpristupnuji RAM
					data_plus  <= '1';  -- Zvysim byte o 1
					next_state <= DATAINC;
				when X"2D" =>  -- '-'
					DATA_EN    <= '1';  -- Zpristupnuji RAM
					data_minus <= '1';  -- Snizim byte o 1
					next_state <= DATADEC;
				when X"5B" =>  -- '['
					next_state <= WHILE_START;
					cyc_inc    <= '1';  -- Zvednu pocet cyklu
				when X"5D" =>  -- ']'
					if (DATA_RDATA = "00000000") then  -- While skoncil
						cyc_dec    <= '1';  -- Ukoncim while
						pc_inc     <= '1';  -- Posunu se dal
						next_state <= INIT;
					else
						pc_inc     <= '0';
						pc_dec     <= '1';
						next_state <= WHILE_END;
					end if;
				when X"2E" =>  -- '.'
					DATA_EN  <= '1';  -- Zpristupnuji RAM
					lcd_wr   <= '1';  -- Zapis na LCD
					next_state <= PUTCHAR;
				when X"2C" =>  -- ','
					next_state <= GETCHAR;
				when X"24" =>  -- '$'
					next_state <= TMP_PUT;
				when X"21" =>  -- '!'
					next_state <= TMP_GET;
				when X"00" =>  -- EOF
					next_state <= PROGRAM_END;
				when others => -- other
					next_state <= INIT;
			end case;


		-- Inkrementuji / dekrementuji ukazatel
		when PTRINC =>
			DATA_EN    <= '1';  -- Zpristupnuji RAM, kdybych chtel hned cist data
			next_state <= FETCH;

		when PTRDEC =>
			DATA_EN    <= '1';  -- Zpristupnuji RAM, kdybych chtel hned cist data
			next_state <= FETCH;


		-- Inkrementuji / dekrementuji v RAM
		when DATAINC =>
			DATA_EN    <= '1';       -- Zpristupnuji RAM
			DATA_RDWR  <= '0';       -- Zapisuji do RAM
			DATA_WDATA <= tmp_byte;  -- Zapisu byte zpet do RAM
			next_state <= FETCH;

		when DATADEC =>
			DATA_EN    <= '1';       -- Zpristupnuji RAM
			DATA_RDWR  <= '0';       -- Zapisuji do RAM
			DATA_WDATA <= tmp_byte;  -- Zapisu byte zpet do RAM
			next_state <= FETCH;


		-- While cyklus
		when WHILE_START =>
			DATA_EN <= '1';  -- Zapnu RAM

			if (DATA_RDATA = "00000000") then  -- Cyklus ani nezacina
				cyc_set    <= '1';  -- Ulozim si pozici ignorovaneho cyklu
				next_state <= WHILE_IGNORE;
			else  -- Zacina / pokracuje cyklus
				next_state <= FETCH;	
			end if;

		when WHILE_IGNORE =>
			pc_inc     <= '1';
			next_state <= WHILE_IGNORE;

			if (CODE_DATA = X"5B") then  -- '['
				cyc_inc <= '1';  -- Nasel jsem zanoreny cyklus - ignoruji ho take
			elsif (CODE_DATA = X"5D") and (cyc_mem = cyc_ignr) then  -- Jsem u posledni ']'
				pc_inc     <= '0';
				cyc_dec    <= '1';  -- Vynuluji pro dalsi pouziti
				next_state <= FETCH;
			elsif (CODE_DATA = X"5D") then
				cyc_dec <= '1';  -- Konec vnoreneho cyklu
			elsif (CODE_DATA = X"00") then -- Narazil jsem na EOF -> error
				next_state <= RUN_ERROR;
			end if;

		when WHILE_END =>
			DATA_EN    <= '1';  -- Zapnu RAM

			if (DATA_RDATA = "00000000") then  -- While skoncil
				cyc_dec    <= '1';  -- Ukoncim while
				pc_inc     <= '1';  -- Posunu se dal
				next_state <= INIT;
			elsif (cyc_mem = "00000000") then  -- While jeste nezacal -> error
				next_state <= RUN_ERROR;
			else  -- Vratim se na zacatek cyklu
				cyc_set    <= '1';
				next_state <= WHILE_END_RETURN;
			end if;

		when WHILE_END_RETURN =>
			next_state <= WHILE_END_RETURN;
			pc_dec     <= '1';  -- Vratim se v kodu
			
			if (CODE_DATA = X"5D") then -- Nasel jsem ']' musim preskocit vnoreny cyklus
				cyc_inc <= '1';
			elsif (CODE_DATA = X"5B") then  -- Nasel jsem '['
				cyc_dec <= '1';
				if (cyc_mem = cyc_ignr) then  -- Jsem u te spravne '[' -> spustim znovu cyklus
					cyc_dec    <= '0';  -- Zvednu pocet cyklu na 1 => takze vlastne nedelam nic
					pc_inc     <= '1';  -- vratim ze zpet na '['
					pc_dec     <= '0';  -- Tento prikaz by mel byt zbytecny...
					next_state <= WHILE_START_WAIT;
				end if;
			end if;

		when WHILE_START_WAIT =>
			pc_inc     <= '1';  -- vratim ze zpet na '['
			next_state <= WHILE_START;



		-- Vypisuji / ctu znak
		when PUTCHAR =>
			DATA_EN  <= '1';  -- Zpristupnuji RAM
			lcd_wr   <= '1';  -- Zapis na LCD

			-- LCD zaneprazdnen - cekam
			if (OUT_BUSY = '1') then
				next_state <= PUTCHAR;
			else
				OUT_WE     <= '1';       -- Zapnu LCD
				OUT_DATA   <= tmp_byte;  -- Zapisu data na LCD
				next_state <= FETCH;
			end if;

		when GETCHAR =>
			IN_REQ <= '1';  -- Chci cist
			-- Muzu cist, jinak pockam
			if (IN_VLD = '1') then
				DATA_RDWR  <= '0';      -- Zapisuji do RAM
				DATA_EN    <= '1';      -- Zpristupnuji RAM
				kb_read    <= '1';
				--DATA_WDATA <= "01000001";  -- Zapisu vstup z klavesnice do RAM
				DATA_WDATA <= tmp_byte;  -- Zapisu vstup z klavesnice do RAM
				next_state <= FETCH;
			else
				next_state <= GETCHAR;
			end if;

		-- TMP promenna
		when TMP_PUT =>
			DATA_EN    <= '1';  -- Zpristupnuji RAM
			tmp_ld     <= '1';  -- Zapis 1B z RAM do tmp_var
			next_state <= FETCH;

		when TMP_GET =>
			DATA_RDWR  <= '0';       -- Zapisuji do RAM
			DATA_EN    <= '1';       -- Zpristupnuji RAM
			DATA_WDATA <= tmp_var;  -- Zapisu promennou do RAM
			next_state <= FETCH;


		-- Error
		when RUN_ERROR =>
			OUT_DATA <= "00100000";
			if (OUT_BUSY = '1') then
				next_state <= RUN_ERROR;
			else
				OUT_WE <= '1';
				next_state <= ERROR1;
			end if;
		when ERROR1 =>
			OUT_DATA <= "01000101";
			if (OUT_BUSY = '1') then
				next_state <= ERROR1;
			else
				OUT_WE <= '1';
				next_state <= ERROR2;
			end if;
		when ERROR2 =>
			OUT_DATA <= "01010010";
			if (OUT_BUSY = '1') then
				next_state <= ERROR2;
			else
				OUT_WE <= '1';
				next_state <= ERROR3;
			end if;
		when ERROR3 =>
			OUT_DATA <= "01010010";
			if (OUT_BUSY = '1') then
				next_state <= ERROR3;
			else
				OUT_WE <= '1';
				next_state <= ERROR4;
			end if;
		when ERROR4 =>
			OUT_DATA <= "01001111";
			if (OUT_BUSY = '1') then
				next_state <= ERROR4;
			else
				OUT_WE <= '1';
				next_state <= ERROR5;
			end if;
		when ERROR5 =>
			OUT_DATA <= "01010010";
			if (OUT_BUSY = '1') then
				next_state <= ERROR5;
			else
				OUT_WE <= '1';
				next_state <= ERROR6;
			end if;
		when ERROR6 =>
			OUT_DATA <= "00100000";
			if (OUT_BUSY = '1') then
				next_state <= ERROR6;
			else
				OUT_WE <= '1';
				next_state <= PROGRAM_END;
			end if;


		-- Konec
		when PROGRAM_END =>
			next_state <= PROGRAM_END;

		when others => null;
    end case;
end process;


end behavioral;
