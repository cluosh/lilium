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

#include "lang/ast/func/func_def.h"

namespace AST {

/**
 * @brief Initialize a function definition
 * @param name Function name
 * @param var_list List of function parameters
 * @param expr Function body/root expression
 */
FuncDef::FuncDef(std::string name, Var *var_list, Expr *expr) {
  this->name = name;
  this->var_list = var_list;
  this->expr = expr;
}

/**
 * @brief Free child nodes
 */
FuncDef::~FuncDef() {
  delete var_list;
  delete expr;
}

/**
 * Attribute a function definition.
 */
void FuncDef::attribute() {
  push_frame();

  // Register variables
  var_list->register_var();

  // Check semantics in wrapped expression
  expr->attribute();

  pop_frame();
}

/**
 * Assign symbols to function definition.
 *
 * @param symbol_tables Symbol tables to be assigned
 */
void FuncDef::set_symbols(SymbolTables *symbol_tables) {
  GlobalExpr::set_symbols(symbol_tables);

  // Assign symbols to parameters and expressions
  if (var_list != nullptr)
    var_list->set_symbols(symbol_tables);
  expr->set_symbols(symbol_tables);
}

}  // namespace AST
