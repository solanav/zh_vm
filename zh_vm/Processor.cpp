#include <stdio.h>
#include <iostream>
#include <string.h>
#include <stdlib.h>

#include "Hardware.h"
#include "Processor.h"
#include "types.h"

#ifdef _WIN32
#include <windows.h>
#endif

#ifdef __linux
#include <unistd.h>
#endif

#define ADDR_STR "[0x%.5x] "

struct Instruction {
	unsigned int instruction_id : 4;
	unsigned int op0 : 6;
	unsigned int op1 : 6;
	unsigned int memory_address : 12;
};

int bin_to_dec(unsigned char *bin, size_t size)
{
	size_t i;
	int result = 0, current_val = 1;

	for (i = size; i > 0; i--)
	{
		if (bin[i - 1] == '\x01')
			result += current_val;
		current_val *= 2;
	}

	return result;
}

Processor::Processor(Hardware assigned_hardware)
{
	hardware = assigned_hardware;
}

Status Processor::eval(unsigned char *program, size_t size)
{
	int instruction_offset = 0, offset = 0, continue_eval = OK, new_offset = -1;
	struct Instruction *ins;
	unsigned char tmp[INS_SIZE];

	ins = (struct Instruction *) calloc(1, sizeof(struct Instruction));
	if (!program || !ins)
		return ERROR;

	instruction_offset = 0;
	while (continue_eval) {
		// Load next instruction
		offset = instruction_offset;
		memcpy(tmp, program + offset, INS_ID_SIZE);
		ins->instruction_id = bin_to_dec(tmp, INS_ID_SIZE);

		offset += INS_ID_SIZE;
		memcpy(tmp, program + offset, INS_OP0_SIZE);
		ins->op0 = bin_to_dec(tmp, INS_OP0_SIZE);

		memcpy(tmp, program + offset, INS_ADDR_SIZE);
		ins->memory_address = bin_to_dec(tmp, INS_ADDR_SIZE);

		offset += INS_OP0_SIZE;
		memcpy(tmp, program + offset, INS_OP1_SIZE);
		ins->op1 = bin_to_dec(tmp, INS_OP1_SIZE);

		// Reset offset set by last instruction
		new_offset = -1;

		// Evaluate instruction
		switch (ins->instruction_id)
		{
		case 0:
			printf(ADDR_STR "exit\n", instruction_offset);
			continue_eval = ERROR;
			break;
		case 1:
			printf(ADDR_STR "movr %d, %d\n", instruction_offset, ins->op0, ins->op1);
			hardware.set_register(ins->op0, hardware.get_register(ins->op1));
			break;
		case 2:
			printf(ADDR_STR "movi %d, %d\n", instruction_offset, ins->op0, ins->op1);
			hardware.set_register(ins->op0, ins->op1);
			break;
		case 3:
			printf(ADDR_STR "addr %d, %d\n", instruction_offset, ins->op0, ins->op1);
			hardware.set_register(ins->op0, hardware.get_register(ins->op0) + hardware.get_register(ins->op1));
			break;
		case 4:
			printf(ADDR_STR "addi %d, %d [%d]\n", instruction_offset, ins->op0, ins->op1, hardware.get_register(ins->op0));
			hardware.set_register(ins->op0, hardware.get_register(ins->op0) + ins->op1);
			break;
		case 5:
			printf(ADDR_STR "subr %d, %d\n", instruction_offset, ins->op0, ins->op1);
			hardware.set_register(ins->op0, hardware.get_register(ins->op0) - hardware.get_register(ins->op1));
			break;
		case 6:
			printf(ADDR_STR "subi %d, %d\n", instruction_offset, ins->op0, ins->op1);
			hardware.set_register(ins->op0, hardware.get_register(ins->op0) - ins->op1);
			break;
		case 7:
			printf(ADDR_STR "jmp %x\n", instruction_offset, ins->memory_address);
			new_offset = ins->memory_address;
			break;
		case 8:
			printf(ADDR_STR "cmp %d %d [%d - %d]\n", 
				instruction_offset, 
				ins->op0, ins->op1, 
				hardware.get_register(ins->op0), 
				hardware.get_register(ins->op1));

			if (hardware.get_register(ins->op0) == hardware.get_register(ins->op1)) {
				hardware.set_flag(0, 1);
			}
			else if (hardware.get_register(ins->op0) > hardware.get_register(ins->op1)) {
				hardware.set_flag(0, 0);
				hardware.set_flag(1, 0);
			}
			else if (hardware.get_register(ins->op0) < hardware.get_register(ins->op1)) {
				hardware.set_flag(0, 0);
				hardware.set_flag(1, 1);
			}
			break;
		case 9:
			printf(ADDR_STR "ja %x\n", instruction_offset, ins->memory_address);
			if (hardware.get_flag(1) == 0 && hardware.get_flag(0) != 1)
				new_offset = ins->memory_address;
			break;
		case 10:
			printf(ADDR_STR "jb %x\n", instruction_offset, ins->memory_address);
			if (hardware.get_flag(1) == 1 && hardware.get_flag(0) != 1)
				new_offset = ins->memory_address;
			break;
		case 11:
			printf(ADDR_STR "je %x\n", instruction_offset, ins->memory_address);
			if (hardware.get_flag(0) == 1)
				new_offset = ins->memory_address;
			break;
		case 12:
			printf(ADDR_STR "wr %x (ra)\n", instruction_offset, ins->memory_address);
			hardware.set_memory(ins->memory_address, hardware.get_register(0));
			break;
		case 13:
			printf(ADDR_STR "re (ra) %x\n", instruction_offset, ins->memory_address);
			hardware.set_register(0, hardware.get_memory(ins->memory_address));
			break;
		default:
			printf("Instruction not found...\n");
			continue_eval = 0;
			break;
		}

		for (int z = 0; z < 8; z+=2) {
			printf("[%d] > %d\n", z, hardware.get_register(z));
		}

		// Check if instruction wants to change offset
		if (new_offset == -1)
			instruction_offset += INS_SIZE;
		else
			instruction_offset = new_offset;
#ifdef _WIN32
		Sleep(1000);
#endif

#ifdef linux
		sleep(1);
#endif
	}

	return OK;
}
