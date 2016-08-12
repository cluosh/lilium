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

#include "vm/bytecode/loader.h"
#include "vm/data/common.h"
#include "vm/opcodes.h"

namespace VM {
namespace ByteCode {

/**
 * Initialize the bytecode loader.
 *
 * @param file Name of the module to be loaded
 */
Loader::Loader(std::string file)
    : moduleName(file),
      module(file, std::ifstream::binary),
      consistent(!module.fail()) { }

/**
 * Read the headers from the bytecode module, meaning:
 * - Magic number
 * - Number of constant pool entries
 * - Number of function table entries
 * - Number of total instructions
 */
void Loader::readHeaders() {
  if (!consistent) {
    logError("File could not be opened");
    return;
  }

  // Check for magic number
  std::vector<uint8_t> buffer(256, 0);
  module.read(reinterpret_cast<char *>(buffer.data()), 2);
  if (module.fail() || buffer[0] != '\x4C' || buffer[1] != '\x49') {
    logError("Specified module is not valid Lilium bytecode");
    return;
  }

  // Load number of constant pool entries
  module.read(reinterpret_cast<char *>(buffer.data()), 2);
  if (module.fail()) {
    logError("Number of constant pool entries not found");
    return;
  }
  numConstants = Data::parse_u16(buffer);

  // Load number of function table entries
  module.read(reinterpret_cast<char *>(buffer.data()), 4);
  if (module.fail()) {
    logError("Number of function table entries not found");
    return;
  }
  numFunctions = Data::parse_u32(buffer);

  // Load total number of instructions
  module.read(reinterpret_cast<char *>(buffer.data()), 8);
  if (module.fail()) {
    logError("Total number of instructions");
    return;
  }
  numInstructions = Data::parse_u64(buffer);
}

/**
 * Log an error message regarding the bytecode loading.
 *
 * @param message Message to be logged
 */
void Loader::logError(const std::string &message) {
  std::cerr << "Module file \"" << moduleName << "\":" << message << "\n";
  consistent = false;
}

}  // namespace ByteCode
}  // namespace VM
