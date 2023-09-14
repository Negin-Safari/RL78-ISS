#pragma once
#include <string>
#define ReaddataImmAdr_8(adr, Y)\
sc_lv<20> num = PrefixAddr();\
Y = Read1Byte(adr.to_uint() + (num).to_uint());\
cout<<"data is: "<< Y <<endl;\

#define Readdata_16(M,Y)\
sc_lv<20> num = PrefixAddr();\
dataA_8b = Read1Byte(M.to_uint() + (num).to_uint());\
dataB_8b = Read1Byte(M.to_uint() + (num).to_uint() + 1);\
Y = (dataB_8b.to_uint() * 256) + dataA_8b.to_uint();\
cout<<"pref isss: "<<num<<endl;\
cout<<"data is: "<< Y <<endl;\
cout<<"address is: "<< M <<endl;\

#define ReaddataOffset_16(M, b, Y)\
dataA_8b = Read1Byte(Read1Byte(R_MMAP(M)).to_uint() + b.to_uint());\
dataB_8b = Read1Byte(Read1Byte(R_MMAP(M)).to_uint() + b.to_uint() + 1);\
Y = (dataB_8b.to_uint() * 256) + dataA_8b.to_uint();\
cout<<"data is: "<< Y <<endl;\

#define ReadReg_16(M,N,Y)        /* 1 means 1 byte fetch */                \
dataA_8b = Read1Byte(R_MMAP(M));\
dataB_8b = Read1Byte(R_MMAP(N));\
Y = (dataB_8b.to_int() * 256) + dataA_8b.to_int();\
cout << " data is: " << Y << endl\

#define ReadReg_8(M,Y)        /* 1 means 1 byte fetch */                \
Y = Read1Byte(R_MMAP(M));\
//cout << "reg data is: " << Y << endl\

#define Readdata_8(M,Y)\
sc_lv<20> num = PrefixAddr();\
Y = Read1Byte(M.to_uint() + (num).to_uint());\
//cout<<"prefix is:"<<num<<endl;\
//cout<<"addr full is: "<<M.to_uint() + (num).to_uint()<<endl;\
//cout<<"data is: "<< Y <<endl;\

#define ReaddataOffsetReg_8(M, adrHL,Y)\
sc_lv<20> num = PrefixAddr();\
Y = Read1Byte(Read1Byte(R_MMAP(M)).to_uint() + adrHL.to_uint() + (num).to_uint());\
cout<<"data is: "<< Y <<endl;\

#define ReaddataOffsetbyte_8(M, adrHL,Y)\
Y = Read1Byte(Read1Byte(R_MMAP(M)).to_uint() + adrHL.to_uint());\
cout<<"data is: "<< Y <<endl;\



#define ReaddataOffsetReg_16(M, adrHL,Y)\
sc_lv<20> num = PrefixAddr();\
sc_lv<8> dl = Read1Byte(Read1Byte(R_MMAP(M)).to_uint() + adrHL.to_uint() + (num).to_uint());\
sc_lv<8> dh = Read1Byte(Read1Byte(R_MMAP(M)).to_uint() + adrHL.to_uint() + (num).to_uint() + 1);\
Y = dh.to_uint()* 256 + dl.to_uint();\
cout<<"data is: "<< Y <<endl;\

#define ReaddataOffsetbyte_16(M, adrHL,Y)\
sc_lv<8> dl = Read1Byte(Read1Byte(R_MMAP(M)).to_uint() + adrHL.to_uint());\
sc_lv<8> dh = Read1Byte(Read1Byte(R_MMAP(M)).to_uint() + adrHL.to_uint() + 1);\
Y = dh.to_uint()* 256 + dl.to_uint();\
cout<<"data is: "<< Y <<endl;\


#define ReaddataOffset2Reg_8(M, N, adrHL,Y)\
sc_lv<20> num = PrefixAddr();\
Y = Read1Byte(Read1Byte(R_MMAP(M)).to_uint() + ((R_MMAP(N)).to_uint() * 256) + adrHL.to_uint() + (num).to_uint());\
cout<<"data is: "<< Y <<endl;\

