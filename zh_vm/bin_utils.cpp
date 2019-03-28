#include <stdio.h>
#include "bin_utils.h"

Status set_bit(Word *data, unsigned long i)
{
	*data |= (1UL << i);
	return OK;
}

Status clear_bit(Word *data, unsigned long i)
{
	*data &= ~(1UL << i);
	return OK;
}

Word get_bit(Word data, unsigned long i, unsigned long n)
{
	unsigned long mask = ((1 << n) - 1) << i;
	return (data & mask) >> i;
}