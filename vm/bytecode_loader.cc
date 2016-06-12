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

#include "vm/bytecode_loader.h"

namespace VM {

/**
 * Load a bytecode module from a file.
 *
 * @param file Filename
 * @return False in case of error
 */
bool BytecodeLoader::load_module(std::string file, Module *module) {
  // Open file and make sure it has been opened correctly
  std::ifstream module_file(file, std::ifstream::ate | std::ifstream::binary);
  if (!module_file.is_open()) {
    std::cerr << "Module file \"" << file << "\" could not be opened.\n";
    return false;
  }

  // Check for magic number
  char buffer[257];
  module_file.read(&buffer[0], 2);
  if (module_file.bad() || buffer[0] != 0xFF || buffer[1] != 0xBC) {
    std::cerr << "Specified module file \"" << file << "\" is not valid "
        << "Lilium bytecode.\n";
    return false;
  }

  // Read number of function table entries
  std::uint32_t num_functions = 0;
  module_file.read(reinterpret_cast<char *>(&num_functions), 3);
  if (module_file.bad()) {
    std::cerr << "Specified module file \"" << file << "\" does not contain "
        << "the number of function table entries.\n";
    return false;
  }

  // Allocate memory for the function table
  FunctionTable *funcs = new (std::nothrow) FunctionTable(num_functions);
  std::string *names = funcs->get_names();
  std::uint64_t *addr = funcs->get_addr();
  std::uint64_t saddr = module->start_address;

  // Read the function table entries
  std::uint8_t len_symbol = 0;
  for (std::uint32_t i = 0; i < num_functions; i++) {
    // Read referring address
    // TODO(cluosh): Swap bytes if byte order is not little endian
    module_file.read(reinterpret_cast<char *>(&addr[i]), 8);
    addr[i] += saddr;

    // Read symbol name
    module_file.read(reinterpret_cast<char *>(&len_symbol), 1);
    module_file.read(&buffer[0], len_symbol);
    buffer[len_symbol] = '\0';
    names[i] = std::string(buffer);
  }

  // Read number of constant pool entries
  std::uint16_t num_const = 0;
  module_file.read(reinterpret_cast<char *>(&num_const), 2);
  if (module_file.bad()) {
    std::cerr << "Specified module file \"" << file << "\" does not contain "
        << "the number of constant pool entries.\n";
    return false;
  }

  // Read constant pool entries from file
  std::uint64_t *constant_pool = new (std::nothrow) std::uint64_t[num_const];
  module_file.read(reinterpret_cast<char *>(&constant_pool[0]), num_const);
  if (module_file.bad()) {
    std::cerr << "Specified module file \"" << file << "\" does not contain "
        << "the specified number of constants.\n";
    return false;
  }

  // Read number of instructions
  std::uint64_t num_instructions = 0;
  module_file.read(reinterpret_cast<char *>(&num_instructions), 8);
  if (module_file.bad()) {
    std::cerr << "Specified module file \"" << file << "\" does not contain "
        << "the number of instructions.\n";
    return false;
  }

  // Store loaded data in module
  module->funcs = funcs;
  module->constant_pool = constant_pool;
  module->num_functions = num_functions;
  module->num_const = num_const;
  module->num_instructions = num_instructions;
  return true;
}

}  // namespace VM
