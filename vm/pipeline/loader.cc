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
#include <memory>
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
 * @param funcTable The linker function table to be filled
 */
void Loader::execute(const std::vector<std::string> &modules,
                     Data::ProgramBuffer *buffer,
                     std::vector<Data::FunctionTableEntry> *funcTable) {
  // Create bytecode loader for each module and read header information
  std::vector<std::unique_ptr<ByteCode::Loader>> loaders(modules.size());
  for (uint64_t i = 0; i < modules.size(); i++) {
    loaders[i] = std::make_unique<ByteCode::Loader>(modules[i]);
    loaders[i]->readHeaders();
  }

  // Allocate memory for offsets
  buffer->constantPoolOffset.resize(modules.size(), 0);
  buffer->functionTableOffset.resize(modules.size(), 0);

  // Find number of table entries
  uint64_t numInstructions = 0;
  uint32_t numFunctions = 0;
  uint16_t numConstants = 0;
  for (size_t i = 0; i < modules.size(); i++) {
    buffer->constantPoolOffset[i] = numConstants;
    buffer->functionTableOffset[i] = numFunctions;

    numInstructions += loaders[i]->getNumInstructions();
    numFunctions += loaders[i]->getNumFunctions();
    numConstants += loaders[i]->getNumConstants();
  }

  // Allocate program memory
  buffer->byteCode.resize(numInstructions, {});
  buffer->constantPool.resize(numConstants, 0);
  funcTable->resize(numFunctions, {});

  // Fill the program buffer and calculate offsets
  uint64_t offsetInstruction = 0;
  uint64_t offsetFunctionTable = 0;
  uint64_t offsetConstantPool = 0;
  for (auto &loader : loaders) {
    loader->readData(buffer, funcTable, offsetInstruction,
                     offsetFunctionTable, offsetConstantPool);
    offsetInstruction += loader->getNumInstructions();
    offsetFunctionTable += loader->getNumFunctions();
    offsetConstantPool += loader->getNumInstructions();
  }
}

}  // namespace Pipeline
}  // namespace VM
