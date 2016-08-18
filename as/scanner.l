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

/*
 * ------------------------------------------------------------
 * FLEX PART
 * ------------------------------------------------------------
 */

%option	noyywrap
%{  
  #include <cstdlib>
  #include <cstdint>
  #include <string>

  #include "vm/opcodes.h"
  #include "parser.tab.hpp"

  // Function prototypes
  void yyerror(std::string msg);
	
  // Linecounter
  uint64_t line = 1;
%}

COMMENT		#.*
LINEFEED	\n
WHITESPACE	[\t ]|{LINEFEED}
NUMBER		(0x[0-9a-fA-F]+)|[0-9]+
LABEL           [a-zA-Z][a-zA-Z0-9]+
LABELDEF        {LABEL}:
REGISTER        r([01]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])

%%
{LINEFEED}      line++;
{COMMENT}       { }
{WHITESPACE}+   { }
{NUMBER}        { yylval.ival = 0x00; return VAL; }
nop             { yylval.ival = 0x00; return OP; }
halt            { yylval.ival = 0x01; return OP; }
load            { yylval.ival = 0x02; return OP_BINARY; }
store           { yylval.ival = 0x03; return OP_BINARY; }
loadi           { yylval.ival = 0x04; return LS_OP; }
move            { yylval.ival = 0x05; return OP_BINARY; }
movew           { yylval.ival = 0x06; return OP_BINARY; }
moveh           { yylval.ival = 0x07; return OP_BINARY; }
shiftl          { yylval.ival = 0x08; return OP_TERNARY; }
shiftr          { yylval.ival = 0x09; return OP_TERNARY; }
add             { yylval.ival = 0x0A; return OP_TERNARY; }
sub             { yylval.ival = 0x0B; return OP_TERNARY; }
mul             { yylval.ival = 0x0C; return OP_TERNARY; }
div             { yylval.ival = 0x0D; return OP_TERNARY; }
neg             { yylval.ival = 0x0E; return OP_BINARY; }
and             { yylval.ival = 0x0F; return OP_TERNARY; }
or              { yylval.ival = 0x10; return OP_TERNARY; }
xor             { yylval.ival = 0x11; return OP_TERNARY; }
not             { yylval.ival = 0x12; return OP_BINARY; }
cmp             { yylval.ival = 0x13; return OP_TERNARY; }
lt              { yylval.ival = 0x14; return OP_TERNARY; }
le              { yylval.ival = 0x15; return OP_TERNARY; }
gt              { yylval.ival = 0x16; return OP_TERNARY; }
ge              { yylval.ival = 0x17; return OP_TERNARY; }
jmp             { yylval.ival = 0x18; return JMP_OP; }
{REGISTER}      { yylval.ival = 0x00; return REG; }
{LABEL}         { yylval.sval = yytext; // COPY STRING
                  return LABEL; }
{LABELDEF}      { yylval.sval = yytext; return LABELDEF; }
.		yyerror(yytext);
%%

/*
 * ------------------------------------------------------------
 * C++ PART
 * ------------------------------------------------------------
 */
