#include <iostream>
#include <vector>
#include "Hardware.h"
#include "Processor.h"
#include "types.h"

int main()
{
	Hardware hardware;
	Processor processor(hardware);
	unsigned char program[] =
		"\x0\x0\x1\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x1"
		"\x0\x0\x1\x0\x0\x0\x0\x0\x0\x1\x0\x0\x0\x0\x0\x1"
		"\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0";

	processor.eval(program, sizeof(program));
	
	//hardware.set_register(0, (Byte)0x1);
	//hardware.set_register(1, (Byte)0x1);

	hardware.print_register(0);
	hardware.print_register(2);
	
#ifdef _WIN32
	getchar();
#endif
	return OK;
}