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
#include "vm/data/vm_common.h"

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
  std::array<uint8_t, 2> magic;
  module.read(reinterpret_cast<char *>(magic.data()), 2);
  if (module.fail() || magic[0] != '\x4C' || magic[1] != '\x49') {
    logError("Specified module is not valid Lilium bytecode");
    return;
  }

  // Load number of constant pool entries
  std::array<uint8_t, 2> constants;
  module.read(reinterpret_cast<char *>(constants.data()), 2);
  if (module.fail()) {
    logError("Number of constant pool entries not found");
    return;
  }
  numConstants = Data::parse_u16(constants);

  // Load number of function table entries
  std::array<uint8_t, 4> functions;
  module.read(reinterpret_cast<char *>(functions.data()), 4);
  if (module.fail()) {
    logError("Number of function table entries not found");
    return;
  }
  numFunctions = Data::parse_u32(functions);

  // Load total number of instructions
  std::array<uint8_t, 8> instructions;
  module.read(reinterpret_cast<char *>(instructions.data()), 8);
  if (module.fail()) {
    logError("Total number of instructions not found");
    return;
  }
  numInstructions = Data::parse_u64(instructions);
  consistent = true;
}

/**
 * Read the actual bytecode data:
 * - Constant pool
 * - Function table
 * - Bytecode instructions
 *
 * @param programBuffer Storage for the complete program data, to be filled
 * @param funcTable Helper function table for the linker, to be filled
 * @param offsetInstructions Offset in the global bytecode array
 * @param offsetFunctions Offset in the global function table
 * @param offsetConstants Offset in the global constant pool
 */
void Loader::readData(Data::ProgramBuffer *programBuffer,
                      std::vector<Data::FunctionTableEntry> *funcTable,
                      uint64_t offsetInstructions,
                      uint64_t offsetFunctionTable,
                      uint64_t offsetConstants) {
  // Check if previous steps have been consistent
  if (!consistent) {
    logError("Previous loading steps have not yielded a consistent result");
    return;
  }

  // Read constants into global constant pool
  uint64_t *constantPool = programBuffer->constantPool.data();
  std::array<uint8_t, 8> constantBuffer;
  for (uint32_t i = 0; i < numConstants; i++) {
    module.read(reinterpret_cast<char *>(constantBuffer.data()), 8);
    if (module.fail()) {
      logError("Could not load constant pool");
      return;
    }

    // Convert buffer to 64 bit integer
    constantPool[offsetConstants + i * 8] = Data::parse_u64(constantBuffer);
  }

  // Read function table entries
  Data::FunctionTableEntry *functionTable = funcTable->data();
  for (uint32_t i = 0; i < numFunctions; i++) {
    // Read function table header
    Data::FunctionHeaderInfo headerInfo = {};
    module.read(reinterpret_cast<char *>(&headerInfo), 2);
    if (module.fail()) {
      logError("Invalid function table header found");
      return;
    }

    // Read function address
    Data::FunctionTableEntry *functionTableEntry =
        &functionTable[offsetFunctionTable + i * sizeof(Data::FunctionTableEntry)];
    module.read(reinterpret_cast<char *>(constantBuffer.data()), 8);
    if (module.fail()) {
      logError("Could not read function address");
      return;
    }
    functionTableEntry->address = Data::parse_u64(constantBuffer);

    // Read function name
    std::vector<char> nameBuffer(headerInfo.nameLength);
    module.read(nameBuffer.data(), headerInfo.nameLength);
    if (module.fail()) {
      logError("Could not read function name");
      return;
    }
    functionTableEntry->name = std::string(nameBuffer.begin(), nameBuffer.end());

    // Read parameter types
    // NOTE: First parameter is return value!
    functionTableEntry->parameterTypes.resize(headerInfo.parameterCount);
    module.read(reinterpret_cast<char *>(functionTableEntry->parameterTypes.data()),
                headerInfo.parameterCount);
    if (module.fail()) {
      logError("Could not read parameter types of a function");
      return;
    }
  }

  // Read instructions into global array
  module.read(reinterpret_cast<char *>(&programBuffer->byteCode[offsetInstructions]),
              numInstructions * sizeof(Data::Instruction));
  if (module.fail()) {
    logError("Could not load bytecode instructions");
    return;
  }
}

/**
 * Log an error message regarding the bytecode loading.
 *
 * @param message Message to be logged
 */
void Loader::logError(const std::string &message) {
  std::cerr << "Module file \"" << moduleName << "\": " << message << "\n";
  consistent = false;
}

/**
 * Retrieve number of instructions.
 *
 * @return Number of instructions
 */
uint64_t Loader::getNumInstructions() const {
  return numInstructions;
}

/**
 * Retrieve number of function table entries.
 *
 * @return Number of function table entries
 */
uint32_t Loader::getNumFunctions() const {
  return numFunctions;
}

/**
 * Retrieve number of constant pool entries.
 *
 * @return Number of constant pool entries
 */
uint16_t Loader::getNumConstants() const {
  return numConstants;
}

}  // namespace ByteCode
}  // namespace VM
