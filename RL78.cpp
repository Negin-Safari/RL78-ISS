#pragma once
#include "issRL78.h"
#include <string>

DECODE_ENTRY Instr_info;
DECODE_ENTRY Instr_info2;
int Reg;

//--------------------------------------------------------------------------------------------
void issRL78::Decode_Dispatch() {

	PCregister = VAL_INIT;
	sc_lv <8> BYTE[4];
	int step = 0;

	while (true) {
		TABLE DECODE_TABLE_MAP1;
		TABLE DECODE_TABLE_MAP2;
		//TABLE DECODE_TABLE_MAP3;
		//TABLE DECODE_TABLE_MAP4;

		writeMem = SC_LOGIC_0;
		BYTE[0] = Fetch1Byte();
		step++;
		cout << "                                             ### PC = " << (PCregister.to_uint() - 1) << " ###" << endl;
		createDecodeTable_1stmap(DECODE_TABLE_MAP1);
		createDecodeTable_2ndmap(DECODE_TABLE_MAP2);
		createMorphTable();
		
		Instr_info = DecodeINST(DECODE_TABLE_MAP1, 0, BYTE[0]);
		type = Instr_info.Inst_CAT;
		Length = Instr_info.Inst_Length;
		int iii;
	//	cout << "////////////////////////////////////////////////////////" << endl;
		if (type == RL78_IT_1_61_2ndMap) {
			BYTE[1] = Fetch1Byte();
			step++;
			Instr_info2 = DecodeINST(DECODE_TABLE_MAP2, 191, BYTE[1]);
			type = Instr_info2.Inst_CAT;
			Length = Instr_info2.Inst_Length;
		}

		if (type != RL78_IT_LAST) {

			for (iii = step; iii < Length; iii++) {
				BYTE[iii] = Fetch1Byte();
			}
		}
		ExecuteInstruction(type, BYTE[0], BYTE[1], BYTE[2], BYTE[3]);
		step = 0;
		//getchar();
	}
}

string regName(int RegS) {
	string n;
	if(RegS == 0)  n = "X";
	else if(RegS == 1) n = "A";
	else if(RegS == 2) n = "C";
	else if (RegS == 3) n = "B";
	else if (RegS == 4) n = "E";
	else if (RegS == 5) n = "D";
	else if (RegS == 6) n = "L";
	else if (RegS == 7) n = "H";

	//cout << "regs-> " << RegS << " case: " << n << endl;
	return n;
}

inline void issRL78::ExecuteInstruction(rl78InstructionType type, sc_lv <8>& Byte1, sc_lv <8>& Byte2, sc_lv <8>& Byte3, sc_lv <8>& Byte4) {

	(this->*MorphFunctions[type])(Byte1, Byte2, Byte3, Byte4);
}

inline sc_lv<20> issRL78::PrefixAddr() {
	sc_lv<8> data_8b;
	//cout << "prefix en: " << PrefixEn << endl;
	if (PrefixEn) {
		PrefixEn = false;
		Reg = ES;
		data_8b = Read1Byte(R_MMAP(Reg));
		return ((data_8b.range(3, 0).to_uint()) * 65536);
	}
	return 0xF0000;
}

inline sc_lv<8> issRL78::Fetch1Byte() {
	sc_lv<8> data;

	while (memReady != '1') {
		readMem = SC_LOGIC_1;
		addrBus = PCregister;
		wait();
	}
	//cout << "                                             ### PC = " << PCregister << " ###" << endl;
	readMem = SC_LOGIC_0;
	data = dataBus->read();
	wait();
	PCregister = PCregister.to_uint() + 1;
	return data;
}

inline sc_lv<8> issRL78::Read1Byte(sc_lv<20> addr) {
	sc_lv<8> data;

	while (memReady != '1') {
		readMem = SC_LOGIC_1;
		addrBus = addr;
		wait();
	}
	readMem = SC_LOGIC_0;
	data = dataBus->read();
	wait();
	return data;
}

inline void issRL78::Write1Byte(sc_lv<8> data2write, sc_lv<20> addr) {

	while (memReady != '1')
	{

		writeMem = SC_LOGIC_1;
		addrBus = addr;
		dataBus = data2write;
		wait();
	}
	writeMem = SC_LOGIC_0;
	dataBus.write("Z");
	wait();


}


///------------------------------------------------------------------------------------------


void issRL78::MORPH_1_0X_ADDW(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4) {
	//cout << "					*** instruction 1_0X_ADDW is executing ***" << endl;
	cout << "------------------------------------------------------------------------------------------------------------------------" << endl;

	int RegS = Byte1.range(2, 0).to_uint();
	cout << " register is" << RegS << endl;
	ReadReg_16(RegS - 1, RegS, dataB_16b); ReadReg_16(X, A, dataA_16b);
	DO_Addition(dataA_16b, dataB_16b, result_16b);
	UpdateFlags_16b_add(dataA_16b, dataB_16b, result_16b); WriteReg_16(X, A, result_16b);
	log << "-> ADDW AX," << regName(RegS - 1) <<""<< regName(RegS) << "\n";
}



void issRL78::MORPH_1_02_ADDW(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4) {
	//cout << "					*** instruction 1_02_ADDW is executing ***" << endl;
	//cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
	Immediate2Byte(addrHL); Readdata_16(addrHL, dataB_16b); ReadReg_16(X, A, dataA_16b);
	DO_Addition(dataA_16b, dataB_16b, result_16b);
	UpdateFlags_16b_add(dataA_16b, dataB_16b, result_16b); WriteReg_16(X, A, result_16b);
}

void issRL78::MORPH_1_04_ADDW(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4) {
	cout << "					*** instruction 1_04_ADDW is executing ***" << endl;
	cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
	Immediate2Byte(dataB_16b); ReadReg_16(X, A, dataA_16b);
	DO_Addition(dataA_16b, dataB_16b, result_16b);
	UpdateFlags_16b_add(dataA_16b, dataB_16b, result_16b); WriteReg_16(X, A, result_16b);
}

void issRL78::MORPH_1_06_ADDW(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4) {
	cout << "					*** instruction 1_06_ADDW is executing ***" << endl;
	cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
	ReaddataOffset_16(saddr, Byte2, dataB_16b); ReadReg_16(X, A, dataA_16b);
	DO_Addition(dataA_16b, dataB_16b, result_16b);
	UpdateFlags_16b_add(dataA_16b, dataB_16b, result_16b); WriteReg_16(X, A, result_16b);
}

void issRL78::MORPH_1_08_XCH(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4) {
	cout << "					*** instruction 1_08_ADDW is executing ***" << endl;
	cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
	ReadReg_8(A, dataB_8b); ReadReg_8(X, dataA_8b);
	WriteReg_8(A, dataA_8b); WriteReg_8(X, dataB_8b);
}

void issRL78::MORPH_1_09_MOV(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4) {
	cout << "					*** instruction 1_09_MOW is executing ***" << endl;
	cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
	Immediate2Byte(addrHL); ReaddataOffsetReg_8(B, addrHL, dataB_8b);
	WriteReg_8(A, dataB_8b);
}

void issRL78::MORPH_1_0A_ADD(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4) {
	cout << "					*** instruction 1_0A_ADD is executing ***" << endl;
	cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
	ReaddataOffset_8(saddr, Byte2, dataA_8b);
	DO_Addition(dataA_8b, Byte3, result_8b);
	UpdateFlags_8b_add(dataA_8b, Byte3, result_8b); WritedataOffset_8(saddr, Byte2, result_8b);
	log << "-> ADD saddr, " << Byte3 << "\n";
}

void issRL78::MORPH_1_0B_ADD(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4) {
	cout << "					*** instruction 1_0A_ADD is executing ***" << endl;
	cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
	ReaddataOffset_8(saddr, Byte2, dataB_8b); ReadReg_8(A, dataA_8b);
	DO_Addition(dataA_8b, dataB_8b, result_8b);
	UpdateFlags_8b_add(dataA_8b, dataB_8b, result_8b); WriteReg_8(A, result_8b);
}

void issRL78::MORPH_1_00_NOP(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4) {
	cout << "					*** instruction 1_00_NOP is executing ***" << endl;
	cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
	wait();
}

void issRL78::MORPH_1_0C_ADD(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4) {
	cout << "					*** instruction 1_0C_ADD is executing ***" << endl;
	cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
	ReadReg_8(A, dataA_8b); DO_Addition(dataA_8b, Byte2, result_8b);
	UpdateFlags_8b_add(dataA_8b, Byte2, result_8b); WriteReg_8(A, result_8b);
}

void issRL78::MORPH_1_0D_ADD(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4) {
	cout << "					*** instruction 1_0D_ADD is executing ***" << endl;
	cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
	ReadReg_16(L, H, dataB_16b); Readdata_8(dataB_16b, dataB_8b); ReadReg_8(A, dataA_8b);
	DO_Addition(dataA_8b, dataB_8b, result_8b);
	UpdateFlags_8b_add(dataA_8b, dataB_8b, result_8b); WriteReg_8(A, result_8b);
}

void issRL78::MORPH_1_0E_ADD(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4) {
	cout << "					*** instruction 1_0E_ADD is executing ***" << endl;
	cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
	ReadReg_16(L, H, dataB_16b); ReaddataOffsetAddr_8(dataB_16b, Byte2, dataB_8b); ReadReg_8(A, dataA_8b);
	DO_Addition(dataA_8b, dataB_8b, result_8b);
	UpdateFlags_8b_add(dataA_8b, dataB_8b, result_8b); WriteReg_8(A, result_8b);
}

void issRL78::MORPH_1_0F_ADD(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4) {
	cout << "					*** instruction 1_0F_ADD is executing ***" << endl;
	cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
	Immediate2Byte(addrHL); Readdata_8(addrHL, dataB_8b); ReadReg_8(A, dataA_8b);
	DO_Addition(dataA_8b, dataB_8b, result_8b);
	UpdateFlags_8b_add(dataA_8b, dataB_8b, result_8b); WriteReg_8(A, result_8b);
}

void issRL78::MORPH_1_10_ADDW(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4) {
	cout << "					*** instruction 1_10_ADDW is executing ***" << endl;
	cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
	ReadSP(dataA_16b); DO_Addition(dataA_16b, Byte2, result_16b); UpdateSP(result_16b);
}

void issRL78::MORPH_1_11_PREFIX(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4) {
	cout << "					*** instruction 1_11_PREFIX is executing ***" << endl;
	cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
	PrefixEn = true;
}

void issRL78::MORPH_1_1X_MOVW(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4) {
	cout << "					*** instruction 1_1X_MOVW is executing ***" << endl;
	cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
	int RegS = Byte1.range(2, 0).to_uint();
	ReadReg_16(X, A, dataA_16b); WriteReg_16(RegS, RegS + 1, dataA_16b);
	log << "-> MOVW " << regName(RegS - 1) << "" << regName(RegS) <<" ,AX"<< "\n";
}

void issRL78::MORPH_1_1Y_MOVW(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4) {
	cout << "					*** instruction 1_1Y_MOVW is executing ***" << endl;
	cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
	int RegS = Byte1.range(2, 0).to_uint();
	ReadReg_16(RegS - 1, RegS, dataA_16b); WriteReg_16(X, A, dataA_16b);
	log << "-> MOVW AX, " << regName(RegS - 1) << "" << regName(RegS) << "\n";
}

void issRL78::MORPH_1_18_MOV(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4) {
	cout << "					*** instruction 1_18_MOV is executing ***" << endl;
	cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
	ReadReg_8(A, dataB_8b);
	Immediate2Byte(addrHL); WritedataOffsetReg_8(B, addrHL, dataB_8b);
}

