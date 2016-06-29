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
#include "vm/bytecode/generator.h"

namespace VM {

/**
 * Initialize the generator with an output stream.
 *
 * @param output Output stream where the bytecode is written to
 */
Generator::Generator(const std::ostream &output) : out(output.rdbuf()) { }

/**
 * Disable/Enable code generation.
 *
 * @param disable Disable/Enable code generation
 */
void Generator::set_disabled(bool disabled) {
  this->disabled = disabled;
}

/**
 * Generate the bytecode header for a module file.
 *
 * @param name Name of the module (limited to 128 bytes)
 * @param num_func Number of function table entries
 * @param num_const Number of constant pool entries
 * @param num_inst Number of instructions (code size)
 */
void Generator::module_header(std::string name,
                              std::uint32_t num_func,
                              std::uint16_t num_const,
                              std::uint64_t num_inst) {
  // Write the magic number
  out.write("\x4C\x49", 2);

  // Print module name (make sure exactly 128 bytes are printed)
  if (name.length() < 128) {
    out.write(name.c_str(), name.size());
    for (std::uint64_t i = name.length(); i < 128; i++)
      out.write("\0", 1);
  } else {
    out.write(name.c_str(), 128);
  }

  // Write number of function table entries
  // Only the lower 24 bit are being used
  out.write(reinterpret_cast<char *>(&num_func), 4);

  // Write number of constants
  out.write(reinterpret_cast<char *>(&num_const), 2);

  // Write number of total instructions
  out.write(reinterpret_cast<char *>(&num_inst), 8);
}

/**
 * Write a function table to the file, based on function addresses.
 *
 * @param func_addr The function addresses, types and names
 */
void Generator::function_table(FuncAddr *func_addr) {
  std::uint32_t func_count = func_addr->get_count();
  std::uint64_t addr;
  std::uint8_t type;
  std::uint8_t len;
  std::string name;

  // Write table entries as bytecode
  for (std::uint32_t i = 0; i < func_count; i++) {
    addr = func_addr->get_addr(i) - 1;
    type = func_addr->get_type(i);
    name = func_addr->get_name(i);
    len = (std::uint8_t) name.length();

    // Check if type is undefined
    if (type == TYPE_COUNT)
      type = TYPE_INT;

    // Write bytecode
    out.write(reinterpret_cast<char *>(&addr), 8);
    out.write(reinterpret_cast<char *>(&type), 1);
    out.write(reinterpret_cast<char *>(&len), 1);
    out.write(name.c_str(), name.length());
  }
}

/**
 * Generate the constant pool of a module.
 *
 * @param const_pool Constants picked up during generation
 */
void Generator::constant_pool(ConstPool *const_pool) {
  // Write constants to module file
  out.write(reinterpret_cast<char *>(&const_pool->at(0)),
            8 * const_pool->size());
}

/**
 * Generate bytecode for a bytecode instruction.
 *
 * @param bc Information for the bytecode instruction
 */
void Generator::instruction(const ByteCode &bc) {
  if (!disabled)
    out.write(reinterpret_cast<char *>(const_cast<VM::ByteCode *>(&bc)), 4);
}

}  // namespace VM
