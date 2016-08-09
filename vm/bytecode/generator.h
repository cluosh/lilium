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
#ifndef VM_BYTECODE_GENERATOR_H_
#define VM_BYTECODE_GENERATOR_H_

#include <ostream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "vm/bytecode/func_addr.h"
#include "vm/module.h"

namespace VM {

// Type definition for the constant pool
typedef std::vector<uint64_t> ConstPool;

/**
 * Class for storing methods allowing to generate bytecode.
 */
class Generator {
 private:
  std::ostream out;
  bool disabled;

 public:
  explicit Generator(const std::ostream &output);
  void set_disabled(bool disabled);

  // Code generation functions
  void module_header(std::string name, uint32_t num_func,
                     uint16_t num_const, uint64_t num_inst);
  void function_table(FuncAddr *func_addr);
  void constant_pool(ConstPool *const_pool);
  void instruction(const Instruction &bc);
};

}  // namespace VM

#endif  // VM_BYTECODE_GENERATOR_H_
