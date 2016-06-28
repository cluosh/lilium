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
 * Free all child nodes.
 */
Program::~Program() {
  for (auto const &expr : expr_list) {
    expr->remove_symbols();
    delete expr;
  }
}

/**
 * Add a new node to the AST.
 *
 * @param expr New global expression
 */
void Program::add(GlobalExpr *expr) {
  expr_list.push_back(expr);
  expr->set_symbols(new SymbolTables());
}

/**
 * Attribute the AST with symbol tables.
 */
void Program::attribute_tree() {
  // Attribute all expressions
  for (auto const &expr : expr_list) {
    attrib_info.next_reg = 0;
    expr->attribute(&attrib_info);
  }
}

/**
 * Generate the bytecode from the AST.
 *
 * @param generator Generator used to write the bytecode
 */
void Program::generate_code(VM::Generator *generator) {
  // Forward-pass for reference resolving
  generator->set_disabled(true);
  attrib_info.code_counter = 0;
  for (auto const &expr : expr_list) {
    expr->generate_code(generator, &attrib_info);
  }

  // Print header
  generator->module_header(
      "Module", static_cast<std::uint32_t>(attrib_info.func_addr.get_count()),
      static_cast<std::uint16_t>(attrib_info.constants.size()),
      static_cast<std::uint64_t>(attrib_info.code_counter));

  // Generate function table
  generator->function_table(&attrib_info.func_addr);

  // Generate code for all expressions
  generator->set_disabled(false);
  attrib_info.code_counter = 0;
  for (auto const &expr : expr_list) {
    expr->generate_code(generator, &attrib_info);
  }
}

}  // namespace AST
