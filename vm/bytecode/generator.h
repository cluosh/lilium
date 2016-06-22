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
#ifndef VM_BYTECODE_GENERATOR_H_
#define VM_BYTECODE_GENERATOR_H_

#include <ostream>
#include <string>

namespace VM {

/**
 * Class for storing methods allowing to generate bytecode.
 */
class Generator {
 private:
  std::ostream out;

 public:
  explicit Generator(const std::ostream &output);

  // Code generation functions
  void module_header(std::string name, std::uint32_t num_func,
                     std::uint16_t num_const, std::uint64_t num_inst);
};

}  // namespace VM

#endif  // VM_BYTECODE_GENERATOR_H_
