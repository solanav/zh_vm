#pragma once
#include <vector>
#include "bin_utils.h"

#define INS_SIZE 16
#define INS_ID_SIZE 4
#define INS_OP0_SIZE 6
#define INS_OP1_SIZE 6
#define INS_ADDR_SIZE 12

#define PAGE_SIZE 65536
#define NUM_PAGES 65536

#define REGS_SIZE INS_SIZE
#define NUM_REGS 64

#define FLAGS_SIZE
#define NUM_FLAGS 32

typedef Byte *Memory_page;
typedef Byte *Register_page;

class Hardware
{
private:
	Memory_page memory_page;
	Register_page register_page;

	// TODO: implement multiple pages
	std::vector<Memory_page> list_memory_pages;
	std::vector<Memory_page> list_register_pages;

	Word current_memory_page = 0;
	Word current_register_page = 0;

public:
	Hardware();
	Memory_page create_memory_page();
	Register_page create_register_page();

	Status set_register(Word register_index, Byte new_value);
	Status set_register(Word register_index, Word new_value);
	Word get_register(Word register_index);

	Status set_memory(Word memory_index, Byte new_value);
	Status set_memory(Word memory_index, Word new_value);
	Word get_memory(Word memory_index);

	// Debug
	void print_register(Word register_index);
	void print_memory(Word memory_index);
};

