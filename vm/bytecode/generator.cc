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
  out.write("\xFF\xBC", 2);

  // Print module name (make sure exactly 128 bytes are printed)
  if (name.length() < 128) {
    out.write(name.c_str(), name.length());
    for (std::uint64_t i = name.length(); i < 128; i++)
      out.write("\0", i);
  } else {
    out.write(name.c_str(), 128);
  }

  // Write number of function table entries (24 bit)
}

}  // namespace VM
