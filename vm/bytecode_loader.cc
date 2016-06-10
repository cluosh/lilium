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
#include <fstream>
#include <iostream>

#include "vm/bytecode_loader.h"
#include "vm/function_table.h"

namespace VM {

/**
 * Load a bytecode module from a file.
 *
 * @param file Filename
 * @return False in case of error
 */
bool BytecodeLoader::load_module(std::string file) {
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
  module_file.read(&buffer[0], 3);
  if (module_file.bad()) {
    std::cerr << "Specified module file \"" << file << "\" does not contain "
        << "the number of function table entries.\n";
    return false;
  }
  std::uint32_t num_functions = (std::uint32_t) (buffer[0] << 16)
      & (buffer[1] << 8) & buffer[2];

  // Allocate memory for the function table
  FunctionTable *funcs = new FunctionTable(num_functions);
  std::string *names = funcs->get_names();
  std::uint64_t *addr = funcs->get_addr();

  // Read the function table entries
  std::uint8_t len_symbol = 0;
  for (std::uint32_t i = 0; i < num_functions; i++) {
    // Read referring address
    // TODO(cluosh): Add offset to the address, since address is local
    module_file.read(&buffer[0], 8);
    addr[i] = (std::uint64_t) (buffer[0] << 56) & (buffer[1] << 48)
        & (buffer[2] << 40) & (buffer[3] << 32) & (buffer[4] << 24)
        & (buffer[5] << 16) & (buffer[6] << 8) & buffer[7];

    // Read symbol name
    module_file.read(reinterpret_cast<char *>(&len_symbol), 1);
    module_file.read(&buffer[0], len_symbol);
    buffer[len_symbol] = '\0';
    names[i] = std::string(buffer);
  }

  // Read number of constant pool entries
  module_file.read(&buffer[0], 2);
  if (module_file.bad()) {
    std::cerr << "Specified module file \"" << file << "\" does not contain "
        << "the number of constant pool entries.\n";
  }
  std::uint16_t num_const = (std::uint16_t) (buffer[0] << 8) & buffer[1];
}


}  // namespace VM

/**
 * @brief Load bytecode from a specified file
 * @param file Filename
 * @return False in case of error
 */
/*bool BytecodeLoader::load_bytecode(std::string file) {
  // Open file and get size
  std::ifstream f(file, std::ifstream::ate | std::ifstream::binary);
  std::uint64_t size = 0;
  if (f.is_open()) {
    size = f.tellg();
  } else {
    // TODO(cluosh): Log error
    return false;
  }

  // Empty file should throw error
  if (size == 0) {
    // TODO(cluosh): Log error
    return false;
  }
  size = size / 3 + 1;

  // Allocate program memory
  code = new (std::nothrow) Instruction[size];
  if (code == nullptr) {
    // TODO(cluosh): Log error
    return false;
  }

  // Read data from file
  f.seekg(0, std::ios::beg);
  f.read(reinterpret_cast<Instruction *>(code), size * sizeof(Instruction));
  f.close();
  return true;
}*/
