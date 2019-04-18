#include <iostream>
#include <vector>
#include <fstream>
#include "Hardware.h"
#include "Processor.h"
#include "types.h"

std::vector<Word> read_program_file(std::string file_name)
{
	std::vector<Word> program;
	std::ifstream input(file_name, std::ios::binary);
	char buffer[2];

	for (size_t i = 0; i < 1024; i += 2)
	{
		input.read(buffer, sizeof(buffer));

		Word ins = (buffer[0] << 8) + buffer[1];
		program.push_back(ins);

		// If ins is exit, save it and stop reading
		if (ins == 0b1110000000000000)
			break;
	}

	input.close();
	 
	return program;
}

int main(int argc, char *argv[])
{
	std::vector<Word> program;
	Hardware hardware;
	Processor processor(hardware);

	if (argc < 2) {
		printf("[ERROR] Please introduce the file path as first parameter... [%s]\n", argv[0]);
		getchar();
		return ERROR;
	}

	program = read_program_file(argv[1]);

	if (processor.eval(program.data(), program.size()) == ERROR)
		printf("[ERROR] Eval failed...\n");

#ifdef _WIN32
	getchar();
#endif

	return OK;
}