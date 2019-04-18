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

struct _Instruction {
	unsigned int instruction_id : 4;
	unsigned int op0 : 4;
	unsigned int op1 : 8;
	unsigned int memory_address : 12;
};

Status load_instruction(Word program, Instruction *ins)
{
	if (!ins)
		return ERROR;

	ins->instruction_id = get_bit(program, INS_SIZE - 4, 4);
	ins->op0 = get_bit(program, INS_SIZE - 8, 4);
	ins->op1 = get_bit(program, INS_SIZE - 16, 8);
	ins->memory_address = get_bit(program, INS_SIZE - 16, 12);

	return OK;
}

Status Processor::system_call(Word call_id)
{
	switch (call_id) {
	case 0:
		putchar(hardware.get_register(0));
		break;
	default:
		printf("[ERROR] System call not found");
		return ERROR;
	}

	return OK;
}

Processor::Processor(Hardware assigned_hardware)
{
	hardware = assigned_hardware;
}

Status Processor::eval(Word *program, size_t size)
{
	int pc_offset = 0;
	int load_offset = 0;
	int jmp_offset = -1;
	int isa_selected = 0;
	int continue_eval = OK;
	Instruction *ins;

	ins = (Instruction *)calloc(1, sizeof(Instruction));
	if (!program || !ins)
		return ERROR;

	// Load program into memory
	for (int i = 0; i < size; i++) {
		hardware.set_memory(i * 2, program[i]);
	}

	pc_offset = 0;
	while (continue_eval) {
		// Get next instruction divided in nice parts
		if (load_instruction(hardware.get_memory(pc_offset), ins) ==
			ERROR)
			return ERROR;

		// Reset load_offset set by last instruction
		jmp_offset = -1;

		// Evaluate instruction
		switch (ins->instruction_id + (isa_selected * 16)) {
		case 0:
			printf(ADDR_STR "nop\n", pc_offset);
			break;
		case 1:
			printf(ADDR_STR "movr %d, %d\n", pc_offset, ins->op0,
				ins->op1);
			hardware.set_register(
				ins->op0,
				(Word)hardware.get_register(ins->op1));
			break;
		case 2:
			printf(ADDR_STR "movi %d, %d\n", pc_offset, ins->op0,
				ins->op1);
			if (ins->op1 <= 0xFF)
				hardware.set_register(ins->op0, (Byte)ins->op1);
			else
				hardware.set_register(ins->op0, (Word)ins->op1);
			break;
		case 3:
			printf(ADDR_STR "addr %d, %d\n", pc_offset, ins->op0,
				ins->op1);
			hardware.set_register(
				ins->op0,
				(Word)(hardware.get_register(ins->op0) +
					hardware.get_register(ins->op1)));
			break;
		case 4:
			printf(ADDR_STR "addi %d, %d [%d]\n", pc_offset,
				ins->op0, ins->op1,
				hardware.get_register(ins->op0));
			hardware.set_register(
				ins->op0,
				(Word)(hardware.get_register(ins->op0) +
					ins->op1));
			break;
		case 5:
			printf(ADDR_STR "subr %d, %d\n", pc_offset, ins->op0,
				ins->op1);
			hardware.set_register(
				ins->op0,
				(Word)(hardware.get_register(ins->op0) -
					hardware.get_register(ins->op1)));
			break;
		case 6:
			printf(ADDR_STR "subi %d, %d\n", pc_offset, ins->op0,
				ins->op1);
			hardware.set_register(
				ins->op0,
				(Word)(hardware.get_register(ins->op0) -
					ins->op1));
			break;
		case 7:
			printf(ADDR_STR "jmp %x\n", pc_offset,
				ins->memory_address);
			pc_offset = ins->memory_address - 2;
			break;
		case 8:
			printf(ADDR_STR "cmp %d %d [%d - %d]\n", pc_offset,
				ins->op0, ins->op1,
				hardware.get_register(ins->op0),
				hardware.get_register(ins->op1));

			if (hardware.get_register(ins->op0) ==
				hardware.get_register(ins->op1)) {
				hardware.set_flag(0, 1);
			}
			else if (hardware.get_register(ins->op0) >
				hardware.get_register(ins->op1)) {
				hardware.set_flag(0, 0);
				hardware.set_flag(1, 0);
			}
			else if (hardware.get_register(ins->op0) <
				hardware.get_register(ins->op1)) {
				hardware.set_flag(0, 0);
				hardware.set_flag(1, 1);
			}
			break;
		case 9:
			printf(ADDR_STR "ja %x\n", pc_offset,
				ins->memory_address);
			if (hardware.get_flag(1) == 0 &&
				hardware.get_flag(0) != 1)
				pc_offset = ins->memory_address - 2;
			break;
		case 10:
			printf(ADDR_STR "jb %x\n", pc_offset,
				ins->memory_address);
			if (hardware.get_flag(1) == 1 &&
				hardware.get_flag(0) != 1)
				pc_offset = ins->memory_address - 2;
			break;
		case 11:
			printf(ADDR_STR "je %x\n", pc_offset,
				ins->memory_address);
			if (hardware.get_flag(0) == 1)
				pc_offset = ins->memory_address - 2;
			break;
		case 12:
			printf(ADDR_STR "wr %x (ra)\n", pc_offset,
				ins->memory_address);
			hardware.set_memory(ins->memory_address,
				hardware.get_register(0));
			break;
		case 13:
			printf(ADDR_STR "re (ra) %x\n", pc_offset,
				ins->memory_address);
			hardware.set_register(0, (Word)(hardware.get_memory(
				ins->memory_address)));
			break;
		case 14:
			printf(ADDR_STR "exit\n", pc_offset);
			continue_eval = ERROR;
			break;
		case 15:
			printf(ADDR_STR "isa %x\n", pc_offset,
				ins->memory_address);
			isa_selected = ins->memory_address;
			break;
		case 16:
			printf(ADDR_STR "push %x %x\n", pc_offset, ins->op0,
				ins->op1);
			hardware.set_stack(hardware.get_register(ins->op0),
				hardware.get_register(ins->op1));
			break;
		case 17:
			printf(ADDR_STR "pop %x %x\n", pc_offset, ins->op0,
				ins->op1);
			hardware.set_register(
				hardware.get_register(ins->op0),
				(Word)(hardware.get_stack(ins->op1)));
			break;
		case 18:
			printf(ADDR_STR "xor %x %x\n", pc_offset, ins->op0,
				ins->op1);
			hardware.set_register(
				ins->op0,
				(Word)(hardware.get_register(ins->op0) ^
					hardware.get_register(ins->op0)));
			break;
		case 19:
			printf(ADDR_STR "and %x %x\n", pc_offset, ins->op0,
				ins->op1);
			hardware.set_register(
				ins->op0,
				(Word)(hardware.get_register(ins->op0) &
					hardware.get_register(ins->op0)));
			break;
		case 20:
			printf(ADDR_STR "or %x %x\n", pc_offset, ins->op0,
				ins->op1);
			hardware.set_register(
				ins->op0,
				(Word)(hardware.get_register(ins->op0) |
					hardware.get_register(ins->op0)));
			break;
		case 21:
			printf(ADDR_STR "not %x\n", pc_offset, ins->op0);
			hardware.set_register(ins->op0,
				(Word)(~hardware.get_register(
					ins->memory_address)));
			break;
		case 22:
			printf(ADDR_STR "mul %x %x\n", pc_offset, ins->op0,
				ins->op1);
			hardware.set_register(
				ins->op0,
				(Word)(hardware.get_register(ins->op0) *
					hardware.get_register(ins->op0)));
			break;
		case 23:
			printf(ADDR_STR "div %x %x\n", pc_offset, ins->op0,
				ins->op1);
			hardware.set_register(
				ins->op0,
				(Word)(hardware.get_register(ins->op0) /
					hardware.get_register(ins->op0)));
			break;
		case 24:
			printf(ADDR_STR "jmpr %x\n", pc_offset,
				ins->memory_address);
			pc_offset += ins->memory_address - 2;
			break;
		case 25:
			printf(ADDR_STR "sc %x\n", pc_offset,
				ins->memory_address);
			system_call(ins->memory_address);
			break;
		case 31:
			printf(ADDR_STR "isa %x\n", pc_offset,
				ins->memory_address);
			isa_selected = ins->memory_address;
			break;
		default:
			printf("Instruction not found...\n");
			continue_eval = 0;
			break;
		}

		// Move pc to next instruction
		pc_offset += 2;

		Sleep(1000);
	}

	return OK;
}
