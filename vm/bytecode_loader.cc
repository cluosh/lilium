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
#include "vm/bytecode_loader.h"

#include <cstdint>
#include <fstream>
#include <string>

/**
 * @brief Load bytecode from a specified file
 * @param file Filename
 * @return False in case of error
 */
bool BytecodeLoader::load_bytecode(std::string file) {
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
}

/**
 * @brief Retrieve the previously read bytecode
 * @return Previously read bytecode
 */
Instruction *BytecodeLoader::get_code() {
  return code;
}
