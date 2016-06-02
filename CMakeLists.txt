cmake_minimum_required(VERSION 3.5)
project(liliumvm)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    bas/Makefile
    bas/parser.y
    bas/scanner.l
    vm/bytecode_loader.cc
    vm/bytecode_loader.h
    vm/interpreter.cc
    vm/opcodes.h)

set(LCC_SRC_FILES
        lang/ast/expr/expr.cc
        lang/ast/expr/expr.h
        lang/ast/func/func_def.cc
        lang/ast/func/func_def.h
        lang/ast/expr/global_expr.h
        lang/ast/expr/binary_expr.h
    lang/ast/program.cc
    lang/ast/program.h
        lang/ast/common/types.h
        lang/ast/data/var.cc
        lang/ast/data/var.h
    lang/parser/controller.cc
    lang/parser/controller.h
        lang/parser/grammar/parser.yy
    lang/parser/scanner.cc
    lang/parser/scanner.h
        lang/parser/grammar/scanner.l lang/ast/expr/binary_expr.cc lang/ast/common/operators.h lang/ast/expr/unary_expr.h lang/ast/expr/unary_expr.cc lang/ast/common/symbol.h lang/ast/expr/global_expr.cc lang/ast/expr/numeric_expr.h)

# BISON/FLEX Targets
find_package(BISON REQUIRED)
find_package(FLEX REQUIRED)
bison_target(lcc_parser lang/parser/grammar/parser.yy ${CMAKE_CURRENT_BINARY_DIR}/lang/parser/parser.tab.cc)
flex_target(lcc_lexer lang/parser/grammar/scanner.l ${CMAKE_CURRENT_BINARY_DIR}/lang/parser/lex.yy.cc)
add_flex_bison_dependency(lcc_lexer lcc_parser)

file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/lang/parser)
include_directories(./ ${CMAKE_CURRENT_BINARY_DIR})
add_executable(lcc
        ${LCC_SRC_FILES}
        ${BISON_lcc_parser_OUTPUTS}
        ${FLEX_lcc_lexer_OUTPUTS}
)