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
#ifndef LANG_AST_DATA_VAR_H_
#define LANG_AST_DATA_VAR_H_

#include <string>

#include "lang/ast/expr/expr.h"

namespace AST {

/**
 * A list of variables/parameters.
 */
class Var : public Expr {
 private:
  std::string name;
  Var *next = nullptr;

 public:
  Var(std::string name, Var *next, VM::Type type);
  ~Var();

  void attribute(FuncAddr *func_addr, Attribute *attr,
                 ConstPool *constants) override;
  void generate_code(VM::Generator *generator) override { }
  void set_symbols(SymbolTables *symbol_tables) override;
  void register_var();
};

}  // namespace AST

#endif  // LANG_AST_DATA_VAR_H_
