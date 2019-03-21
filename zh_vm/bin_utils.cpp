#include <stdio.h>
#include "bin_utils.h"

Status set_bit(Byte *data, unsigned long i)
{
	*data |= (1UL << i);
	return OK;
}

Status clear_bit(Byte *data, unsigned long i)
{
	*data &= ~(1UL << i);
	return OK;
}

Byte get_bit(Byte data, unsigned long i)
{
	return ((1UL << i) & data) != 0;
}