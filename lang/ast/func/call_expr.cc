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
#include "lang/ast/func/call_expr.h"

namespace AST {

/**
 * Create a call expression with an id (function to be called) and
 * a list of expressions to be passed as parameters.
 *
 * @param name Name of the function to be called
 * @param expr_list List of parameter expressions
 * @param next Next expression in expression list
 */
CallExpr::CallExpr(std::string name, Expr *expr_list, Expr *next)
    : Expr(VM::TYPE_COUNT, next) {
  this->name = name;
  this->expr_list = expr_list;
}

/**
 * Attribute this call expression.
 *
 * @param func_addr Pointer to map of function addresses
 * @param attr Attribute containing current code position count and
 *             next register
 * @param constants Constant pool of the module
 */
void CallExpr::attribute(FuncAddr *func_addr, Attribute *attr,
                         ConstPool *constants) {
  if (expr_list != nullptr)
    expr_list->attribute(func_addr, attr, constants);
}

/**
 * Pass the symbol tables down to the expressions in the parameterlist.
 *
 * @param symbol_tables Symbol tables to be passed down
 */
void CallExpr::set_symbols(SymbolTables *symbol_tables) {
  if (expr_list != nullptr)
    expr_list->set_symbols(symbol_tables);
}

}  // namespace AST
