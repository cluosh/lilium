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
 * @param attrib_info Info needed for attribution and code generation
 *                    later on
 */
void FuncDef::attribute(AttribInfo *attrib_info) {
  push_frame();

  // Register variables
  if (var_list != nullptr)
    var_list->register_var(200);

  // Check semantics in wrapped expression and set result register
  resultReg = attrib_info->nextReg;
  expr->attribute(attrib_info);

  // Store function name if it hasn't been stored yet
  const auto &functionAddress = attrib_info->functionAddress.find(name);
  if (functionAddress == attrib_info->functionAddress.end()) {
    attrib_info->functionAddress.insert({name, attrib_info->functionTable.size()});

    // Create entry in function table
    VM::Data::FunctionTableEntry entry = {};
    entry.name = name;
    entry.address = 0;

    // Create parameter list
    entry.parameterTypes.push_back(expr->get_type());
    for (Var *var = var_list; var != nullptr; var = var->next)
      entry.parameterTypes.push_back(var->get_type());
    attrib_info->functionTable.push_back(entry);
  } else {
    // TODO(cluosh): Check parameter types
  }

  pop_frame();
}

/**
 * Generate code for the function definition.
 *
 * @param generator Code generator helper class
 * @param attrib_info Attribute information needed for code generation
 */
void FuncDef::generate_code(VM::ByteCode::Generator *generator,
                            AttribInfo *attrib_info) {
  auto functionIndex = attrib_info->functionAddress.find(name);
  if (functionIndex == attrib_info->functionAddress.end()) {
    std::cerr << "Could not find function \""
        << name
        << "\" in function name table.\n";
    return;
  }

  // Store code address in function
  uint32_t local_addr = functionIndex->second;
  attrib_info->functionTable[local_addr].address = attrib_info->codeCounter;
  expr->generate_code(generator, attrib_info);

  // Add return instruction
  VM::Data::Instruction bc = {};
  bc.opcode = VM::OP_RETURN;
  generator->instruction(bc);
  attrib_info->codeCounter += 1;
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