void issRL78::MORPH_1_19_MOV(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4) {
	cout << "					*** instruction 1_19_MOV is executing ***" << endl;
	cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
	Immediate2Byte(addrHL); WritedataOffsetReg_8(B, addrHL, Byte4);
}

void issRL78::MORPH_1_1A_ADDC(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4) {
	cout << "					*** instruction 1_1A_ADDC is executing ***" << endl;
	cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
	ReaddataOffset_8(saddr, Byte2, dataA_8b);
	DO_AdditionC(dataA_8b, Byte3, result_8b);
	UpdateFlags_8b_add(dataA_8b, Byte3, result_8b); WritedataOffset_8(saddr, Byte2, result_8b);
}

void issRL78::MORPH_1_1B_ADDC(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4) {
	cout << "					*** instruction 1_1B_ADDC is executing ***" << endl;
	cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
	ReaddataOffset_8(saddr, Byte2, dataB_8b); ReadReg_8(A, dataA_8b);
	DO_AdditionC(dataA_8b, dataB_8b, result_8b);
	UpdateFlags_8b_add(dataA_8b, dataB_8b, result_8b); WriteReg_8(A, result_8b);
}

void issRL78::MORPH_1_1C_ADDC(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4) {
	cout << "					*** instruction 1_1C_ADDC is executing ***" << endl;
	cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
	ReadReg_8(A, dataA_8b); DO_AdditionC(dataA_8b, Byte2, result_8b);
	UpdateFlags_8b_add(dataA_8b, Byte2, result_8b); WriteReg_8(A, result_8b);
}

void issRL78::MORPH_1_1D_ADDC(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4) {
	cout << "					*** instruction 1_1D_ADDC is executing ***" << endl;
	cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
	ReadReg_16(L, H, dataB_16b); Readdata_8(dataB_16b, dataB_8b); ReadReg_8(A, dataA_8b);
	DO_AdditionC(dataA_8b, dataB_8b, result_8b);
	UpdateFlags_8b_add(dataA_8b, dataB_8b, result_8b); WriteReg_8(A, result_8b);
}

void issRL78::MORPH_1_1E_ADDC(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4) {
	cout << "					*** instruction 1_1E_ADDC is executing ***" << endl;
	cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
	ReadReg_16(L, H, dataB_16b); ReaddataOffsetAddr_8(dataB_16b, Byte2, dataB_8b); ReadReg_8(A, dataA_8b);
	DO_AdditionC(dataA_8b, dataB_8b, result_8b);
	UpdateFlags_8b_add(dataA_8b, dataB_8b, result_8b); WriteReg_8(A, result_8b);
}

void issRL78::MORPH_1_1F_ADDC(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4) {
	cout << "					*** instruction 1_1F_ADDC is executing ***" << endl;
	cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
	Immediate2Byte(addrHL); Readdata_8(addrHL, dataB_8b); ReadReg_8(A, dataA_8b);
	DO_AdditionC(dataA_8b, dataB_8b, result_8b);
	UpdateFlags_8b_add(dataA_8b, dataB_8b, result_8b); WriteReg_8(A, result_8b);
}

void issRL78::MORPH_1_20_SUBW(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4) {
	cout << "					*** instruction 1_20_SUBW is executing ***" << endl;
	cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
	ReadSP(dataA_16b); DO_Subtraction(dataA_16b, Byte2, result_16b); UpdateSP(result_16b);
}

void issRL78::MORPH_1_22_SUBW(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4) {
	cout << "					*** instruction 1_22_SUBW is executing ***" << endl;
	cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
	Immediate2Byte(addrHL); Readdata_16(addrHL, dataB_16b); ReadReg_16(X, A, dataA_16b);
	DO_Subtraction(dataA_16b, dataB_16b, result_16b);
	UpdateFlags_16b_sub(dataA_16b, dataB_16b, result_16b); WriteReg_16(X, A, result_16b);
}

void issRL78::MORPH_1_2X_SUBW(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4) {
	cout << "					*** instruction 1_23_SUBW is executing ***" << endl;
	cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
	int RegS = Byte1.range(2, 0).to_uint();
	ReadReg_16(RegS - 1, RegS, dataB_16b); ReadReg_16(X, A, dataA_16b);
	DO_Subtraction(dataA_16b, dataB_16b, result_16b);
	UpdateFlags_16b_sub(dataA_16b, dataB_16b, result_16b); WriteReg_16(X, A, result_16b);
	log << "-> SUBW AX, " << regName(RegS - 1) << "" << regName(RegS) << "\n";
}

void issRL78::MORPH_1_24_SUBW(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4) {
	cout << "					*** instruction 1_24_SUBW is executing ***" << endl;
	cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
	Immediate2Byte(dataB_16b); ReadReg_16(X, A, dataA_16b);
	DO_Subtraction(dataA_16b, dataB_16b, result_16b);
	UpdateFlags_16b_sub(dataA_16b, dataB_16b, result_16b); WriteReg_16(X, A, result_16b);
}

void issRL78::MORPH_1_26_SUBW(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4) {
	cout << "					*** instruction 1_26_SUBW is executing ***" << endl;
	cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
	ReaddataOffset_16(saddr, Byte2, dataB_16b); ReadReg_16(X, A, dataA_16b);
	DO_Subtraction(dataA_16b, dataB_16b, result_16b);
	UpdateFlags_16b_sub(dataA_16b, dataB_16b, result_16b); WriteReg_16(X, A, result_16b);
}

void issRL78::MORPH_1_28_MOV(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4) {
	cout << "					*** instruction 1_28_MOV is executing ***" << endl;
	cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
	Immediate2Byte(addrHL); ReadReg_8(A, dataA_8b);
	WritedataOffsetReg_8(C, addrHL, dataA_8b);
}

void issRL78::MORPH_1_29_MOV(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4) {
	cout << "					*** instruction 1_29_MOV is executing ***" << endl;
	cout << "------------------------------------------------------------------------------------------------------------------------" << endl;

	Immediate2Byte(addrHL); ReaddataOffsetReg_8(C, addrHL, dataB_8b);
	WriteReg_8(A, dataB_8b);
}

void issRL78::MORPH_1_2A_SUB(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4) {
	cout << "					*** instruction 1_2A_SUB is executing ***" << endl;
	cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
	ReaddataOffset_8(saddr, Byte2, dataA_8b);
	DO_Subtraction(dataA_8b, Byte3, result_8b);
	UpdateFlags_8b_sub(dataA_8b, Byte3, result_8b); WritedataOffset_8(saddr, Byte2, result_8b);
}

void issRL78::MORPH_1_2B_SUB(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4) {
	cout << "					*** instruction 1_2B_SUB is executing ***" << endl;
	cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
	ReaddataOffset_8(saddr, Byte2, dataB_8b); ReadReg_8(A, dataA_8b);
	DO_Subtraction(dataA_8b, dataB_8b, result_8b);
	UpdateFlags_8b_sub(dataA_8b, dataB_8b, result_8b); WriteReg_8(A, result_8b);
}

void issRL78::MORPH_1_2C_SUB(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4) {
	cout << "					*** instruction 1_2C_SUB is executing ***" << endl;
	cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
	ReadReg_8(A, dataA_8b); DO_Subtraction(dataA_8b, Byte2, result_8b);
	UpdateFlags_8b_sub(dataA_8b, Byte2, result_8b); WriteReg_8(A, result_8b);
}

void issRL78::MORPH_1_2D_SUB(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4) {
	cout << "					*** instruction 1_2D_SUB is executing ***" << endl;
	cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
	ReadReg_16(L, H, dataB_16b); Readdata_8(dataB_16b, dataB_8b); ReadReg_8(A, dataA_8b);
	DO_Subtraction(dataA_8b, dataB_8b, result_8b);
	UpdateFlags_8b_sub(dataA_8b, dataB_8b, result_8b); WriteReg_8(A, result_8b);
}

void issRL78::MORPH_1_2E_SUB(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4) {
	cout << "					*** instruction 1_2E_SUB is executing ***" << endl;
	cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
	ReadReg_16(L, H, dataB_16b); ReaddataOffsetAddr_8(dataB_16b, Byte2, dataB_8b); ReadReg_8(A, dataA_8b);
	DO_Subtraction(dataA_8b, dataB_8b, result_8b);
	UpdateFlags_8b_sub(dataA_8b, dataB_8b, result_8b); WriteReg_8(A, result_8b);
}

void issRL78::MORPH_1_2F_SUB(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4) {
	cout << "					*** instruction 1_2F_SUB is executing ***" << endl;
	cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
	Immediate2Byte(addrHL); Readdata_8(addrHL, dataB_8b); ReadReg_8(A, dataA_8b);
	DO_Subtraction(dataA_8b, dataB_8b, result_8b);
	UpdateFlags_8b_sub(dataA_8b, dataB_8b, result_8b); WriteReg_8(A, result_8b);
}

void issRL78::MORPH_1_3X_MOVW(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4) {
	cout << "					*** instruction 1_30_MOVW is executing ***" << endl;
	cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
	int RegS = Byte1.range(2, 0).to_uint();
	Immediate2Byte(dataB_16b); WriteReg_16(RegS, RegS + 1, dataB_16b);
	log << "-> MOVW  " << regName(RegS - 1) << "" << regName(RegS)<<", "<< dataB_16b << "\n";
}

void issRL78::MORPH_1_3Y_XCHW(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4) {
	cout << "					*** instruction 1_33_XCHW is executing ***" << endl;
	cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
	int RegS = Byte1.range(2, 0).to_uint();
	ReadReg_16(X, A, dataA_16b); ReadReg_16(RegS - 1, RegS, dataB_16b);
	WriteReg_16(RegS - 1, RegS, dataA_16b); WriteReg_16(X, A, dataB_16b);
	log << "-> XCHW AX, " << regName(RegS - 1) << "" << regName(RegS) << "\n";
}

void issRL78::MORPH_1_38_MOV(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4) {
	cout << "					*** instruction 1_38_MOV is executing ***" << endl;
	cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
	Immediate2Byte(addrHL); WritedataOffsetReg_8(C, addrHL, Byte4);
}

void issRL78::MORPH_1_39_MOV(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4) {
	cout << "					*** instruction 1_39_MOV is executing ***" << endl;
	cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
	Immediate2Byte(addrHL); WritedataOffset2Reg_8(C, B, addrHL, Byte4);
}

void issRL78::MORPH_1_3A_SUBC(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4) {
	cout << "					*** instruction 1_3A_SUBC is executing ***" << endl;
	cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
	ReaddataOffset_8(saddr, Byte2, dataA_8b);
	DO_SubtractionC(dataA_8b, Byte3, result_8b);
	UpdateFlags_8b_sub(dataA_8b, Byte3, result_8b); WritedataOffset_8(saddr, Byte2, result_8b);
}

void issRL78::MORPH_1_3B_SUBC(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4) {
	cout << "					*** instruction 1_3B_SUBC is executing ***" << endl;
	cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
	ReaddataOffset_8(saddr, Byte2, dataB_8b); ReadReg_8(A, dataA_8b);
	DO_SubtractionC(dataA_8b, dataB_8b, result_8b);
	UpdateFlags_8b_sub(dataA_8b, dataB_8b, result_8b); WriteReg_8(A, result_8b);
}

