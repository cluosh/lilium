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

#include "vm/data/program_buffer.h"

namespace VM {
namespace ByteCode {

/**
 * Class for storing methods allowing to generate bytecode.
 */
class Generator {
 private:
  std::ostream out;
  bool disabled;

 public:
  explicit Generator(const std::ostream &output);
  void setDisabled(bool disabled);

  // Code generation functions
  void moduleHeader(uint16_t numConstants, uint32_t numFunctions, uint64_t numInstructions);
  void functionTable(std::vector<Data::FunctionTableEntry> functionTable);
  void constantPool(const std::vector<uint64_t> &constantPool);
  void instruction(const Data::Instruction &instruction);
};

}  // namespace ByteCode
}  // namespace VM

#endif  // VM_BYTECODE_GENERATOR_H_
