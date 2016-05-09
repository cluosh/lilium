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
  class Node;
  }
}

%parse-param { Scanner &scanner }
%parse-param { AST::Node &node }

%code {
  #include <iostream>
  #include <cstdlib>
  #include <fstream>

  #include "lang/ast/node.h"
  #include "lang/parser/scanner.h"

  #undef yylex
  #define yylex scanner.yylex
}

%define api.value.type variant
%define parse.assert

%token LE GE DEF LET IF OR AND NOT NUM
%token ERROR
%token <std::string> ID

%locations

%type <AST::Node *> Expr FuncDef Params ParamList

%%
Program:
  | Program FuncDef { node.add($2); }
  | Program Expr { node.add($2); }
  ;
FuncDef:
  '(' DEF ID '(' Params ')' Expr ')' { AST::Node *n = new AST::Node($3);
                                       n->add($5); n->add($7); $$ = n; }
  ;
Params:
  { $$ = new AST::Node("nil"); }
  | ParamList { $$ = $1; }
  ;
ParamList:
  ParamList ',' ID { AST::Node *n = new AST::Node($3);
                     n->add($1); $$ = n; }
  | ID { $$ = new AST::Node($1); }
  ;
Expr:
  ID { $$ = new AST::Node($1); }
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
