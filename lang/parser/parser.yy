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

%skeleton "lalr1.cc"
%require "3.0"
%defines
%define api.namespace {Parser}
%define parser_class_name {Parser}

%code requires {
  namespace Parser {
  class Scanner;
  }
  namespace AST {
  class Program;
  class FuncDef;
  class Expr;
  class Var;
  }
}

%parse-param { Scanner &scanner }
%parse-param { AST::Program &ast }

%code {
  #include <iostream>
  #include <cstdlib>
  #include <fstream>

  #include "lang/ast/program.h"
  #include "lang/ast/func_def.h"
  #include "lang/ast/expr.h"
  #include "lang/ast/var.h"
  #include "lang/parser/scanner.h"

  #undef yylex
  #define yylex scanner.yylex
}

%define api.value.type variant
%define parse.assert

%token LE GE DEF LET IF OR AND NOT
%token TSP TDP
%token ERROR
%token <std::uint64_t> INT
%token <double> FLOAT
%token <std::string> ID

%locations

%type <AST::FuncDef *> FuncDef
%type <AST::Expr *> Expr
%type <AST::Var *> Params

%%
Program:
  | Program FuncDef { ast.add($2); }
  | Program Expr { ast.add($2); }
  ;
FuncDef:
 '(' DEF ID '(' Params ')' Expr ')' { $$ = new AST::FuncDef($3,$5,$7); }
  ;
Params:
  { $$ = nullptr; }
  | ID Params { $$ = new AST::Var($1, $2, AST::TYPE_INT); }
  | ID TSP Params { $$ = new AST::Var($1, $3, AST::TYPE_SINGLEP); }
  | ID TDP Params { $$ = new AST::Var($1, $3, AST::TYPE_DOUBLEP); }
Expr:
  ID { $$ = new AST::Expr(); }
  | INT { $$ = new AST::Expr(AST::TYPE_INT); }
  | FLOAT { $$ = new AST::Expr(AST::TYPE_SINGLEP); }
  | '(' '+' Expr Expr ')' { $$ = new AST::Expr(); }
  ;
%%

/**
 * @brief Print a parsing error
 * @param l Error location
 * @param msg Error message
 */
void Parser::Parser::error(const location_type &l, const std::string &msg) {
  std::cerr << "ERROR: " << msg << ", Line: " << l << "\n";
}
