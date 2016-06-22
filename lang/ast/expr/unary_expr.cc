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
#include "lang/ast/expr/unary_expr.h"

namespace AST {

/**
 * Initialize an unary expression with an enclosed expression and operator.
 *
 * @param expr Enclosed expression
 * @param op Unary operation
 * @param next Next expression in list
 */
UnaryExpr::UnaryExpr(Expr *fst, UnaryOperator op, Expr *next)
    : Expr(VM::TYPE_COUNT, next) {
  this->fst = fst;
  this->op = op;

  // Update type of expression
  choose_type(fst, nullptr);
}

/**
 * Clean up inner expression.
 */
UnaryExpr::~UnaryExpr() {
  delete fst;
}

/**
 * Assign symbols to operand and expression.
 *
 * @param symbol_tables Symbol tables to be assigned
 */
void UnaryExpr::set_symbols(SymbolTables *symbol_tables) {
  Expr::set_symbols(symbol_tables);

  // Assign symbols to operand
  fst->set_symbols(symbol_tables);
}

}  // namespace AST
