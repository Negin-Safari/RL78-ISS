#include <iostream>
#include "Memory.h"
#include <systemc.h>
#include "issRL78.h"

SC_MODULE (embsystem)
{
public:
	sc_in <sc_logic> clk;

	sc_signal <sc_logic> memReady, readMem, writeMem;
	sc_signal <sc_logic> chsel;

	sc_signal <sc_lv <20>> addrBus;
	sc_signal <sc_lv<8>, SC_MANY_WRITERS> dataBus; 


	issRL78 *InstructionModuleEmb;
	memory <20,8> *memoryModule;

	SC_CTOR(embsystem)
	{
		InstructionModuleEmb = new issRL78("InstructionSetModuleEmb");
		(*InstructionModuleEmb) (clk, memReady, dataBus, readMem, writeMem, addrBus);
		
		memoryModule = new memory <20, 8>("memoryModule");
		(*memoryModule) (clk, readMem, writeMem, chsel, addrBus, dataBus, memReady, dataBus);

		SC_METHOD (Modeling);
			sensitive << clk;
	}
	void Modeling();


};

void embsystem::Modeling()
{
	chsel = SC_LOGIC_1;
}