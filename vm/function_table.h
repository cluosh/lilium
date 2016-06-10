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
#ifndef VM_FUNCTION_TABLE_H_
#define VM_FUNCTION_TABLE_H_

#include <cstdint>
#include <string>

namespace VM {

/**
 * Store all function symbols in a modules table.
 */
class FunctionTable {
 private:
  std::string *names;
  std::uint64_t *global_addr;

 public:
  explicit FunctionTable(std::uint32_t size);

  std::string *get_names();
  std::uint64_t *get_addr();
};

}  // namespace VM

#endif  // VM_FUNCTION_TABLE_H_
