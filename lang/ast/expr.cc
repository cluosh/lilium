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
#include <string>

#include "lang/ast/expr.h"
#include "lang/ast/types.h"

namespace AST {

/**
 * @brief Initialize an expression.
 * @param type Expression type, use TYPE_COUNT for type inferring
 */
Expr::Expr(Type type) {
  this->type = type;
}

/**
 * @brief Generate textual representation of the expression
 * @return Expression representation
 */
std::string Expr::to_string() {
  return to_string("\n");
}

/**
 * @brief Generate textual representation of the expression
 * @param indentation Indentation string for correct tree drawing
 * @return Expression representation
 */
std::string Expr::to_string(std::string indentation) {
  std::string type_string;
  switch (type) {
  case TYPE_INT:
    type_string = "Integer";
    break;
  case TYPE_SINGLEP:
    type_string = "Single-Precision";
    break;
  case TYPE_DOUBLEP:
    type_string = "Double-Precision";
    break;
  case TYPE_STRING:
    type_string = "String";
    break;
  default:
    type_string = "Not yet inferred";
  }
  return indentation + "  - Expression: " + type_string;
}

}  // namespace AST
