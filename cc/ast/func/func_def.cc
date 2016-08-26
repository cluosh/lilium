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

#include "cc/ast/func/func_def.h"
#include "vm/constants/opcodes.h"

namespace AST {

/**
 * Initialize a function definition.
 *
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
 * Free child nodes.
 */
FuncDef::~FuncDef() {
  delete var_list;
  delete expr;
}

/**
 * Attribute a function definition.
 *
 * @param info Info needed for attribution and code generation later on
 */
void FuncDef::attribute(AttribInfo *info) {
  push_frame();

  // Start with register one, zero is return register
  resultReg = 0;
  info->nextReg = 1;

  // Creaty function table entry
  VM::Data::FunctionTableEntry entry{};
  entry.name = name;
  entry.external = false;

  // Register function arguments
  if (var_list != nullptr)
    var_list->register_var(info->nextReg);

  // Check semantics in wrapped expression, set result register and reserve maximum space
  info->nextReg = entry.parameterTypes.size();
  info->maxReg = info->nextReg;
  expr->attribute(info);

  // Fill function table entry with type information
  entry.parameterTypes.push_back(expr->get_type());
  for (Var *var = var_list; var != nullptr; var = var->next)
    entry.parameterTypes.push_back(var->get_type());

  // Use maximum register index as reservation marker
  entry.reservation = info->maxReg;

  // Store function name if it hasn't been stored yet
  const auto &address = info->functionAddress.find(name);
  if (address == info->functionAddress.end()) {
    info->functionAddress.insert({name, info->functionTable.size()});
    info->functionTable.push_back(entry);
  } else {
    const auto &function = info->functionTable[address->second];

    // Check if this is a function redefinition (if external is false, it's a legit definition)
    if (!function.external)
      throw std::runtime_error("Function \"" + name + "\" has been defined more than once");

    // No redefinition, no check if types are equal
    if (function.parameterTypes != entry.parameterTypes) {
      throw std::runtime_error("Function \"" + name + "\" has been used with different "
                               "parameter types");
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
void FuncDef::generate_code(VM::ByteCode::Generator *generator, AttribInfo *info) {
  const auto &index = info->functionAddress.find(name);
  if (index == info->functionAddress.end())
    throw std::runtime_error("Could not find function \"" + name + "\" in function name table");

  // Store code address in function
  uint32_t functionIndex = index->second;
  info->functionTable[functionIndex].address = info->codeCounter;
  expr->generate_code(generator, info);

  // Add return instruction
  VM::Data::Instruction bc{};
  bc.opcode = VM::OP_RETURN;
  generator->instruction(bc);

  // Increase code (instruction) counter
  info->codeCounter += 1;
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
