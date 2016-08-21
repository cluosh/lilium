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
#include <cstdlib>
#include <string>
#include <iostream>

#include "cc/ast/data/var.h"

namespace AST {

/**
 * Initialize a variable list node.
 *
 * @param name Name of the variable
 * @param next Next var in list
 * @param type Type of the variable
 */
Var::Var(std::string name, Var *next, VM::Type type) : Expr(type, nullptr) {
  this->name = name;
  this->next = next;
}

/**
 * Free all variable list nodes.
 */
Var::~Var() {
  delete next;
}

/**
 * Check, whether the variable is defined and whether the type is
 * an acceptable type.
 *
 * @param attrib_info Info needed for attribution and code generation
 *                    later on
 */
void Var::attribute(AttribInfo *attrib_info) {
  // Check whether the variable is defined
  const Symbol *sym = symbol(name);
  if (sym == nullptr) {
    // Variable undefined
    std::cerr << "Variable " << name << " undefined\n";
    std::exit(EXIT_FAILURE);
  } else if (sym->type != get_type() && get_type() != VM::TYPE_COUNT) {
    std::cerr << "Variable " << name << " has been used with "
        << "different types\n";
    std::exit(EXIT_FAILURE);
  } else {
    // Set the correct register and correct type for this variable
    resultReg = sym->reg;
  }
}

/**
 * Assign symbols to this variable.
 *
 * @param symbol_tables Symbol tables to be assigned
 */
void Var::set_symbols(SymbolTables *symbol_tables) {
  Expr::set_symbols(symbol_tables);

  // Assign symbol table to next variable, if defined
  if (next != nullptr)
    next->set_symbols(symbol_tables);
}

/**
 * Add a variable as symbol to the symbol table, propagate to other
 * Variables in list.
 *
 * @param reg The parameter register, where this and subsequent variables
 *            should be stored
 */
void Var::register_var(uint8_t reg) {
  // Check, whether the variable is registered already
  if (symbol(name) != nullptr) {
    // Variable already assigned
    std::cerr << "Symbol \"" << name << "\" is already defined\n";
    std::exit(EXIT_FAILURE);
  }

  // Create symbol and add it to the table
  Symbol sym = {reg, get_type()};
  add_symbol(name, sym);
  resultReg = reg;

  // TODO(cluosh): Variables on stack if there are no registers left

  // Register other vars in list
  reg += 1;
  if (next != nullptr)
    next->register_var(reg);
}

}  // namespace AST
