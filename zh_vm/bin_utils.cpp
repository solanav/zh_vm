#include <stdio.h>
#include "bin_utils.h"
#include "Hardware.h"

Status set_bit(Byte data, Byte value, unsigned int i)
{
	printf(">> %x\n", value << i);
	data = data | (value << i);
	return data;
}

Byte get_bit(Byte data, unsigned int i)
{
	Byte result = 0b1;
	result = result << i;
	return data & result;
}