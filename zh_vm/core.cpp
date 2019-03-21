#include <iostream>
#include <vector>
#include "Hardware.h"
#include "types.h"

int main()
{
	Hardware hardware;
	//Byte test0 = 0xFF;
	Word test1 = 0xAABB;

	hardware.set_memory(1, test1);
	hardware.print_memory(1);

	return OK;
}