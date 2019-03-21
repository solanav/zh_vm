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
		"\x0\x0\x1\x0" "\x0\x0\x0\x0\x0\x1\x0\x0\x0\x0\x1\x0"
		"\x0\x0\x0\x0" "\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0";

	processor.eval(program, sizeof(program));
	hardware.print_register(BX);

	while (1) {}

	return OK;
}