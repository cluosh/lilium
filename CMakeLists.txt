cmake_minimum_required(VERSION 3.5)
project(liliumvm)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

set(CONTROLLER_SRC_FILES
        controller/common.h controller/main.cc)

set(BAS_SRC_FILES
        bas/codegen/generator.h
        bas/parser/grammar/scanner.l
        bas/codegen/generator.h)

set(VM_SRC_FILES
        vm/interpreter.cc
        vm/opcodes.h
        vm/function_table.h
        vm/function_table.cc
        vm/interpreter.h
        vm/module.h
        vm/module.cc
        vm/bytecode/bytecode_loader.cc
        vm/bytecode/bytecode_loader.h
        vm/types/types.h)

set(LCC_SRC_FILES
        lang/ast/expr/expr.cc
        lang/ast/expr/expr.h
        lang/ast/func/func_def.cc
        lang/ast/func/func_def.h
        lang/ast/expr/global_expr.h
        lang/ast/expr/binary_expr.h
        lang/ast/program.cc
        lang/ast/program.h
        lang/ast/data/var.cc
        lang/ast/data/var.h
        lang/parser/controller.cc
        lang/parser/controller.h
        lang/parser/grammar/parser.yy
        lang/parser/scanner.cc
        lang/parser/scanner.h
        lang/parser/grammar/scanner.l
        lang/ast/expr/binary_expr.cc
        lang/ast/common/operators.h
        lang/ast/expr/unary_expr.h
        lang/ast/expr/unary_expr.cc
        lang/ast/common/symbol.h
        lang/ast/expr/global_expr.cc
        lang/ast/func/call_expr.h
        lang/ast/func/call_expr.cc
        lang/ast/common/attribute.h
        lang/ast/expr/const.h
        lang/ast/expr/const.cc
        vm/bytecode/generator.h
        vm/bytecode/generator.cc
        vm/bytecode/func_addr.h
        vm/bytecode/func_addr.cc lang/ast/expr/cond_expr.h lang/ast/expr/cond_expr.cc)

# BISON/FLEX Targets
find_package(BISON REQUIRED)
find_package(FLEX REQUIRED)
bison_target(bas_parser bas/parser/grammar/parser.yy ${CMAKE_CURRENT_BINARY_DIR}/bas/parser/parser.tab.cc)
flex_target(bas_lexer bas/parser/grammar/scanner.l ${CMAKE_CURRENT_BINARY_DIR}/bas/parser/lex.yy.cc)
bison_target(lcc_parser lang/parser/grammar/parser.yy ${CMAKE_CURRENT_BINARY_DIR}/lang/parser/parser.tab.cc)
flex_target(lcc_lexer lang/parser/grammar/scanner.l ${CMAKE_CURRENT_BINARY_DIR}/lang/parser/lex.yy.cc)
add_flex_bison_dependency(lcc_lexer lcc_parser)

# LCC
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/lang/parser)
include_directories(./ ${CMAKE_CURRENT_BINARY_DIR})
add_library(lcc STATIC
        ${LCC_SRC_FILES}
        ${BISON_lcc_parser_OUTPUTS}
        ${FLEX_lcc_lexer_OUTPUTS}
)

# VM
add_library(vm STATIC
        ${VM_SRC_FILES})

# BAS
add_library(bas STATIC
        ${BAS_SRC_FILES}
        ${BISON_bas_parser_OUTPUTS}
        ${FLEX_bas_lexer_OUTPUTS})

# Main controller
add_executable(lilium
        ${CONTROLLER_SRC_FILES})
target_link_libraries(lilium vm lcc)