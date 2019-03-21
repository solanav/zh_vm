#include <iostream>
#include <vector>
#include "Hardware.h"

Hardware::Hardware()
{
	// Create initial memory page
	memory_page = Hardware::create_memory_page();
	list_memory_pages.push_back(memory_page);

	// Create registers
	register_page = Hardware::create_register_page();
	list_register_pages.push_back(register_page);
}

Memory_page Hardware::create_memory_page()
{
	Byte *memory_page = (Byte *) calloc(PAGE_SIZE, sizeof(Byte));
	if (memory_page)
		return memory_page;

	return NULL;
}

Register_page Hardware::create_register_page()
{
	Byte *register_page = (Byte *) calloc(NUM_REGS, sizeof(Byte) * 2);
	if (register_page)
		return register_page;

	return NULL;
}

Status Hardware::set_register(Word register_index, Byte new_value)
{
	register_page[register_index] = new_value;
	return OK;
}

Status Hardware::set_register(Word register_index, Word new_value)
{
	Byte high, low;

	high = new_value / 0x100;
	low = new_value % 0x100;

	register_page[register_index] = high;
	register_page[register_index + 1] = low;

	return OK;
}

Word Hardware::get_register(Word register_index)
{
	if (register_index % 2 == 0)
		return register_page[register_index] * 0x100 + register_page[register_index + 1];

	return register_page[register_index];
}

Status Hardware::set_memory(Word memory_index, Byte new_value)
{
	memory_page[memory_index] = new_value;

	return OK;
}

Status Hardware::set_memory(Word memory_index, Word new_value)
{
	Byte high, low;

	high = new_value / 0x100;
	low = new_value % 0x100;

	memory_page[memory_index] = high;
	memory_page[memory_index + 1] = low;

	return OK;
}

Word Hardware::get_memory(Word memory_index)
{
	return memory_page[memory_index] * 0x100 + memory_page[memory_index + 1];
}

void Hardware::print_register(Word register_index)
{
	printf("0x%.2x\n", get_register(register_index));
}

void Hardware::print_memory(Word memory_index)
{
	printf("0x%.4x\n", get_memory(memory_index));
}