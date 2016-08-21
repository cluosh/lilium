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
#include "cc/ast/func/call_expr.h"
#include "vm/data/vm_common.h"
#include "vm/constants/opcodes.h"

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

  // Calculate number of parameters
  num_param = 0;
  for (Expr *it = expr_list; it != nullptr; it = it->next)
    num_param += 1;
}

/**
 * Attribute this call expression.
 *
 * @param attrib_info Info needed for attribution and code generation
 *                    later on
 */
void CallExpr::attribute(AttribInfo *attrib_info) {
  // Fill the expressions with parameter registers
  uint8_t param_reg = 200;

  // Loop over list of expressions
  resultReg = attrib_info->nextReg;
  for (Expr *it = expr_list; it != nullptr; it = it->next) {
    attrib_info->nextReg = resultReg + num_param;
    it->attribute(attrib_info);
    it->resultReg = param_reg++;
  }
  attrib_info->nextReg = static_cast<uint8_t>(resultReg + 1);
}

/**
 * Pass the symbol tables down to the expressions in the parameterlist.
 *
 * @param symbol_tables Symbol tables to be passed down
 */
void CallExpr::set_symbols(SymbolTables *symbol_tables) {
  Expr::set_symbols(symbol_tables);
  if (expr_list != nullptr)
    expr_list->set_symbols(symbol_tables);
}

/**
 * Generate code for this function call.
 *
 * @param generator The bytecode generator
 * @param attrib_info Info from attribution
 */
void CallExpr::generate_code(VM::ByteCode::Generator *generator,
                             AttribInfo *attrib_info) {
  // Fill the expressions with parameter registers
  uint8_t param_reg = 200;

  // Make sure function is defined
  auto functionIndex = attrib_info->functionAddress.find(name);
  if (functionIndex == attrib_info->functionAddress.end()) {
    // TODO(cluosh): Print error message
    return;
  }

  // Get information about the function to be called
  uint32_t local_addr = functionIndex->second;
  uint64_t addr = attrib_info->functionTable[local_addr].address;

  // Assign local index to the call
  VM::Data::Instruction call;
  const auto &localIndex = VM::Data::buffer_u32(local_addr);
  std::copy(localIndex.begin(), localIndex.begin() + 3, std::begin(call.op));

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

  // Move parameters to safe registers
  /*for (uint8_t i = 0; i < num_param; i++) {
    if (symbol_by_reg(param_reg + i) == nullptr)
      break;
    VM::Data::Instruction bc;
    bc.op[0] = VM::OP_MOV;
    bc.op[1] =
  }*/

  // Generate parameter expressions
  for (Expr *it = expr_list; it != nullptr; it = it->next)
    it->generate_code(generator, attrib_info);

  // Generate call
  generator->instruction(call);
  attrib_info->codeCounter += 1;
}

}  // namespace AST
