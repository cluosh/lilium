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
#ifndef VM_INTERPRETER_H_
#define VM_INTERPRETER_H_

#include <cstdint>
#include <memory>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "vm/module.h"

namespace VM {

/**
 * A function symbol table entry
 */
struct FuncSym {
  std::string name;
  uint32_t local_addr;
  uint16_t module_id;
};

/**
 * The main interpreter class. Execution, memory and symbol-management.
 */
class Interpreter {
 private:
  // Linking stage
  std::unordered_map<std::string, FuncSym> function_symbols;
  std::vector<std::unique_ptr<Module>> modules;
  std::vector<FuncSym> unresolved_symbols;
  uint16_t num_modules = 0;

  // Execution stage
  std::vector<Instruction> code;
  std::vector<uint64_t> functionTable;
  std::vector<uint64_t> constantPool;
  uint64_t entry_point;

 public:
  Interpreter();

  bool add_module(std::string filename);
  bool link();
  int execute();
};

}  // namespace VM

#endif  // VM_INTERPRETER_H_
