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

#include "lang/ast/func_def.h"

namespace AST {

/**
 * @brief Initialize a function definition
 * @param name Function name
 * @param var_list List of function parameters
 * @param expr_list Function body/root expression
 */
FuncDef::FuncDef(std::string name, Var *var_list, Expr *expr_list) {
  this->name = name;
  this->var_list = var_list;
  this->expr_list = expr_list;
}

/**
 * @brief Free child nodes
 */
FuncDef::~FuncDef() {
  delete var_list;
  delete expr_list;
}

}  // namespace AST
