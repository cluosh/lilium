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
#ifndef VM_MODULE_H_
#define VM_MODULE_H_

#include <cstdint>
#include <string>
#include <vector>

#include "vm/types/types.h"

namespace VM {

/**
 * Bytecode structure.
 */
struct Instruction {
  uint8_t opcode;
  uint8_t op[3];
};

/**
 * Entry in the function table.
 */
struct FunctionTableEntry {
  std::string name;
  uint64_t address;
  Type type;
};

/**
 * A module: Representation of one bytecode module, corresponding to one
 * bytecode file. Contains function symbols and constant pool entries before
 * the linking stage.
 */
class Module {
 public:
  std::vector<Instruction> code;
  std::vector<FunctionTableEntry> functionTable;
  std::vector<uint64_t> constantPool;
  std::string module_name;

  explicit Module(uint16_t module_id);
  ~Module();
};

}  // namespace VM

#endif  // VM_MODULE_H_
