#pragma once
#define    VAL_INIT				0x0000
#define    PCregister			ST.PC
#define    PSW_reg				ST.psw_reg
#define    PrefixEn             ST.prefix
#define	   R_MMAP(Reg)		       getRegAddr(Reg, PSW_reg.RBS)
#define    X        0
#define    A        1
#define    C        2
#define    B        3
#define    E        4
#define    D        5
#define    L        6
#define    H        7
#define    saddr    8
#define    ES       9
#define    sfr      10
#define    SPH      11
#define    SPL      12
#define    CS       13
#define    PSW      14


typedef struct RL78Struct {
	struct psw {
		bool IE = 0;
		bool Z = 0;
		sc_lv<2> RBS = "00";
		bool AC = 0;
		bool ISP1 = 0, ISP0 = 0;
		bool CY = 0;

	}psw_reg;
	bool prefix = false;
	int adrSpace;
	sc_lv<20> PC;
	sc_lv<16> SP;
};

