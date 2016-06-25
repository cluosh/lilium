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
#ifndef LANG_AST_FUNC_FUNC_DEF_H_
#define LANG_AST_FUNC_FUNC_DEF_H_

#include <string>

#include "lang/ast/expr/expr.h"
#include "lang/ast/expr/global_expr.h"
#include "lang/ast/data/var.h"

namespace AST {

/**
 * A function definition node in the AST.
 */
class FuncDef : public GlobalExpr {
 private:
  std::string name;
  Var *var_list = nullptr;
  Expr *expr = nullptr;

 public:
  FuncDef(std::string name, Var *var_list, Expr *expr);
  ~FuncDef();

  void attribute(FuncAddr *func_addr, Attribute *attr,
                 ConstPool *constants) override;
  void set_symbols(SymbolTables *symbol_tables) override;
};

}  // namespace AST

#endif  // LANG_AST_FUNC_FUNC_DEF_H_
