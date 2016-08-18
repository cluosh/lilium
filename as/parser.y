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
 * BISON PART
 * ------------------------------------------------------------
 */

// C++-headers
%{
  #include <cstdlib>
  #include <cstdint>
  #include <string>
  #include <iostream>

  // Function prototypes
  int yylex(void);
  void yyerror(std::string msg);
%}

// Data storage for data token
%union {
  uint64_t ival;
  char *sval;
}

// Instruction tokens
%token <ival> OP
%token <ival> OP_BINARY
%token <ival> OP_TERNARY
%token <ival> LS_OP
%token <ival> JMP_OP

// Token carrying data
%token <ival> REG
%token <ival> VAL
%token <sval> LABEL
%token <sval> LABELDEF

// Grammar
%%
Program:
  | Instructions
  ;
Instructions:
  Instructions Instruction
  | Instruction
  ;
Instruction:
  OP  { std::cout << "OP " << $1 << "\n"; }
  | OP_BINARY REG REG
  | OP_TERNARY REG REG REG { std::cout << "OP " << $1 << "\n"; }
  | LS_OP REG VAL
  | JMP_OP REG
  | JMP_OP LABEL
  | JMP_OP REG REG
  | JMP_OP LABEL REG
  | LABELDEF {std::cout << "LABELDEF " << $1 << "\n"; }
  ;
%%

/*
 * -----------------------------------------------------------
 * MAIN C++ PART
 * -----------------------------------------------------------
 */

/**
 * @brief Quit on syntax error
 * @param msg Error message
 */
void yyerror(std::string msg) {
  std::cerr << "ERROR: " << msg << "\n";
  exit(EXIT_FAILURE);
}

/**
 * @brief Main entry point
 * @return EXIT_SUCCESS, if no errors happened
 */
int main(void) {
  (void) yyparse();
  return EXIT_SUCCESS;
}