#define Readdata2Reg_8(M, N, Y)\
sc_lv<20> num = PrefixAddr();\
Y = Read1Byte(Read1Byte(R_MMAP(M)).to_uint() + ((R_MMAP(N)).to_uint() * 256) + (num).to_uint());\
cout<<"data is: "<< Y <<endl;\

#define Readdata2Reg_16(M, N, Y)\
sc_lv<20> num = PrefixAddr();\
sc_lv<8> dl = Read1Byte(Read1Byte(R_MMAP(M)).to_uint() + ((R_MMAP(N)).to_uint() * 256) + (num).to_uint());\
sc_lv<8> dh = Read1Byte(Read1Byte(R_MMAP(M)).to_uint() + ((R_MMAP(N)).to_uint() * 256) + (num).to_uint() + 1);\
Y = dh.to_uint()* 256 + dl.to_uint();\
cout<<"data is: "<< Y <<endl;\



#define ReaddataOffset2Reg_16(M, N, adrHL,Y)\
sc_lv<20> num = PrefixAddr();\
sc_lv<8> dl = Read1Byte(Read1Byte(R_MMAP(M)).to_uint() + ((R_MMAP(N)).to_uint() * 256) + adrHL.to_uint() + (num).to_uint());\
sc_lv<8> dh = Read1Byte(Read1Byte(R_MMAP(M)).to_uint() + ((R_MMAP(N)).to_uint() * 256) + adrHL.to_uint() + (num).to_uint() + 1);\
Y = dh.to_uint()* 256 + dl.to_uint();\
cout<<"data is: "<< Y <<endl;\


#define ReaddataOffsetAddr_8(M, b, Y)\
sc_lv<20> num = PrefixAddr();\
dataA_8b = Read1Byte((M).to_uint() + b.to_uint() + (num).to_uint());\
Y = dataA_8b.to_uint();\
cout<<"data is: "<< Y <<endl;\

#define ReaddataOffsetSP_8(b, Y)\
sc_lv<16> adr;\
ReadSP(adr);\
Y = Read1Byte((adr).to_uint() + b.to_uint() + 0xF0000);\
cout<<"data is: "<< Y <<endl;\

#define ReaddataOffsetSP_16(b, Y)\
sc_lv<16> adr;\
ReadSP(adr);\
sc_lv<8> dl = Read1Byte((adr).to_uint() + b.to_uint() + 0xF0000);\
sc_lv<8> dh = Read1Byte((adr).to_uint() + b.to_uint() + 0xF0000 + 1);\
Y = dh.to_uint()* 256 + dl.to_uint();\
cout<<"data is: "<< Y <<endl;\



#define ReaddataOffset_8(M, b, Y)\
dataA_8b = Read1Byte(Read1Byte(R_MMAP(M)).to_uint() + b.to_uint());\
Y =  dataA_8b.to_uint();\
cout<<"data is: "<< Y <<endl;\

#define Immediate2Byte(addr)\
addr = Byte3.to_uint() * 256 + Byte2.to_uint();\
//cout<<"addrHL:  "<<addr<<endl;\

#define ImmediateData2Byte(addr)\
addr = Byte4.to_uint() * 256 + Byte3.to_uint();\
cout<<"addrHL:  "<<addr<<endl;\

#define DO_Addition(op1, op2, result, n)\
result =  op1.to_int() + op2.to_int();\
//cout << "addition result: " << result << endl;\

#define DO_Multiplication(op1, op2, result)\
result =  op1.to_uint() * op2.to_uint();\
cout << "addition result: " << result << endl;\

#define DO_AdditionC(op1, op2, result)\
cout<<"CY is: "<<PSW_reg.CY<<endl;\
result =  op1.to_int() + op2.to_int() + PSW_reg.CY;\
cout << "addition result: " << result << endl;\

#define DO_Subtraction(op1, op2, result)\
result =  op1.to_int() - op2.to_int();\
cout << "subtraction result: " << result << endl;\

