#pragma once
#include <stdint.h>
#include "types.h"

typedef uint8_t Byte;
typedef uint16_t Word;

Status set_bit(Word *data, unsigned long i);
Status clear_bit(Word *data, unsigned long i);
Word get_bit(Word data, unsigned long i, unsigned long n);
