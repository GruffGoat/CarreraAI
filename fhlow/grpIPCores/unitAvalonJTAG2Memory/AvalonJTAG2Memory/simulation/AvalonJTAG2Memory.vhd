-- AvalonJTAG2Memory.vhd

-- Generated using ACDS version 15.0 145

library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.numeric_std.all;

entity AvalonJTAG2Memory is
	port (
		clk_clk              : in  std_logic                     := '0';             --          clk.clk
		clk_reset_reset      : in  std_logic                     := '0';             --    clk_reset.reset
		master_address       : out std_logic_vector(31 downto 0);                    --       master.address
		master_readdata      : in  std_logic_vector(31 downto 0) := (others => '0'); --             .readdata
		master_read          : out std_logic;                                        --             .read
		master_write         : out std_logic;                                        --             .write
		master_writedata     : out std_logic_vector(31 downto 0);                    --             .writedata
		master_waitrequest   : in  std_logic                     := '0';             --             .waitrequest
		master_readdatavalid : in  std_logic                     := '0';             --             .readdatavalid
		master_byteenable    : out std_logic_vector(3 downto 0);                     --             .byteenable
		master_reset_reset   : out std_logic                                         -- master_reset.reset
	);
end entity AvalonJTAG2Memory;

architecture rtl of AvalonJTAG2Memory is
	component AvalonJTAG2Memory_master_0 is
		port (
			clk_clk              : in  std_logic                     := 'X';             -- clk
			clk_reset_reset      : in  std_logic                     := 'X';             -- reset
			master_address       : out std_logic_vector(31 downto 0);                    -- address
			master_readdata      : in  std_logic_vector(31 downto 0) := (others => 'X'); -- readdata
			master_read          : out std_logic;                                        -- read
			master_write         : out std_logic;                                        -- write
			master_writedata     : out std_logic_vector(31 downto 0);                    -- writedata
			master_waitrequest   : in  std_logic                     := 'X';             -- waitrequest
			master_readdatavalid : in  std_logic                     := 'X';             -- readdatavalid
			master_byteenable    : out std_logic_vector(3 downto 0);                     -- byteenable
			master_reset_reset   : out std_logic                                         -- reset
		);
	end component AvalonJTAG2Memory_master_0;

begin

	master_0 : component AvalonJTAG2Memory_master_0
		port map (
			clk_clk              => clk_clk,              --          clk.clk
			clk_reset_reset      => clk_reset_reset,      --    clk_reset.reset
			master_address       => master_address,       --       master.address
			master_readdata      => master_readdata,      --             .readdata
			master_read          => master_read,          --             .read
			master_write         => master_write,         --             .write
			master_writedata     => master_writedata,     --             .writedata
			master_waitrequest   => master_waitrequest,   --             .waitrequest
			master_readdatavalid => master_readdatavalid, --             .readdatavalid
			master_byteenable    => master_byteenable,    --             .byteenable
			master_reset_reset   => master_reset_reset    -- master_reset.reset
		);

end architecture rtl; -- of AvalonJTAG2Memory