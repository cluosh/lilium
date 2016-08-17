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
#include <unordered_map>

#include "vm/data/program_buffer.h"
#include "vm/pipeline/linker.h"

namespace VM {
namespace Pipeline {

/**
 * Resolve cross-module references, build function tables, and offsets with
 * module information.
 *
 * @param buffer Buffer containing loaded functions and bytecode
 */
void Linker::execute(Data::ProgramBuffer *buffer) {
  std::unordered_map<std::string, Data::FunctionEntry> functions;
  std::unordered_map<uint64_t, uint64_t> functionIndex;
  const auto &funcOffsets = buffer->functionTableOffset;
  const auto &funcTable = buffer->linkerFunctionTable;

  // Store defined functions in map
  for (uint64_t index = 0; index < funcOffsets.size(); index++) {
    uint64_t upper = index == funcOffsets.size() - 1 ? funcTable.size() : funcOffsets[index + 1];

    for (uint64_t i = funcOffsets[index]; i < upper; i++) {
      if (funcTable[i].address != 0xFFFFFFFFFFFFFFFF
          || functions.find(funcTable[i].name) != functions.end()) {
        // TODO(cluosh): Error message and abort (duplicate function name)
      } else {
        functions[funcTable[i].name] = {funcTable[i].address + funcOffsets[i], index};
        functionIndex[funcTable[i].address] = i;
      }
    }
  }

  // Build runtime function table and resolve functions
  buffer->functionTable.resize(funcTable.size(), {});
  for (uint64_t i = 0; i < funcTable.size(); i++) {
  }
}

}  // namespace Pipeline
}  // namespace VM