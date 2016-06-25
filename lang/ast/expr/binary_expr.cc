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
#include "lang/ast/expr/binary_expr.h"

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
    : Expr(VM::TYPE_COUNT, next) {
  this->fst = fst;
  this->snd = snd;
  this->op = op;

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
 * Attribute a binary expression.
 *
 * @param func_addr Pointer to map of function addresses
 * @param attr Attribute containing current code position count and
 *             next register
 * @param constants Constant pool of the module
 */
void BinaryExpr::attribute(FuncAddr *func_addr, Attribute *attr,
                           ConstPool *constants) {
  // Set result register and increase register counter
  result_reg = attr->next_reg;
  attr->next_reg += 1;

  // Attribute the operands
  fst->attribute(func_addr, attr, constants);
  snd->attribute(func_addr, attr, constants);

  // Reset attribute register counter
  attr->next_reg -= 1;
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
