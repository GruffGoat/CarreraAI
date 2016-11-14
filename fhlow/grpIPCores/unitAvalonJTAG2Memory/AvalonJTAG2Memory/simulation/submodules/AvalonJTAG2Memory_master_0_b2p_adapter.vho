--IP Functional Simulation Model
--VERSION_BEGIN 15.0 cbx_mgl 2015:04:22:18:06:50:SJ cbx_simgen 2015:04:22:18:04:08:SJ  VERSION_END


-- Copyright (C) 1991-2015 Altera Corporation. All rights reserved.
-- Your use of Altera Corporation's design tools, logic functions 
-- and other software and tools, and its AMPP partner logic 
-- functions, and any output files from any of the foregoing 
-- (including device programming or simulation files), and any 
-- associated documentation or information are expressly subject 
-- to the terms and conditions of the Altera Program License 
-- Subscription Agreement, the Altera Quartus II License Agreement,
-- the Altera MegaCore Function License Agreement, or other 
-- applicable license agreement, including, without limitation, 
-- that your use is for the sole purpose of programming logic 
-- devices manufactured by Altera and sold by Altera or its 
-- authorized distributors.  Please refer to the applicable 
-- agreement for further details.

-- You may only use these simulation model output files for simulation
-- purposes and expressly not for synthesis or any other purposes (in which
-- event Altera disclaims all warranties of any kind).


--synopsys translate_off
 USE work.all;


 LIBRARY sgate;
 USE sgate.sgate_pack.all;

--synthesis_resources = mux21 1 oper_less_than 1 
 LIBRARY ieee;
 USE ieee.std_logic_1164.all;

 ENTITY  AvalonJTAG2Memory_master_0_b2p_adapter IS 
	 PORT 
	 ( 
		 clk	:	IN  STD_LOGIC;
		 in_channel	:	IN  STD_LOGIC_VECTOR (7 DOWNTO 0);
		 in_data	:	IN  STD_LOGIC_VECTOR (7 DOWNTO 0);
		 in_endofpacket	:	IN  STD_LOGIC;
		 in_ready	:	OUT  STD_LOGIC;
		 in_startofpacket	:	IN  STD_LOGIC;
		 in_valid	:	IN  STD_LOGIC;
		 out_data	:	OUT  STD_LOGIC_VECTOR (7 DOWNTO 0);
		 out_endofpacket	:	OUT  STD_LOGIC;
		 out_ready	:	IN  STD_LOGIC;
		 out_startofpacket	:	OUT  STD_LOGIC;
		 out_valid	:	OUT  STD_LOGIC;
		 reset_n	:	IN  STD_LOGIC
	 ); 
 END AvalonJTAG2Memory_master_0_b2p_adapter;

 ARCHITECTURE RTL OF AvalonJTAG2Memory_master_0_b2p_adapter IS

	 ATTRIBUTE synthesis_clearbox : natural;
	 ATTRIBUTE synthesis_clearbox OF RTL : ARCHITECTURE IS 1;
	 SIGNAL	wire_avalonjtag2memory_master_0_b2p_adapter_out_valid_2m_dataout	:	STD_LOGIC;
	 SIGNAL  wire_avalonjtag2memory_master_0_b2p_adapter_lessthan0_1_a	:	STD_LOGIC_VECTOR (7 DOWNTO 0);
	 SIGNAL  wire_avalonjtag2memory_master_0_b2p_adapter_lessthan0_1_b	:	STD_LOGIC_VECTOR (7 DOWNTO 0);
	 SIGNAL  wire_gnd	:	STD_LOGIC;
	 SIGNAL  wire_avalonjtag2memory_master_0_b2p_adapter_lessthan0_1_o	:	STD_LOGIC;
 BEGIN

	wire_gnd <= '0';
	in_ready <= out_ready;
	out_data <= ( in_data(7 DOWNTO 0));
	out_endofpacket <= in_endofpacket;
	out_startofpacket <= in_startofpacket;
	out_valid <= wire_avalonjtag2memory_master_0_b2p_adapter_out_valid_2m_dataout;
	wire_avalonjtag2memory_master_0_b2p_adapter_out_valid_2m_dataout <= in_valid AND NOT(wire_avalonjtag2memory_master_0_b2p_adapter_lessthan0_1_o);
	wire_avalonjtag2memory_master_0_b2p_adapter_lessthan0_1_a <= ( "0" & "0" & "0" & "0" & "0" & "0" & "0" & "0");
	wire_avalonjtag2memory_master_0_b2p_adapter_lessthan0_1_b <= ( in_channel(7 DOWNTO 0));
	avalonjtag2memory_master_0_b2p_adapter_lessthan0_1 :  oper_less_than
	  GENERIC MAP (
		sgate_representation => 0,
		width_a => 8,
		width_b => 8
	  )
	  PORT MAP ( 
		a => wire_avalonjtag2memory_master_0_b2p_adapter_lessthan0_1_a,
		b => wire_avalonjtag2memory_master_0_b2p_adapter_lessthan0_1_b,
		cin => wire_gnd,
		o => wire_avalonjtag2memory_master_0_b2p_adapter_lessthan0_1_o
	  );

 END RTL; --AvalonJTAG2Memory_master_0_b2p_adapter
--synopsys translate_on
--VALID FILE
