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
#ifndef VM_BYTECODE_BYTECODE_LOADER_H_
#define VM_BYTECODE_BYTECODE_LOADER_H_

#include <cstdint>
#include <fstream>
#include <string>

#include "vm/module.h"

namespace VM {
namespace ByteCode {

/**
 * Loader class, can load bytecode modules (which are specified correctly).
 */
class Loader {
 private:
  std::string moduleName;
  std::ifstream module;
  uint64_t numInstructions = 0;
  uint32_t numFunctions = 0;
  uint16_t numConstants = 0;
  bool consistent = false;

 public:
  Loader(std::string file);
  void readHeaders();

  void logError(const std::string &message);
};

}  // namespace ByteCode
}  // namespace VM

#endif  // VM_BYTECODE_BYTECODE_LOADER_H_