void issRL78::MORPH_1_3C_SUBC(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4) {
	cout << "					*** instruction 1_3C_SUBC is executing ***" << endl;
	cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
	ReadReg_8(A, dataA_8b); DO_SubtractionC(dataA_8b, Byte2, result_8b);
	UpdateFlags_8b_sub(dataA_8b, Byte2, result_8b); WriteReg_8(A, result_8b);
}

void issRL78::MORPH_1_3D_SUBC(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4) {
	cout << "					*** instruction 1_3D_SUBC is executing ***" << endl;
	cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
	ReadReg_16(L, H, dataB_16b); Readdata_8(dataB_16b, dataB_8b); ReadReg_8(A, dataA_8b);
	DO_SubtractionC(dataA_8b, dataB_8b, result_8b);
	UpdateFlags_8b_sub(dataA_8b, dataB_8b, result_8b); WriteReg_8(A, result_8b);
}

void issRL78::MORPH_1_3E_SUBC(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4) {
	cout << "					*** instruction 1_3E_SUBC is executing ***" << endl;
	cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
	ReadReg_16(L, H, dataB_16b); ReaddataOffsetAddr_8(dataB_16b, Byte2, dataB_8b); ReadReg_8(A, dataA_8b);
	DO_SubtractionC(dataA_8b, dataB_8b, result_8b);
	UpdateFlags_8b_sub(dataA_8b, dataB_8b, result_8b); WriteReg_8(A, result_8b);
}

void issRL78::MORPH_1_3F_SUBC(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4) {
	cout << "					*** instruction 1_3F_SUBC is executing ***" << endl;
	cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
	Immediate2Byte(addrHL); Readdata_8(addrHL, dataB_8b); ReadReg_8(A, dataA_8b);
	DO_SubtractionC(dataA_8b, dataB_8b, result_8b);
	UpdateFlags_8b_sub(dataA_8b, dataB_8b, result_8b); WriteReg_8(A, result_8b);
}

void issRL78::MORPH_1_40_CMP(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4) {
	cout << "					*** instruction 1_40_CMP is executing ***" << endl;
	cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
	Immediate2Byte(addrHL); Readdata_8(addrHL, dataA_8b);
	DO_Subtraction(dataA_8b, Byte4, result_8b);
	UpdateFlags_8b_sub(dataA_8b, dataB_8b, result_8b);
}

void issRL78::MORPH_1_41_MOV(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4) {
	cout << "					*** instruction 1_41_MOV is executing ***" << endl;
	cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
	WriteReg_8(ES, Byte2);
}

void issRL78::MORPH_1_42_CMPW(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4) {
	cout << "					*** instruction 1_42_CMPW is executing ***" << endl;
	cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
	Immediate2Byte(addrHL); Readdata_16(addrHL, dataB_16b); ReadReg_16(X, A, dataA_16b);
	DO_Subtraction(dataA_16b, dataB_16b, result_16b);
	UpdateFlags_16b_sub(dataA_16b, dataB_16b, result_16b);
}

void issRL78::MORPH_1_4X_CMPW(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4) {
	cout << "					*** instruction 1_43_CMPW is executing ***" << endl;
	cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
	int RegS = Byte1.range(2, 0).to_uint();
	ReadReg_16(RegS - 1, RegS, dataB_16b); ReadReg_16(X, A, dataA_16b);
	DO_Subtraction(dataA_16b, dataB_16b, result_16b);
	UpdateFlags_16b_sub(dataA_16b, dataB_16b, result_16b);
	log << "-> CMPW AX, " << regName(RegS - 1) << "" << regName(RegS) << "\n";
}

void issRL78::MORPH_1_44_CMPW(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4) {
	cout << "					*** instruction 1_44_CMPW is executing ***" << endl;
	cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
	Immediate2Byte(dataB_16b); ReadReg_16(X, A, dataA_16b);
	DO_Subtraction(dataA_16b, dataB_16b, result_16b);
	UpdateFlags_16b_sub(dataA_16b, dataB_16b, result_16b);
}

void issRL78::MORPH_1_46_CMPW(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4) {
	cout << "					*** instruction 1_46_CMPW is executing ***" << endl;
	cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
	ReaddataOffset_16(saddr, Byte2, dataB_16b); ReadReg_16(X, A, dataA_16b);
	DO_Subtraction(dataA_16b, dataB_16b, result_16b);
	UpdateFlags_16b_sub(dataA_16b, dataB_16b, result_16b);
}

void issRL78::MORPH_1_48_MOV(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4) {
	cout << "					*** instruction 1_48_MOV is executing ***" << endl;
	cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
	Immediate2Byte(addrHL); ReadReg_8(A, dataA_8b); WritedataOffset2Reg_8(C, B, addrHL, dataA_8b);
}

void issRL78::MORPH_1_49_MOV(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4) {
	cout << "					*** instruction 1_49_MOV is executing ***" << endl;
	cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
	Immediate2Byte(addrHL); ReaddataOffset2Reg_8(C, B, addrHL, dataA_8b);
	WriteReg_8(A, dataA_8b);
}

void issRL78::MORPH_1_4A_CMP(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4) {
	cout << "					*** instruction 1_4A_CMP is executing ***" << endl;
	cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
	ReaddataOffset_8(saddr, Byte2, dataA_8b);
	DO_Subtraction(dataA_8b, Byte3, result_8b);
	UpdateFlags_8b_sub(dataA_8b, Byte3, result_8b);
}

void issRL78::MORPH_1_4B_CMP(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4) {
	cout << "					*** instruction 1_4B_CMP is executing ***" << endl;
	cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
	ReaddataOffset_8(saddr, Byte2, dataB_8b); ReadReg_8(A, dataA_8b);
	DO_Subtraction(dataA_8b, dataB_8b, result_8b);
	UpdateFlags_8b_sub(dataA_8b, dataB_8b, result_8b);
}

void issRL78::MORPH_1_4C_CMP(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4) {
	cout << "					*** instruction 1_4C_CMP is executing ***" << endl;
	cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
	ReadReg_8(A, dataA_8b); DO_Subtraction(dataA_8b, Byte2, result_8b);
	UpdateFlags_8b_sub(dataA_8b, Byte2, result_8b);
}

void issRL78::MORPH_1_4D_CMP(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4) {
	cout << "					*** instruction 1_4D_CMP is executing ***" << endl;
	cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
	ReadReg_16(L, H, dataB_16b); Readdata_8(dataB_16b, dataB_8b); ReadReg_8(A, dataA_8b);
	DO_Subtraction(dataA_8b, dataB_8b, result_8b);
	UpdateFlags_8b_sub(dataA_8b, dataB_8b, result_8b);
}

void issRL78::MORPH_1_4E_CMP(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4) {
	cout << "					*** instruction 1_4E_CMP is executing ***" << endl;
	cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
	ReadReg_16(L, H, dataB_16b); ReaddataOffsetAddr_8(dataB_16b, Byte2, dataB_8b); ReadReg_8(A, dataA_8b);
	DO_Subtraction(dataA_8b, dataB_8b, result_8b);
	UpdateFlags_8b_sub(dataA_8b, dataB_8b, result_8b);
}

void issRL78::MORPH_1_4F_CMP(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4) {
	cout << "					*** instruction 1_4F_CMP is executing ***" << endl;
	cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
	Immediate2Byte(addrHL); Readdata_8(addrHL, dataB_8b); ReadReg_8(A, dataA_8b);
	DO_Subtraction(dataA_8b, dataB_8b, result_8b);
	UpdateFlags_8b_sub(dataA_8b, dataB_8b, result_8b);
}

void issRL78::MORPH_1_5X_MOV(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4) {
	cout << "					*** instruction 1_5X_MOV is executing ***" << endl;
	cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
	int RegS = Byte1.range(2, 0).to_uint();
	//cout << "regsss: " << RegS << endl;
	WriteReg_8(RegS, Byte2);
	log << "-> MOV " << regName(RegS) <<", "<< Byte2 << "\n";
}

void issRL78::MORPH_1_58_MOVW(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4) {
	cout << "					*** instruction 1_58_MOVW is executing ***" << endl;
	cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
	Immediate2Byte(addrHL); ReadReg_16(X, A, dataA_16b); WritedataOffsetReg_16(B, addrHL, dataA_16b);
}

void issRL78::MORPH_1_59_MOVW(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4) {
	cout << "					*** instruction 1_59_MOVW is executing ***" << endl;
	cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
	Immediate2Byte(addrHL); ReaddataOffsetReg_16(B, addrHL, dataA_16b);
	WriteReg_16(X, A, dataA_16b);
}

void issRL78::MORPH_1_5A_AND(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4) {
	cout << "					*** instruction 1_sA_AND is executing ***" << endl;
	cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
	ReaddataOffset_8(saddr, Byte2, dataA_8b);
	DO_AND(dataA_8b, Byte3, result_8b);
	UpdateFlags_logic(result_8b); WritedataOffset_8(saddr, Byte2, result_8b);
}

void issRL78::MORPH_1_5B_AND(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4) {
	cout << "					*** instruction 1_5B_AND is executing ***" << endl;
	cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
	ReaddataOffset_8(saddr, Byte2, dataB_8b); ReadReg_8(A, dataA_8b);
	DO_AND(dataA_8b, dataB_8b, result_8b);
	UpdateFlags_logic(result_8b); WriteReg_8(A, result_8b);
}

void issRL78::MORPH_1_5C_AND(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4) {
	cout << "					*** instruction 1_5C_AND is executing ***" << endl;
	cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
	ReadReg_8(A, dataA_8b); DO_AND(dataA_8b, Byte2, result_8b);
	UpdateFlags_logic(result_8b); WriteReg_8(A, result_8b);
}

void issRL78::MORPH_1_5D_AND(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4) {
	cout << "					*** instruction 1_5D_AND is executing ***" << endl;
	cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
	ReadReg_16(L, H, dataB_16b); Readdata_8(dataB_16b, dataB_8b); ReadReg_8(A, dataA_8b);
	DO_AND(dataA_8b, dataB_8b, result_8b);
	UpdateFlags_logic(result_8b); WriteReg_8(A, result_8b);
}

void issRL78::MORPH_1_5E_AND(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4) {
	cout << "					*** instruction 1_5E_AND is executing ***" << endl;
	cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
	ReadReg_16(L, H, dataB_16b); ReaddataOffsetAddr_8(dataB_16b, Byte2, dataB_8b); ReadReg_8(A, dataA_8b);
	DO_AND(dataA_8b, dataB_8b, result_8b);
	UpdateFlags_logic(result_8b); WriteReg_8(A, result_8b);
}

void issRL78::MORPH_1_5F_AND(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4) {
	cout << "					*** instruction 1_5F_AND is executing ***" << endl;
	cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
	Immediate2Byte(addrHL); Readdata_8(addrHL, dataB_8b); ReadReg_8(A, dataA_8b);
	DO_AND(dataA_8b, dataB_8b, result_8b);
	UpdateFlags_logic(result_8b); WriteReg_8(A, result_8b);
}

void issRL78::MORPH_1_6X_MOV(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4) {
	cout << "					*** instruction 1_60_MOV is executing ***" << endl;
	cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
	int RegS = Byte1.range(2, 0).to_uint();
	ReadReg_8(RegS, dataA_8b); WriteReg_8(A, dataA_8b);
	log << "-> MOV  A, " << regName(RegS) << "\n";
}

void issRL78::MORPH_1_68_MOVW(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4) {
	cout << "					*** instruction 1_68_MOVW is executing ***" << endl;
	cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
	Immediate2Byte(addrHL); ReadReg_16(X, A, dataA_16b); WritedataOffsetReg_16(C, addrHL, dataA_16b);
}

