#pragma once
#include <iostream>
#include <systemc.h>
#include <string>
#define    AX       0
#define    BC       2
#define    DE       4
#define    HL       6
#define    X        0
#define    A        1
#define    C        2
#define    B        3
#define    E        4
#define    D        5
#define    L        6
#define    H        7
typedef enum rl78InstructionTypeE {

	// 1st MAP
	RL78_IT_1_00_NOP,
	RL78_IT_1_0X_ADDW,
	RL78_IT_1_02_ADDW,
	RL78_IT_1_03_ADDW,
	RL78_IT_1_04_ADDW,
	RL78_IT_1_05_ADDW,
	RL78_IT_1_06_ADDW,
	RL78_IT_1_07_ADDW,
	RL78_IT_1_08_XCH,
	RL78_IT_1_09_MOV,
	RL78_IT_1_0A_ADD,
	RL78_IT_1_0B_ADD,
	RL78_IT_1_0C_ADD,
	RL78_IT_1_0D_ADD,
	RL78_IT_1_0E_ADD,
	RL78_IT_1_0F_ADD,
	RL78_IT_1_10_ADDW,
	RL78_IT_1_11_PREFIX,
	RL78_IT_1_1X_MOVW,
	RL78_IT_1_1Y_MOVW,
	RL78_IT_1_14_MOVW,
	RL78_IT_1_15_MOVW,
	RL78_IT_1_16_MOVW,
	RL78_IT_1_17_MOVW,
	RL78_IT_1_18_MOV,
	RL78_IT_1_19_MOV,
	RL78_IT_1_1A_ADDC,
	RL78_IT_1_1B_ADDC,
	RL78_IT_1_1C_ADDC,
	RL78_IT_1_1D_ADDC,
	RL78_IT_1_1E_ADDC,
	RL78_IT_1_1F_ADDC,
	RL78_IT_1_20_SUBW,
	RL78_IT_1_22_SUBW,
	RL78_IT_1_24_SUBW,
	RL78_IT_1_26_SUBW,
	RL78_IT_1_2X_SUBW,
	RL78_IT_1_28_MOV,
	RL78_IT_1_29_MOV,
	RL78_IT_1_2A_SUB,
	RL78_IT_1_2B_SUB,
	RL78_IT_1_2C_SUB,
	RL78_IT_1_2D_SUB,
	RL78_IT_1_2E_SUB,
	RL78_IT_1_2F_SUB, /////////
	RL78_IT_1_31_4thMap,
	RL78_IT_1_3X_MOVW,
	RL78_IT_1_3Y_XCHW,
	RL78_IT_1_38_MOV,
	RL78_IT_1_39_MOV,
	RL78_IT_1_3A_SUBC,
	RL78_IT_1_3B_SUBC,
	RL78_IT_1_3C_SUBC,
	RL78_IT_1_3D_SUBC,
	RL78_IT_1_3E_SUBC,
	RL78_IT_1_3F_SUBC,
	RL78_IT_1_40_CMP,
	RL78_IT_1_41_MOV,
	RL78_IT_1_42_CMPW,
	RL78_IT_1_44_CMPW,
	RL78_IT_1_46_CMPW,
	RL78_IT_1_4X_CMPW,
	RL78_IT_1_48_MOV,
	RL78_IT_1_49_MOV,
	RL78_IT_1_4A_CMP,
	RL78_IT_1_4B_CMP,
	RL78_IT_1_4C_CMP,
	RL78_IT_1_4D_CMP,
	RL78_IT_1_4E_CMP,
	RL78_IT_1_4F_CMP, /////
	RL78_IT_1_5X_MOV,
	RL78_IT_1_58_MOVW,
	RL78_IT_1_59_MOVW,
	RL78_IT_1_5A_AND,
	RL78_IT_1_5B_AND,
	RL78_IT_1_5C_AND,
	RL78_IT_1_5D_AND,
	RL78_IT_1_5E_AND,
	RL78_IT_1_5F_AND,
	RL78_IT_1_61_2ndMap,
	RL78_IT_1_6X_MOV,
	RL78_IT_1_68_MOVW,
	RL78_IT_1_69_MOVW,
	RL78_IT_1_6A_OR,
	RL78_IT_1_6B_OR,
	RL78_IT_1_6C_OR,
	RL78_IT_1_6D_OR,
	RL78_IT_1_6E_OR,
	RL78_IT_1_6F_OR, ///////////////////////
	RL78_IT_1_7X_MOV,
	RL78_IT_1_71_3rdMap,
	RL78_IT_1_78_MOVW,
	RL78_IT_1_79_MOVW,
	RL78_IT_1_7A_XOR,
	RL78_IT_1_7B_XOR,
	RL78_IT_1_7C_XOR,
	RL78_IT_1_7D_XOR,
	RL78_IT_1_7E_XOR,
	RL78_IT_1_7F_XOR,
	RL78_IT_1_8X_INC,
	RL78_IT_1_88_MOV,
	RL78_IT_1_89_MOV,
	RL78_IT_1_8A_MOV,
	RL78_IT_1_8B_MOV,
	RL78_IT_1_8C_MOV,
	RL78_IT_1_8D_MOV,
	RL78_IT_1_8E_MOV,
	RL78_IT_1_8F_MOV, /////////////////
	RL78_IT_1_9X_DEC,
	RL78_IT_1_98_MOV,
	RL78_IT_1_99_MOV,
	RL78_IT_1_9A_MOV,
	RL78_IT_1_9B_MOV,
	RL78_IT_1_9C_MOV,
	RL78_IT_1_9D_MOV,
	RL78_IT_1_9E_MOV,
	RL78_IT_1_9F_MOV,
	RL78_IT_1_A0_INC,
	RL78_IT_1_A2_INCW,
	RL78_IT_1_A4_INC,
	RL78_IT_1_A6_INCW,
	RL78_IT_1_AX_INCW,
	RL78_IT_1_A8_MOVW,
	RL78_IT_1_A9_MOVW,
	RL78_IT_1_AA_MOVW,
	RL78_IT_1_AB_MOVW,
	RL78_IT_1_AC_MOVW,
	RL78_IT_1_AD_MOVW,
	RL78_IT_1_AE_MOVW,
	RL78_IT_1_AF_MOVW,
	RL78_IT_1_B0_DEC,
	RL78_IT_1_B2_DECW,
	RL78_IT_1_B4_DEC,
	RL78_IT_1_B6_DECW,
	RL78_IT_1_BX_DECW,
	RL78_IT_1_B8_MOVW,
	RL78_IT_1_B9_MOVW,
	RL78_IT_1_BA_MOVW,
	RL78_IT_1_BB_MOVW,
	RL78_IT_1_BC_MOVW,
	RL78_IT_1_BD_MOVW,
	RL78_IT_1_BE_MOVW,
	RL78_IT_1_BF_MOVW,
	RL78_IT_1_CX_POP,
	RL78_IT_1_CY_PUSH,
	RL78_IT_1_C8_MOV,
	RL78_IT_1_C9_MOVW,
	RL78_IT_1_CA_MOV,
	RL78_IT_1_CB_MOVW,
	RL78_IT_1_CC_MOV,
	RL78_IT_1_CD_MOV,
	RL78_IT_1_CE_MOV,
	RL78_IT_1_CF_MOV,
	RL78_IT_1_DX_CMP0, ///////////////
	RL78_IT_1_D4_CMP0,
	RL78_IT_1_D5_CMP0,
	RL78_IT_1_D6_MULU,
	RL78_IT_1_D7_RET,
	RL78_IT_1_D8_MOV,
	RL78_IT_1_D9_MOV,
	RL78_IT_1_DA_MOVW,
	RL78_IT_1_DB_MOVW,
	RL78_IT_1_DC_BC,
	RL78_IT_1_DD_BZ,
	RL78_IT_1_DE_BNC,
	RL78_IT_1_DF_BNZ,
	RL78_IT_1_EX_ONEB,
	RL78_IT_1_E4_ONEB,
	RL78_IT_1_E5_ONEB,
	RL78_IT_1_E6_ONEW,
	RL78_IT_1_E7_ONEW,
	RL78_IT_1_E8_MOV,
	RL78_IT_1_E9_MOV,
	RL78_IT_1_EA_MOVW,
	RL78_IT_1_EB_MOVW,
	RL78_IT_1_EC_BR,
	RL78_IT_1_ED_BR,
	RL78_IT_1_EE_BR,
	RL78_IT_1_EF_BR,
	RL78_IT_1_FX_CLRB,
	RL78_IT_1_F4_CLRB,
	RL78_IT_1_F5_CLRB,
	RL78_IT_1_F6_CLRW,
	RL78_IT_1_F7_CLRW,
	RL78_IT_1_F8_MOV,
	RL78_IT_1_F9_MOV,
	RL78_IT_1_FA_MOVW,
	RL78_IT_1_FB_MOVW,
	RL78_IT_1_FC_CALL,
	RL78_IT_1_FD_CALL,
	RL78_IT_1_FE_CALL,

	///////MAP2//////
	RL78_IT_2_0X_ADD,
	RL78_IT_2_09_ADDW,
	RL78_IT_2_0Y_ADD,
	RL78_IT_2_1X_ADDC,
	RL78_IT_2_1Y_ADDC,
	RL78_IT_2_2X_SUB,
	RL78_IT_2_29_SUBW,
	RL78_IT_2_2Y_SUB,
	RL78_IT_2_3X_SUBC,
	RL78_IT_2_3Y_SUBC,
	RL78_IT_2_4X_CMP,
	RL78_IT_2_49_CMPW,
	RL78_IT_2_4Y_CMP,
	RL78_IT_2_5X_AND,
	RL78_IT_2_59_INC,
	RL78_IT_2_5Y_AND,
	RL78_IT_2_6X_OR,
	RL78_IT_2_69_DEC,
	RL78_IT_2_6Y_OR,
	RL78_IT_2_7X_XOR,
	RL78_IT_2_79_INCW,
	RL78_IT_2_7Y_XOR,
	RL78_IT_2_80_ADD,
	RL78_IT_2_82_ADD,
	RL78_IT_2_89_DECW,
	RL78_IT_2_8X_XCH,
	RL78_IT_2_90_ADDC,
	RL78_IT_2_92_ADDC,
	RL78_IT_2_A0_SUB,
	RL78_IT_2_A2_SUB,
	RL78_IT_2_A8_XCH,
	RL78_IT_2_A9_XCH,
	RL78_IT_2_AA_XCH,
	RL78_IT_2_AB_XCH,
	RL78_IT_2_AC_XCH,
	RL78_IT_2_AD_XCH,
	RL78_IT_2_AE_XCH,
	RL78_IT_2_AF_XCH,
	RL78_IT_2_B0_SUBC,
	RL78_IT_2_B2_SUBC,
	RL78_IT_2_B8_MOV,
	RL78_IT_2_B9_XCH,
	RL78_IT_2_C0_CMP,
	RL78_IT_2_C2_CMP,
	RL78_IT_2_C3_BH,
	RL78_IT_2_C8_SKC,
	RL78_IT_2_C9_MOV,
	RL78_IT_2_CB_BR,
	RL78_IT_2_CC_BRK,
	RL78_IT_2_CD_POP,
	RL78_IT_2_CE_MOVS,
	RL78_IT_2_D0_AND,
	RL78_IT_2_D2_AND,
	RL78_IT_2_D3_BNH,
	RL78_IT_2_D8_SKNC,
	RL78_IT_2_D9_MOV,
	RL78_IT_2_DB_ROR,
	RL78_IT_2_DC_ROLC,
	RL78_IT_2_DD_PUSH,
	RL78_IT_2_DE_CMPS,
	RL78_IT_2_E0_OR,
	RL78_IT_2_E2_OR,
	RL78_IT_2_E3_SKH,
	RL78_IT_2_E8_SKZ,
	RL78_IT_2_E9_MOV,
	RL78_IT_2_EB_ROL,
	RL78_IT_2_EC_RETB,
	RL78_IT_2_ED_HALT,
	RL78_IT_2_F0_XOR,
	RL78_IT_2_F2_XOR,
	RL78_IT_2_F3_SKNH,
	RL78_IT_2_F8_SKNZ,
	RL78_IT_2_F9_MOV,
	RL78_IT_2_FB_RORC,
	RL78_IT_2_FC_RETI,
	RL78_IT_2_FD_STOP,
	RL78_IT_2_XE_ROLWC,
	RL78_IT_2_XF_SEL,
	RL78_IT_2_XA_CALL,
	RL78_IT_2_XX_CALLT,
	RL78_IT_LAST

} rl78InstructionType;


