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
#ifndef LANG_AST_EXPR_CONST_H_
#define LANG_AST_EXPR_CONST_H_

#include <cstdint>

#include "cc/ast/expr/expr.h"

namespace AST {

/**
 * A constant expression, corresponds to an entry in
 * the constant pool of the module.
 */
class Const : public Expr {
 private:
  std::int64_t value;
  uint16_t cp_index = 0;

 public:
  Const(std::int64_t value, VM::Type type, Expr *next);

  void attribute(AttribInfo *attrib_info) override;
  void generate_code(VM::Generator *generator,
                     AttribInfo *attrib_info) override;
};

}  // namespace AST

#endif  // LANG_AST_EXPR_CONST_H_