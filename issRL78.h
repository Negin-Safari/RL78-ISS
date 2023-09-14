#include <iostream>
#include <systemc.h>
#include <string>
#include "RL78Decode.h"
#include "RL78Structure.h"
#include "RL78Morph.h"
#include "Debug.h"
using namespace std;


SC_MODULE(issRL78) {
	sc_in <sc_logic> clk;
	sc_in <sc_logic> memReady;
	sc_inout <sc_lv<8>> dataBus;
	sc_out <sc_logic> readMem, writeMem;
	sc_out <sc_lv<20>> addrBus;
	struct RL78Struct ST;
	typedef void (issRL78::*DispathFNC)(sc_lv <8>, sc_lv <8>, sc_lv <8>, sc_lv <8>);
	DispathFNC MorphFunctions[RL78_IT_LAST];
	debug log;

	SC_HAS_PROCESS(issRL78);
	////---------------------------------------------Constructor------------------------------------//
	SC_CTOR(issRL78) {
		log.debug_state = DEBUG_ENABLE;
		ST.adrSpace = int(pow(2, 20));
		SC_THREAD(abstractSimulation);
		sensitive << clk.pos();
	}

	////-----------------------------------------Member Functions------------------------------------//
	void abstractSimulation();

	void checkFirstMap(rl78InstructionType type, sc_lv <8>& Byte1, sc_lv <8>& Byte2, sc_lv <8>& Byte3, sc_lv <8>& Byte4);
	sc_lv<8> Fetch1Byte();
	sc_lv<8> Read1Byte(sc_lv<20> addr);
	void Write1Byte(sc_lv<8> data2write, sc_lv<20> addr);
	sc_lv<20> PrefixAddr();
	void createMorphTable();
	////-----------------------------------Morph Function Declerations-------------------------------//
	void MORPH_1_00_NOP(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4);
	void MORPH_1_0X_ADDW(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4);
	void MORPH_1_02_ADDW(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4);
	void MORPH_1_04_ADDW(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4);
	void MORPH_1_06_ADDW(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4);
	void MORPH_1_08_XCH(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4);
	void MORPH_1_09_MOV(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4);
	void MORPH_1_0A_ADD(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4);
	void MORPH_1_0B_ADD(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4);
	void MORPH_1_0C_ADD(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4);
	void MORPH_1_0D_ADD(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4);
	void MORPH_1_0E_ADD(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4);
	void MORPH_1_0F_ADD(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4);
	void MORPH_1_10_ADDW(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4);
	void MORPH_1_11_PREFIX(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4);
	void MORPH_1_1X_MOVW(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4);
	void MORPH_1_1Y_MOVW(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4);
	void MORPH_1_18_MOV(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4);
	void MORPH_1_19_MOV(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4);
	void MORPH_1_1A_ADDC(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4);
	void MORPH_1_1B_ADDC(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4);
	void MORPH_1_1C_ADDC(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4);
	void MORPH_1_1D_ADDC(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4);
	void MORPH_1_1E_ADDC(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4);
	void MORPH_1_1F_ADDC(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4);
	void MORPH_1_20_SUBW(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4);
	void MORPH_1_22_SUBW(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4);
	void MORPH_1_2X_SUBW(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4);
	void MORPH_1_24_SUBW(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4);
	void MORPH_1_26_SUBW(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4);
	void MORPH_1_28_MOV(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4);
	void MORPH_1_29_MOV(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4);
	void MORPH_1_2A_SUB(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4);
	void MORPH_1_2B_SUB(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4);
	void MORPH_1_2C_SUB(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4);
	void MORPH_1_2D_SUB(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4);
	void MORPH_1_2E_SUB(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4);
	void MORPH_1_2F_SUB(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4);
	void MORPH_1_3X_MOVW(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4);

	void MORPH_1_3Y_XCHW(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4);
	void MORPH_1_38_MOV(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4);
	void MORPH_1_39_MOV(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4);
	void MORPH_1_3A_SUBC(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4);
	void MORPH_1_3B_SUBC(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4);
	void MORPH_1_3C_SUBC(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4);
	void MORPH_1_3D_SUBC(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4);
	void MORPH_1_3E_SUBC(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4);
	void MORPH_1_3F_SUBC(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4);
	void MORPH_1_40_CMP(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4);
	void MORPH_1_41_MOV(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4);
	void MORPH_1_42_CMPW(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4);
	void MORPH_1_4X_CMPW(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4);
	void MORPH_1_44_CMPW(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4);
	void MORPH_1_46_CMPW(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4);
	void MORPH_1_48_MOV(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4);
	void MORPH_1_49_MOV(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4);
	void MORPH_1_4A_CMP(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4);
	void MORPH_1_4B_CMP(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4);
	void MORPH_1_4C_CMP(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4);
	void MORPH_1_4D_CMP(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4);
	void MORPH_1_4E_CMP(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4);
	void MORPH_1_4F_CMP(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4);
	void MORPH_1_5X_MOV(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4);
	void MORPH_1_58_MOVW(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4);
	void MORPH_1_59_MOVW(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4);
	void MORPH_1_5A_AND(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4);
	void MORPH_1_5B_AND(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4);
	void MORPH_1_5C_AND(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4);
	void MORPH_1_5D_AND(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4);
	void MORPH_1_5E_AND(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4);
	void MORPH_1_5F_AND(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4);
	void MORPH_1_6X_MOV(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4);
	void MORPH_1_68_MOVW(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4);
	void MORPH_1_69_MOVW(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4);
	void MORPH_1_6A_OR(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4);
	void MORPH_1_6B_OR(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4);
	void MORPH_1_6C_OR(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4);
	void MORPH_1_6D_OR(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4);
	void MORPH_1_6E_OR(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4);
	void MORPH_1_6F_OR(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4);
	void MORPH_1_7X_MOV(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4);
	void MORPH_1_78_MOVW(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4);
	void MORPH_1_79_MOVW(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4);
	void MORPH_1_7A_XOR(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4);
	void MORPH_1_7B_XOR(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4);
	void MORPH_1_7C_XOR(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4);
	void MORPH_1_7D_XOR(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4);
	void MORPH_1_7E_XOR(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4);
	void MORPH_1_7F_XOR(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4);
	void MORPH_1_8X_INC(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4);

	void MORPH_1_88_MOV(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4);
	void MORPH_1_89_MOV(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4);
	void MORPH_1_8A_MOV(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4);
	void MORPH_1_8B_MOV(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4);
	void MORPH_1_8C_MOV(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4);
	void MORPH_1_8D_MOV(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4);
	void MORPH_1_8E_MOV(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4);
	void MORPH_1_8F_MOV(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4);
	void MORPH_1_9X_DEC(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4);
	void MORPH_1_98_MOV(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4);
	void MORPH_1_99_MOV(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4);
	void MORPH_1_9A_MOV(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4);
	void MORPH_1_9B_MOV(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4);
	void MORPH_1_9C_MOV(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4);
	void MORPH_1_9D_MOV(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4);
	void MORPH_1_9E_MOV(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4);
	void MORPH_1_9F_MOV(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4);
	void MORPH_1_A0_INC(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4);
	void MORPH_1_AX_INCW(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4);
	void MORPH_1_A2_INCW(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4);
	void MORPH_1_A4_INC(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4);
	void MORPH_1_A6_INCW(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4);
	void MORPH_1_A8_MOVW(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4);
	void MORPH_1_A9_MOVW(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4);
	void MORPH_1_AA_MOVW(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4);
	void MORPH_1_AB_MOVW(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4);
	void MORPH_1_AC_MOVW(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4);
	void MORPH_1_AD_MOVW(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4);
	void MORPH_1_AE_MOVW(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4);
	void MORPH_1_AF_MOVW(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4);
	void MORPH_1_B0_DEC(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4);
	void MORPH_1_BX_DECW(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4);
	void MORPH_1_B2_DECW(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4);
	void MORPH_1_B4_DEC(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4);
	void MORPH_1_B6_DECW(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4);
	void MORPH_1_B8_MOVW(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4);
	void MORPH_1_B9_MOVW(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4);
	void MORPH_1_BA_MOVW(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4);
	void MORPH_1_BB_MOVW(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4);
	void MORPH_1_BC_MOVW(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4);
	void MORPH_1_BD_MOVW(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4);
	void MORPH_1_BE_MOVW(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4);
	void MORPH_1_BF_MOVW(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4);
	void MORPH_1_CX_POP(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4);
	void MORPH_1_CY_PUSH(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4);
	void MORPH_1_C8_MOV(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4);
	void MORPH_1_C9_MOVW(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4);
	void MORPH_1_CA_MOV(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4);
	void MORPH_1_CB_MOVW(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4);
	void MORPH_1_CC_MOV(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4);
	void MORPH_1_CD_MOV(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4);
	void MORPH_1_CE_MOV(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4);
	void MORPH_1_CF_MOV(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4);
	void MORPH_1_DX_CMP0(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4);
	void MORPH_1_D4_CMP0(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4);
	void MORPH_1_D5_CMP0(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4);
	void MORPH_1_D6_MULU(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4);
	void MORPH_1_D7_RET(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4);
	void MORPH_1_D8_MOV(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4);
	void MORPH_1_D9_MOV(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4);
	void MORPH_1_DA_MOVW(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4);
	void MORPH_1_DB_MOVW(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4);
	void MORPH_1_DC_BC(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4);
	void MORPH_1_DD_BZ(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4);
	void MORPH_1_DE_BNC(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4);
	void MORPH_1_DF_BNZ(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4);
	void MORPH_1_EX_ONEB(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4);
	void MORPH_1_E4_ONEB(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4);
	void MORPH_1_E5_ONEB(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4);
	void MORPH_1_E6_ONEW(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4);
	void MORPH_1_E7_ONEW(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4);
	void MORPH_1_E8_MOV(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4);
	void MORPH_1_E9_MOV(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4);
	void MORPH_1_EA_MOVW(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4);
	void MORPH_1_EB_MOVW(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4);
	void MORPH_1_EC_BR(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4);
	void MORPH_1_ED_BR(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4);
	void MORPH_1_EE_BR(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4);
	void MORPH_1_EF_BR(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4);
	void MORPH_1_FX_CLRB(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4);
	void MORPH_1_F4_CLRB(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4);
	void MORPH_1_F5_CLRB(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4);
	void MORPH_1_F6_CLRW(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4);
	void MORPH_1_F8_MOV(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4);
	void MORPH_1_F9_MOV(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4);
	void MORPH_1_F7_CLRW(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4);
	void MORPH_1_FA_MOVW(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4);
	void MORPH_1_FB_MOVW(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4);
	void MORPH_1_FC_CALL(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4);
	void MORPH_1_FD_CALL(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4);
	void MORPH_1_FE_CALL(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4);
	void MORPH_2_0X_ADD(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4);
	void MORPH_2_09_ADDW(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4);
};





