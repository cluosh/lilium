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
#ifndef LANG_PARSER_CONTROLLER_H_
#define LANG_PARSER_CONTROLLER_H_

#include <istream>
#include <string>

#include "lang/ast/program.h"
#include "lang/parser/scanner.h"
#include "lang/parser/parser.tab.hh"

namespace Parser {

class Controller {
 private:
  Parser *parser = nullptr;
  Scanner *scanner = nullptr;
  AST::Program *ast = nullptr;

 public:
  ~Controller();

  bool parse(std::string filename);
  bool parse(const std::istream &is);
};

}  // namespace Parser

#endif  // LANG_PARSER_CONTROLLER_H_
