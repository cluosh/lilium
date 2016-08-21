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

#include <cstdint>
#include <string>

#include "cc/ast/common/attribute.h"
#include "cc/ast/common/symbol.h"
#include "vm/bytecode/generator.h"

namespace AST {

/**
 * A global definition in a program, either a expression without variables
 * or a function definition.
 */
class GlobalExpr {
 private:
  SymbolTables *symbol_tables = nullptr;

 public:
  uint8_t resultReg;

  virtual ~GlobalExpr() = default;

  // Retrieve name of the expression, if it's a function
  virtual std::string get_name();

  // Attribution of the expressions
  virtual void attribute(AttribInfo *attrib_info) = 0;
  virtual void generate_code(VM::ByteCode::Generator *generator, AttribInfo *attrib_info) = 0;
  virtual void set_symbols(SymbolTables *symbol_tables);
  void remove_symbols();
  uint8_t get_result_reg();

 protected:
  Symbol *symbol(std::string name);
  Symbol *symbol_by_reg(uint8_t reg);
  void add_symbol(std::string name, Symbol symbol);
  void push_frame();
  void pop_frame();
};

}  // namespace AST

#endif  // LANG_AST_EXPR_GLOBAL_EXPR_H_
