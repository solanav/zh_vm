#pragma once
#include "types.h"

typedef unsigned char Byte;

Status set_bit(Byte data, Byte value, unsigned int i);
Byte get_bit(Byte data, unsigned int i);
