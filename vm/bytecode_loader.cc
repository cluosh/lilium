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
  if (module_file.fail() || buffer[0] != '\xFF' || buffer[1] != '\xBC') {
    std::cerr << "Specified module file \"" << file << "\" is not valid "
        << "Lilium bytecode.\n";
    return false;
  }

  // Read module name
  module_file.read(&buffer[0], 128);
  if (module_file.fail()) {
    std::cerr << "Specified module file \"" << file << "\" does not contain "
        << "a valid module name.\n";
    return false;
  }
  buffer[128] = '\0';
  std::string module_name(&buffer[0]);

  // Read number of function table entries
  std::uint32_t num_functions = 0;
  module_file.read(reinterpret_cast<char *>(&num_functions), 3);
  if (module_file.fail()) {
    std::cerr << "Module \"" << module_name << "\" does not contain "
        << "the number of function table entries.\n";
    return false;
  }

  // Read number of constant pool entries
  std::uint16_t num_const = 0;
  module_file.read(reinterpret_cast<char *>(&num_const), 2);
  if (module_file.fail()) {
    std::cerr << "Module \"" << module_name << "\" does not contain "
        << "the number of constant pool entries.\n";
    return false;
  }

  // Read number of instructions
  std::uint64_t num_instructions = 0;
  module_file.read(reinterpret_cast<char *>(&num_instructions), 8);
  if (module_file.fail()) {
    std::cerr << "Module \"" << module_name << "\" does not contain "
        << "the number of instructions.\n";
    return false;
  }

  // Allocate memory for the function table
  FunctionTable *funcs = new (std::nothrow) FunctionTable(num_functions);
  if (funcs == nullptr) {
    std::cerr << "Could not allocate memory for function table of \""
        << module_name << "\".\n";
    return false;
  }

  // Read the function table entries
  // TODO(cluosh): Better way of reading function table at once?
  std::string *names = funcs->get_names();
  std::uint64_t *addr = funcs->get_addr();
  std::uint16_t *module_ids = funcs->get_module_ids();
  std::uint8_t len_symbol = 0;
  std::uint16_t module_id = module->module_id;
  for (std::uint32_t i = 0; i < num_functions; i++) {
    // Read referring address and symbol name
    // TODO(cluosh): Swap bytes if byte order is not little endian
    module_file.read(reinterpret_cast<char *>(&addr[i]), 8);
    module_file.read(reinterpret_cast<char *>(&len_symbol), 1);
    module_file.read(&buffer[0], len_symbol);

    // RDo error check and assignments
    if (module_file.fail()) {
      std::cerr << "Error while reading function table in \""
          << module_name << "\".\n";
      return false;
    }
    module_ids[i] = module_id;
    buffer[len_symbol] = '\0';
    names[i] = std::string(buffer);
  }

  // Allocate constant pool
  std::uint64_t *constant_pool = new (std::nothrow) std::uint64_t[num_const];
  if (constant_pool == nullptr) {
    std::cerr << "Could not allocate memory for constant pool in \""
        << module_name << "\".\n";
    return false;
  }

  // Read constant pool entries from file
  module_file.read(reinterpret_cast<char *>(&constant_pool[0]), num_const);
  if (module_file.fail()) {
    std::cerr << "Module \"" << module_name << "\" does not contain "
        << "the specified number of constants.\n";
    return false;
  }

  // Allocate byte code array
  ByteCode *code = new (std::nothrow) ByteCode[num_instructions];
  if (code == nullptr) {
    std::cerr << "Could not allocate memory for byte code in \""
        << module_name << "\".\n";
    return false;
  }

  // Read bytecode from file
  module_file.read(reinterpret_cast<char *>(&code[0]), num_instructions);
  if (module_file.fail()) {
    std::cerr << "Could not read byte code from file \""
        << module_name << "\".\n";
  }

  // Store loaded data in module
  module->funcs = funcs;
  module->constant_pool = constant_pool;
  module->code = code;
  module->module_name = module_name;
  module->num_functions = num_functions;
  return true;
}

}  // namespace VM
