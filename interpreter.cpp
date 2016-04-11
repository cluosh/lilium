#include <cstdlib>
#include <cstdint>
#include <iostream>

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
#define DP() goto *token_table[code[pc++][0]]

/**
 * @brief Computed goto test
 */
int main() {	
	// Demo-opcodes
	static const std::uint8_t code[][3] = {
		{ OP_ADD, REG_00, REG_01},
		{ OP_ADD, REG_01, REG_00},
		{ OP_MUL, REG_00, REG_01},
		{ OP_HLT, REG_00, REG_01}
	};

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
		reg[code[pc - 1][2]] += reg[code[pc - 1][1]];
		DP();
	op_sub:
		reg[code[pc - 1][2]] -= reg[code[pc - 1][1]];
		DP();
	op_mul:
		reg[code[pc - 1][2]] *= reg[code[pc - 1][1]];
		DP();
	op_hlt:
		std::cout << "Interpreter halted\n";
		return EXIT_SUCCESS;
	}

	// Should never be executed
	return EXIT_SUCCESS;
}
