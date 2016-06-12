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

#include "vm/function_table.h"

namespace VM {

/**
 * Bytecode structure.
 */
struct ByteCode {
  std::uint8_t opcode;
  std::uint8_t op[3];
};

/**
 * A module: Representation of one bytecode module, corresponding to one
 * bytecode file. Contains function symbols and constant pool entries.
 */
class Module {
 public:
  ByteCode *code = nullptr;
  FunctionTable *funcs = nullptr;
  std::string module_name;
  std::uint64_t *constant_pool = nullptr;
  std::uint64_t num_instructions = 0;
  std::uint32_t num_functions = 0;
  std::uint16_t num_const = 0;
  std::uint16_t module_id = 0;

  explicit Module(std::uint16_t module_id);
  ~Module();
};

}  // namespace VM

#endif  // VM_MODULE_H_
