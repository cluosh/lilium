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
#ifndef LANG_AST_EXPR_GLOBAL_EXPR_H_
#define LANG_AST_EXPR_GLOBAL_EXPR_H_

#include <string>

#include "lang/ast/common/symbol.h"

namespace AST {

// Type definition for function addresses
typedef std::unordered_map<std::string, std::uint64_t> FuncAddr;

/**
 * A global definition in a program, either a expression without variables
 * or a function definition.
 */
class GlobalExpr {
 private:
  SymbolTables *symbol_tables = nullptr;

 public:
  virtual ~GlobalExpr() = default;

  // Retrieve name of the expression, if it's a function
  virtual std::string get_name();

  // Attribution of the expressions
  virtual void attribute(FuncAddr *func_addr) = 0;
  virtual void set_symbols(SymbolTables *symbol_tables);
  void remove_symbols();

 protected:
  const Symbol *symbol(std::string name);
  void add_symbol(std::string name, Symbol symbol);
  void push_frame();
  void pop_frame();
};

}  // namespace AST

#endif  // LANG_AST_EXPR_GLOBAL_EXPR_H_
