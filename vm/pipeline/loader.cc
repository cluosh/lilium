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
#include <vector>
#include <vm/data/program_buffer.h>

#include "vm/bytecode/loader.h"
#include "vm/pipeline/loader.h"

namespace VM {
namespace Pipeline {

/**
 * Load a list of modules and fill the program buffer; If there are errors
 * during the loading stage of some modules, linking is still attempted.
 *
 * @param modules Array of module-filenames to be loaded
 * @param buffer The program buffer to be filled
 */
void Loader::execute(const std::vector<std::string> &modules, Data::ProgramBuffer *buffer) {
  // Create bytecode loader for each module and read header information
  std::vector<ByteCode::Loader> loaders;
  loaders.resize(modules.size());
  for (const auto &module : modules) {
    ByteCode::Loader loader(module);
    loader.readHeaders();
    loaders.push_back(loader);
  }

  // Find number of table entries
  uint64_t numInstructions = 0;
  uint32_t numFunctions = 0;
  uint16_t numConstants = 0;
  for (const auto &loader : loaders) {
    numInstructions += loader.getNumInstructions();
    numFunctions += loader.getNumFunctions();
    numConstants += loader.getNumConstants();
  }

  // Allocate program memory
  buffer->byteCode.resize(numInstructions, Data::Instruction{});
  buffer->constantPool.resize(numConstants, 0);

  // Fill the program buffer and calculate offsets
  uint64_t offsetInstruction = 0;
  uint64_t offsetFunctionTable = 0;
  uint64_t offsetConstantPool = 0;
  for (const auto &loader : loaders) {
    loader.readData(buffer, offsetInstruction, offsetFunctionTable, offsetConstantPool);
    offsetInstruction += loader.getNumInstructions();

  }
}

}  // namespace Pipeline
}  // namespace VM