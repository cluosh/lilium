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
#include <vector>

#include "objdump/pipeline/dump.h"
#include "vm/pipeline/loader.h"

/**
 * Main interpreter entry point.
 *
 * @param argc Number of arguments
 * @param argv Arguments
 * @return 0 in case of success, 1 otherwise
 */
int main(int argc, char **argv) {
  // Make sure there is at least one parameter (file to be dumped)
  if (argc == 0) {
    return 1;
  } else if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " file \n";
    return 1;
  }

  // Program buffer and pipeline stages
  VM::Data::ProgramBuffer programBuffer;
  VM::Pipeline::Loader loader;
  ObjDump::Pipeline::Dump dump(std::cout);

  // PRE-LOADING STAGE
  // Store module files to be executed in vector
  // and create linker function table storage
  std::vector<VM::Data::FunctionTableEntry> linkerFunctionTable;
  const std::vector<std::string> modules = {std::string(argv[1]) + ".mod"};

  // LOADING STAGE
  // Load all specified bytecode modules
  loader.execute(modules, &programBuffer, &linkerFunctionTable);

  // DUMPING STAGE
  dump.execute(std::string(argv[1]), programBuffer, linkerFunctionTable);
  return 0;
}