void issRL78::MORPH_1_69_MOVW(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4) {
	cout << "					*** instruction 1_69_MOVW is executing ***" << endl;
	cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
	Immediate2Byte(addrHL); ReaddataOffsetReg_16(C, addrHL, dataA_16b);
	WriteReg_16(X, A, dataA_16b);
}

void issRL78::MORPH_1_6A_OR(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4) {
	cout << "					*** instruction 1_6A_OR is executing ***" << endl;
	cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
	ReaddataOffset_8(saddr, Byte2, dataA_8b);
	DO_OR(dataA_8b, Byte3, result_8b);
	UpdateFlags_logic(result_8b);  WritedataOffset_8(saddr, Byte2, result_8b);
}

void issRL78::MORPH_1_6B_OR(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4) {
	cout << "					*** instruction 1_6B_OR is executing ***" << endl;
	cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
	ReaddataOffset_8(saddr, Byte2, dataB_8b); ReadReg_8(A, dataA_8b);
	DO_OR(dataA_8b, dataB_8b, result_8b);
	UpdateFlags_logic(result_8b); WriteReg_8(A, result_8b);
}

void issRL78::MORPH_1_6C_OR(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4) {
	cout << "					*** instruction 1_6C_OR is executing ***" << endl;
	cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
	ReadReg_8(A, dataA_8b); DO_OR(dataA_8b, Byte2, result_8b);
	UpdateFlags_logic(result_8b); WriteReg_8(A, result_8b);
}

void issRL78::MORPH_1_6D_OR(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4) {
	cout << "					*** instruction 1_6D_OR is executing ***" << endl;
	cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
	ReadReg_16(L, H, dataB_16b); Readdata_8(dataB_16b, dataB_8b); ReadReg_8(A, dataA_8b);
	DO_OR(dataA_8b, dataB_8b, result_8b);
	UpdateFlags_logic(result_8b); WriteReg_8(A, result_8b);
}

void issRL78::MORPH_1_6E_OR(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4) {
	cout << "					*** instruction 1_6E_OR is executing ***" << endl;
	cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
	ReadReg_16(L, H, dataB_16b); ReaddataOffsetAddr_8(dataB_16b, Byte2, dataB_8b); ReadReg_8(A, dataA_8b);
	DO_OR(dataA_8b, dataB_8b, result_8b);
	UpdateFlags_logic(result_8b); WriteReg_8(A, result_8b);
}

void issRL78::MORPH_1_6F_OR(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4) {
	cout << "					*** instruction 1_6F_OR is executing ***" << endl;
	cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
	Immediate2Byte(addrHL); Readdata_8(addrHL, dataB_8b); ReadReg_8(A, dataA_8b);
	DO_OR(dataA_8b, dataB_8b, result_8b);
	UpdateFlags_logic(result_8b); WriteReg_8(A, result_8b);
}

void issRL78::MORPH_1_7X_MOV(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4) {
	cout << "					*** instruction 1_7X_MOV is executing ***" << endl;
	cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
	int RegS = Byte1.range(2, 0).to_uint();
	ReadReg_8(A, dataA_8b); WriteReg_8(RegS, dataA_8b);
	log << "-> MOV " << regName(RegS) << ", A     A = " << dataA_8b << "\n";
}

void issRL78::MORPH_1_78_MOVW(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4) {
	cout << "					*** instruction 1_78_MOVW is executing ***" << endl;
	cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
	Immediate2Byte(addrHL); ReadReg_16(X, A, dataA_16b); WritedataOffset2Reg_16(C, B, addrHL, dataA_16b);
}

void issRL78::MORPH_1_79_MOVW(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4) {
	cout << "					*** instruction 1_79_MOVW is executing ***" << endl;
	cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
	Immediate2Byte(addrHL); ReaddataOffset2Reg_16(C, B, addrHL, dataA_16b);
	WriteReg_16(X, A, dataA_16b);
}

void issRL78::MORPH_1_7A_XOR(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4) {
	cout << "					*** instruction 1_7A_XOR is executing ***" << endl;
	cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
	ReaddataOffset_8(saddr, Byte2, dataA_8b);
	DO_XOR(dataA_8b, Byte3, result_8b);
	UpdateFlags_logic(result_8b);  WritedataOffset_8(saddr, Byte2, result_8b);
}

void issRL78::MORPH_1_7B_XOR(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4) {
	cout << "					*** instruction 1_7B_XOR is executing ***" << endl;
	cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
	ReaddataOffset_8(saddr, Byte2, dataB_8b); ReadReg_8(A, dataA_8b);
	DO_XOR(dataA_8b, dataB_8b, result_8b);
	UpdateFlags_logic(result_8b); WriteReg_8(A, result_8b);
}

void issRL78::MORPH_1_7C_XOR(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4) {
	cout << "					*** instruction 1_7C_XOR is executing ***" << endl;
	cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
	ReadReg_8(A, dataA_8b); DO_XOR(dataA_8b, Byte2, result_8b);
	UpdateFlags_logic(result_8b); WriteReg_8(A, result_8b);
}

void issRL78::MORPH_1_7D_XOR(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4) {
	cout << "					*** instruction 1_7D_XOR is executing ***" << endl;
	cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
	ReadReg_16(L, H, dataB_16b); Readdata_8(dataB_16b, dataB_8b); ReadReg_8(A, dataA_8b);
	DO_XOR(dataA_8b, dataB_8b, result_8b);
	UpdateFlags_logic(result_8b); WriteReg_8(A, result_8b);
}

void issRL78::MORPH_1_7E_XOR(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4) {
	cout << "					*** instruction 1_7E_XOR is executing ***" << endl;
	cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
	ReadReg_16(L, H, dataB_16b); ReaddataOffsetAddr_8(dataB_16b, Byte2, dataB_8b); ReadReg_8(A, dataA_8b);
	DO_XOR(dataA_8b, dataB_8b, result_8b);
	UpdateFlags_logic(result_8b); WriteReg_8(A, result_8b);
}

void issRL78::MORPH_1_7F_XOR(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4) {
	cout << "					*** instruction 1_7F_XOR is executing ***" << endl;
	cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
	Immediate2Byte(addrHL); Readdata_8(addrHL, dataB_8b); ReadReg_8(A, dataA_8b);
	DO_XOR(dataA_8b, dataB_8b, result_8b);
	UpdateFlags_logic(result_8b); WriteReg_8(A, result_8b);
}

void issRL78::MORPH_1_8X_INC(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4) {
	cout << "					*** instruction 1_8X_INC is executing ***" << endl;
	cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
	int RegS = Byte1.range(2, 0).to_uint();
	ReadReg_8(RegS, dataA_8b);
	DO_INC(dataA_8b, result_8b);
	UpdateFlags_inc(dataA_8b, result_8b); WriteReg_8(RegS, result_8b);
	log << "-> INC " << regName(RegS) <<"   "<< regName(RegS) <<" = "<< dataA_8b<<"  Result = "<< result_8b << "\n";
}

void issRL78::MORPH_1_88_MOV(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4) {
	cout << "					*** instruction 1_88_MOV is executing ***" << endl;
	cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
	ReaddataOffsetSP_8(Byte2, dataA_8b)  WriteReg_8(A, dataA_8b);
}

void issRL78::MORPH_1_89_MOV(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4) {
	cout << "					*** instruction 1_89_MOV is executing ***" << endl;
	cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
	Readdata2Reg_8(E, D, dataA_8b);  WriteReg_8(A, dataA_8b);
}

void issRL78::MORPH_1_8A_MOV(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4) {
	cout << "					*** instruction 1_8A_MOV is executing ***" << endl;
	cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
	ReaddataOffset2Reg_8(E, D, Byte2, dataA_8b);  WriteReg_8(A, dataA_8b);
}

void issRL78::MORPH_1_8B_MOV(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4) {
	cout << "					*** instruction 1_8B_MOV is executing ***" << endl;
	cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
	Readdata2Reg_8(L, H, dataA_8b);  WriteReg_8(A, dataA_8b);
}

void issRL78::MORPH_1_8C_MOV(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4) {
	cout << "					*** instruction 1_8C_MOV is executing ***" << endl;
	cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
	ReaddataOffset2Reg_8(L, H, Byte2, dataA_8b);  WriteReg_8(A, dataA_8b);
}

void issRL78::MORPH_1_8D_MOV(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4) {
	cout << "					*** instruction 1_8D_MOV is executing ***" << endl;
	cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
	ReaddataOffsetReg_8(saddr, Byte2, dataA_8b);  WriteReg_8(A, dataA_8b);
}

void issRL78::MORPH_1_8E_MOV(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4) {
	cout << "					*** instruction 1_8E_MOV is executing ***" << endl;
	cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
	ReaddataOffsetbyte_8(sfr, Byte2, dataA_8b);  WriteReg_8(A, dataA_8b); // sfr
}

void issRL78::MORPH_1_8F_MOV(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4) {
	cout << "					*** instruction 1_8F_MOV is executing ***" << endl;
	cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
	Immediate2Byte(addrHL); Readdata_8(addrHL, dataA_8b);  WriteReg_8(A, dataA_8b);
}

void issRL78::MORPH_1_9X_DEC(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4) {
	cout << "					*** instruction 1_9X_DEC is executing ***" << endl;
	cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
	int RegS = Byte1.range(2, 0).to_uint();
	ReadReg_8(RegS, dataA_8b);
	DO_DEC(dataA_8b, result_8b);
	UpdateFlags_dec(dataA_8b, result_8b); WriteReg_8(RegS, result_8b);
	log << "-> DEC " << regName(RegS) << "\n";
}

void issRL78::MORPH_1_98_MOV(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4) {
	cout << "					*** instruction 1_98_MOV is executing ***" << endl;
	cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
	ReadReg_8(A, dataA_8b); WritedataOffsetSP_8(Byte2, dataA_8b);
}

void issRL78::MORPH_1_99_MOV(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4) {
	cout << "					*** instruction 1_99_MOV is executing ***" << endl;
	cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
	ReadReg_8(A, dataA_8b); Writedata2Reg_8(E, D, dataA_8b);
}

void issRL78::MORPH_1_9A_MOV(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4) {
	cout << "					*** instruction 1_9A_MOV is executing ***" << endl;
	cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
	ReadReg_8(A, dataA_8b); WritedataOffset2Reg_8(E, D, Byte2, dataA_8b);
}

void issRL78::MORPH_1_9B_MOV(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4) {
	cout << "					*** instruction 1_9B_MOV is executing ***" << endl;
	cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
	ReadReg_8(A, dataA_8b); Writedata2Reg_8(L, H, dataA_8b);
}

void issRL78::MORPH_1_9C_MOV(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4) {
	cout << "					*** instruction 1_9C_MOV is executing ***" << endl;
	cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
	ReadReg_8(A, dataA_8b); WritedataOffset2Reg_8(L, H, Byte2, dataA_8b);
}

void issRL78::MORPH_1_9D_MOV(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4) {
	cout << "					*** instruction 1_9D_MOV is executing ***" << endl;
	cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
	ReadReg_8(A, dataA_8b); WritedataOffsetReg_8(saddr, Byte2, dataA_8b);
}

void issRL78::MORPH_1_9E_MOV(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4) {
	cout << "					*** instruction 1_9E_MOV is executing ***" << endl;
	cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
	ReadReg_8(A, dataA_8b); sfrUpdates(dataA_8b); WritedataOffsetbyte_8(sfr, Byte2, dataA_8b); // sfr
}

