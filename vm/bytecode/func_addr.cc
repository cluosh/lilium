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
#include "vm/bytecode/func_addr.h"

namespace VM {

/**
 * Add a new function to the storage container.
 *
 * @param name Name of the function
 * @param addr Address of the function
 * @param type Type of the function
 */
void FuncAddr::add_func(std::string name, uint64_t addr, Type type) {
  local_addr[name] = count;
  this->addr.push_back(addr);
  this->type.push_back(type);
  this->name.push_back(name);
  count += 1;
}

/**
 * Get the type of a function.
 *
 * @param local_addr The module-local address of the function
 * @return Type of the given function
 */
Type FuncAddr::get_type(uint32_t local_addr) {
  if (local_addr >= type.size())
    return TYPE_COUNT;
  return type[local_addr];
}

/**
 * Get the address of a function.
 *
 * @param local_addr The module-local address of the function
 * @return Address to the position in module code
 */
uint64_t FuncAddr::get_addr(uint32_t local_addr) {
  if (local_addr >= addr.size())
    return 0;
  return addr[local_addr];
}

/**
 * Set the address of a function.
 *
 * @param local_addr Address of the function in this data structure
 * @param addr Global address of function in this module (code pointer)
 */
void FuncAddr::set_addr(uint32_t local_addr, uint64_t addr) {
  this->addr[local_addr] = addr;
}

/**
 * Check if a function has been declared already.
 *
 * @param name Name of the function
 * @return True, if a function has been declared or used
 */
bool FuncAddr::func_declared(std::string name) {
  return local_addr.find(name) != local_addr.end();
}

/**
 * Get the local address for a function.
 *
 * @param name Name of the function
 * @return Local address in this data structure
 */
uint32_t FuncAddr::get_local_addr(std::string name) {
  if (local_addr.find(name) == local_addr.end())
    return 0;
  return local_addr[name];
}

/**
 * Get number of functions used in this module.
 *
 * @return Number of functions stored in this structure
 */
uint32_t FuncAddr::get_count() {
  return count;
}

/**
 * Find the name by a local address.
 *
 * @param local_addr Local address of the function
 * @return Name of the function
 */
std::string FuncAddr::get_name(uint32_t local_addr) {
  return name[local_addr];
}

}  // namespace VM
