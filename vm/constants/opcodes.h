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

#include <array>

namespace VM {

/**
 * List of opcodes with a short description.
 */
enum OpCode: uint8_t {
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
  OP_HALT,
  OP_COUNT
};

namespace Constants {

/**
 * List of operation types.
 */
enum OpTypes: uint8_t {
  OPT_NULLARY,
  OPT_UNARY,
  OPT_BINARY,
  OPT_TERNARY,
  OPT_CALL,
  OPT_CONST,
  OPT_COUNT
};

/**
 * Constant expression containing lookup table for operations.
 */
constexpr std::array<const char *, OP_COUNT> operationTable{
    "nop",
    "calli",
    "calle",
    "lcalli",
    "lcalle",
    "return",
    "addi",
    "subi",
    "muli",
    "divi",
    "gt",
    "loadi",
    "jmpc",
    "jmp",
    "mov",
    "halt"
};

/**
 * Constant expression containing lookup table for operation types.
 */
constexpr std::array<OpTypes, OP_COUNT> opTypeTable{
    OPT_NULLARY,
    OPT_CALL,
    OPT_CALL,
    OPT_CALL,
    OPT_CALL,
    OPT_NULLARY,
    OPT_TERNARY,
    OPT_TERNARY,
    OPT_TERNARY,
    OPT_TERNARY,
    OPT_TERNARY,
    OPT_CONST,
    OPT_CONST,
    OPT_CONST,
    OPT_BINARY,
    OPT_NULLARY
};

}  // namespace Constants
}  // namespace VM

#endif  // VM_OPCODES_H_
