#include <iostream>
#include <vector>
#include "Hardware.h"

int main()
{
	Hardware hardware;
	Memory_page memory_page;

	memory_page = hardware.create_memory_page();

	Byte test = 0b0;
	set_bit(test, 0b1, 7);
	printf("%x\n", test);

	while (1) {}
}