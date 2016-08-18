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

#include "cc/ast/expr/global_expr.h"
#include "vm/bytecode/generator.h"
#include "vm/types/types.h"

namespace AST {

/**
 * An expression in the AST.
 */
class Expr : public GlobalExpr {
 private:
  VM::Type type;
  bool last = false;

 public:
  Expr *next = nullptr;

  Expr(VM::Type type, Expr *next);
  virtual ~Expr() = default;

  virtual void attribute(AttribInfo *attrib_info) = 0;
  virtual void generate_code(VM::ByteCode::Generator *generator,
                             AttribInfo *attrib_info) = 0;
  virtual void set_symbols(SymbolTables *symbol_tables);

  virtual void set_last(bool last);
  VM::Type get_type();

 protected:
  bool is_last();
  void choose_type(Expr *expr1, Expr *expr2);
};

}  // namespace AST

#endif  // LANG_AST_EXPR_EXPR_H_