#define DO_CMP0(op1, result)\
result =  op1.to_int() - 0;\
cout << "subtraction result: " << result << endl;\

#define DO_SubtractionC(op1, op2, result)\
cout<<"CY is: "<<PSW_reg.CY<<endl;\
result =  op1.to_int() - op2.to_int() - PSW_reg.CY;\
cout << "subtraction result: " << result << endl;\

#define DO_AND(op1, op2, result)\
result =  op1.to_int() & op2.to_int();\
cout << "and result: " << result << endl;\

#define DO_OR(op1, op2, result)\
result =  op1.to_int() | op2.to_int();\
cout << "or result: " << result << endl;\

#define DO_XOR(op1, op2, result)\
result =  op1.to_int() ^ op2.to_int();\
cout << "xor result: " << result << endl;\

#define DO_INC(op1, result)\
result =  op1.to_int() + 1;\
//cout << "inc result: " << result << endl;\

#define DO_DEC(op1, result)\
result =  op1.to_int() - 1;\
cout << "dec result: " << result << endl;\

#define WritedataImmAdr_8(adrHL,Y){\
sc_lv<20> num = PrefixAddr();\
Write1Byte(Y, adrHL.to_uint() + (num).to_uint());\
}


#define WritedataOffsetbyte_8(M, adrHL,Y)\
Write1Byte(Y, Read1Byte(R_MMAP(M)).to_uint() + adrHL.to_uint());\

#define WritedataOffsetSP_8(b, Y)\
sc_lv<16> adr;\
ReadSP(adr);\
Write1Byte(Y, (adr).to_uint() + b.to_uint() + 0xF0000);\

#define Writedata2Reg_8(M, N, Y)\
sc_lv<20> num = PrefixAddr();\
Write1Byte(Y, Read1Byte(R_MMAP(M)).to_uint() + ((R_MMAP(N)).to_uint() * 256) + (num).to_uint());\

#define WritedataOffsetReg_8(M, adrHL,data)\
sc_lv<20> num = PrefixAddr();\
Write1Byte(data ,Read1Byte(R_MMAP(M)).to_uint() + adrHL.to_uint() + (num).to_uint());\

#define WritedataOffset2Reg_8(M, N, adrHL,data)\
sc_lv<20> num = PrefixAddr();\
Write1Byte(data, Read1Byte(R_MMAP(M)).to_uint() + ((R_MMAP(N)).to_uint() * 256) + adrHL.to_uint() + (num).to_uint());\

#define WritedataOffset2Reg_16(M, N, adrHL,data)\
sc_lv<20> num = PrefixAddr();\
Write1Byte(data.range(7,0), Read1Byte(R_MMAP(M)).to_uint() + ((R_MMAP(N)).to_uint() * 256) + adrHL.to_uint() + (num).to_uint());\
Write1Byte(data.range(15,8), Read1Byte(R_MMAP(M)).to_uint() + ((R_MMAP(N)).to_uint() * 256) + adrHL.to_uint() + (num).to_uint() + 1);\

#define WritedataOffsetReg_16(M, adrHL,data)\
sc_lv<20> num = PrefixAddr();\
Write1Byte(data.range(15,8) ,Read1Byte(R_MMAP(M)).to_uint() + adrHL.to_uint() + (num).to_uint() + 1);\
Write1Byte(data.range(7,0) ,Read1Byte(R_MMAP(M)).to_uint() + adrHL.to_uint() + (num).to_uint());\

#define Writedata2Reg_16(M, N, data)\
sc_lv<20> num = PrefixAddr();\
Write1Byte(data.range(7,0), Read1Byte(R_MMAP(M)).to_uint() + ((R_MMAP(N)).to_uint() * 256) + (num).to_uint());\
Write1Byte(data.range(15,8), Read1Byte(R_MMAP(M)).to_uint() + ((R_MMAP(N)).to_uint() * 256) + (num).to_uint() + 1);\


