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
#ifndef LANG_PARSER_SCANNER_H_
#define LANG_PARSER_SCANNER_H_

#include <cstdint>

#ifndef yyFlexLexerOnce
#include <FlexLexer.h>
#endif

#include "cc/parser/parser.tab.hh"
#include "cc/parser/location.hh"

namespace Parser {

class Scanner : public yyFlexLexer {
 private:
  Parser::semantic_type *yylval = nullptr;
  Parser::location_type *loc = nullptr;

 public:
  explicit Scanner(std::istream *in) : yyFlexLexer(in) {
    loc = new Parser::location_type();
  }

  // Method body created by flex (scanner.l definitions)
  using yyFlexLexer::yylex;
  virtual int yylex(Parser::semantic_type * const lval,
                    Parser::location_type *location);

  // Safe number conversion
  std::int64_t parse_integer(const char *text);
  double parse_double(const char *text);
  float parse_float(const char *text);
};

}  // namespace Parser

#endif  // LANG_PARSER_SCANNER_H_
