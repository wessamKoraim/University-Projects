LIBRARY IEEE;
USE IEEE.STD_LOGIC_1164.ALL;
USE IEEE.STD_LOGIC_UNSIGNED.ALL;

entity PWM is
	port(	clk: in std_logic;
			W: in std_logic_vector (3 downto 0);
			Y: out std_logic);
end PWM;

Architecture behavioral of PWM is
signal clk1: std_logic;

begin
	process(clk)
		variable count: integer := 0;
		begin
			if(clk'event and clk = '1') then
				count := count + 1;
				case W is
					when "0000" =>
						clk1 <= '1';
					when "0001" =>
						if( count <= 1) then clk1 <= '1';
						elsif (count > 1) then clk1 <= '0';
						end if;
					when "0010" =>
						if( count <= 2) then clk1 <= '1';
						elsif (count > 2) then clk1 <= '0';
						end if;
					when "0011" =>
						if( count <= 3) then clk1 <= '1';
						elsif (count > 3) then clk1 <= '0';
						end if;
					when "0100" =>
						if( count <= 4) then clk1 <= '1';
						elsif (count > 4) then clk1 <= '0';
						end if;
					when "0101" =>
						if( count <= 5) then clk1 <= '1';
						elsif (count > 5) then clk1 <= '0';
						end if;
					when "0110" =>
						if( count <= 6) then clk1 <= '1';
						elsif (count > 6) then clk1 <= '0';
						end if;
					when "0111" =>
						if( count <= 7) then clk1 <= '1';
						elsif (count > 7) then clk1 <= '0';
						end if;
					when "1000" =>
						if( count <= 8) then clk1 <= '1';
						elsif (count > 8) then clk1 <= '0';
						end if;
					when "1001" =>
						if( count <= 9) then clk1 <= '1';
						elsif (count > 9) then clk1 <= '0';
						end if;
					when "1010" =>
						if( count <= 10) then clk1 <= '1';
						elsif (count > 10) then clk1 <= '0';
						end if;
					when "1011" =>
						if( count <= 11) then clk1 <= '1';
						elsif (count > 11) then clk1 <= '0';
						end if;
					when "1100" =>
						if( count <= 12) then clk1 <= '1';
						elsif (count > 12) then clk1 <= '0';
						end if;
					when "1101" =>
						if( count <= 13) then clk1 <= '1';
						elsif (count > 13) then clk1 <= '0';
						end if;
					when "1110" =>
						if( count <= 14) then clk1 <= '1';
						elsif (count > 14) then clk1 <= '0';
						end if;
					when "1111" =>
						if( count <= 15) then clk1 <= '1';
						elsif (count > 15) then clk1 <= '0';
						end if;
					when others => clk1 <= clk1;
				end case;
				
				if(count = 16) then
					count := 0;
				end if;
			end if;
	end process;
	Y <= clk1;
end behavioral;