#define WritedataOffsetbyte_16(M, adrHL,data)\
Write1Byte(data.range(7,0), Read1Byte(R_MMAP(M)).to_uint() + adrHL.to_uint());\
Write1Byte(data.range(15,8), Read1Byte(R_MMAP(M)).to_uint() + adrHL.to_uint() + 1);\



#define WriteReg_16(M,N,result)        /* 1 means 1 byte fetch */                \
Write1Byte(result.range(7, 0),R_MMAP(M));\
Write1Byte(result.range(15, 8),R_MMAP(N))


#define Writedata_16(M,data){\
sc_lv<20> num = PrefixAddr();\
Write1Byte(data.range(7,0), M.to_uint() + (num).to_uint());\
Write1Byte(data.range(15,8), M.to_uint() + (num).to_uint() + 1);\
}


#define WritedataOffsetSP_16(b, data)\
sc_lv<16> adr;\
ReadSP(adr);\
Write1Byte(data.range(7,0), (adr).to_uint() + b.to_uint() + 0xF0000);\
Write1Byte(data.range(15,8), (adr).to_uint() + b.to_uint() + 0xF0000 + 1);\


#define WriteReg_8(M,result)        /* 1 means 1 byte fetch */                \
Write1Byte(result.range(7, 0),R_MMAP(M));\

#define WritedataOffset_8(M, b, result)\
Write1Byte(result.range(7, 0), Read1Byte(R_MMAP(M)).to_uint() + b.to_uint());\

#define UpdateFlags_add(op1, op2, result, n)\
PSW_reg.AC=fillAC(op1.bit(4), op2.bit(4), result.bit(3));\
PSW_reg.Z=fillZ(result.to_int());\
PSW_reg.CY=fillCY(op1.range(n, n), op2.range(n, n), result.range(n, n), false);\ 

#define UpdatePSW(){\
sc_lv<8> psw;\
psw = (PSW_reg.IE * 128) + (PSW_reg.Z * 64) + ((PSW_reg.RBS.range(1, 1)).to_uint() * 32) + (PSW_reg.AC * 16) + ((PSW_reg.RBS.range(0, 0)).to_uint() * 8) + (PSW_reg.ISP1 * 4) + (PSW_reg.ISP0 * 2) + PSW_reg.CY;\
Write1Byte(psw, 0xFFFFA);\
cout << "CY AC Z flags:  " << PSW_reg.CY<<PSW_reg.AC<<PSW_reg.Z << endl;\
}

#define UpdateSP(data){\
Reg = SPL;\
Write1Byte(data.range(7, 0), R_MMAP(Reg));\
Reg = SPH;\
Write1Byte(data.range(15, 8), R_MMAP(Reg));\
}

#define Pop2ByteSP(d){\
sc_lv<20> spAdr;\
Reg = SPH;\
sc_lv<8> sph = Read1Byte(R_MMAP(Reg));\
Reg = SPL;\
sc_lv<8> spl = Read1Byte(R_MMAP(Reg));\
spAdr = (sph.to_uint() * 256) + spl.to_uint() + 0xF0000;\
cout << "SPPPP isss" << spAdr << endl;\
sc_lv<8> d0 = Read1Byte(spAdr);\
sc_lv<8> d1 = Read1Byte(spAdr.to_uint() + 1);\
sc_lv<16> d = d1.to_uint() * 256 + d0.to_uint();\
sc_lv<20> new_spAdr = spAdr.to_uint() + 2;\
UpdateSP(new_spAdr);\
cout<<"pop done"<<endl;\
}

#define Push2ByteSP(d){\
sc_lv<8> dl = d.range(7,0);\
sc_lv<8> dh = d.range(15, 8);\
sc_lv<20> spAdr;\
Reg = SPH;\
sc_lv<8> sph = Read1Byte(R_MMAP(Reg));\
Reg = SPL;\
sc_lv<8> spl = Read1Byte(R_MMAP(Reg));\
spAdr = (sph.to_uint() * 256) + spl.to_uint() + 0xF0000;\
Write1Byte(dh, spAdr.to_uint() - 1);\
Write1Byte(dl, spAdr.to_uint() - 2);\
sc_lv<20> new_spAdr = spAdr.to_uint() - 2;\
UpdateSP(new_spAdr);\
cout<<"push done"<<endl;\
}

