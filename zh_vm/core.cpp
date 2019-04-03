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
		0b0010000000000001,
		0b0010000100000001,
		0b1100100000000000,
		0b1111000000000001,
		0b0010000000000000,
		0b1111000000000000,
		0b1101100000000000,
		0b0010000000000000,
		0b0010000101000001,
		0b1111000000000001,
		0b1001000000000000,
		0b1111000000000000,
		0b1110000000000000
	};

	if (processor.eval(program, sizeof(program)) == ERROR)
		printf("[ERROR] Eval failed...\n");

#ifdef _WIN32
	getchar();
#endif
	return OK;
}