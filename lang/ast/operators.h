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
#ifndef LANG_AST_OPERATORS_H_
#define LANG_AST_OPERATORS_H_

namespace AST {

/**
 * Enumeration describing all binary operators.
 */
enum BinaryOperator {
  BINARY_ADD,
  BINARY_SUB,
  BINARY_MUL,
  BINARY_DIV,
  BINARY_AND,
  BINARY_OR,
  BINARY_GT,
  BINARY_LT,
  BINARY_GE,
  BINARY_LE,
  BINARY_EQ,
  BINARY_NE,
  BINARY_COUNT
};

}  // namespace AST

#endif  // LANG_AST_OPERATORS_H_
