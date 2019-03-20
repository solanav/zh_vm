#include <iostream>
#include <vector>
#include "Hardware.h"

Hardware::Hardware()
{
	Memory_page memory_page;
	std::vector<Memory_page> list_memory_pages;

	// Create initial memory page
	memory_page = Hardware::create_memory_page();
	list_memory_pages.push_back(memory_page);

	// Create registers

	// Create flags

}

Memory_page Hardware::create_memory_page()
{
	unsigned char memory_page[PAGE_SIZE];
	return memory_page;
}