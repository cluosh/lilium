#include <cstdlib>
#include <cstdint>
#include <iostream>
#include <fstream>

// OPCODES
#define OP_ADD 0
#define OP_SUB 1
#define OP_MUL 2
#define OP_HLT 3
#define OP_LOA 4
#define OP_STR 5

#define REG_00 0
#define REG_01 1

// Dispatching macro
#define DP() goto *token_table[code[pc++]]

/**
 * @brief Computed goto test
 * @param argc Number of arguments
 * @param argv Arguments as strings
 */
int main(int argc, char *argv[]) {
	// Check if a parameter was given
	if (argc != 2)
		return EXIT_FAILURE;

	// Find size of file
	std::ifstream f(argv[1], std::ifstream::ate | std::ifstream::binary);
	std::uint64_t size = 0;
	if (f.is_open())
		size = f.tellg();
	if (size == 0)
		return EXIT_FAILURE;
	size = size / 3 + 1;

	// Allocate code space
	std::uint8_t *code = new std::uint8_t[size];
	std::uint8_t *op0 = new std::uint8_t[size];
	std::uint8_t *op1 = new std::uint8_t[size];

	// Read bytecode from file
	f.seekg(std::ios::beg);
	std::uint64_t pos;
	for (pos = 0, size -= 1; f.good() && pos < size; pos++) {
		f.read((char *)&code[pos], 1);
		f.read((char *)&op0[pos], 1);
		f.read((char *)&op1[pos], 1);
	}
	f.close();

	// Add halt operation at the end
	code[pos] = OP_HLT;
	op0[pos] = 0;
	op1[pos] = 0;

	// Registers
	static std::uint64_t reg[] = { 1, 1 };

	// Program counter
	std::uint64_t pc = 0;
	
	// Jump-table for token threaded code
	static const void *token_table[] = {
		&&op_add,
		&&op_sub,
		&&op_mul,
		&&op_hlt
	};

	std::cout << token_table[1] << "\n";

	// Main dispatch loop
	DP();
	while (1) {	
	op_add:
		reg[op1[pc - 1]] += reg[op0[pc - 1]];
		std::cout << "OP_ADD " << reg[0] << " " << reg[1] << "\n";
		DP();
	op_sub:
		reg[op1[pc - 1]] -= reg[op0[pc - 1]];
		std::cout << "OP_SUB " << reg[0] << " " << reg[1] << "\n";
		DP();
	op_mul:
		reg[op1[pc - 1]] *= reg[op0[pc - 1]];
		std::cout << "OP_MUL " << reg[0] << " " << reg[1] << "\n";
		DP();
	op_hlt:
		std::cout << "Interpreter halted\n";
		break;
	}

	// Cleanup and exit
	delete[] code;
	delete[] op0;
	delete[] op1;
	return EXIT_SUCCESS;
}
