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

#include "lang/ast/var.h"

namespace AST {

/**
 * @brief Initialize a variable list node
 * @param name Name of the variable
 */
Var::Var(std::string name) {
  this->name = name;
}

/**
 * @brief Free all variable list nodes
 */
Var::~Var() {
  delete next;
}

/**
 * @brief Assign pointer to next element
 * @param next New pointer to next element
 */
void Var::set_next(Var *next) {
  this->next = next;
}

/**
 * @brief Create textual representation of variable list
 * @param indentation Indentation level in tree
 * @return Variable names + indentation
 */
std::string Var::to_string(std::string indentation) {
  std::string out = indentation + "  - " + name;
  if (next != nullptr)
    out += next->to_string(indentation);
  return out;
}

}  // namespace AST