void issRL78::MORPH_1_9F_MOV(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4) {
	cout << "					*** instruction 1_9F_MOV is executing ***" << endl;
	cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
	Immediate2Byte(addrHL);  ReadReg_8(A, dataA_8b);  WritedataImmAdr_8(addrHL, dataA_8b);
}

void issRL78::MORPH_1_A0_INC(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4) {
	cout << "					*** instruction 1_A0_INC is executing ***" << endl;
	cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
	Immediate2Byte(addrHL);  ReaddataImmAdr_8(addrHL, dataA_8b);
	DO_INC(dataA_8b, result_8b);
	UpdateFlags_inc(dataA_8b, result_8b);
	WritedataImmAdr_8(addrHL, result_8b);
}

void issRL78::MORPH_1_AX_INCW(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4) {
	cout << "					*** instruction 1_A1_INCW is executing ***" << endl;
	cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
	int RegS = Byte1.range(2, 0).to_uint();
	ReadReg_16(RegS - 1, RegS, dataA_16b);
	DO_INC(dataA_16b, result_16b);
	UpdateFlags_inc(dataA_16b, result_16b); WriteReg_16(RegS - 1, RegS, result_16b);
	log << "-> DEC " << regName(RegS) << "\n";
}

void issRL78::MORPH_1_A2_INCW(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4) {
	cout << "					*** instruction 1_A2_INCW is executing ***" << endl;
	cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
	Immediate2Byte(addrHL);  Readdata_16(addrHL, dataA_16b);
	DO_INC(dataA_16b, result_16b);
	UpdateFlags_inc(dataA_16b, result_16b);
	Writedata_16(addrHL, result_16b);
}

void issRL78::MORPH_1_A4_INC(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4) {
	cout << "					*** instruction 1_A4_INC is executing ***" << endl;
	cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
	ReaddataOffsetbyte_8(saddr, Byte2, dataA_8b);
	DO_INC(dataA_8b, result_8b);
	UpdateFlags_inc(dataA_8b, result_8b);
	WritedataOffsetbyte_8(saddr, Byte2, result_8b);
}

void issRL78::MORPH_1_A6_INCW(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4) {
	cout << "					*** instruction 1_A6_INCW is executing ***" << endl;
	cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
	ReaddataOffsetbyte_16(saddr, Byte2, dataA_16b);
	DO_INC(dataA_16b, result_16b);
	UpdateFlags_inc(dataA_16b, result_16b); WritedataOffsetbyte_16(saddr, Byte2, result_16b);
}

void issRL78::MORPH_1_A8_MOVW(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4) {
	cout << "					*** instruction 1_A8_MOVW is executing ***" << endl;
	cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
	ReaddataOffsetSP_16(Byte2, dataA_16b);
	WriteReg_16(X, A, dataA_16b);
}

void issRL78::MORPH_1_A9_MOVW(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4) {
	cout << "					*** instruction 1_A9_MOVW is executing ***" << endl;
	cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
	Readdata2Reg_16(E, D, dataA_16b);
	WriteReg_16(X, A, dataA_16b);
}

void issRL78::MORPH_1_AA_MOVW(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4) {
	cout << "					*** instruction 1_AA_MOVW is executing ***" << endl;
	cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
	ReaddataOffset2Reg_16(E, D, Byte2, dataA_16b);
	WriteReg_16(X, A, dataA_16b);
}

void issRL78::MORPH_1_AB_MOVW(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4) {
	cout << "					*** instruction 1_AB_MOVW is executing ***" << endl;
	cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
	Readdata2Reg_16(L, H, dataA_16b);
	WriteReg_16(X, A, dataA_16b);
}

void issRL78::MORPH_1_AC_MOVW(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4) {
	cout << "					*** instruction 1_AC_MOVW is executing ***" << endl;
	cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
	ReaddataOffset2Reg_16(L, H, Byte2, dataA_16b);
	WriteReg_16(X, A, dataA_16b);
}

void issRL78::MORPH_1_AD_MOVW(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4) {
	cout << "					*** instruction 1_AD_MOVW is executing ***" << endl;
	cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
	ReaddataOffsetbyte_16(saddr, Byte2, dataA_16b);
	WriteReg_16(X, A, dataA_16b);
}

void issRL78::MORPH_1_AE_MOVW(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4) {
	cout << "					*** instruction 1_AE_MOVW is executing ***" << endl;
	cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
	ReaddataOffsetbyte_16(sfr, Byte2, dataA_16b);
	WriteReg_16(X, A, dataA_16b);
}

void issRL78::MORPH_1_AF_MOVW(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4) {
	cout << "					*** instruction 1_AF_MOVW is executing ***" << endl;
	cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
	Immediate2Byte(addrHL); Readdata_16(addrHL, dataA_16b);
	WriteReg_16(X, A, dataA_16b);
}

void issRL78::MORPH_1_B0_DEC(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4) {
	cout << "					*** instruction 1_B0_DEC is executing ***" << endl;
	cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
	Immediate2Byte(addrHL);  ReaddataImmAdr_8(addrHL, dataA_8b);
	DO_DEC(dataA_8b, result_8b);
	UpdateFlags_dec(dataA_8b, result_8b); WritedataImmAdr_8(addrHL, result_8b);
}

void issRL78::MORPH_1_BX_DECW(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4) {
	cout << "					*** instruction 1_BX_DECW is executing ***" << endl;
	cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
	int RegS = Byte1.range(2, 0).to_uint();
	ReadReg_16(RegS - 1, RegS, dataA_16b);
	DO_DEC(dataA_16b, result_16b);
	UpdateFlags_dec(dataA_16b, result_16b); WriteReg_16(RegS - 1, RegS, result_16b);
	log << "-> DECW " << regName(RegS - 1)<<", "<< regName(RegS) << "\n";
}

void issRL78::MORPH_1_B2_DECW(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4) {
	cout << "					*** instruction 1_B2_DECW is executing ***" << endl;
	cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
	Immediate2Byte(addrHL);  Readdata_16(addrHL, dataA_16b);
	DO_DEC(dataA_16b, result_16b);
	UpdateFlags_dec(dataA_16b, result_16b); Writedata_16(addrHL, result_16b);
}

void issRL78::MORPH_1_B4_DEC(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4) {
	cout << "					*** instruction 1_B4_DEC is executing ***" << endl;
	cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
	ReaddataOffsetbyte_8(saddr, Byte2, dataA_8b);
	DO_DEC(dataA_8b, result_8b);
	UpdateFlags_dec(dataA_8b, result_8b); WritedataOffsetbyte_8(saddr, Byte2, result_8b);
}

void issRL78::MORPH_1_B6_DECW(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4) {
	cout << "					*** instruction 1_B6_DECW is executing ***" << endl;
	cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
	ReaddataOffsetbyte_16(saddr, Byte2, dataA_16b);
	DO_DEC(dataA_16b, result_16b);
	UpdateFlags_dec(dataA_16b, result_16b); WritedataOffsetbyte_16(saddr, Byte2, result_16b);
}

void issRL78::MORPH_1_B8_MOVW(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4) {
	cout << "					*** instruction 1_B8_MOVW is executing ***" << endl;
	cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
	ReadReg_16(X, A, dataA_16b);
	WritedataOffsetSP_16(Byte2, dataA_16b);
}

void issRL78::MORPH_1_B9_MOVW(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4) {
	cout << "					*** instruction 1_B9_MOVW is executing ***" << endl;
	cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
	ReadReg_16(X, A, dataA_16b);
	Writedata2Reg_16(E, D, dataA_16b);
}

void issRL78::MORPH_1_BA_MOVW(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4) {
	cout << "					*** instruction 1_BA_MOVW is executing ***" << endl;
	cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
	ReadReg_16(X, A, dataA_16b);
	WritedataOffset2Reg_16(E, D, Byte2, dataA_16b);
}

void issRL78::MORPH_1_BB_MOVW(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4) {
	cout << "					*** instruction 1_BB_MOVW is executing ***" << endl;
	cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
	ReadReg_16(X, A, dataA_16b);
	Writedata2Reg_16(L, H, dataA_16b);
}

void issRL78::MORPH_1_BC_MOVW(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4) {
	cout << "					*** instruction 1_BC_MOVW is executing ***" << endl;
	cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
	ReadReg_16(X, A, dataA_16b);
	WritedataOffset2Reg_16(L, H, Byte2, dataA_16b);
}

void issRL78::MORPH_1_BD_MOVW(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4) {
	cout << "					*** instruction 1_BD_MOVW is executing ***" << endl;
	cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
	ReadReg_16(X, A, dataA_16b);
	WritedataOffsetbyte_16(saddr, Byte2, dataA_16b);
}

void issRL78::MORPH_1_BE_MOVW(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4) {
	cout << "					*** instruction 1_BE_MOVW is executing ***" << endl;
	cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
	ReadReg_16(X, A, dataA_16b);
	WritedataOffsetbyte_16(sfr, Byte2, dataA_16b);
}

void issRL78::MORPH_1_BF_MOVW(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4) {
	cout << "					*** instruction 1_BF_MOVW is executing ***" << endl;
	cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
	Immediate2Byte(addrHL); ReadReg_16(X, A, dataA_16b);
	Writedata_16(addrHL, dataA_16b);
}

void issRL78::MORPH_1_CX_POP(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4) {
	cout << "					*** instruction 1_C0_POP is executing ***" << endl;
	cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
	int RegS = Byte1.range(2, 0).to_uint();
	Pop2ByteSP(dataA_16b);
	WriteReg_16(RegS, RegS + 1, dataA_16b);
	log << "-> POP " << regName(RegS) << regName(RegS + 1) << "\n";
}

void issRL78::MORPH_1_CY_PUSH(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4) {
	cout << "					*** instruction 1_C1_PUSH is executing ***" << endl;
	cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
	int RegS = Byte1.range(2, 0).to_uint();
	ReadReg_16(RegS - 1, RegS, dataA_16b);
	Push2ByteSP(dataA_16b);
	log << "-> PUSH " << regName(RegS - 1) << regName(RegS) << "\n";
}

void issRL78::MORPH_1_C8_MOV(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4) {
	cout << "					*** instruction 1_C8_MOV is executing ***" << endl;
	cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
	WritedataOffsetSP_8(Byte2, Byte3);
}

void issRL78::MORPH_1_C9_MOVW(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4) {
	cout << "					*** instruction 1_C9_MOVW is executing ***" << endl;
	cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
	ImmediateData2Byte(dataA_16b);
	WritedataOffsetbyte_16(saddr, Byte2, dataA_16b);
}

void issRL78::MORPH_1_CA_MOV(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4) {
	cout << "					*** instruction 1_CA_MOV is executing ***" << endl;
	cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
	WritedataOffset2Reg_8(E, D, Byte2, Byte3);
}

void issRL78::MORPH_1_CB_MOVW(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4) {
	cout << "					*** instruction 1_CB_MOVW is executing ***" << endl;
	cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
	ImmediateData2Byte(dataA_16b);
	WritedataOffsetbyte_16(sfr, Byte2, dataA_16b);
}

void issRL78::MORPH_1_CC_MOV(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4) {
	cout << "					*** instruction 1_CC_MOV is executing ***" << endl;
	cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
	WritedataOffset2Reg_8(L, H, Byte2, Byte3);
}

void issRL78::MORPH_1_CD_MOV(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4) {
	cout << "					*** instruction 1_CD_MOV is executing ***" << endl;
	cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
	WritedataOffsetbyte_8(saddr, Byte2, Byte3);
}

