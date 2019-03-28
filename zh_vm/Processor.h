#pragma once

#include "Hardware.h"

#define INS_SIZE 16
#define INS_ID_SIZE 4
#define INS_OP0_SIZE 6
#define INS_OP1_SIZE 6
#define INS_ADDR_SIZE 12

struct Instruction {
	unsigned int instruction_id : 4;
	unsigned int op0 : 6;
	unsigned int op1 : 6;
	unsigned int memory_address : 12;
};

class Processor
{
private:
	Hardware hardware;

public:
	Processor(Hardware hardware);
	Status eval(Word *program, size_t size);
	Instruction *load_instruction(int pc_offset, Word *program, Instruction *ins);
};