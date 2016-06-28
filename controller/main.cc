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

#include "lang/parser/controller.h"
#include "vm/interpreter.h"

/**
 * Main entry point. Check arguments and decide whether to compile bytecode
 * or execute it.
 *
 * @param argc Number of arguments
 * @param argv Arguments
 * @return 0 if no error occurred
 */
int main(int argc, char **argv) {
  // Check if there are at least 3 parameters if repl is not active
  if (argc == 0) {
    std::cerr << "Program name not specified.\n";
    return 1;
  } else if (argc != 1 && argc < 3) {
    std::cerr << "Usage: " << argv[0] << " [compile | execute] file1 file2 "
        << "file3 ...\n";
    return 1;
  }

  // Check, which program mode has been selected
  if (argc < 2) {
    std::cerr << "REPL not yet implemented.\n";
    return 1;
  } else if (std::string(argv[1]) == "execute") {
    VM::Interpreter interpreter;

    // Load all specified modules into the interpreter
    for (unsigned int i = 2; i < argc; i++) {
      if (!interpreter.add_module(std::string(argv[i]))) {
        std::cerr << "One of the specified modules failed to load, abort...\n";
        return 1;
      }
    }

    // Link modules
    if (!interpreter.link()) {
      std::cerr << "Couldn't link modules, abort...\n";
      return 1;
    }

    // Execute code
    if (!interpreter.execute()) {
      std::cerr << "Error during execution.\n";
      return 1;
    }
  } else if (std::string(argv[1]) == "compile") {
    Parser::Controller controller;

    // Compile all files to bytecode
    bool error = false;
    for (unsigned int i = 2; i < argc; i++) {
      if (!controller.parse(std::string(argv[i]))) {
        std::cerr << "Compile errors in \"" << argv[i] << "\".\n";
        error = true;
      }
    }

    // Check if there were compile errors
    if (error) {
      std::cerr << "There were compile errors.\n";
      return 1;
    }
  }
  return 0;
}