void issRL78::MORPH_1_CE_MOV(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4) {
	cout << "					*** instruction 1_CE_MOV is executing ***" << endl;
	cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
	sfrUpdates(Byte3); WritedataOffsetbyte_8(sfr, Byte2, Byte3);
}

void issRL78::MORPH_1_CF_MOV(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4) {
	cout << "					*** instruction 1_CF_MOV is executing ***" << endl;
	cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
	Immediate2Byte(addrHL);
	WritedataOffsetSP_8(addrHL, Byte4);
}

void issRL78::MORPH_1_DX_CMP0(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4) {
	cout << "					*** instruction 1_DX_CMP0 is executing ***" << endl;
	cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
	int RegS = Byte1.range(1, 0).to_uint();
	ReadReg_8(RegS, dataA_8b);
	DO_CMP0(dataA_8b, result_8b);
	UpdateFlags_cmp0(result_8b);
	log << "-> CMP0 " << regName(RegS) << "\n";
}

void issRL78::MORPH_1_D4_CMP0(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4) {
	cout << "					*** instruction 1_D4_CMP0 is executing ***" << endl;
	cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
	ReaddataOffset_8(saddr, Byte2, dataA_8b);
	DO_CMP0(dataA_8b, result_8b);
	UpdateFlags_cmp0(result_8b);
}

void issRL78::MORPH_1_D5_CMP0(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4) {
	cout << "					*** instruction 1_D5_CMP0 is executing ***" << endl;
	cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
	Immediate2Byte(addrHL); Readdata_8(addrHL, dataA_8b);
	DO_CMP0(dataA_8b, result_8b);
	UpdateFlags_cmp0(result_8b);
}

void issRL78::MORPH_1_D6_MULU(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4) {
	cout << "					*** instruction 1_D6_MULU is executing ***" << endl;
	cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
	ReadReg_8(A, dataA_8b); ReadReg_8(X, dataB_8b);
	DO_Multiplication(dataA_8b, dataB_8b, result_16b);
	WriteReg_16(X, A, result_16b);
}

void issRL78::MORPH_1_D7_RET(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4) {
	cout << "					*** instruction 1_D7_RET is executing ***" << endl;
	cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
	Ret();
}

void issRL78::MORPH_1_D8_MOV(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4) {
	cout << "					*** instruction 1_D8_MOV is executing ***" << endl;
	cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
	ReaddataOffset_8(saddr, Byte2, dataA_8b);
	WriteReg_8(X, dataA_8b);
}

void issRL78::MORPH_1_D9_MOV(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4) {
	cout << "					*** instruction 1_D9_MOV is executing ***" << endl;
	cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
	Immediate2Byte(addrHL); Readdata_8(addrHL, dataA_8b);  WriteReg_8(X, dataA_8b);
	log << "-> MOV  A, mem("  << addrHL.to_uint() + 0xFF000 <<")"<< "\n";
}

void issRL78::MORPH_1_DA_MOVW(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4) {
	cout << "					*** instruction 1_DA_MOVW is executing ***" << endl;
	cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
	ReaddataOffsetbyte_16(saddr, Byte2, dataA_16b);
	WriteReg_16(C, B, dataA_16b);
}

void issRL78::MORPH_1_DB_MOVW(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4) {
	cout << "					*** instruction 1_DB_MOVW is executing ***" << endl;
	cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
	Immediate2Byte(addrHL); Readdata_16(addrHL, dataA_16b);
	WriteReg_16(C, B, dataA_16b);
}

void issRL78::MORPH_1_DC_BC(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4) {
	cout << "					*** instruction 1_DC_BC is executing ***" << endl;
	cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
	Br(PSW_reg.CY);
}

void issRL78::MORPH_1_DD_BZ(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4) {
	cout << "					*** instruction 1_DD_BZ is executing ***" << endl;
	cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
	Br(PSW_reg.Z);
}

void issRL78::MORPH_1_DE_BNC(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4) {
	cout << "					*** instruction 1_DE_BNC is executing ***" << endl;
	cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
	BrN(PSW_reg.CY);
}

void issRL78::MORPH_1_DF_BNZ(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4) {
	cout << "					*** instruction 1_DF_BNZ is executing ***" << endl;
	cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
	BrN(PSW_reg.Z);
	log << "-> BNZ " << Byte2 << "\n";

}

void issRL78::MORPH_1_EX_ONEB(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4) {
	cout << "					*** instruction 1_E0_ONEB is executing ***" << endl;
	cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
	int RegS = Byte1.range(1, 0).to_uint();
	dataA_8b = 1; WriteReg_8(RegS, dataA_8b);
	log << "-> ONEB " << regName(RegS) << "\n";
}

void issRL78::MORPH_1_E4_ONEB(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4) {
	cout << "					*** instruction 1_E4_ONEB is executing ***" << endl;
	cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
	dataA_8b = 1; WriteReg_8(B, dataA_8b);
	WritedataOffsetbyte_8(saddr, Byte2, dataA_8b);
}

void issRL78::MORPH_1_E5_ONEB(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4) {
	cout << "					*** instruction 1_E5_ONEB is executing ***" << endl;
	cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
	Immediate2Byte(addrHL);  dataA_8b = 1;  WritedataImmAdr_8(addrHL, dataA_8b);
}

void issRL78::MORPH_1_E6_ONEW(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4) {
	cout << "					*** instruction 1_E6_ONEW is executing ***" << endl;
	cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
	dataA_16b = 1;  WriteReg_16(X, A, dataA_16b);
}

void issRL78::MORPH_1_E7_ONEW(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4) {
	cout << "					*** instruction 1_E7_ONEW is executing ***" << endl;
	cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
	dataA_16b = 1;  WriteReg_16(C, B, dataA_16b);
}

void issRL78::MORPH_1_E8_MOV(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4) {
	cout << "					*** instruction 1_E8_MOV is executing ***" << endl;
	cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
	ReaddataOffset_8(saddr, Byte2, dataA_8b); WriteReg_8(B, dataA_8b);
}

void issRL78::MORPH_1_E9_MOV(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4) {
	cout << "					*** instruction 1_E9_MOV is executing ***" << endl;
	cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
	Immediate2Byte(addrHL); Readdata_8(addrHL, dataA_8b);
	cout << "addr HL is:" << addrHL << endl;
	WriteReg_8(B, dataA_8b);
}

void issRL78::MORPH_1_EA_MOVW(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4) {
	cout << "					*** instruction 1_E9_MOV is executing ***" << endl;
	cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
	ReaddataOffset_16(saddr, Byte2, dataA_16b); WriteReg_16(E, D, dataA_16b);
}

void issRL78::MORPH_1_EB_MOVW(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4) {
	cout << "					*** instruction 1_E9_MOV is executing ***" << endl;
	cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
	Immediate2Byte(addrHL); Readdata_16(addrHL, dataA_16b);
	WriteReg_16(E, D, dataA_16b);
}

void issRL78::MORPH_1_EC_BR(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4) {
	cout << "					*** instruction 1_EC_BR is executing ***" << endl;
	cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
	PCregister = (Byte3.to_uint() * 256) + Byte2.to_uint() + ((Byte4.range(3, 0)).to_uint() * 65536);
}

void issRL78::MORPH_1_ED_BR(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4) {
	cout << "					*** instruction 1_ED_BR is executing ***" << endl;
	cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
	PCregister = (Byte3.to_uint() * 256) + Byte2.to_uint();
}

void issRL78::MORPH_1_EE_BR(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4) {
	cout << "					*** instruction 1_EE_BR is executing ***" << endl;
	cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
	Immediate2Byte(dataA_16b);
	PCregister = dataA_16b.to_int() + PCregister.to_uint();
}

void issRL78::MORPH_1_EF_BR(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4) {
	cout << "					*** instruction 1_EF_BR is executing ***" << endl;
	cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
	PCregister = Byte2.to_int() + PCregister.to_uint();
}

void issRL78::MORPH_1_FX_CLRB(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4) {
	cout << "					*** instruction 1_FX_CLRB is executing ***" << endl;
	cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
	int RegS = Byte1.range(1, 0).to_uint();
	dataA_8b = 0; WriteReg_8(RegS, dataA_8b);
	log << "-> CLRB " << regName(RegS) << "\n";
}

void issRL78::MORPH_1_F4_CLRB(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4) {
	cout << "					*** instruction 1_F4_CLRB is executing ***" << endl;
	cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
	dataA_8b = 0; WriteReg_8(B, dataA_8b);
	WritedataOffsetbyte_8(saddr, Byte2, dataA_8b);
}

void issRL78::MORPH_1_F5_CLRB(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4) {
	cout << "					*** instruction 1_F5_CLRB is executing ***" << endl;
	cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
	Immediate2Byte(addrHL);  dataA_8b = 0;  WritedataImmAdr_8(addrHL, dataA_8b);
}

void issRL78::MORPH_1_F6_CLRW(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4) {
	cout << "					*** instruction 1_F6_CLRW is executing ***" << endl;
	cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
	dataA_16b = 0;  WriteReg_16(X, A, dataA_16b);
}

void issRL78::MORPH_1_F7_CLRW(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4) {
	cout << "					*** instruction 1_F7_CLRW is executing ***" << endl;
	cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
	dataA_16b = 0;  WriteReg_16(C, B, dataA_16b);
}

void issRL78::MORPH_1_F8_MOV(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4) {
	cout << "					*** instruction 1_F8_MOV is executing ***" << endl;
	cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
	ReaddataOffset_8(saddr, Byte2, dataA_8b); WriteReg_8(C, dataA_8b);
}

void issRL78::MORPH_1_F9_MOV(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4) {
	cout << "					*** instruction 1_F9_MOV is executing ***" << endl;
	cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
	Immediate2Byte(addrHL); Readdata_8(addrHL, dataA_8b);
	WriteReg_8(C, dataA_8b);
}

void issRL78::MORPH_1_FA_MOVW(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4) {
	cout << "					*** instruction 1_E9_MOV is executing ***" << endl;
	cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
	ReaddataOffset_16(saddr, Byte2, dataA_16b); WriteReg_16(L, H, dataA_16b);
}

void issRL78::MORPH_1_FB_MOVW(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4) {
	cout << "					*** instruction 1_E9_MOV is executing ***" << endl;
	cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
	Immediate2Byte(addrHL); Readdata_16(addrHL, dataA_16b);
	WriteReg_16(L, H, dataA_16b);
}

void issRL78::MORPH_1_FC_CALL(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4) {
	cout << "					*** instruction 1_FC_CALL is executing ***" << endl;
	cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
	dataA_16b = PCregister.range(19, 16).to_uint(); Push2ByteSP(dataA_16b);
	dataB_16b = PCregister; Push2ByteSP(dataB_16b);
	PCregister = (Byte3.to_uint() * 256) + Byte2.to_uint() + ((Byte4.range(3, 0)).to_uint() * 65536);
}

void issRL78::MORPH_1_FD_CALL(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4) {
	cout << "					*** instruction 1_FD_CALL is executing ***" << endl;
	cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
	dataA_16b = PCregister.range(19, 16).to_uint(); Push2ByteSP(dataA_16b);
	dataB_16b = PCregister; Push2ByteSP(dataB_16b);
	PCregister = (Byte3.to_uint() * 256) + Byte2.to_uint();
}

void issRL78::MORPH_1_FE_CALL(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4) {
	cout << "					*** instruction 1_FE_CALL is executing ***" << endl;
	cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
	dataA_16b = PCregister.range(19, 16).to_uint(); Push2ByteSP(dataA_16b);
	dataB_16b = PCregister; Push2ByteSP(dataB_16b);
	PCregister = (Byte3.to_uint() * 256) + Byte2.to_uint() + PCregister.to_uint();
}

