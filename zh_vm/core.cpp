#include <iostream>
#include <vector>
#include "Hardware.h"
#include "Processor.h"
#include "types.h"

int main()
{
	Hardware hardware;
	Processor processor(hardware);
	
	Word program[] = {
		0b0010000000000001, // Set ax to 0101
		0b0010000001000001,
		0b1100100000000000, // Write ax to 100000000000b
		0b0010000000000000, // Clear ax
		0b0010000001000000,
		0b1101100000000000, // Read 100000000000b to ax
		0b1111000000000001, // Change to isa page 1
		0b1000000000000100, // jmpr 2
		0b0101000000000000, // Do not with ax
		0b1111000000000000, // Change to isa page 0
		0b1110000000000000
	};

	processor.eval(program, sizeof(program));

	// Should be 0101
	hardware.print_memory(0x800);
	// Should be fefe
	hardware.print_register(0);

#ifdef _WIN32
	getchar();
#endif
	return OK;
}