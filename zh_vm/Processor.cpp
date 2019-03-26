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

Instruction *load_instruction(int pc_offset, unsigned char *program)
{
	int load_offset;
	unsigned char tmp[INS_SIZE];

	struct Instruction *ins;

	ins = (struct Instruction *) calloc(1, sizeof(struct Instruction));
	if (!program || !ins)
		return ERROR;

	load_offset = pc_offset;
	memcpy(tmp, program + load_offset, INS_ID_SIZE);
	ins->instruction_id = bin_to_dec(tmp, INS_ID_SIZE);

	load_offset += INS_ID_SIZE;
	memcpy(tmp, program + load_offset, INS_OP0_SIZE);
	ins->op0 = bin_to_dec(tmp, INS_OP0_SIZE);

	memcpy(tmp, program + load_offset, INS_ADDR_SIZE);
	ins->memory_address = bin_to_dec(tmp, INS_ADDR_SIZE);

	load_offset += INS_OP0_SIZE;
	memcpy(tmp, program + load_offset, INS_OP1_SIZE);
	ins->op1 = bin_to_dec(tmp, INS_OP1_SIZE);

	return ins;
}

Processor::Processor(Hardware assigned_hardware)
{
	hardware = assigned_hardware;
}

Status Processor::eval(unsigned char *program, size_t size)
{
	Instruction *ins;
	int pc_offset = 0;
	int load_offset = 0;
	int jmp_offset = -1;
	int isa_selected = 0;
	int continue_eval = OK;

	pc_offset = 0;
	while (continue_eval) {
		ins = load_instruction(pc_offset, program);

		// Reset load_offset set by last instruction
		jmp_offset = -1;

		// Evaluate instruction
		switch (ins->instruction_id + (isa_selected * INS_SIZE))
		{
		case 0:
			printf(ADDR_STR "nop\n", pc_offset);
			break;
		case 1:
			printf(ADDR_STR "movr %d, %d\n", pc_offset, ins->op0, ins->op1);
			hardware.set_register(ins->op0, hardware.get_register(ins->op1));
			break;
		case 2:
			printf(ADDR_STR "movi %d, %d\n", pc_offset, ins->op0, ins->op1);
			if (ins->op1 <= 0xFF)
				hardware.set_register(ins->op0, (Byte)ins->op1);
			else 
				hardware.set_register(ins->op0, (Word)ins->op1);
			break;
		case 3:
			printf(ADDR_STR "addr %d, %d\n", pc_offset, ins->op0, ins->op1);
			hardware.set_register(ins->op0, hardware.get_register(ins->op0) + hardware.get_register(ins->op1));
			break;
		case 4:
			printf(ADDR_STR "addi %d, %d [%d]\n", pc_offset, ins->op0, ins->op1, hardware.get_register(ins->op0));
			hardware.set_register(ins->op0, hardware.get_register(ins->op0) + ins->op1);
			break;
		case 5:
			printf(ADDR_STR "subr %d, %d\n", pc_offset, ins->op0, ins->op1);
			hardware.set_register(ins->op0, hardware.get_register(ins->op0) - hardware.get_register(ins->op1));
			break;
		case 6:
			printf(ADDR_STR "subi %d, %d\n", pc_offset, ins->op0, ins->op1);
			hardware.set_register(ins->op0, hardware.get_register(ins->op0) - ins->op1);
			break;
		case 7:
			printf(ADDR_STR "jmp %x\n", pc_offset, ins->memory_address);
			jmp_offset = ins->memory_address;
			break;
		case 8:
			printf(ADDR_STR "cmp %d %d [%d - %d]\n", 
				pc_offset, 
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
			printf(ADDR_STR "ja %x\n", pc_offset, ins->memory_address);
			if (hardware.get_flag(1) == 0 && hardware.get_flag(0) != 1)
				jmp_offset = ins->memory_address;
			break;
		case 10:
			printf(ADDR_STR "jb %x\n", pc_offset, ins->memory_address);
			if (hardware.get_flag(1) == 1 && hardware.get_flag(0) != 1)
				jmp_offset = ins->memory_address;
			break;
		case 11:
			printf(ADDR_STR "je %x\n", pc_offset, ins->memory_address);
			if (hardware.get_flag(0) == 1)
				jmp_offset = ins->memory_address;
			break;
		case 12:
			printf(ADDR_STR "wr %x (ra)\n", pc_offset, ins->memory_address);
			hardware.set_memory(ins->memory_address, hardware.get_register(0));
			break;
		case 13:
			printf(ADDR_STR "re (ra) %x\n", pc_offset, ins->memory_address);
			hardware.set_register(0, hardware.get_memory(ins->memory_address));
			break;
		case 14:
			printf(ADDR_STR "exit\n", pc_offset);
			continue_eval = ERROR;
			break;
		case 15:
			printf(ADDR_STR "isa %x\n", pc_offset, ins->memory_address);
			isa_selected = ins->memory_address;
			break;
		case 16:
			printf(ADDR_STR "push %x %x\n", pc_offset, ins->op0, ins->op1);
			hardware.set_stack(ins->op0, hardware.get_register(ins->op1));
			break;
		case 17:
			printf(ADDR_STR "pop %x %x\n", pc_offset, ins->op0, ins->op1);
			hardware.set_register(ins->op0, hardware.get_stack(ins->op1));
			break;
		case 31:
			printf(ADDR_STR "isa %x\n", pc_offset, ins->memory_address);
			isa_selected = ins->memory_address;
			break;
		default:
			printf("Instruction not found...\n");
			continue_eval = 0;
			break;
		}

		// Check if instruction wants to change load_offset
		if (jmp_offset == -1)
			pc_offset += INS_SIZE;
		else
			pc_offset = jmp_offset;
#ifdef _WIN32
		Sleep(1000);
#endif

#ifdef linux
		sleep(1);
#endif
	}

	return OK;
}
