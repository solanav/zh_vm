#pragma once
#include "types.h"

typedef unsigned char Byte; // at least 1 byte
typedef unsigned int Word; // at least 2 bytes

Status set_bit(Byte *data, unsigned long i);
Status clear_bit(Byte *data, unsigned long i);
Byte get_bit(Byte data, unsigned long i);