#define CallT(addr){\
Push2ByteSP(0, PCregister.range(19, 16).to_uint());\
Push2ByteSP(PCregister.range(15, 8).to_uint(), PCregister.range(7, 0).to_uint());\
PCregister = (addr.to_uint()) + (addr.to_uint() + 1) * 256;\
cout<<"CallT done"<<endl;\
}

#define Ret(){\
sc_lv<16> a;\
sc_lv<16> b;\
Pop2ByteSP(a);\
Pop2ByteSP(b);\
PCregister = a.to_uint() + ((b.range(3, 0)).to_uint() * 65536);\
}

#define Br(cond){\
if (cond) {\
	PCregister = PCregister.to_uint() + Byte2.to_int();\
}\
}

#define BrN(cond){\
if (!cond) {\
	PCregister = PCregister.to_int() + Byte2.to_int();\
cout<<"Branched to: "<<PCregister<<endl;\
}\
else{\
cout << "Didnt Branch" << endl;\
}\
}

#define ReadSP(s){\
Reg = SPH;\
sc_lv<8> sph = Read1Byte(R_MMAP(Reg));\
Reg = SPL;\
sc_lv<8> spl = Read1Byte(R_MMAP(Reg));\
s = (sph.to_uint() * 256) + spl.to_uint();\
}

#define UpdateFlags_8b_add(op1, op2, result){\
PSW_reg.AC=fillAC(op1.bit(4), op2.bit(4), result.bit(3));\
PSW_reg.Z=fillZ(result.to_int());\
PSW_reg.CY=fillCY(op1.range(7, 7), op2.range(7, 7), result.range(7, 7), false);\
UpdatePSW();\
}

#define UpdateFlags_8b_sub(op1, op2, result) {\
PSW_reg.AC=(op1.range(3, 0).to_uint() < op2.range(3, 0).to_uint()) ? 1 : 0;\
PSW_reg.Z=fillZ(result.to_int());\
PSW_reg.CY=fillCY(op1.range(7, 7), op2.range(7, 7), result.range(7, 7), true);\
UpdatePSW();\
}

#define UpdateFlags_16b_add(op1, op2, result) {\
	PSW_reg.AC=fillAC(op1.bit(4), op2.bit(4), result.bit(3));\
	PSW_reg.Z=fillZ(result.to_int());\
	PSW_reg.CY=fillCY(op1.range(15, 15), op2.range(15, 15), result.range(15, 15), false);\
	UpdatePSW();\
}

#define UpdateFlags_16b_sub(op1, op2, result) {\
	PSW_reg.AC = (op1.range(3, 0).to_uint() < op2.range(3, 0).to_uint()) ? 1 : 0;\
	PSW_reg.Z=fillZ(result.to_int());\
	PSW_reg.CY=fillCY(op1.range(15, 15), op2.range(15, 15), result.range(15, 15), true);\
	UpdatePSW();\
}

#define UpdateFlags_logic(result) {\
	PSW_reg.Z=fillZ(result.to_int());\
	UpdatePSW();\
}

#define UpdateFlags_inc(op1, result) {\
     PSW_reg.Z=fillZ(result.to_int());\
    sc_lv<1> z = "0";\
	 PSW_reg.AC=fillAC(op1.bit(4), z.bit(0), result_8b.bit(4));\
	UpdatePSW();\
}

#define UpdateFlags_dec(op1, result) {\
    PSW_reg.Z=fillZ(result.to_int());\
	PSW_reg.AC = (dataA_8b.range(3, 0).to_uint() < 1) ? 1 : 0;\
	UpdatePSW();\
}

#define UpdateFlags_cmp0(result) {\
    PSW_reg.CY = 0;\
    PSW_reg.Z=fillZ(result.to_uint());\
    PSW_reg.AC = 0;\
	UpdatePSW();\
}

