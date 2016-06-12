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
#include <new>
#include <string>

#include "vm/function_table.h"

namespace VM {

/**
 * Initialize a function table with the size. The size is fixed
 * for one module and can't be changed after initialization.
 *
 * @param size Size of the function table
 */
FunctionTable::FunctionTable(std::uint32_t size) {
  // Allocate memory
  names = new (std::nothrow) std::string[size];
  addr = new (std::nothrow) std::uint64_t[size];
  module_id = new (std::nothrow) std::uint16_t[size];
}

/**
 * Free the allocated memory.
 */
FunctionTable::~FunctionTable() {
  delete names;
  delete addr;
  delete module_id;
}

/**
 * Retrieve the names of all functions.
 *
 * @return String array
 */
std::string *FunctionTable::get_names() {
  return names;
}

/**
 * Retrieve the addresses of all functions.
 *
 * @return Address array
 */
std::uint64_t *FunctionTable::get_addr() {
  return addr;
}

/**
 * Retrieve the module ids for all functions.
 *
 * @return Array with module ids
 */
std::uint16_t *FunctionTable::get_module_ids() {
  return module_id;
}

}  // namespace VM
