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
  class BinaryExpr;
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
  #include "lang/ast/func/func_def.h"
  #include "lang/ast/func/call_expr.h"
  #include "lang/ast/expr/expr.h"
  #include "lang/ast/expr/binary_expr.h"
  #include "lang/ast/expr/unary_expr.h"
  #include "lang/ast/expr/const_double.h"
  #include "lang/ast/expr/const_int.h"
  #include "lang/ast/expr/const_single.h"
  #include "lang/ast/data/var.h"
  #include "lang/ast/common/operators.h"
  #include "lang/parser/scanner.h"
  #include "vm/types/types.h"

  #undef yylex
  #define yylex scanner.yylex
}

%define api.value.type variant
%define parse.assert

%token LE GE DEF LET IF OR AND NOT
%token TSP TDP
%token ERROR
%token <std::int64_t> INT
%token <float> SINGLEP
%token <double> DOUBLEP
%token <std::string> ID

%locations

%type <AST::FuncDef *> FuncDef
%type <AST::Expr *> Expr ExprList
%type <AST::BinaryExpr *> PlusExprList MinusExprList MulExprList DivExprList
%type <AST::BinaryExpr *> AndExprList OrExprList
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
  | ID Params { $$ = new AST::Var($1, $2, VM::TYPE_INT); }
  | ID TSP Params { $$ = new AST::Var($1, $3, VM::TYPE_SINGLEP); }
  | ID TDP Params { $$ = new AST::Var($1, $3, VM::TYPE_DOUBLEP); }
Expr:
  ID { $$ = new AST::Var($1, nullptr, VM::TYPE_COUNT); }
  | INT { $$ = new AST::ConstInt($1, nullptr); }
  | SINGLEP { $$ = new AST::ConstSingle($1, nullptr); }
  | DOUBLEP { $$ = new AST::ConstDouble($1, nullptr); }
  | '(' '+' PlusExprList ')' { $$ = $3; }
  | '(' '-' MinusExprList ')' { $$ = $3; }
  | '(' '*' MulExprList ')' { $$ = $3; }
  | '(' '/' DivExprList ')' { $$ = $3; }
  | '(' AND AndExprList ')' { $$ = $3; }
  | '(' OR OrExprList ')' { $$ = $3; }
  | '(' '>' Expr Expr ')' { $$ = new AST::BinaryExpr($3, $4, AST::BINARY_GT, nullptr); }
  | '(' '<' Expr Expr ')' { $$ = new AST::BinaryExpr($3, $4, AST::BINARY_LT, nullptr); }
  | '(' GE Expr Expr ')' { $$ = new AST::BinaryExpr($3, $4, AST::BINARY_GE, nullptr); }
  | '(' LE Expr Expr ')' { $$ = new AST::BinaryExpr($3, $4, AST::BINARY_LE, nullptr); }
  | '(' '=' Expr Expr ')' { $$ = new AST::BinaryExpr($3, $4, AST::BINARY_EQ, nullptr); }
  | '(' '!' Expr Expr ')' { $$ = new AST::BinaryExpr($3, $4, AST::BINARY_NE, nullptr); }
  | '(' '-' Expr ')' { $$ = new AST::UnaryExpr($3, AST::UNARY_NEG, nullptr); }
  | '(' NOT Expr ')' { $$ = new AST::UnaryExpr($3, AST::UNARY_NOT, nullptr); }
  | '(' ID ExprList ')' { $$ = new AST::CallExpr($2, $3, nullptr); }
  ;
ExprList:
  { $$ = nullptr; }
  | ExprList Expr { $2->set_next($1); $$ = $2; }
PlusExprList:
  Expr Expr { $$ = new AST::BinaryExpr($1, $2, AST::BINARY_ADD, nullptr); }
  | PlusExprList Expr { $$ = new AST::BinaryExpr($1, $2, AST::BINARY_ADD, nullptr); }
  ;
MinusExprList:
  Expr Expr { $$ = new AST::BinaryExpr($1, $2, AST::BINARY_SUB, nullptr); }
  | MinusExprList Expr { $$ = new AST::BinaryExpr($1, $2, AST::BINARY_SUB, nullptr); }
  ;
MulExprList:
  Expr Expr { $$ = new AST::BinaryExpr($1, $2, AST::BINARY_MUL, nullptr); }
  | MulExprList Expr { $$ = new AST::BinaryExpr($1, $2, AST::BINARY_MUL, nullptr); }
  ;
DivExprList:
  Expr Expr { $$ = new AST::BinaryExpr($1, $2, AST::BINARY_DIV, nullptr); }
  | DivExprList Expr { $$ = new AST::BinaryExpr($1, $2, AST::BINARY_DIV, nullptr); }
  ;
AndExprList:
  Expr Expr { $$ = new AST::BinaryExpr($1, $2, AST::BINARY_AND, nullptr); }
  | AndExprList Expr { $$ = new AST::BinaryExpr($1, $2, AST::BINARY_AND, nullptr); }
  ;
OrExprList:
  Expr Expr { $$ = new AST::BinaryExpr($1, $2, AST::BINARY_OR, nullptr); }
  | OrExprList Expr { $$ = new AST::BinaryExpr($1, $2, AST::BINARY_OR, nullptr); }
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
