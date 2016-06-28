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
#ifndef VM_BYTECODE_FUNC_ADDR_H_
#define VM_BYTECODE_FUNC_ADDR_H_

#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>

#include "vm/types/types.h"

namespace VM {

/**
 * Class for handling the storage of function addresses within modules.
 */
class FuncAddr {
 private:
  std::vector<std::uint64_t> addr;
  std::vector<Type> type;
  std::vector<std::string> name;
  std::unordered_map<std::string, std::uint32_t> local_addr;
  std::uint32_t count = 0;

 public:
  void add_func(std::string, std::uint64_t addr, Type type);
  std::uint64_t get_addr(std::uint32_t local_addr);
  void set_addr(std::uint32_t local_addr, std::uint64_t addr);
  Type get_type(std::uint32_t local_addr);
  std::uint32_t get_local_addr(std::string);
  bool func_declared(std::string);
  std::uint32_t get_count();
  std::string get_name(std::uint32_t local_addr);
};

}  // namespace VM

#endif  // VM_BYTECODE_FUNC_ADDR_H_
