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
#include "lang/ast/expr/global_expr.h"

namespace AST {

/**
 * Remove and deallocate symbols.
 */
void GlobalExpr::remove_symbols() {
  delete symbol_tables;
  symbol_tables = nullptr;
}

/**
 * Assign symbols to the global expression.
 *
 * @param symbol_tables Stack of symbol tables to be assigned
 */
void GlobalExpr::set_symbols(SymbolTables *symbol_tables) {
  this->symbol_tables = symbol_tables;
}

/**
 * Look for a variable in the symbol tables and return it's symbol. If
 * the variable is not defined, nullptr is returned.
 *
 * @param name Name of the variable
 * @return Pointer to the symbol or nullptr if not found
 */
const Symbol *GlobalExpr::symbol(std::string name) {
  for (auto const &symbol_table : *symbol_tables) {
    auto it = symbol_table.find(name);
    if (it != symbol_table.end())
      return &it->second;
  }
  return nullptr;
}

/**
 * Add a symbol to the current top of the symbol table stack.
 *
 * @param name Name of the symbol
 * @param symbol Symbol to be added
 */
void GlobalExpr::add_symbol(std::string name, Symbol symbol) {
  auto symbol_table = &symbol_tables->back();
  if (symbol_table->find(name) != symbol_table->end()) {
    // TODO(cluosh): Symbol already defined
    return;
  }
  symbol_table->insert({name, symbol});
}

/**
 * Push a new scope frame to the symbol table stack.
 */
void GlobalExpr::push_frame() {
  symbol_tables->push_back(SymbolTable());
}

/**
 * Pop the top scope frame from the symbol table stack.
 */
void GlobalExpr::pop_frame() {
  symbol_tables->pop_back();
}

/**
 * Return empty string, function name is implemented by func_def
 *
 * @return Empty string
 */
std::string GlobalExpr::get_name() {
  return "";
}

/**
 * Return register of this expression node.
 *
 * @return Register ID
 */
std::uint8_t GlobalExpr::get_result_reg() {
  return result_reg;
}

}  // namespace AST
