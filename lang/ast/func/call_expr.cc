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
#include "lang/ast/func/call_expr.h"
#include "vm/opcodes.h"

namespace AST {

/**
 * Create a call expression with an id (function to be called) and
 * a list of expressions to be passed as parameters.
 *
 * @param name Name of the function to be called
 * @param expr_list List of parameter expressions
 * @param next Next expression in expression list
 */
CallExpr::CallExpr(std::string name, Expr *expr_list, Expr *next)
    : Expr(VM::TYPE_COUNT, next) {
  this->name = name;
  this->expr_list = expr_list;
}

/**
 * Attribute this call expression.
 *
 * @param attrib_info Info needed for attribution and code generation
 *                    later on
 */
void CallExpr::attribute(AttribInfo *attrib_info) {
  // Fill the expressions with parameter registers
  std::uint8_t param_reg = 200;

  // Loop over list of expressions
  result_reg = attrib_info->next_reg;
  for (Expr *it = expr_list; it != nullptr; it = it->next) {
    attrib_info->next_reg = static_cast<uint8_t>(result_reg + 1);
    it->attribute(attrib_info);
    it->result_reg = param_reg++;
  }
  attrib_info->next_reg = static_cast<uint8_t>(result_reg + 1);
}

/**
 * Pass the symbol tables down to the expressions in the parameterlist.
 *
 * @param symbol_tables Symbol tables to be passed down
 */
void CallExpr::set_symbols(SymbolTables *symbol_tables) {
  if (expr_list != nullptr)
    expr_list->set_symbols(symbol_tables);
}

/**
 * Generate code for this function call.
 *
 * @param generator The bytecode generator
 * @param attrib_info Info from attribution
 */
void CallExpr::generate_code(VM::Generator *generator,
                             AttribInfo *attrib_info) {
  // Get information about the function to be called
  std::uint32_t local_addr = attrib_info->func_addr.get_local_addr(name);
  std::uint64_t addr = attrib_info->func_addr.get_addr(local_addr);
  VM::ByteCode call;
  call.all = local_addr << 8;

  // Check if this call is a tail call
  if (is_last()) {
    if (addr != 0)
      call.op[0] = VM::OP_LCALLI;
    else
      call.op[0] = VM::OP_LCALLE;
  } else {
    // TODO(cluosh): Stack
    if (addr != 0)
      call.op[0] = VM::OP_CALLI;
    else
      call.op[0] = VM::OP_CALLE;
  }

  // Generate parameter expressions
  for (Expr *it = expr_list; it != nullptr; it = it->next)
    it->generate_code(generator, attrib_info);

  // Generate call
  generator->instruction(call);
  attrib_info->code_counter += 1;
}

}  // namespace AST
