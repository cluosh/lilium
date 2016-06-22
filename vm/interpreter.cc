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
#include <cstdint>
#include <iostream>
#include <fstream>
#include <string>
#include <utility>
#include <vector>

#include "vm/bytecode/bytecode_loader.h"
#include "vm/interpreter.h"

namespace VM {

/**
 * Set initial container sizes.
 */
Interpreter::Interpreter() {
  // Set initial entry symbol
  entry_point = { nullptr, 0xFFFFFFFF, 0xFFFF };

  // Just some values, not empirically relevant, should record some statistics
  function_symbols.reserve(100);
  unresolved_symbols.reserve(100);
  modules.reserve(20);
}

/**
 * Load a bytecode module into the interpreter.
 *
 * @param filename Path to the bytecode module
 * @return False if an error happened during bytecode parsing
 */
bool Interpreter::add_module(std::string filename) {
  // Create a new module
  std::uint16_t num_modules = this->num_modules;
  modules.push_back(std::unique_ptr<Module>(
      new (std::nothrow) Module(num_modules)));
  Module *module = modules.back().get();

  // Load bytecode into module
  bool loaded = BytecodeLoader::load_module(filename, module);
  if (!loaded) {
    std::cerr << "Could not load bytecode module \"" << filename << "\"\n";
    return false;
  }

  // Read module function symbols
  std::uint32_t num_functions = module->num_functions;
  std::string *names = module->funcs->get_names();
  std::uint64_t *addr = module->funcs->get_addr();
  for (std::uint32_t i = 0; i < num_functions; i++) {
    // Check if function symbol has already been declared
    const auto symbol_entry = function_symbols.find(names[i]);
    if (symbol_entry != function_symbols.end()) {
      std::cerr << "Function \""
          << names[i]
          << "\" has already been declared "
          << " in module \""
          << modules[symbol_entry->second.module_id]->module_name
          << "\".\n";
      return false;
    }

    // Add function symbol to table if it's a function local to the module
    FuncSym sym = { names[i].c_str(), i, num_modules};
    if (addr[i] != 0xFFFFFFFFFFFFFFFF) {
      function_symbols[names[i]] = sym;

      // Check if the symbol is an entry point
      if (names[i] == "start")
        entry_point = sym;
    } else {
      unresolved_symbols.push_back(sym);
    }
  }

  // Increase number of modules and return
  this->num_modules++;
  return true;
}

/**
 * Link all function symbols in all modules.
 *
 * @return False, if a function symbol couldn't be found
 */
bool Interpreter::link() {
  // Check if an entry point has been found
  if (entry_point.local_addr == 0xFFFFFFFF) {
    std::cerr << "No \"start\" method has been defined in the given modules\n";
    return false;
  }

  // Resolve all unresolved symbols
  for (const auto &symbol : unresolved_symbols) {
    const auto symbol_entry = function_symbols.find(symbol.name);
    if (symbol_entry == function_symbols.end()) {
      std::cerr << "Unresolved symbol \""
          << symbol.name
          << "\" in module \""
          << modules[symbol.module_id]->module_name
          << "\".\n";
      return false;
    }

    // Resolve symbol in the module
    FunctionTable *funcs = modules[symbol.module_id]->funcs;
    FunctionTable *funcs_defined =
        modules[symbol_entry->second.module_id]->funcs;

    // Check the types, if the types are correct, continue
    if (funcs->get_types()[symbol.local_addr]
        != funcs_defined->get_addr()[symbol_entry->second.local_addr]) {
      std::cerr << "Symbol \""
          << symbol.name
          << "\" has been declared with different types.\n";
      return false;
    }

    // Link function addresses
    funcs->get_addr()[symbol.local_addr] =
        funcs_defined->get_addr()[symbol_entry->second.local_addr];
    funcs->get_module_ids()[symbol.local_addr] =
        symbol_entry->second.module_id;
  }

  // Remove unused data in function tables
  for (const auto &module : modules)
    module->funcs->remove_unused();

  // Cleanup table of unresolved functions
  unresolved_symbols.clear();
  return true;
}

/**
 * Start code execution at entry point.
 */
bool Interpreter::execute() {
  // Check if entry point is defined
  if (entry_point.local_addr == 0xFFFFFFFF) {
    std::cerr << "No entry point defined\n";
    return false;
  }

  // Set code pointer to the current entry point module
  Module *module = modules[entry_point.module_id].get();
  ByteCode *code = module->code;
  std::uint64_t *func_addr = module->funcs->get_addr();
  std::uint16_t *func_module = module->funcs->get_module_ids();
  std::uint64_t pc = entry_point.local_addr - 1;

  // Call stack for function calls
  Module *cs_modules[20];
  std::uint64_t cs_pc[20];
  std::uint64_t cs_top = 0;

  // Register definitions
  std::uint64_t reg[256];

  // Jump-table for token threaded code
  static void *token_table[] = {
      && op_nop,
      && op_calli,
      && op_calle,
      && op_return,
      && op_addi,
      && op_subi,
      && op_muli,
      && op_divi,
      && op_halt
  };

  // Macro for easy dispatching
#define DP() goto *token_table[code[++pc].op[0]];

  // Main dispatch loop
  DP();
  op_nop:
    DP();
  op_calli:
    // Store module and program counter on stack
    cs_modules[cs_top] = module;
    cs_pc[cs_top] = pc;
    cs_top++;

    // Jump to new address in code
    pc = func_addr[code[pc].all & 0xFFFFFF];
    DP();
  op_calle:
    // Store module and program counter on stack
    cs_modules[cs_top] = module;
    cs_pc[cs_top] = pc;
    cs_top++;

    // Change to new module and jump to new address in code
    pc = func_addr[code[pc].all & 0xFFFFFF];
    module = modules[func_module[code[pc].all & 0xFFFFFF]].get();
    func_addr = module->funcs->get_addr();
    func_module = module->funcs->get_module_ids();
    code = module->code;
    DP();
  op_return:
    // Retrieve module and program counter from stack
    cs_top--;
    module = cs_modules[cs_top];
    pc = cs_pc[cs_top];

    // Change to old module
    func_addr = module->funcs->get_addr();
    func_module = module->funcs->get_module_ids();
    code = module->code;
    DP();
  op_addi:
    reg[code[pc].op[3]] = reg[code[pc].op[1]] + reg[code[pc].op[2]];
    DP();
  op_subi:
    reg[code[pc].op[3]] = reg[code[pc].op[1]] - reg[code[pc].op[2]];
    DP();
  op_muli:
    reg[code[pc].op[3]] = reg[code[pc].op[1]] * reg[code[pc].op[2]];
    DP();
  op_divi:
    reg[code[pc].op[3]] = reg[code[pc].op[1]] / reg[code[pc].op[2]];
    DP();
  op_halt:
    return true;

  // This should be unreachable
  return false;
}

}  // namespace VM
