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
#ifndef VM_OPCODES_H_
#define VM_OPCODES_H_

namespace VM {

/**
 * List of opcodes.
 */
enum OpCode : uint8_t {
  OP_NOP,
  OP_CALLI,
  OP_CALLE,
  OP_LCALLI,
  OP_LCALLE,
  OP_RETURN,
  OP_ADDI,
  OP_SUBI,
  OP_MULI,
  OP_DIVI,
  OP_GT,
  OP_LOADI,
  OP_JMPC,
  OP_JMP,
  OP_MOV,
  OP_HALT
};

}  // namespace VM

#endif  // VM_OPCODES_H_
