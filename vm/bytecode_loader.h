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
#ifndef VM_BYTECODE_LOADER_H_
#define VM_BYTECODE_LOADER_H_
#include <cstdint>
#include <string>

struct Instruction {
  std::uint8_t opcode;
  std::uint8_t op1;
  std::uint8_t op2;
  std::uint8_t op3;
}

class BytecodeLoader {
 private:
  Instruction *code = nullptr;

 public:
  bool load_bytecode(std::string file);
  Instruction *get_code();
};

#endif  // VM_BYTECODE_LOADER_H_
