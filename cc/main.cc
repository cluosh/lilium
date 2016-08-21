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

#include "cc/parser/controller.h"

/**
 * Main compiler entrance point.
 *
 * @param argc Number of arguments
 * @param argv Arguments
 * @return 0 if successful, 1 otherwise
 */
int main(int argc, char *argv[]) {
  // Make sure there is at least one parameter (file to be compiled)
  if (argc == 0) {
    return 1;
  } else if (argc < 2) {
    std::cerr << "Usage: " << argv[0] << " file [file ...] \n";
    return 1;
  }

  // Compile each file (maybe in parallel?)
  Parser::Controller compiler;
  for (int i = 1; i < argc; i++) {
    if (!compiler.parse(std::string(argv[i]))) {
      std::cerr << "Compile errors in \"" << argv[i] << "\", compilation aborted.\n";
      return 1;
    }
  }
  return 0;
}