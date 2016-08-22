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
#include "cc/ast/expr/const.h"
#include "vm/data/vm_common.h"
#include "vm/constants/opcodes.h"

namespace AST {

/**
 * Initialize a constant expression.
 *
 * @param value Value of the constant expression
 * @param type Type of the expression
 * @param next Next expression in expression list
 */
Const::Const(std::int64_t value, VM::Type type, Expr *next)
    : Expr(type, next),
      value(value) { }

/**
 * Attribute a constant expression.
 *
 * @param attrib_info Attribution information
 */
void Const::attribute(AttribInfo *attrib_info) {
  // Assign register
  resultReg = attrib_info->nextReg;
  attrib_info->nextReg++;

  // Store constant in constant pool
  cp_index = static_cast<uint16_t>(attrib_info->constants.size());
  attrib_info->constants.push_back(static_cast<uint64_t>(value));
}

/**
 * Generate code for this constant expression.
 *
 * @param generator The bytecode generator helper
 * @param attrib_info Attribution information from previous passes
 */
void Const::generate_code(VM::ByteCode::Generator *generator, AttribInfo *attrib_info) {
  // Load constant from pool into result register
  const auto &constantIndex = VM::Data::buffer_u16(cp_index);
  VM::Data::Instruction bc;
  bc.opcode = VM::OP_LOADI;
  bc.op[0] = resultReg;
  std::copy(constantIndex.begin(), constantIndex.end(), std::begin(bc.op) + 1);
  generator->instruction(bc);
  attrib_info->codeCounter += 1;
}

}  // namespace AST
