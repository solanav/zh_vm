#pragma once

#include "Hardware.h"

#define INS_SIZE 16
#define INS_ID_SIZE 4
#define INS_OP0_SIZE 6
#define INS_OP1_SIZE 6
#define INS_ADDR_SIZE 12

class Processor
{
private:
	Hardware hardware;

public:
	Processor(Hardware hardware);
	Status eval(unsigned char *program, size_t size);
};