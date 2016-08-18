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

#include "vm/data/program_buffer.h"
#include "vm/pipeline/interpreter.h"

// Dispatching macro, increasing program counter and jump
#define DISPATCH() goto *token_table[code[++pc].opcode]

namespace VM {
namespace Pipeline {

/**
 * Execute previously linked bytecode.
 *
 * @param buffer Buffer containing previously linked bytecode
 */
void Interpreter::execute(const Data::ProgramBuffer &buffer) {
  // Rename program buffers
  const Data::Instruction *code = buffer.byteCode.data();

  // Registers, stack, machine state setup
  uint64_t registers[256];
  uint64_t pc = 0xFFFFFFFFFFFFFFFF;

  // Jump table for token threaded code
  const void * const token_table[] = {
      && op_nop,
      && op_calli,
      && op_calle,
      && op_lcalli,
      && op_lcalle,
      && op_return,
      && op_addi,
      && op_subi,
      && op_muli,
      && op_divi,
      && op_gt,
      && op_loadi,
      && op_jmpc,
      && op_jmp,
      && op_mov,
      && op_halt
  };

  // Main dispatch loop
  while(true) {
    DISPATCH();

    op_nop:
      DISPATCH();
    op_calli:
      DISPATCH();
    op_calle:
      DISPATCH();
    op_lcalli:
      DISPATCH();
    op_lcalle:
      DISPATCH();
    op_return:
      DISPATCH();
    op_addi:
      DISPATCH();
    op_subi:
      DISPATCH();
    op_muli:
      DISPATCH();
    op_divi:
      DISPATCH();
    op_gt:
      DISPATCH();
    op_loadi:
      DISPATCH();
    op_jmpc:
      DISPATCH();
    op_jmp:
      DISPATCH();
    op_mov:
      DISPATCH();
    op_halt:
      break;
  }
}

}  // namespace Pipeline
}  // namespace VM
