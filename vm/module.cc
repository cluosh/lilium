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
#include <iostream>
#include <fstream>

#include "vm/module.h"

namespace VM {

/**
 * Create module with starting address, which is assigned by
 * the interpreter.
 *
 * @param start_address Start of address space for module
 */
Module::Module(std::uint64_t start_address) {
  this->start_address = start_address;
}

/**
 * Free all allocated data structures.
 */
Module::~Module() {
  delete code;
  delete funcs;
  delete constant_pool;
}

}  // namespace VM

