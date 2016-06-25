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
#ifndef LANG_AST_EXPR_EXPR_H_
#define LANG_AST_EXPR_EXPR_H_

#include <string>

#include "vm/types/types.h"
#include "lang/ast/expr/global_expr.h"

namespace AST {

/**
 * An expression in the AST.
 */
class Expr : public GlobalExpr {
 private:
  VM::Type type;
  Expr *next = nullptr;

 public:
  Expr(VM::Type type, Expr *next);
  virtual ~Expr() = default;

  virtual void attribute(FuncAddr *func_addr, Attribute *attr,
                         ConstPool *constants) = 0;
  virtual void set_symbols(SymbolTables *symbol_tables);

  void set_next(Expr *next);
 protected:
  void choose_type(Expr *expr1, Expr *expr2);
  VM::Type get_type();
};

}  // namespace AST

#endif  // LANG_AST_EXPR_EXPR_H_