void issRL78::MORPH_2_0X_ADD(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4) {
	cout << "					*** instruction 2_0X_ADD is executing ***" << endl;
	cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
	int Reg2 = Byte2.range(2, 0).to_uint();
	ReadReg_8(A, dataA_8b); ReadReg_8(Reg2, dataB_8b);
	DO_Addition(dataA_8b, dataB_8b, result_8b);
	UpdateFlags_8b_add(dataA_8b, dataB_8b, result_8b); WriteReg_8(A, result_8b);
	log << "-> ADD A, " << regName(Reg2) << "     A = "<< dataA_8b<<"   "<< regName(Reg2) <<" = " << dataB_8b<< "                 <<{ Result = " << result_8b<<" }>>" << "\n";
}

void issRL78::MORPH_2_09_ADDW(sc_lv <8> Byte1, sc_lv <8> Byte2, sc_lv <8> Byte3, sc_lv <8> Byte4) {
	cout << "					*** instruction 2_09_ADDW is executing ***" << endl;
	cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
	ReaddataOffset2Reg_16(L, H, Byte3, dataA_16b);
	ReadReg_16(X, A, dataB_16b);
	DO_Addition(dataA_16b, dataB_16b, result_16b);
	UpdateFlags_16b_add(dataA_16b, dataB_16b, result_16b); WriteReg_16(X, A, result_16b);
}