struct DECODE_ENTRY {
	int Inst_Length;
	rl78InstructionType Inst_CAT;
	sc_lv<8> pattern;
	sc_lv<8> Matchvalue;
};

typedef DECODE_ENTRY TABLE[RL78_IT_LAST];


inline void INSERT_MATCH_ENTRY(TABLE table, rl78InstructionType Inst_Name, int Inst_Length, sc_lv<8> inst_pattern, sc_lv<8> match_value) {
	table[Inst_Name].Inst_Length = Inst_Length;
	table[Inst_Name].Inst_CAT = Inst_Name;
	table[Inst_Name].pattern = inst_pattern;
	table[Inst_Name].Matchvalue = match_value;

}



inline DECODE_ENTRY DecodeINST(TABLE table, int map_offset, sc_lv<8> inst) {
	int i = map_offset;
	//cout << "integer i is: " << i << endl;
	bool found = false;
	while ((i < RL78_IT_LAST) && !found) {
		//cout << "pattern num " << i << " : " << table[i].pattern << endl;
		if ((inst & table[i].pattern) == table[i].Matchvalue) {
			found = true;
			return table[i];
		}
		found = false;
		i++;

	}
	cout << "is it found? " << found << endl;
}


inline void  createDecodeTable_1stmap(TABLE table) {
	INSERT_MATCH_ENTRY(table, RL78_IT_1_00_NOP, 1, "11111111", "00000000");
	INSERT_MATCH_ENTRY(table, RL78_IT_1_0X_ADDW, 1, "11111001", "00000001");
	INSERT_MATCH_ENTRY(table, RL78_IT_1_02_ADDW, 3, "11111111", "00000010");
	INSERT_MATCH_ENTRY(table, RL78_IT_1_04_ADDW, 3, "11111111", "00000100");
	INSERT_MATCH_ENTRY(table, RL78_IT_1_06_ADDW, 2, "11111111", "00000110");
	INSERT_MATCH_ENTRY(table, RL78_IT_1_08_XCH, 1, "11111111", "00001000");
	INSERT_MATCH_ENTRY(table, RL78_IT_1_09_MOV, 3, "11111111", "00001001");
	INSERT_MATCH_ENTRY(table, RL78_IT_1_0A_ADD, 3, "11111111", "00001010");
	INSERT_MATCH_ENTRY(table, RL78_IT_1_0B_ADD, 2, "11111111", "00001011");
	INSERT_MATCH_ENTRY(table, RL78_IT_1_0C_ADD, 3, "11111111", "00001100");
	INSERT_MATCH_ENTRY(table, RL78_IT_1_0D_ADD, 1, "11111111", "00001101");
	INSERT_MATCH_ENTRY(table, RL78_IT_1_0E_ADD, 2, "11111111", "00001110");
	INSERT_MATCH_ENTRY(table, RL78_IT_1_0F_ADD, 3, "11111111", "00001111");
	INSERT_MATCH_ENTRY(table, RL78_IT_1_10_ADDW, 1, "11111111", "00010000");/////////////
	INSERT_MATCH_ENTRY(table, RL78_IT_1_11_PREFIX, 1, "11111111", "00010001");
	INSERT_MATCH_ENTRY(table, RL78_IT_1_1X_MOVW, 1, "11111001", "00010000");
	INSERT_MATCH_ENTRY(table, RL78_IT_1_1Y_MOVW, 1, "11111001", "00010001");
	INSERT_MATCH_ENTRY(table, RL78_IT_1_18_MOV, 3, "11111111", "00011000");
	INSERT_MATCH_ENTRY(table, RL78_IT_1_19_MOV, 4, "11111111", "00011001");
	INSERT_MATCH_ENTRY(table, RL78_IT_1_1A_ADDC, 3, "11111111", "00011010");
	INSERT_MATCH_ENTRY(table, RL78_IT_1_1B_ADDC, 2, "11111111", "00011011");
	INSERT_MATCH_ENTRY(table, RL78_IT_1_1C_ADDC, 2, "11111111", "00011100");
	INSERT_MATCH_ENTRY(table, RL78_IT_1_1D_ADDC, 1, "11111111", "00011101");
	INSERT_MATCH_ENTRY(table, RL78_IT_1_1E_ADDC, 2, "11111111", "00011110");
	INSERT_MATCH_ENTRY(table, RL78_IT_1_1F_ADDC, 3, "11111111", "00011111");
	INSERT_MATCH_ENTRY(table, RL78_IT_1_20_SUBW, 2, "11111111", "00100000");//////////
	INSERT_MATCH_ENTRY(table, RL78_IT_1_22_SUBW, 3, "11111111", "00100010");
	INSERT_MATCH_ENTRY(table, RL78_IT_1_24_SUBW, 3, "11111111", "00100100");
	INSERT_MATCH_ENTRY(table, RL78_IT_1_26_SUBW, 2, "11111111", "00100110");
	INSERT_MATCH_ENTRY(table, RL78_IT_1_2X_SUBW, 1, "11111001", "00100001");
	INSERT_MATCH_ENTRY(table, RL78_IT_1_28_MOV, 3, "11111111", "00101000");
	INSERT_MATCH_ENTRY(table, RL78_IT_1_29_MOV, 3, "11111111", "00101001");
	INSERT_MATCH_ENTRY(table, RL78_IT_1_2A_SUB, 3, "11111111", "00101010");
	INSERT_MATCH_ENTRY(table, RL78_IT_1_2B_SUB, 2, "11111111", "00101011");
	INSERT_MATCH_ENTRY(table, RL78_IT_1_2C_SUB, 2, "11111111", "00101100");
	INSERT_MATCH_ENTRY(table, RL78_IT_1_2D_SUB, 1, "11111111", "00101101");
	INSERT_MATCH_ENTRY(table, RL78_IT_1_2E_SUB, 2, "11111111", "00101110");
	INSERT_MATCH_ENTRY(table, RL78_IT_1_2F_SUB, 3, "11111111", "00101111");
	INSERT_MATCH_ENTRY(table, RL78_IT_1_31_4thMap, 1, "11111111", "00110001");//////////****
	INSERT_MATCH_ENTRY(table, RL78_IT_1_3X_MOVW, 3, "11111001", "00110000");
	INSERT_MATCH_ENTRY(table, RL78_IT_1_3Y_XCHW, 1, "11111001", "00110001");
	INSERT_MATCH_ENTRY(table, RL78_IT_1_38_MOV, 4, "11111111", "00111000");
	INSERT_MATCH_ENTRY(table, RL78_IT_1_39_MOV, 4, "11111111", "00111001");
	INSERT_MATCH_ENTRY(table, RL78_IT_1_3A_SUBC, 3, "11111111", "00111010");
	INSERT_MATCH_ENTRY(table, RL78_IT_1_3B_SUBC, 2, "11111111", "00111011");
	INSERT_MATCH_ENTRY(table, RL78_IT_1_3C_SUBC, 2, "11111111", "00111100");
	INSERT_MATCH_ENTRY(table, RL78_IT_1_3D_SUBC, 1, "11111111", "00111101");
	INSERT_MATCH_ENTRY(table, RL78_IT_1_3E_SUBC, 2, "11111111", "00111110");
	INSERT_MATCH_ENTRY(table, RL78_IT_1_3F_SUBC, 3, "11111111", "00111111");
	INSERT_MATCH_ENTRY(table, RL78_IT_1_40_CMP, 4, "11111111", "01000000");
	INSERT_MATCH_ENTRY(table, RL78_IT_1_41_MOV, 2, "11111111", "01000001"); /////
	INSERT_MATCH_ENTRY(table, RL78_IT_1_42_CMPW, 3, "11111111", "01000010");
	INSERT_MATCH_ENTRY(table, RL78_IT_1_44_CMPW, 3, "11111111", "01000100");
	INSERT_MATCH_ENTRY(table, RL78_IT_1_46_CMPW, 2, "11111111", "01000110");
	INSERT_MATCH_ENTRY(table, RL78_IT_1_4X_CMPW, 1, "11111001", "01000001");
	INSERT_MATCH_ENTRY(table, RL78_IT_1_48_MOV, 3, "11111111", "01001000");
	INSERT_MATCH_ENTRY(table, RL78_IT_1_49_MOV, 3, "11111111", "01001001");
	INSERT_MATCH_ENTRY(table, RL78_IT_1_4A_CMP, 3, "11111111", "01001010");
	INSERT_MATCH_ENTRY(table, RL78_IT_1_4B_CMP, 2, "11111111", "01001011");
	INSERT_MATCH_ENTRY(table, RL78_IT_1_4C_CMP, 2, "11111111", "01001100");
	INSERT_MATCH_ENTRY(table, RL78_IT_1_4D_CMP, 1, "11111111", "01001101");
	INSERT_MATCH_ENTRY(table, RL78_IT_1_4E_CMP, 2, "11111111", "01001110");
	INSERT_MATCH_ENTRY(table, RL78_IT_1_4F_CMP, 3, "11111111", "01001111");
	INSERT_MATCH_ENTRY(table, RL78_IT_1_5X_MOV, 2, "11111000", "01010000");
	INSERT_MATCH_ENTRY(table, RL78_IT_1_58_MOVW, 3, "11111111", "01011000");
	INSERT_MATCH_ENTRY(table, RL78_IT_1_59_MOVW, 3, "11111111", "01011001");
	INSERT_MATCH_ENTRY(table, RL78_IT_1_5A_AND, 3, "11111111", "01011010");
	INSERT_MATCH_ENTRY(table, RL78_IT_1_5B_AND, 2, "11111111", "01011011");
	INSERT_MATCH_ENTRY(table, RL78_IT_1_5C_AND, 2, "11111111", "01011100");
	INSERT_MATCH_ENTRY(table, RL78_IT_1_5D_AND, 1, "11111111", "01011101");
	INSERT_MATCH_ENTRY(table, RL78_IT_1_5E_AND, 2, "11111111", "01011110");
	INSERT_MATCH_ENTRY(table, RL78_IT_1_5F_AND, 3, "11111111", "01011111");
	INSERT_MATCH_ENTRY(table, RL78_IT_1_61_2ndMap, 1, "11111111", "01100001"); ////******
	INSERT_MATCH_ENTRY(table, RL78_IT_1_6X_MOV, 1, "11111000", "01100000");
	INSERT_MATCH_ENTRY(table, RL78_IT_1_68_MOVW, 3, "11111111", "01101000");
	INSERT_MATCH_ENTRY(table, RL78_IT_1_69_MOVW, 3, "11111111", "01101001");
	INSERT_MATCH_ENTRY(table, RL78_IT_1_6A_OR, 3, "11111111", "01101010");
	INSERT_MATCH_ENTRY(table, RL78_IT_1_6B_OR, 2, "11111111", "01101011");
	INSERT_MATCH_ENTRY(table, RL78_IT_1_6C_OR, 2, "11111111", "01101100");
	INSERT_MATCH_ENTRY(table, RL78_IT_1_6D_OR, 1, "11111111", "01101101");
	INSERT_MATCH_ENTRY(table, RL78_IT_1_6E_OR, 2, "11111111", "01101110");
	INSERT_MATCH_ENTRY(table, RL78_IT_1_6F_OR, 3, "11111111", "01101111");
	INSERT_MATCH_ENTRY(table, RL78_IT_1_7X_MOV, 1, "11111000", "01110000");
	INSERT_MATCH_ENTRY(table, RL78_IT_1_71_3rdMap, 1, "11111111", "01110001"); /////*****
	INSERT_MATCH_ENTRY(table, RL78_IT_1_78_MOVW, 3, "11111111", "01111000");
	INSERT_MATCH_ENTRY(table, RL78_IT_1_79_MOVW, 3, "11111111", "01111001");
	INSERT_MATCH_ENTRY(table, RL78_IT_1_7A_XOR, 3, "11111111", "01111010");
	INSERT_MATCH_ENTRY(table, RL78_IT_1_7B_XOR, 2, "11111111", "01111011");
	INSERT_MATCH_ENTRY(table, RL78_IT_1_7C_XOR, 2, "11111111", "01111100");
	INSERT_MATCH_ENTRY(table, RL78_IT_1_7D_XOR, 1, "11111111", "01111101");
	INSERT_MATCH_ENTRY(table, RL78_IT_1_7E_XOR, 2, "11111111", "01111110");
	INSERT_MATCH_ENTRY(table, RL78_IT_1_7F_XOR, 3, "11111111", "01111111");
	INSERT_MATCH_ENTRY(table, RL78_IT_1_8X_INC, 1, "11111000", "10000000"); /////1 byte
	INSERT_MATCH_ENTRY(table, RL78_IT_1_88_MOV, 2, "11111111", "10001000");
	INSERT_MATCH_ENTRY(table, RL78_IT_1_89_MOV, 1, "11111111", "10001001");
	INSERT_MATCH_ENTRY(table, RL78_IT_1_8A_MOV, 2, "11111111", "10001010");
	INSERT_MATCH_ENTRY(table, RL78_IT_1_8B_MOV, 1, "11111111", "10001011");
	INSERT_MATCH_ENTRY(table, RL78_IT_1_8C_MOV, 2, "11111111", "10001100");
	INSERT_MATCH_ENTRY(table, RL78_IT_1_8D_MOV, 2, "11111111", "10001101");
	INSERT_MATCH_ENTRY(table, RL78_IT_1_8E_MOV, 2, "11111111", "10001110");
	INSERT_MATCH_ENTRY(table, RL78_IT_1_8F_MOV, 3, "11111111", "10001111");
	INSERT_MATCH_ENTRY(table, RL78_IT_1_9X_DEC, 1, "11111000", "10010000");
	INSERT_MATCH_ENTRY(table, RL78_IT_1_98_MOV, 2, "11111111", "10011000");
	INSERT_MATCH_ENTRY(table, RL78_IT_1_99_MOV, 1, "11111111", "10011001");
	INSERT_MATCH_ENTRY(table, RL78_IT_1_9A_MOV, 2, "11111111", "10011010");
	INSERT_MATCH_ENTRY(table, RL78_IT_1_9B_MOV, 1, "11111111", "10011011");
	INSERT_MATCH_ENTRY(table, RL78_IT_1_9C_MOV, 2, "11111111", "10011100");
	INSERT_MATCH_ENTRY(table, RL78_IT_1_9D_MOV, 2, "11111111", "10011101");
	INSERT_MATCH_ENTRY(table, RL78_IT_1_9E_MOV, 2, "11111111", "10011110");
	INSERT_MATCH_ENTRY(table, RL78_IT_1_9F_MOV, 3, "11111111", "10011111");
	INSERT_MATCH_ENTRY(table, RL78_IT_1_A0_INC, 3, "11111111", "10100000");
	INSERT_MATCH_ENTRY(table, RL78_IT_1_A2_INCW, 3, "11111111", "10100010");
	INSERT_MATCH_ENTRY(table, RL78_IT_1_A4_INC, 2, "11111111", "10100100");
	INSERT_MATCH_ENTRY(table, RL78_IT_1_A6_INCW, 2, "11111111", "10100110");
	INSERT_MATCH_ENTRY(table, RL78_IT_1_AX_INCW, 1, "11111001", "10100001");
	INSERT_MATCH_ENTRY(table, RL78_IT_1_A8_MOVW, 2, "11111111", "10101000");
	INSERT_MATCH_ENTRY(table, RL78_IT_1_A9_MOVW, 1, "11111111", "10101001");
	INSERT_MATCH_ENTRY(table, RL78_IT_1_AA_MOVW, 2, "11111111", "10101010");
	INSERT_MATCH_ENTRY(table, RL78_IT_1_AB_MOVW, 1, "11111111", "10101011");
	INSERT_MATCH_ENTRY(table, RL78_IT_1_AC_MOVW, 2, "11111111", "10101100");
	INSERT_MATCH_ENTRY(table, RL78_IT_1_AD_MOVW, 2, "11111111", "10101101");
	INSERT_MATCH_ENTRY(table, RL78_IT_1_AE_MOVW, 2, "11111111", "10101110");
	INSERT_MATCH_ENTRY(table, RL78_IT_1_AF_MOVW, 3, "11111111", "10101111");
	INSERT_MATCH_ENTRY(table, RL78_IT_1_B0_DEC, 3, "11111111", "10110000");
	INSERT_MATCH_ENTRY(table, RL78_IT_1_B2_DECW, 3, "11111111", "10110001");
	INSERT_MATCH_ENTRY(table, RL78_IT_1_B4_DEC, 2, "11111111", "10110010");
	INSERT_MATCH_ENTRY(table, RL78_IT_1_B6_DECW, 2, "11111111", "10110011");
	INSERT_MATCH_ENTRY(table, RL78_IT_1_BX_DECW, 1, "11111001", "10110001");
	INSERT_MATCH_ENTRY(table, RL78_IT_1_B8_MOVW, 2, "11111111", "10111000");
	INSERT_MATCH_ENTRY(table, RL78_IT_1_B9_MOVW, 1, "11111111", "10111001");
	INSERT_MATCH_ENTRY(table, RL78_IT_1_BA_MOVW, 2, "11111111", "10111010");
	INSERT_MATCH_ENTRY(table, RL78_IT_1_BB_MOVW, 1, "11111111", "10111011");
	INSERT_MATCH_ENTRY(table, RL78_IT_1_BC_MOVW, 2, "11111111", "10111100");
	INSERT_MATCH_ENTRY(table, RL78_IT_1_BD_MOVW, 2, "11111111", "10111101");
	INSERT_MATCH_ENTRY(table, RL78_IT_1_BE_MOVW, 2, "11111111", "10111110");
	INSERT_MATCH_ENTRY(table, RL78_IT_1_BF_MOVW, 3, "11111111", "10111111");
	INSERT_MATCH_ENTRY(table, RL78_IT_1_CX_POP, 1, "11111001", "11000000");
	INSERT_MATCH_ENTRY(table, RL78_IT_1_CY_PUSH, 1, "11111001", "11000001");
	INSERT_MATCH_ENTRY(table, RL78_IT_1_C8_MOV, 3, "11111111", "11001000");
	INSERT_MATCH_ENTRY(table, RL78_IT_1_C9_MOVW, 1, "11111111", "11001001");
	INSERT_MATCH_ENTRY(table, RL78_IT_1_CA_MOV, 3, "11111111", "11001010");
	INSERT_MATCH_ENTRY(table, RL78_IT_1_CB_MOVW, 1, "11111111", "11001011");
	INSERT_MATCH_ENTRY(table, RL78_IT_1_CC_MOV, 3, "11111111", "11001100");
	INSERT_MATCH_ENTRY(table, RL78_IT_1_CD_MOV, 3, "11111111", "11001101");
	INSERT_MATCH_ENTRY(table, RL78_IT_1_CE_MOV, 3, "11111111", "11001110");
	INSERT_MATCH_ENTRY(table, RL78_IT_1_CF_MOV, 4, "11111111", "11001111");
	INSERT_MATCH_ENTRY(table, RL78_IT_1_DX_CMP0, 1, "11111100", "11010000");
	INSERT_MATCH_ENTRY(table, RL78_IT_1_D4_CMP0, 2, "11111111", "11010100");
	INSERT_MATCH_ENTRY(table, RL78_IT_1_D5_CMP0, 3, "11111111", "11010101");
	INSERT_MATCH_ENTRY(table, RL78_IT_1_D6_MULU, 1, "11111111", "11010110");
	INSERT_MATCH_ENTRY(table, RL78_IT_1_D7_RET, 1, "11111111", "11010111");
	INSERT_MATCH_ENTRY(table, RL78_IT_1_D8_MOV, 2, "11111111", "11011000");
	INSERT_MATCH_ENTRY(table, RL78_IT_1_D9_MOV, 3, "11111111", "11011001");
	INSERT_MATCH_ENTRY(table, RL78_IT_1_DA_MOVW, 2, "11111111", "11011010");
	INSERT_MATCH_ENTRY(table, RL78_IT_1_DB_MOVW, 3, "11111111", "11011011");
	INSERT_MATCH_ENTRY(table, RL78_IT_1_DC_BC, 2, "11111111", "11011100");
	INSERT_MATCH_ENTRY(table, RL78_IT_1_DD_BZ, 2, "11111111", "11011101");
	INSERT_MATCH_ENTRY(table, RL78_IT_1_DE_BNC, 2, "11111111", "11011110");
	INSERT_MATCH_ENTRY(table, RL78_IT_1_DF_BNZ, 2, "11111111", "11011111");
	INSERT_MATCH_ENTRY(table, RL78_IT_1_EX_ONEB, 1, "11111100", "11100000");
	INSERT_MATCH_ENTRY(table, RL78_IT_1_E4_ONEB, 2, "11111111", "11100100");
	INSERT_MATCH_ENTRY(table, RL78_IT_1_E5_ONEB, 3, "11111111", "11100101");
	INSERT_MATCH_ENTRY(table, RL78_IT_1_E6_ONEW, 1, "11111111", "11100110");
	INSERT_MATCH_ENTRY(table, RL78_IT_1_E7_ONEW, 1, "11111111", "11100111");
	INSERT_MATCH_ENTRY(table, RL78_IT_1_E8_MOV, 2, "11111111", "11101000");
	INSERT_MATCH_ENTRY(table, RL78_IT_1_E9_MOV, 3, "11111111", "11101001");
	INSERT_MATCH_ENTRY(table, RL78_IT_1_EA_MOVW, 2, "11111111", "11101010");
	INSERT_MATCH_ENTRY(table, RL78_IT_1_EB_MOVW, 3, "11111111", "11101011");
	INSERT_MATCH_ENTRY(table, RL78_IT_1_EC_BR, 4, "11111111", "11101100");
	INSERT_MATCH_ENTRY(table, RL78_IT_1_ED_BR, 3, "11111111", "11100101");
	INSERT_MATCH_ENTRY(table, RL78_IT_1_EE_BR, 3, "11111111", "11100110");
	INSERT_MATCH_ENTRY(table, RL78_IT_1_EF_BR, 2, "11111111", "11100111");
	INSERT_MATCH_ENTRY(table, RL78_IT_1_FX_CLRB, 1, "11111100", "11110000");
	INSERT_MATCH_ENTRY(table, RL78_IT_1_F4_CLRB, 2, "11111111", "11110100");
	INSERT_MATCH_ENTRY(table, RL78_IT_1_F5_CLRB, 3, "11111111", "11110101");
	INSERT_MATCH_ENTRY(table, RL78_IT_1_F6_CLRW, 1, "11111111", "11110110");
	INSERT_MATCH_ENTRY(table, RL78_IT_1_F7_CLRW, 1, "11111111", "11110111");
	INSERT_MATCH_ENTRY(table, RL78_IT_1_F8_MOV, 2, "11111111", "11111000");
	INSERT_MATCH_ENTRY(table, RL78_IT_1_F9_MOV, 3, "11111111", "11111001");
	INSERT_MATCH_ENTRY(table, RL78_IT_1_FA_MOVW, 2, "11111111", "11111010");
	INSERT_MATCH_ENTRY(table, RL78_IT_1_FB_MOVW, 3, "11111111", "11111011");
	INSERT_MATCH_ENTRY(table, RL78_IT_1_FC_CALL, 4, "11111111", "11111100");
	INSERT_MATCH_ENTRY(table, RL78_IT_1_FD_CALL, 3, "11111111", "11111101");
	INSERT_MATCH_ENTRY(table, RL78_IT_1_FE_CALL, 3, "11111111", "11111110");
}


inline void  createDecodeTable_2ndmap(TABLE table) {
	INSERT_MATCH_ENTRY(table, RL78_IT_2_0X_ADD, 2, "11110000", "00000000"); ////11111000
	INSERT_MATCH_ENTRY(table, RL78_IT_2_09_ADDW, 3, "11111111", "00001001");
	//INSERT_MATCH_ENTRY(table, RL78_IT_2_0Y_ADD, 2, 0, AX, "11111000", "00001000");
	//cout << "second map created" << endl;
}
