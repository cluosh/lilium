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

%{
  #include <iostream>
  #include <string>
  
  // Lexer
  int yylex(void);

  // Error handling
  namespace parse {
    void yyerror(std::string msg);
  }
%}

// Token declarations
%token LE GE DEF LET IF OR AND NOT NUM ID

// Attributes
@attributes { char *name; } ID

@traversal @postorder t

// Grammar
%%
Program:
	| Program FuncDef
	| Program Expr
	;
FuncDef:
	'(' DEF ID '(' Params ')' Expr ')'
	;
Params:
	| ParamList
	;
ParamList:
	ParamList ',' ID
	| ID
	;
Expr:
	ID
	;
%%

/**
 * @brief Quit on syntax error
 * @param txt Error string
 */
void parse::yyerror(std::string txt) {
  std::cerr << "ERROR: \"" << txt << "\n";
}

/**
 * @brief Main entry point
 * @param argc Number of arguments
 * @param argv Arguments
 */
int main(void) {
  (void) yyparse();
  return 0;
}
