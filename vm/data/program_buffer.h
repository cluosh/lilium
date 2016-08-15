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
#ifndef VM_DATA_PROGRAM_BUFFER_H_
#define VM_DATA_PROGRAM_BUFFER_H_

#include <vector>

namespace VM {
namespace Data {

/**
 * Bytecode structure.
 */
struct Instruction {
  uint8_t opcode;
  uint8_t op[3];
};

/**
 * Store bytecode and all other relevent information in this buffer object.
 */
struct ProgramBuffer {
  std::vector<Instruction> byteCode;
  std::vector<uint64_t> constantPool;
};

}  // namespace Data
}  // namespace VM

#endif  // VM_DATA_PROGRAM_BUFFER_H_
