#include <iostream>
#include <vector>
#include "Hardware.h"
#include "bin_utils.h"

Hardware::Hardware()
{
	// Create initial memory page
	memory_page = Hardware::create_memory_page();
	list_memory_pages.push_back(memory_page);

	// Create registers
	register_page = Hardware::create_register_page();
	list_register_pages.push_back(register_page);

	// Create flags
	flag_page = Hardware::create_flag_page();
	list_flag_pages.push_back(flag_page);
}

Memory_page Hardware::create_memory_page()
{
	Byte *memory_page = (Byte *) calloc(PAGE_SIZE, sizeof(Byte));
	if (!memory_page)
		return NULL;

	return memory_page;
}

Register_page Hardware::create_register_page()
{
	Byte *register_page = (Byte *) calloc(NUM_REGS, sizeof(Byte) * 2);
	if (!register_page)
		return NULL;

	return register_page;
}

Flag_page Hardware::create_flag_page()
{
	Byte *flag_page = (Byte *) calloc(FLAG_ARRAY_SIZE, sizeof(Byte));
	if (!flag_page)
		return NULL;

	return flag_page;
}

Status Hardware::set_register(Word register_index, Byte new_value)
{
	if (register_index >= NUM_REGS)
		return ERROR;

	register_page[register_index] = new_value;
	return OK;
}

Status Hardware::set_register(Word register_index, Word new_value)
{
	Byte high, low;

	if (register_index >= NUM_REGS)
		return ERROR;

	high = new_value / 0x100;
	low = new_value % 0x100;

	register_page[register_index] = high;
	register_page[register_index + 1] = low;

	return OK;
}

Word Hardware::get_register(Word register_index)
{
	if (register_index >= NUM_REGS)
		return ERROR;

	if (register_index % 2 == 0)
		return register_page[register_index] * 0x100 + register_page[register_index + 1];

	return register_page[register_index];
}

Status Hardware::set_memory(Word memory_index, Byte new_value)
{
	if (memory_index >= PAGE_SIZE)
		return ERROR;

	memory_page[memory_index] = new_value;

	return OK;
}

Status Hardware::set_memory(Word memory_index, Word new_value)
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

Word Hardware::get_memory(Word memory_index)
{
	if (memory_index >= PAGE_SIZE)
		return ERROR;

	return memory_page[memory_index] * 0x100 + memory_page[memory_index + 1];
}

Status Hardware::set_flag(Word flag_index, Byte new_value)
{
	int bit, byte;
	Byte tmp = 0;

	if (flag_index >= NUM_FLAGS)
		return ERROR;

	byte = flag_index / 8;
	bit = flag_index - (byte * 8);

	(new_value == 0) ? clear_bit(&tmp, bit) : set_bit(&tmp, bit);

	flag_page[byte] = tmp;

	return OK;
}

Word Hardware::get_flag(Word flag_index)
{
	int bit, byte;

	if (flag_index >= NUM_FLAGS)
		return ERROR;

	byte = flag_index / 8;
	bit = flag_index - (byte * 8);

	return get_bit(flag_page[byte], bit);
}


void Hardware::print_register(Word register_index)
{
	printf("0x%.4x\n", get_register(register_index));
}

void Hardware::print_memory(Word memory_index)
{
	printf("0x%.4x\n", get_memory(memory_index));
}

void Hardware::print_flag(Word flag_index)
{
	printf("0x%x\n", get_flag(flag_index));
}