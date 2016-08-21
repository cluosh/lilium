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
#include <algorithm>

#include "vm/bytecode/generator.h"
#include "vm/data/vm_common.h"

namespace VM {
namespace ByteCode {

/**
 * Initialize the generator with an output stream.
 *
 * @param output Output stream where the bytecode is written to
 */
Generator::Generator(const std::ostream &output) : out(output.rdbuf()) {}

/**
 * Disable/Enable code generation.
 *
 * @param disable Disable/Enable code generation
 */
void Generator::setDisabled(bool disabled) {
  this->disabled = disabled;
}

/**
 * Generate the bytecode header for a module file.
 *
 * @param numFunctions Number of function table entries
 * @param numConstants Number of constant pool entries
 * @param numInstructions Number of instructions (code size)
 */
void Generator::moduleHeader(uint16_t numConstants,
                             uint32_t numFunctions,
                             uint64_t numInstructions) {
  // Write the magic number
  out.write("\x4C\x49", 2);

  // Write number of constants
  out.write(reinterpret_cast<const char *>(Data::buffer_u16(numConstants).data()), 2);

  // Write number of function table entries
  // Only the lower 24 bit are being used, use 32 bit anyways
  out.write(reinterpret_cast<const char *>(Data::buffer_u32(numFunctions).data()), 4);

  // Write number of total instructions
  out.write(reinterpret_cast<const char *>(Data::buffer_u64(numInstructions).data()), 8);
}

/**
 * Write a function table to the file, based on function addresses.
 *
 * @param functionTable The function addresses, types and names
 */
void Generator::functionTable(std::vector<Data::FunctionTableEntry> functionTable) {
  uint8_t nameLength;
  uint8_t parameterCount;

  // Sort the function table by the addresses
  const auto &func = [](const VM::Data::FunctionTableEntry &a,
                        const VM::Data::FunctionTableEntry &b) -> bool {
    return a.address < b.address;
  };
  std::sort(functionTable.begin(), functionTable.end(), func);

  // Write table entries as bytecode
  for (const auto &function : functionTable) {
    // Write the header of the function (length of name and count of parameters)
    // NOTE: First parameter is return value!
    nameLength = static_cast<uint8_t>(function.name.length());
    parameterCount = static_cast<uint8_t>(function.parameterTypes.size());
    out.write(reinterpret_cast<char *>(&nameLength), 1);
    out.write(reinterpret_cast<char *>(&parameterCount), 1);

    // Write function address and name
    out.write(reinterpret_cast<const char *>(Data::buffer_u64(function.address).data()), 8);
    out.write(function.name.c_str(), nameLength);

    // Write function parameter types
    out.write(reinterpret_cast<const char *>(function.parameterTypes.data()), parameterCount);
  }
}

/**
 * Generate the constant pool of a module.
 *
 * @param constantPool Constants picked up during generation
 */
void Generator::constantPool(const std::vector<uint64_t> &constantPool) {
  // Write constants to module file
  for (const auto &constant : constantPool)
    out.write(reinterpret_cast<const char *>(Data::buffer_u64(constant).data()), 8);
}

/**
 * Generate bytecode for a bytecode instruction.
 *
 * @param instruction Information for the bytecode instruction
 */
void Generator::instruction(const Data::Instruction &instruction) {
  if (disabled)
    return;
  out.write(reinterpret_cast<const char *>(&instruction), 4);
}

}  // namespace ByteCode
}  // namespace VM
