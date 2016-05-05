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

%option	noyywrap
%{  
  #include <cstdlib>
  #include <cstdint>
  #include <iostream>
  #include <string>
  
  #include "parser.tab.hpp"

  // Error handling
  namespace scan {
    void yyerror(std::string msg);
  }
	
  // Linecounter
  std::uint64_t line = 1;  
%}

COMMENT		#.*
WHITESPACE	[\t \n]
LETTER		[a-zA-Z]
NUMBER		([0-9]+)|(0x[0-9a-fA-F]+)
IDENTIFIER	({LETTER}|_)({LETTER}|[0-9]|_)*

%%
","		{ return ','; }
"("		{ return '('; }
")"		{ return ')'; }
"+"		{ return '+'; }
"-"		{ return '-'; }
"*"		{ return '*'; }
"/"		{ return '/'; }
">"		{ return '>'; }
"<"		{ return '<'; }
"="		{ return '='; }
"!"		{ return '!'; }
"<="		{ return LE; }
">="		{ return GE; }
defun		{ return DEF; }
let		{ return LET; }
if		{ return IF; }
or		{ return OR; }
and		{ return AND; }
not		{ return NOT; }
{NUMBER}	{ return NUM; }
{IDENTIFIER}	{ return ID; @{ @ID.name@ = std::string(yytext); @} }
{COMMENT}	{ }
\n		{ line++; }
{WHITESPACE}+	{ }
.		{ scan::yyerror(std::string(yytext)); }
%%

/**
 * @brief Quit on lexical error
 * @param txt Error string
 */
void scan::yyerror(std::string txt) {
  std::cerr << "ERROR: \"" << txt << "\" in line " << line << "\n";
}
