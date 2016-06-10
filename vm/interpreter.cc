/*
 * Copyright (C) 2016  Michael Pucher (cluosh)
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */
#include <cstdint>
#include <iostream>
#include <fstream>

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
    return 1;

  // Find size of file
  std::ifstream f(argv[1], std::ifstream::ate | std::ifstream::binary);
  std::uint64_t size = 0;
  if (f.is_open())
    size = f.tellg();
  if (size == 0)
    return 1;
  size = size / 3 + 1;

  // Allocate code space
  std::uint8_t *code = new std::uint8_t[size];
  std::uint8_t *op0 = new std::uint8_t[size];
  std::uint8_t *op1 = new std::uint8_t[size];

  // Read bytecode from file
  f.seekg(std::ios::beg);
  std::uint64_t pos;
  for (pos = 0, size -= 1; f.good() && pos < size; pos++) {
    f.read(reinterpret_cast<char *>(&code[pos]), 1);
    f.read(reinterpret_cast<char *>(&op0[pos]), 1);
    f.read(reinterpret_cast<char *>(&op1[pos]), 1);
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
    && op_nop,
    && op_hlt,
    && op_add,
    && op_sub,
    && op_mul,
    && op_loa,
    && op_lov,
    && op_str
  };

  // Main dispatch loop
  DP();
  while (1) {
  op_nop:
    DP();
  op_hlt:
    std::cout << "Interpreter halted\n";
    break;
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
  op_loa:
  op_lov:
    // TODO(cluosh): Bad code, byte order?
    reg[op0[pc - 1]] = code[pc + 2];
    reg[op0[pc - 1]] = (reg[op0[pc - 1]] << 8) | op1[pc + 1];
    reg[op0[pc - 1]] = (reg[op0[pc - 1]] << 8) | op0[pc + 1];
    reg[op0[pc - 1]] = (reg[op0[pc - 1]] << 8) | code[pc + 1];
    reg[op0[pc - 1]] = (reg[op0[pc - 1]] << 8) | op1[pc];
    reg[op0[pc - 1]] = (reg[op0[pc - 1]] << 8) | op0[pc];
    reg[op0[pc - 1]] = (reg[op0[pc - 1]] << 8) | code[pc];
    reg[op0[pc - 1]] = (reg[op0[pc - 1]] << 8) | op1[pc - 1];
    std::cout << "OP_LOV " << reg[op0[pc - 1]] << "\n";
    pc += 2;
    DP();
  op_str:
    DP();
  }

  // Cleanup and exit
  delete[] code;
  delete[] op0;
  delete[] op1;
  return 0;
}