inline  void issRL78::createMorphTable() {
	MorphFunctions[RL78_IT_1_00_NOP] = &issRL78::MORPH_1_00_NOP;
	MorphFunctions[RL78_IT_1_0X_ADDW] = &issRL78::MORPH_1_0X_ADDW;
	MorphFunctions[RL78_IT_1_02_ADDW] = &issRL78::MORPH_1_02_ADDW;
	MorphFunctions[RL78_IT_1_04_ADDW] = &issRL78::MORPH_1_04_ADDW;
	MorphFunctions[RL78_IT_1_06_ADDW] = &issRL78::MORPH_1_06_ADDW;
	MorphFunctions[RL78_IT_1_08_XCH] = &issRL78::MORPH_1_08_XCH;
	MorphFunctions[RL78_IT_1_09_MOV] = &issRL78::MORPH_1_09_MOV;
	MorphFunctions[RL78_IT_1_0A_ADD] = &issRL78::MORPH_1_0A_ADD;
	MorphFunctions[RL78_IT_1_0B_ADD] = &issRL78::MORPH_1_0B_ADD;
	MorphFunctions[RL78_IT_1_0C_ADD] = &issRL78::MORPH_1_0C_ADD;
	MorphFunctions[RL78_IT_1_0D_ADD] = &issRL78::MORPH_1_0D_ADD;
	MorphFunctions[RL78_IT_1_0E_ADD] = &issRL78::MORPH_1_0E_ADD;
	MorphFunctions[RL78_IT_1_0F_ADD] = &issRL78::MORPH_1_0F_ADD;


	MorphFunctions[RL78_IT_1_10_ADDW] = &issRL78::MORPH_1_10_ADDW;
	MorphFunctions[RL78_IT_1_11_PREFIX] = &issRL78::MORPH_1_11_PREFIX;
	MorphFunctions[RL78_IT_1_1X_MOVW] = &issRL78::MORPH_1_1X_MOVW;
	MorphFunctions[RL78_IT_1_1Y_MOVW] = &issRL78::MORPH_1_1Y_MOVW;
	MorphFunctions[RL78_IT_1_18_MOV] = &issRL78::MORPH_1_18_MOV;
	MorphFunctions[RL78_IT_1_19_MOV] = &issRL78::MORPH_1_19_MOV;
	MorphFunctions[RL78_IT_1_1A_ADDC] = &issRL78::MORPH_1_1A_ADDC;
	MorphFunctions[RL78_IT_1_1B_ADDC] = &issRL78::MORPH_1_1B_ADDC;
	MorphFunctions[RL78_IT_1_1C_ADDC] = &issRL78::MORPH_1_1C_ADDC;
	MorphFunctions[RL78_IT_1_1D_ADDC] = &issRL78::MORPH_1_1D_ADDC;
	MorphFunctions[RL78_IT_1_1E_ADDC] = &issRL78::MORPH_1_1E_ADDC;
	MorphFunctions[RL78_IT_1_1F_ADDC] = &issRL78::MORPH_1_1F_ADDC;


	MorphFunctions[RL78_IT_1_20_SUBW] = &issRL78::MORPH_1_20_SUBW;
	MorphFunctions[RL78_IT_1_22_SUBW] = &issRL78::MORPH_1_22_SUBW;
	MorphFunctions[RL78_IT_1_24_SUBW] = &issRL78::MORPH_1_24_SUBW;
	MorphFunctions[RL78_IT_1_26_SUBW] = &issRL78::MORPH_1_26_SUBW;
	MorphFunctions[RL78_IT_1_2X_SUBW] = &issRL78::MORPH_1_2X_SUBW;
	MorphFunctions[RL78_IT_1_28_MOV] = &issRL78::MORPH_1_28_MOV;
	MorphFunctions[RL78_IT_1_29_MOV] = &issRL78::MORPH_1_29_MOV;
	MorphFunctions[RL78_IT_1_29_MOV] = &issRL78::MORPH_1_09_MOV;
	MorphFunctions[RL78_IT_1_2A_SUB] = &issRL78::MORPH_1_2A_SUB;
	MorphFunctions[RL78_IT_1_2B_SUB] = &issRL78::MORPH_1_2B_SUB;
	MorphFunctions[RL78_IT_1_2C_SUB] = &issRL78::MORPH_1_2C_SUB;
	MorphFunctions[RL78_IT_1_2D_SUB] = &issRL78::MORPH_1_2D_SUB;
	MorphFunctions[RL78_IT_1_2E_SUB] = &issRL78::MORPH_1_2E_SUB;
	MorphFunctions[RL78_IT_1_2F_SUB] = &issRL78::MORPH_1_2F_SUB;


	MorphFunctions[RL78_IT_1_3X_MOVW] = &issRL78::MORPH_1_3X_MOVW;
	MorphFunctions[RL78_IT_1_3Y_XCHW] = &issRL78::MORPH_1_3Y_XCHW;
	MorphFunctions[RL78_IT_1_38_MOV] = &issRL78::MORPH_1_38_MOV;
	MorphFunctions[RL78_IT_1_39_MOV] = &issRL78::MORPH_1_39_MOV;
	MorphFunctions[RL78_IT_1_3A_SUBC] = &issRL78::MORPH_1_3A_SUBC;
	MorphFunctions[RL78_IT_1_3B_SUBC] = &issRL78::MORPH_1_3B_SUBC;
	MorphFunctions[RL78_IT_1_3C_SUBC] = &issRL78::MORPH_1_3C_SUBC;
	MorphFunctions[RL78_IT_1_3D_SUBC] = &issRL78::MORPH_1_3D_SUBC;
	MorphFunctions[RL78_IT_1_3E_SUBC] = &issRL78::MORPH_1_3E_SUBC;
	MorphFunctions[RL78_IT_1_3F_SUBC] = &issRL78::MORPH_1_3F_SUBC;


	MorphFunctions[RL78_IT_1_40_CMP] = &issRL78::MORPH_1_40_CMP;
	MorphFunctions[RL78_IT_1_41_MOV] = &issRL78::MORPH_1_41_MOV;
	MorphFunctions[RL78_IT_1_42_CMPW] = &issRL78::MORPH_1_42_CMPW;
	MorphFunctions[RL78_IT_1_44_CMPW] = &issRL78::MORPH_1_44_CMPW;
	MorphFunctions[RL78_IT_1_46_CMPW] = &issRL78::MORPH_1_46_CMPW;
	MorphFunctions[RL78_IT_1_4X_CMPW] = &issRL78::MORPH_1_4X_CMPW;
	MorphFunctions[RL78_IT_1_48_MOV] = &issRL78::MORPH_1_48_MOV;
	MorphFunctions[RL78_IT_1_49_MOV] = &issRL78::MORPH_1_49_MOV;
	MorphFunctions[RL78_IT_1_4A_CMP] = &issRL78::MORPH_1_4A_CMP;
	MorphFunctions[RL78_IT_1_4B_CMP] = &issRL78::MORPH_1_4B_CMP;
	MorphFunctions[RL78_IT_1_4C_CMP] = &issRL78::MORPH_1_4C_CMP;
	MorphFunctions[RL78_IT_1_4D_CMP] = &issRL78::MORPH_1_4D_CMP;
	MorphFunctions[RL78_IT_1_4E_CMP] = &issRL78::MORPH_1_4E_CMP;
	MorphFunctions[RL78_IT_1_4F_CMP] = &issRL78::MORPH_1_4F_CMP;


	MorphFunctions[RL78_IT_1_5X_MOV] = &issRL78::MORPH_1_5X_MOV;
	MorphFunctions[RL78_IT_1_58_MOVW] = &issRL78::MORPH_1_58_MOVW;
	MorphFunctions[RL78_IT_1_59_MOVW] = &issRL78::MORPH_1_59_MOVW;
	MorphFunctions[RL78_IT_1_5A_AND] = &issRL78::MORPH_1_5A_AND;
	MorphFunctions[RL78_IT_1_5B_AND] = &issRL78::MORPH_1_5B_AND;
	MorphFunctions[RL78_IT_1_5C_AND] = &issRL78::MORPH_1_5C_AND;
	MorphFunctions[RL78_IT_1_5D_AND] = &issRL78::MORPH_1_5D_AND;
	MorphFunctions[RL78_IT_1_5E_AND] = &issRL78::MORPH_1_5E_AND;
	MorphFunctions[RL78_IT_1_5F_AND] = &issRL78::MORPH_1_5F_AND;


	MorphFunctions[RL78_IT_1_6X_MOV] = &issRL78::MORPH_1_6X_MOV;
	//
	MorphFunctions[RL78_IT_1_68_MOVW] = &issRL78::MORPH_1_68_MOVW;
	MorphFunctions[RL78_IT_1_69_MOVW] = &issRL78::MORPH_1_69_MOVW;
	MorphFunctions[RL78_IT_1_6A_OR] = &issRL78::MORPH_1_6A_OR;
	MorphFunctions[RL78_IT_1_6B_OR] = &issRL78::MORPH_1_6B_OR;
	MorphFunctions[RL78_IT_1_6C_OR] = &issRL78::MORPH_1_6C_OR;
	MorphFunctions[RL78_IT_1_6D_OR] = &issRL78::MORPH_1_6D_OR;
	MorphFunctions[RL78_IT_1_6E_OR] = &issRL78::MORPH_1_6E_OR;
	MorphFunctions[RL78_IT_1_6F_OR] = &issRL78::MORPH_1_6F_OR;


	MorphFunctions[RL78_IT_1_7X_MOV] = &issRL78::MORPH_1_7X_MOV;
	MorphFunctions[RL78_IT_1_78_MOVW] = &issRL78::MORPH_1_78_MOVW;
	MorphFunctions[RL78_IT_1_79_MOVW] = &issRL78::MORPH_1_79_MOVW;
	MorphFunctions[RL78_IT_1_7A_XOR] = &issRL78::MORPH_1_7A_XOR;
	MorphFunctions[RL78_IT_1_7B_XOR] = &issRL78::MORPH_1_7B_XOR;
	MorphFunctions[RL78_IT_1_7C_XOR] = &issRL78::MORPH_1_7C_XOR;
	MorphFunctions[RL78_IT_1_7D_XOR] = &issRL78::MORPH_1_7D_XOR;
	MorphFunctions[RL78_IT_1_7E_XOR] = &issRL78::MORPH_1_7E_XOR;
	MorphFunctions[RL78_IT_1_7F_XOR] = &issRL78::MORPH_1_7F_XOR;


	MorphFunctions[RL78_IT_1_8X_INC] = &issRL78::MORPH_1_8X_INC;
	MorphFunctions[RL78_IT_1_88_MOV] = &issRL78::MORPH_1_88_MOV;
	MorphFunctions[RL78_IT_1_89_MOV] = &issRL78::MORPH_1_89_MOV;
	MorphFunctions[RL78_IT_1_8A_MOV] = &issRL78::MORPH_1_8A_MOV;
	MorphFunctions[RL78_IT_1_8B_MOV] = &issRL78::MORPH_1_8B_MOV;
	MorphFunctions[RL78_IT_1_8C_MOV] = &issRL78::MORPH_1_8C_MOV;
	MorphFunctions[RL78_IT_1_8D_MOV] = &issRL78::MORPH_1_8D_MOV;
	MorphFunctions[RL78_IT_1_8E_MOV] = &issRL78::MORPH_1_8E_MOV;
	MorphFunctions[RL78_IT_1_8F_MOV] = &issRL78::MORPH_1_8F_MOV;


	MorphFunctions[RL78_IT_1_9X_DEC] = &issRL78::MORPH_1_9X_DEC;
	MorphFunctions[RL78_IT_1_98_MOV] = &issRL78::MORPH_1_98_MOV;
	MorphFunctions[RL78_IT_1_99_MOV] = &issRL78::MORPH_1_99_MOV;
	MorphFunctions[RL78_IT_1_9A_MOV] = &issRL78::MORPH_1_9A_MOV;
	MorphFunctions[RL78_IT_1_9B_MOV] = &issRL78::MORPH_1_9B_MOV;
	MorphFunctions[RL78_IT_1_9C_MOV] = &issRL78::MORPH_1_9C_MOV;
	MorphFunctions[RL78_IT_1_9D_MOV] = &issRL78::MORPH_1_9D_MOV;
	MorphFunctions[RL78_IT_1_9E_MOV] = &issRL78::MORPH_1_9E_MOV;
	MorphFunctions[RL78_IT_1_9F_MOV] = &issRL78::MORPH_1_9F_MOV;


	MorphFunctions[RL78_IT_1_A0_INC] = &issRL78::MORPH_1_A0_INC;
	MorphFunctions[RL78_IT_1_A2_INCW] = &issRL78::MORPH_1_A2_INCW;
	MorphFunctions[RL78_IT_1_A4_INC] = &issRL78::MORPH_1_A4_INC;
	MorphFunctions[RL78_IT_1_A6_INCW] = &issRL78::MORPH_1_A6_INCW;
	MorphFunctions[RL78_IT_1_AX_INCW] = &issRL78::MORPH_1_AX_INCW;
	MorphFunctions[RL78_IT_1_A8_MOVW] = &issRL78::MORPH_1_A8_MOVW;
	MorphFunctions[RL78_IT_1_A9_MOVW] = &issRL78::MORPH_1_A9_MOVW;
	MorphFunctions[RL78_IT_1_AA_MOVW] = &issRL78::MORPH_1_AA_MOVW;
	MorphFunctions[RL78_IT_1_AB_MOVW] = &issRL78::MORPH_1_AB_MOVW;
	MorphFunctions[RL78_IT_1_AC_MOVW] = &issRL78::MORPH_1_AC_MOVW;
	MorphFunctions[RL78_IT_1_AD_MOVW] = &issRL78::MORPH_1_AD_MOVW;
	MorphFunctions[RL78_IT_1_AE_MOVW] = &issRL78::MORPH_1_AE_MOVW;
	MorphFunctions[RL78_IT_1_AF_MOVW] = &issRL78::MORPH_1_AF_MOVW;


	MorphFunctions[RL78_IT_1_B0_DEC] = &issRL78::MORPH_1_B0_DEC;
	MorphFunctions[RL78_IT_1_B2_DECW] = &issRL78::MORPH_1_B2_DECW;
	MorphFunctions[RL78_IT_1_B4_DEC] = &issRL78::MORPH_1_B4_DEC;
	MorphFunctions[RL78_IT_1_B6_DECW] = &issRL78::MORPH_1_B6_DECW;
	MorphFunctions[RL78_IT_1_BX_DECW] = &issRL78::MORPH_1_BX_DECW;
	MorphFunctions[RL78_IT_1_B8_MOVW] = &issRL78::MORPH_1_B8_MOVW;
	MorphFunctions[RL78_IT_1_B9_MOVW] = &issRL78::MORPH_1_B9_MOVW;
	MorphFunctions[RL78_IT_1_BA_MOVW] = &issRL78::MORPH_1_BA_MOVW;
	MorphFunctions[RL78_IT_1_BB_MOVW] = &issRL78::MORPH_1_BB_MOVW;
	MorphFunctions[RL78_IT_1_BC_MOVW] = &issRL78::MORPH_1_BC_MOVW;
	MorphFunctions[RL78_IT_1_BD_MOVW] = &issRL78::MORPH_1_BD_MOVW;
	MorphFunctions[RL78_IT_1_BE_MOVW] = &issRL78::MORPH_1_BE_MOVW;
	MorphFunctions[RL78_IT_1_BF_MOVW] = &issRL78::MORPH_1_BF_MOVW;


	MorphFunctions[RL78_IT_1_CX_POP] = &issRL78::MORPH_1_CX_POP;
	MorphFunctions[RL78_IT_1_CY_PUSH] = &issRL78::MORPH_1_CY_PUSH;
	MorphFunctions[RL78_IT_1_C8_MOV] = &issRL78::MORPH_1_C8_MOV;
	MorphFunctions[RL78_IT_1_C9_MOVW] = &issRL78::MORPH_1_C9_MOVW;
	MorphFunctions[RL78_IT_1_CA_MOV] = &issRL78::MORPH_1_CA_MOV;
	MorphFunctions[RL78_IT_1_CB_MOVW] = &issRL78::MORPH_1_CB_MOVW;
	MorphFunctions[RL78_IT_1_CC_MOV] = &issRL78::MORPH_1_CC_MOV;
	MorphFunctions[RL78_IT_1_CD_MOV] = &issRL78::MORPH_1_CD_MOV;
	MorphFunctions[RL78_IT_1_CE_MOV] = &issRL78::MORPH_1_CE_MOV;
	MorphFunctions[RL78_IT_1_CF_MOV] = &issRL78::MORPH_1_CF_MOV;


	MorphFunctions[RL78_IT_1_DX_CMP0] = &issRL78::MORPH_1_DX_CMP0;
	MorphFunctions[RL78_IT_1_D4_CMP0] = &issRL78::MORPH_1_D4_CMP0;
	MorphFunctions[RL78_IT_1_D5_CMP0] = &issRL78::MORPH_1_D5_CMP0;
	MorphFunctions[RL78_IT_1_D6_MULU] = &issRL78::MORPH_1_D6_MULU;
	MorphFunctions[RL78_IT_1_D7_RET] = &issRL78::MORPH_1_D7_RET;
	MorphFunctions[RL78_IT_1_D8_MOV] = &issRL78::MORPH_1_D8_MOV;
	MorphFunctions[RL78_IT_1_D9_MOV] = &issRL78::MORPH_1_D9_MOV;
	MorphFunctions[RL78_IT_1_DA_MOVW] = &issRL78::MORPH_1_DA_MOVW;
	MorphFunctions[RL78_IT_1_DB_MOVW] = &issRL78::MORPH_1_DB_MOVW;
	MorphFunctions[RL78_IT_1_DC_BC] = &issRL78::MORPH_1_DC_BC;
	MorphFunctions[RL78_IT_1_DD_BZ] = &issRL78::MORPH_1_DD_BZ;
	MorphFunctions[RL78_IT_1_DE_BNC] = &issRL78::MORPH_1_DE_BNC;
	MorphFunctions[RL78_IT_1_DF_BNZ] = &issRL78::MORPH_1_DF_BNZ;


	MorphFunctions[RL78_IT_1_EX_ONEB] = &issRL78::MORPH_1_EX_ONEB;
	MorphFunctions[RL78_IT_1_E4_ONEB] = &issRL78::MORPH_1_E4_ONEB;
	MorphFunctions[RL78_IT_1_E5_ONEB] = &issRL78::MORPH_1_E5_ONEB;
	MorphFunctions[RL78_IT_1_E6_ONEW] = &issRL78::MORPH_1_E6_ONEW;
	MorphFunctions[RL78_IT_1_E7_ONEW] = &issRL78::MORPH_1_E7_ONEW;
	MorphFunctions[RL78_IT_1_E8_MOV] = &issRL78::MORPH_1_E8_MOV;
	MorphFunctions[RL78_IT_1_E9_MOV] = &issRL78::MORPH_1_E9_MOV;
	MorphFunctions[RL78_IT_1_EA_MOVW] = &issRL78::MORPH_1_EA_MOVW;
	MorphFunctions[RL78_IT_1_EB_MOVW] = &issRL78::MORPH_1_EB_MOVW;
	MorphFunctions[RL78_IT_1_EC_BR] = &issRL78::MORPH_1_EC_BR;
	MorphFunctions[RL78_IT_1_ED_BR] = &issRL78::MORPH_1_ED_BR;
	MorphFunctions[RL78_IT_1_EE_BR] = &issRL78::MORPH_1_EE_BR;
	MorphFunctions[RL78_IT_1_EF_BR] = &issRL78::MORPH_1_EF_BR;


	MorphFunctions[RL78_IT_1_FX_CLRB] = &issRL78::MORPH_1_FX_CLRB;
	MorphFunctions[RL78_IT_1_F4_CLRB] = &issRL78::MORPH_1_F4_CLRB;
	MorphFunctions[RL78_IT_1_F5_CLRB] = &issRL78::MORPH_1_F5_CLRB;
	MorphFunctions[RL78_IT_1_F6_CLRW] = &issRL78::MORPH_1_F6_CLRW;
	MorphFunctions[RL78_IT_1_F7_CLRW] = &issRL78::MORPH_1_F7_CLRW;
	MorphFunctions[RL78_IT_1_F8_MOV] = &issRL78::MORPH_1_F8_MOV;
	MorphFunctions[RL78_IT_1_F9_MOV] = &issRL78::MORPH_1_F9_MOV;
	MorphFunctions[RL78_IT_1_FA_MOVW] = &issRL78::MORPH_1_FA_MOVW;
	MorphFunctions[RL78_IT_1_FB_MOVW] = &issRL78::MORPH_1_FB_MOVW;
	MorphFunctions[RL78_IT_1_FC_CALL] = &issRL78::MORPH_1_FC_CALL;
	MorphFunctions[RL78_IT_1_FD_CALL] = &issRL78::MORPH_1_FD_CALL;
	MorphFunctions[RL78_IT_1_FE_CALL] = &issRL78::MORPH_1_FE_CALL;
	MorphFunctions[RL78_IT_2_0X_ADD] = &issRL78::MORPH_2_0X_ADD;
	MorphFunctions[RL78_IT_2_09_ADDW] = &issRL78::MORPH_2_09_ADDW;


}


