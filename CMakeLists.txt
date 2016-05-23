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
    lang/ast/expr.cc
    lang/ast/expr.h
    lang/ast/func_def.cc
    lang/ast/func_def.h
    lang/ast/global_expr.h
    lang/ast/plus_expr.h
    lang/ast/program.cc
    lang/ast/program.h
    lang/ast/types.h
    lang/ast/var.cc
    lang/ast/var.h
    lang/parser/controller.cc
    lang/parser/controller.h
    lang/parser/parser.yy
    lang/parser/scanner.cc
    lang/parser/scanner.h
    lang/parser/scanner.l)

# BISON/FLEX Targets
find_package(BISON REQUIRED)
find_package(FLEX REQUIRED)
bison_target(lccParser lang/parser/parser.yy ${CMAKE_CURRENT_BINARY_DIR}/lang/parser/parser.tab.cc)
flex_target(lccLexer lang/parser/scanner.l ${CMAKE_CURRENT_BINARY_DIR}/lang/parser/lex.yy.cc)
add_flex_bison_dependency(lccLexer lccParser)

file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/lang/parser)
include_directories(./ ${CMAKE_CURRENT_BINARY_DIR})
add_executable(lcc
        ${LCC_SRC_FILES}
        ${BISON_lccParser_OUTPUTS}
        ${FLEX_lccLexer_OUTPUTS}
)