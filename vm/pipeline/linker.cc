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
#include <utility>

#include "vm/data/program_buffer.h"
#include "vm/pipeline/linker.h"

namespace VM {
namespace Pipeline {

/**
 * Resolve cross-module references, build function tables, and offsets with
 * module information.
 *
 * @param buffer Buffer containing loaded functions and bytecode
 * @param funcTable
 */
void Linker::execute(Data::ProgramBuffer *buffer,
                     const std::vector<Data::FunctionTableEntry> &funcTable) {
  std::unordered_map<std::string, Data::FunctionEntry> functions;
  std::unordered_map<uint64_t, uint64_t> functionIndex;
  const auto &funcOffsets = buffer->functionTableOffset;

  // Store defined functions in map
  for (uint64_t index = 0; index < funcOffsets.size(); index++) {
    uint64_t upper = index == funcOffsets.size() - 1 ? funcTable.size() : funcOffsets[index + 1];
    uint64_t lower = funcOffsets[index];

    // Look up indices between the corresponding offsets
    for (uint64_t i = lower; i < upper; i++) {
      // Check if address is valid
      // Valid address implies a function definition
      // Invalid address implies unresolved function
      if (funcTable[i].address != 0xFFFFFFFFFFFFFFFF)
        continue;

      // Insert a function definition into the temporary table
      auto function = functions.insert({funcTable[i].name, {funcTable[i].address + lower, index}});
      if (!function.second)
        throw std::runtime_error("Found duplicate function '" + funcTable[i].name + "'");
      else
        functionIndex[funcTable[i].address] = i;
    }
  }

  // Build runtime function table and resolve functions
  buffer->functionTable.resize(funcTable.size(), {});
  for (uint64_t i = 0; i < funcTable.size(); i++) {
    // Look up function in the reference map
    auto function = functions.find(funcTable[i].name);
    if (function == functions.end())
      throw std::runtime_error("Could not resolve function '" + funcTable[i].name + "'");

    // TODO(cluosh): Typecheck

    // Store function reference in actual function table
    buffer->functionTable[i] = function->second;
  }
}

}  // namespace Pipeline
}  // namespace VM