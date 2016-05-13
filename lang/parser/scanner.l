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

%option nodefault
%option yyclass = "Parser::Scanner"
%option noyywrap
%option c++

%{
  #include <string>

  #include "lang/parser/scanner.h"

  // yyFlexScanner implementation
  #undef YY_DECL
  #define YY_DECL int Parser::Scanner::yylex(\
    Parser::Parser::semantic_type * const lval, \
    Parser::Parser::location_type *location)

  // Additional parameters
  #define YY_USER_ACTION loc->step(); loc->columns(yyleng);
%}

COMMENT         #.*
WHITESPACE      [\t \n]
LETTER          [a-zA-Z]
INTEGER         ([0-9]+)|(0x[0-9a-fA-F]+)
FLOAT           ([-+]?[0-9]*\.?[0-9]+([eE][-+]?[0-9]+)?)
IDENTIFIER      ({LETTER}|_)({LETTER}|[0-9]|_)*

%%
%{
  // Initialization
  yylval = lval;
%}

","             { return ','; }
"("             { return '('; }
")"             { return ')'; }
"+"             { return '+'; }
"-"             { return '-'; }
"*"             { return '*'; }
"/"             { return '/'; }
">"             { return '>'; }
"<"             { return '<'; }
"="             { return '='; }
"!"             { return '!'; }
"<="            { return Parser::Parser::token::LE; }
">="            { return Parser::Parser::token::GE; }
defun           { return Parser::Parser::token::DEF; }
let             { return Parser::Parser::token::LET; }
if              { return Parser::Parser::token::IF; }
or              { return Parser::Parser::token::OR; }
and             { return Parser::Parser::token::AND; }
not             { return Parser::Parser::token::NOT; }
":: SP"         { return Parser::Parser::token::TSP; }
":: DP"         { return Parser::Parser::token::TDP; }
{INTEGER}       { yylval->build<std::uint64_t>(parse_integer(yytext));
                  return Parser::Parser::token::INT; }
{FLOAT}         { yylval->build<double>(parse_float(yytext));
                  return Parser::Parser::token::FLOAT; }
{IDENTIFIER}    { yylval->build<std::string>(yytext);
                  return Parser::Parser::token::ID; }
\n		{ loc->lines(); }
{COMMENT}       { }
{WHITESPACE}+   { }
.               { return Parser::Parser::token::ERROR; }
%%
