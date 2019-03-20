#pragma once
#include <vector>
#include "bin_utils.h"

#define INS_SIZE 16
#define INS_ID_SIZE 4
#define INS_OP0_SIZE 6
#define INS_OP1_SIZE 6
#define INS_ADDR_SIZE 12

#define PAGE_SIZE 4096
#define NUM_PAGES 4096
#define REGS_SIZE INS_SIZE
#define NUM_REGS 64
#define NUM_FLAGS 32

typedef Byte *Memory_page;

class Hardware
{
private:
	std::vector<Memory_page> list_memory_pages;
	
public:
	Hardware();
	Memory_page create_memory_page();
	void set_bit(Byte data, Byte value, unsigned int i);
	Byte get_bit(Byte data, unsigned int i);
};

