#include <iostream>
#include <vector>
#include "Hardware.h"
#include "bin_utils.h"

Hardware::Hardware()
{
	// Create initial memory page
	memory_page = Hardware::create_memory_page();
	list_memory_pages.push_back(memory_page);

	// Create initial stack
	stack = Hardware::create_stack();
	list_stacks.push_back(stack);

	// Create registers
	register_page = Hardware::create_register_page();
	list_register_pages.push_back(register_page);

	// Create flags
	flag_page = Hardware::create_flag_page();
	list_flag_pages.push_back(flag_page);
}

Memory_page Hardware::create_memory_page()
{
	Byte *memory_page = (Byte *)calloc(PAGE_SIZE, sizeof(Byte));
	if (!memory_page)
		return NULL;

	return memory_page;
}

Register_page Hardware::create_register_page()
{
	Byte *register_page = (Byte *)calloc(NUM_REGS * 2, sizeof(Byte));
	if (!register_page)
		return NULL;

	return register_page;
}

Flag_page Hardware::create_flag_page()
{
	Byte *flag_page = (Byte *)calloc(FLAG_ARRAY_SIZE, sizeof(Byte));
	if (!flag_page)
		return NULL;

	return flag_page;
}

Stack Hardware::create_stack()
{
	Byte *stack = (Byte *)calloc(STACK_SIZE, sizeof(Byte));
	if (!stack)
		return NULL;

	return stack;
}

Status Hardware::set_register(unsigned long register_index, Word new_value)
{
	Byte high, low;

	if (register_index >= NUM_REGS)
		return ERROR;

	if (register_index % 3) { 
		// Full register
		high = new_value / 0x100;
		low = new_value % 0x100;

		register_page[register_index] = high;
		register_page[register_index + 1] = low;
	}
	else {
		// Half register
		register_page[register_index] = new_value;
	}

	return OK;
}

Word Hardware::get_register(unsigned long register_index)
{
	if (register_index >= NUM_REGS)
		return ERROR;

	if (register_index % 3) {
		// Full register
		return register_page[register_index] * 0x100 + register_page[register_index + 1];
	}
	else {
		// Half register
		return register_page[register_index];
	}
}

Status Hardware::set_memory(unsigned long memory_index, Byte new_value)
{
	if (memory_index >= PAGE_SIZE)
		return ERROR;

	memory_page[memory_index] = new_value;

	return OK;
}

Status Hardware::set_memory(unsigned long memory_index, Word new_value)
{
	Byte high, low;

	if (memory_index >= PAGE_SIZE)
		return ERROR;

	high = new_value / 0x100;
	low = new_value % 0x100;

	memory_page[memory_index] = high;
	memory_page[memory_index + 1] = low;

	return OK;
}

Word Hardware::get_memory(unsigned long memory_index)
{
	if (memory_index >= PAGE_SIZE)
		return ERROR;

	return memory_page[memory_index] * 0x100 + memory_page[memory_index + 1];
}

Status Hardware::set_flag(unsigned long flag_index, Byte new_value)
{
	int bit, byte;
	Word tmp = 0;

	if (flag_index >= NUM_FLAGS)
		return ERROR;

	byte = flag_index / 8;
	bit = flag_index - (byte * 8);

	(new_value == 0) ? clear_bit(&tmp, bit) : set_bit(&tmp, bit);

	flag_page[byte] = tmp;

	return OK;
}

Word Hardware::get_flag(unsigned long flag_index)
{
	int bit, byte;

	if (flag_index >= NUM_FLAGS)
		return ERROR;

	byte = flag_index / 8;
	bit = flag_index - (byte * 8);

	return get_bit(flag_page[byte], bit, 1);
}

Status Hardware::set_stack(unsigned long stack_index, Word new_value)
{
	Byte high, low;

	if (stack_index >= PAGE_SIZE)
		return ERROR;

	high = new_value / 0x100;
	low = new_value % 0x100;

	stack[stack_index] = high;
	stack[stack_index + 1] = low;

	return OK;
}

Word Hardware::get_stack(unsigned long stack_index)
{
	if (stack_index >= PAGE_SIZE)
		return ERROR;

	return stack[stack_index] * 0x100 + stack[stack_index + 1];
}

void Hardware::print_register(unsigned long register_index)
{
	printf("0x%.4x\n", get_register(register_index));
}

void Hardware::print_memory(unsigned long memory_index)
{
	printf("0x%.4x\n", get_memory(memory_index));
}

void Hardware::print_flag(unsigned long flag_index)
{
	printf("0x%x\n", get_flag(flag_index));
}

void Hardware::print_stack(unsigned long stack_index)
{
	printf("0x%.4x\n", get_stack(stack_index));
}