#define sfrUpdates(b){\
if (Byte2 == 0xFA) {\
	cout << "PSW inst" << endl;\
	PSW_reg.IE = (b.range(7, 7)).to_uint();\
	PSW_reg.Z = (b.range(6, 6)).to_uint();\
	PSW_reg.RBS.range(1, 1) = (b.range(5, 5)).to_uint();\
	PSW_reg.AC = (b.range(4, 4)).to_uint();\
	PSW_reg.RBS.range(0, 0) = (b.range(3, 3)).to_uint();\
	PSW_reg.ISP1 = (b.range(2, 2)).to_uint();\
	PSW_reg.ISP0 = (b.range(1, 1)).to_uint();\
	PSW_reg.CY = (b.range(0, 0)).to_uint();\
}\
else if (Byte2 == 0xFD) {\
	cout << "ES inst" << endl;\
}\
else if (Byte2 == 0xFC) {\
	cout << "CS inst" << endl;\
}\
}



inline bool fillZ(int result_9b)
{
	bool out;
	if ((result_9b == 0))
	{
		out = 1;
	}
	else
		out = 0;
	return out;
}



inline bool fillCY(sc_lv<1> msb1, sc_lv<1> msb2, sc_lv<1> msbO, bool sub) { // n-1-1-1  sc_logic op1, sc_logic op2, sc_logic result
	bool out;
	if (sub == false)
	{
		if ((msb1.bit(0) == SC_LOGIC_1) && (msb2.bit(0) == SC_LOGIC_1) && (msbO.bit(0) == SC_LOGIC_0))
		{
			out = 1;
		}
		else if ((msb1.bit(0) == SC_LOGIC_0) && (msb2.bit(0) == SC_LOGIC_0) && (msbO.bit(0) == SC_LOGIC_1))
		{
			out = 1;
		}
		else
			out = 0;
	}
	else
	{
		if ((msb1.bit(0) == SC_LOGIC_0) && (msb2.bit(0) == SC_LOGIC_1) && (msbO.bit(0) == SC_LOGIC_1))
		{
			out = 1;
		}
		else if ((msb1.bit(0) == SC_LOGIC_1) && (msb2.bit(0) == SC_LOGIC_0) && (msbO.bit(0) == SC_LOGIC_0))
		{
			out = 1;
		}
		else
			out = 0;
	}
	return out;
}


inline bool fillAC(sc_logic op1, sc_logic op2, sc_logic result)
{
	bool out;
	if ((op1 == 1) && (op2 == 1) && (result == 1))
	{
		out = 1;
	}
	else if ((op1 == 1) && (op2 == 0) && (result == 0))
	{
		out = 1;
	}
	else if ((op1 == 0) && (op2 == 1) && (result == 0))
	{
		out = 1;
	}
	else if ((op1 == 0) && (op2 == 0) && (result == 1))
	{
		out = 1;
	}
	else
		out = 0;

	return out;
}



inline sc_lv<20> getRegAddr(int operand, sc_lv<2> Bank_Sel) {

	sc_lv<20> addr;
	if (operand == saddr) {
		addr = 0xFFE20;
		return addr;
	}
	else if (operand == ES) {
		addr = 0xFFFFD;
		return addr;
	}
	else if (operand == sfr) {
		addr = 0xFFF00;
		return addr;
	}
	else if (operand == SPH) {
		addr = 0xFFFF9;
		return addr;
	}
	else if (operand == SPL) {
		addr = 0xFFFF8;
		return addr;
	}
	else if (operand == CS) {
		addr = 0xFFFFC;
		return addr;
	}
	else if (operand == PSW) {
		addr = 0xFFFFA;
		return addr;
	}

	switch (Bank_Sel.to_uint()) {
	case 0: addr = 0xFFEF8; break;
	case 1: addr = 0xFFEF0; break;
	case 2: addr = 0xFFEE8; break;
	case 3: addr = 0xFFEE0; break;
	default:				break;
	}

	addr = addr.to_int() + operand;
	return addr;
}




