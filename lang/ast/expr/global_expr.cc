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

}  // namespace AST