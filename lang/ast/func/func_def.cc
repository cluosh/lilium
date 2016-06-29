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
#include <iostream>
#include <string>

#include "lang/ast/func/func_def.h"
#include "vm/opcodes.h"

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

  // Mark expression as tail expression
  expr->set_last(true);
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
 *
 * @param attrib_info Info needed for attribution and code generation
 *                    later on
 */
void FuncDef::attribute(AttribInfo *attrib_info) {
  push_frame();

  // Register variables
  if (var_list != nullptr)
    var_list->register_var();

  // Check semantics in wrapped expression and set result register
  result_reg = attrib_info->next_reg;
  expr->attribute(attrib_info);

  // Store function name if it hasn't been stored yet
  if (!attrib_info->func_addr.func_declared(name)) {
    attrib_info->func_addr.add_func(name, 0, expr->get_type());
  } else {
    std::uint32_t local_addr = attrib_info->func_addr.get_local_addr(name);
    if (attrib_info->func_addr.get_type(local_addr) != expr->get_type()) {
      std::cerr << "Two different types for function \"" << name << "\" "
          << "have been declared, abort.\n";
      std::exit(EXIT_FAILURE);
    }
  }

  pop_frame();
}

/**
 * Generate code for the function definition.
 *
 * @param generator Code generator helper class
 * @param attrib_info Attribute information needed for code generation
 */
void FuncDef::generate_code(VM::Generator *generator,
                            AttribInfo *attrib_info) {
  if (!attrib_info->func_addr.func_declared(name)) {
    std::cerr << "Could not find function \""
        << name
        << "\" in function name table.\n";
    return;
  }

  // Store code address in function
  std::uint32_t local_addr = attrib_info->func_addr.get_local_addr(name);
  attrib_info->func_addr.set_addr(local_addr, attrib_info->code_counter);
  expr->generate_code(generator, attrib_info);

  // Add return instruction
  VM::ByteCode bc;
  bc.all = 0;
  bc.op[0] = VM::OP_RETURN;
  generator->instruction(bc);
  attrib_info->code_counter += 1;
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
