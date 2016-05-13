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

#include "lang/ast/program.h"

namespace AST {

/**
 * @brief Free all child nodes
 */
Program::~Program() {
  for (auto const &expr : expr_list) {
    delete expr;
  }
}

/**
 * @brief Add a new node to the AST
 * @param expr New global expression
 */
void Program::add(GlobalExpr *expr) {
  expr_list.push_back(expr);
}

/**
 * @brief Create a textual representation of the AST
 * @return Textual representation of the AST
 */
std::string Program::to_string() {
  std::string out = "Program";
  for (auto const &expr : expr_list) {
    out += expr->to_string();
  }
  out += "\n";
  return out;
}

}  // namespace AST
