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
#include <iostream>

#include "lang/ast/expr/expr.h"

namespace AST {

/**
 * Initialize an expression.
 *
 * @param type Expression type, use TYPE_COUNT for type inferring
 */
Expr::Expr(VM::Type type, Expr *next) {
  this->type = type;
}

/**
 * Call global symbol setting if symbols are set.
 *
 * @param symbol_tables Symbol tables to be set
 */
void Expr::set_symbols(SymbolTables *symbol_tables) {
  GlobalExpr::set_symbols(symbol_tables);

  // Pass symbol to other expressions in expression list
  if (next != nullptr)
    next->set_symbols(symbol_tables);
}

/**
 * Set the next expression in an expression list.
 *
 * @param next Next expression in the list
 */
void Expr::set_next(Expr *next) {
  this->next = next;
}

/**
 * Choose the new type of an expression by comparing two expressions.
 * If the types are incompatible, an error is being thrown.
 * If one of the expression is null, the type of the other expression
 * is inherited. If both expressions are null, the type is left unchanged.
 *
 * @param expr1 First expression to be compared
 * @param expr2 Second expression to be compared
 */
void Expr::choose_type(Expr *expr1, Expr *expr2) {
  // Check expressions
  if (expr1 == nullptr && expr2 == nullptr) {
    return;
  } else if (expr1 == nullptr) {
    type = expr2->type;
    return;
  } else if (expr2 == nullptr) {
    type = expr1->type;
    return;
  }

  // Check if type are equivalent
  if (expr1->type != expr2->type) {
    std::cerr << "Type mismatch during expression parsing.\n";

    // TODO(cluosh): Throw appropriate exception
  }
}

/**
 * Retrieve the type of an expression.
 *
 * @return Type of this expression
 */
VM::Type Expr::get_type() {
  return type;
}

}  // namespace AST
