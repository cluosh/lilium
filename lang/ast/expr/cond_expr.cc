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
#include "lang/ast/expr/cond_expr.h"
#include "vm/opcodes.h"

namespace AST {

/**
 * Initialize a conditional expression.
 *
 * @param condition The conditional expression
 * @param fst Expression to be executed if condition is true
 * @param snd Expression to be executed if condition is false
 */
CondExpr::CondExpr(Expr *condition, Expr *fst, Expr *snd)
    : Expr(VM::TYPE_COUNT, nullptr) {
  this->condition = condition;
  this->fst = fst;
  this->snd = snd;
  choose_type(fst, snd);
}

/**
 * Attribute the conditional expression.
 *
 * @param attrib_info Attribution information
 */
void CondExpr::attribute(AttribInfo *attrib_info) {
  // Set result register
  result_reg = attrib_info->next_reg;

  // Reserve two constants in the constant pool for conditional jump
  fst_cp_index = static_cast<uint16_t>(attrib_info->constants.size());
  attrib_info->constants.push_back(0);
  snd_cp_index = static_cast<uint16_t>(attrib_info->constants.size());
  attrib_info->constants.push_back(0);

  // Attribute condition and choices
  condition->attribute(attrib_info);
  attrib_info->next_reg = result_reg;
  fst->attribute(attrib_info);
  attrib_info->next_reg = result_reg;
  snd->attribute(attrib_info);
}

/**
 * Generate code for the conditional expression.
 *
 * @param generator The bytecode generator
 * @param attrib_info Attribution information
 */
void CondExpr::generate_code(VM::Generator *generator,
                             AttribInfo *attrib_info) {
  // Generate code for the condition check
  condition->generate_code(generator, attrib_info);

  // Generate code for conditional jump
  VM::Instruction bc;
  bc.op[0] = VM::OP_JMPC;
  bc.op[1] = condition->result_reg;
  bc.op[2] = static_cast<uint8_t>(snd_cp_index & 0xFF);
  bc.op[3] = static_cast<uint8_t>(snd_cp_index >> 8);
  generator->instruction(bc);
  attrib_info->code_counter += 1;

  // Generate code for first and second expression
  fst->generate_code(generator, attrib_info);
  bc.op[0] = VM::OP_JMP;
  bc.op[1] = 0;
  bc.op[2] = static_cast<uint8_t>(fst_cp_index & 0xFF);
  bc.op[3] = static_cast<uint8_t>(fst_cp_index >> 8);
  generator->instruction(bc);
  attrib_info->code_counter += 1;
  attrib_info->constants[snd_cp_index] = attrib_info->code_counter - 1;
  snd->generate_code(generator, attrib_info);
  attrib_info->constants[fst_cp_index] = attrib_info->code_counter - 1;
}

/**
 * Assign symbols to this expression and the operands.
 *
 * @param symbol_tables Symbol tables to be assigned
 */
void CondExpr::set_symbols(SymbolTables *symbol_tables) {
  Expr::set_symbols(symbol_tables);

  // Assign symbols to operands
  condition->set_symbols(symbol_tables);
  fst->set_symbols(symbol_tables);
  snd->set_symbols(symbol_tables);
}

/**
 * Pass last flag on for tail optimization.
 *
 * @param last New last flag
 */
void CondExpr::set_last(bool last) {
  Expr::set_last(last);

  // First and second expression are tails
  fst->set_last(last);
  snd->set_last(last);
}

}  // namespace AST