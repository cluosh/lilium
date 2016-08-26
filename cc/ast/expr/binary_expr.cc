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

#include "cc/ast/expr/binary_expr.h"
#include "vm/constants/types.h"

namespace AST {

/**
 * Initialize a plus expression with two operands.
 *
 * @param fst First operand
 * @param snd Second operand
 * @param op Operator for this binary expression node
 * @param next Next expression in list
 */
BinaryExpr::BinaryExpr(Expr *fst, Expr *snd, BinaryOperator op, Expr *next)
  : Expr(VM::Type::TYPE_COUNT, next),
    fst (fst),
    snd(snd),
    op(op) {
  // Update type of expression
  choose_type(fst, snd);
}

/**
 * Cleanup allocated expressions.
 */
BinaryExpr::~BinaryExpr() {
  delete fst;
  delete snd;
}

/**
 * Pick a typed operation for this binary expression.
 */
VM::OpCode BinaryExpr::pick_typed() {
  switch (op) {
    case BINARY_ADD:
      return VM::OP_ADDI;
    case BINARY_SUB:
      return VM::OP_SUBI;
    case BINARY_MUL:
      return VM::OP_MULI;
    case BINARY_DIV:
      return VM::OP_DIVI;
    case BINARY_GT:
      return VM::OP_GT;
    default:
      return VM::OP_NOP;
  }
}

/**
 * Attribute a binary expression.
 *
 * @param info Info needed for attribution and code generation later on
 */
void BinaryExpr::attribute(AttribInfo *info) {
  // Set result register and increase register counter
  resultReg = info->nextReg;
  info->maxReg = std::max(info->maxReg, info->nextReg);

  // Attribute operands
  info->nextReg += 1;
  fst->attribute(info);
  snd->attribute(info);

  // Update type of expression
  choose_type(fst, snd);
}

/**
 * Generate code for binary expressions.
 *
 * @param generator Code generator helper class
 * @param attrib_info Attribute information needed for code generation
 */
void BinaryExpr::generate_code(VM::ByteCode::Generator *generator,
                               AttribInfo *attrib_info) {
  // Generate bytecode of child nodes first
  fst->generate_code(generator, attrib_info);
  snd->generate_code(generator, attrib_info);

  // Generate the bytecode instruction
  VM::Data::Instruction bc;
  bc.opcode = static_cast<uint8_t>(pick_typed());
  bc.op[0] = fst->resultReg;
  bc.op[1] = snd->resultReg;
  bc.op[2] = resultReg;
  generator->instruction(bc);
  attrib_info->codeCounter++;
}

/**
 * Assign symbols to this expression and the operands.
 *
 * @param symbol_tables Symbol tables to be assigned
 */
void BinaryExpr::set_symbols(SymbolTables *symbol_tables) {
  Expr::set_symbols(symbol_tables);

  // Assign symbols to operands
  fst->set_symbols(symbol_tables);
  snd->set_symbols(symbol_tables);
}

}  // namespace AST
