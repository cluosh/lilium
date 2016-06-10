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
#ifndef LANG_AST_EXPR_CONST_INT_H_
#define LANG_AST_EXPR_CONST_INT_H_

#include <cstdint>

#include "lang/ast/expr/expr.h"

namespace AST {

/**
 * A constant integer expression.
 */
class ConstInt : public Expr {
 private:
  std::int64_t value;

 public:
  explicit ConstInt(std::int64_t value);

  void attribute() { }
};

}  // namespace AST

#endif  // LANG_AST_EXPR_CONST_INT_H_
