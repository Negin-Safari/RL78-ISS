#include <iostream>
#include <systemc.h>
//#include "InstructionTest.h"
#include "systemTest.h"

int sc_main(int argc, char *argv[])
{
	sc_report_handler::set_actions (SC_ID_VECTOR_CONTAINS_LOGIC_VALUE_,
                                SC_DO_NOTHING);
	sc_report_handler::set_actions (SC_WARNING, SC_DO_NOTHING);
	systemTest* TOP = new systemTest("systemTest_TB");

	sc_trace_file* VCDFile;
	VCDFile = sc_create_vcd_trace_file("system_Main");
	sc_trace(VCDFile, TOP -> clk, "clk");
	sc_start(1800,SC_NS);
	/*cout << "-------------------------MEMORY(A) = " << TOP->systemModule->memoryModule->mem[0xFFEF9] << " --------------------" << endl;
	cout << "-------------------------MEMORY(X) = " << TOP->systemModule->memoryModule->mem[0xFFEF8] << " --------------------" << endl;
	cout << "-------------------------MEMORY(B) = " << TOP->systemModule->memoryModule->mem[0xFFEFB] << " --------------------" << endl;
	cout << "-------------------------MEMORY(C) = " << TOP->systemModule->memoryModule->mem[0xFFEFA] << " --------------------" << endl;
	cout << "-------------------------MEMORY(D) = " << TOP->systemModule->memoryModule->mem[0xFFEFD] << " --------------------" << endl;
	cout << "-------------------------MEMORY(E) = " << TOP->systemModule->memoryModule->mem[0xFFEFC] << " --------------------" << endl;
	cout << "-------------------------MEMORY(H) = " << TOP->systemModule->memoryModule->mem[0xFFEFF] << " --------------------" << endl;
	cout << "-------------------------MEMORY(L) = " << TOP->systemModule->memoryModule->mem[0xFFEFE] << " --------------------" << endl;*/
	getchar();
	return 0;
}