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
	std::ifstream f(argv[2], std::ifstream::ate | std::ifstream::binary);
	std::ifstream::pos_type size = 0;
	if (f.is_open())
		size = f.tellg();
	if (size == 0)
		return EXIT_FAILURE;

	// Allocate code space
	std::uint8_t *code = new std::uint8_t[3 * size];

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

	// Main dispatch loop
	DP();
	while (1) {	
	op_add:
		reg[code[pc - 1 + 2]] += reg[code[pc - 1 + 1]];
		DP();
	op_sub:
		reg[code[pc - 1 + 2]] -= reg[code[pc - 1 + 1]];
		DP();
	op_mul:
		reg[code[pc - 1 + 2]] *= reg[code[pc - 1 + 1]];
		DP();
	op_hlt:
		std::cout << "Interpreter halted\n";
		return EXIT_SUCCESS;
	}

	// Should never be executed
	return EXIT_SUCCESS;
}
