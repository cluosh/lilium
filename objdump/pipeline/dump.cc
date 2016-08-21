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
#include <algorithm>
#include <fstream>

#include "objdump/pipeline/dump.h"
#include "vm/constants/opcodes.h"
#include "vm/constants/types.h"

namespace OBJDUMP {
namespace Pipeline {

/**
 * Initialize the dump with an output stream.
 *
 * @param output Output stream where the bytecode is written to
 */
Dump::Dump(const std::ostream &output) : out(output.rdbuf()) { }

/**
 * Dump all the information in a program buffer in a textual form.
 *
 * @param module The name of the module
 * @param programBuffer Program buffer to be dumped
 * @param functions The function table prepared for the linker
 */
void Dump::execute(const std::string &module,
                   const VM::Data::ProgramBuffer &programBuffer,
                   const std::vector<VM::Data::FunctionTableEntry> &functions) {
  // Print module header
  out << "        .module " << module << "\n";
  for (const auto &function : functions) {
    if (function.address != 0xFFFFFFFFFFFFFFFF)
      continue;
    out << "        .extern " << function.name << "\n";
  }

  // Write instructions in textual form to the output
  const auto &code = programBuffer.byteCode;
  for (uint32_t functionIndex = 0; functionIndex < functions.size(); functionIndex++) {
    // Exit, if we find the first external function
    const auto &function = functions[functionIndex];
    if (function.address == 0xFFFFFFFFFFFFFFFF)
      break;

    // Print function headers
    out << function.name << ":\n";
    for (unsigned int i = 1; i < function.parameterTypes.size(); i++) {
      out << "        # Parameter " << i << " type: "
          << VM::Constants::typeTable[function.parameterTypes[i]] << "\n";
    }
    out << "        # Return type:      "
        << VM::Constants::typeTable[function.parameterTypes[0]] << "\n\n";

    // Print instructions for this function
    const uint64_t maxAddress = functionIndex < functions.size() - 1 ?
                                functions[functionIndex + 1].address : code.size();
    for (uint64_t i = function.address; i < maxAddress; i++) {
      out << "        " << static_cast<unsigned int>(code[i].opcode) << " "
          << static_cast<unsigned int>(code[i].op[0]) << " "
          << static_cast<unsigned int>(code[i].op[1]) << " "
          << static_cast<unsigned int>(code[i].op[2]) << "\n";
    }
  }
}

}  // namespace Pipeline
}  // namespace OBJDUMP