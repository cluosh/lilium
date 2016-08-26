cmake_minimum_required(VERSION 3.5)
project(lilium)

set(AS_SRC_FILES
        as/codegen/generator.h
        as/parser/grammar/scanner.l
        as/codegen/generator.h)

set(LIB_SRC_FILES
        vm/constants/opcodes.h
        vm/bytecode/loader.cc
        vm/bytecode/loader.h
        vm/bytecode/generator.h
        vm/bytecode/generator.cc
        vm/constants/types.h
        vm/pipeline/loader.h
        vm/pipeline/linker.h
        vm/pipeline/interpreter.h
        vm/pipeline/loader.cc
        vm/pipeline/linker.cc
        vm/pipeline/interpreter.cc
        vm/data/program_buffer.h
        vm/data/vm_common.h
        cc/ast/expr/expr.cc
        cc/ast/expr/expr.h
        cc/ast/func/func_def.cc
        cc/ast/func/func_def.h
        cc/ast/expr/global_expr.h
        cc/ast/expr/binary_expr.h
        cc/ast/program.cc
        cc/ast/program.h
        cc/ast/data/var.cc
        cc/ast/data/var.h
        cc/parser/controller.cc
        cc/parser/controller.h
        cc/parser/grammar/parser.yy
        cc/parser/scanner.cc
        cc/parser/scanner.h
        cc/parser/grammar/scanner.l
        cc/ast/expr/binary_expr.cc
        cc/ast/common/operators.h
        cc/ast/expr/unary_expr.h
        cc/ast/expr/unary_expr.cc
        cc/ast/common/symbol.h
        cc/ast/expr/global_expr.cc
        cc/ast/func/call_expr.h
        cc/ast/func/call_expr.cc
        cc/ast/common/attribute.h
        cc/ast/expr/const.h
        cc/ast/expr/const.cc
        cc/ast/expr/cond_expr.h
        cc/ast/expr/cond_expr.cc
        objdump/pipeline/dump.h
        objdump/pipeline/dump.cc)

set(OBJDUMP_SRC_FILES objdump/main.cc)

set(VM_SRC_FILES
        vm/constants/opcodes.h
        vm/bytecode/loader.cc
        vm/bytecode/loader.h
        vm/constants/types.h
        vm/pipeline/loader.h
        vm/pipeline/linker.h
        vm/pipeline/interpreter.h
        vm/pipeline/loader.cc
        vm/pipeline/linker.cc
        vm/pipeline/interpreter.cc
        vm/data/program_buffer.h
        vm/data/vm_common.h
        vm/main.cc)

set(CC_SRC_FILES
        cc/main.cc)

# BISON/FLEX Targets
find_package(BISON 3.0.4 REQUIRED)
find_package(FLEX 2.6.1 REQUIRED)
bison_target(as_parser as/parser/grammar/parser.yy ${CMAKE_CURRENT_BINARY_DIR}/as/parser/parser.tab.cc)
flex_target(as_lexer as/parser/grammar/scanner.l ${CMAKE_CURRENT_BINARY_DIR}/as/parser/lex.yy.cc)
bison_target(cc_parser cc/parser/grammar/parser.yy ${CMAKE_CURRENT_BINARY_DIR}/cc/parser/parser.tab.cc)
flex_target(cc_lexer cc/parser/grammar/scanner.l ${CMAKE_CURRENT_BINARY_DIR}/cc/parser/lex.yy.cc)
add_flex_bison_dependency(cc_lexer cc_parser)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/cc/parser)
include_directories(./ ${CMAKE_CURRENT_BINARY_DIR})

# lilium
add_library(lilium
  ${LIB_SRC_FILES}
  ${BISON_cc_parser_OUTPUTS}
  ${FLEX_cc_lexer_OUTPUTS})
target_compile_features(lilium PUBLIC cxx_generic_lambdas)

# lilium-as
# add_executable(lilium-as
#        ${AS_SRC_FILES}
#        ${BISON_as_parser_OUTPUTS}
#        ${FLEX_as_lexer_OUTPUTS})

# lilium-cc
add_executable(lilium-cc
  cc/main.cc)
target_link_libraries(lilium-cc lilium)
target_compile_features(lilium-cc PUBLIC cxx_generic_lambdas)

# lilium-objdump
add_executable(lilium-objdump
  objdump/main.cc)
target_link_libraries(lilium-objdump lilium)
target_compile_features(lilium-objdump PUBLIC cxx_generic_lambdas)

# lilium-vm
add_executable(lilium-vm
  vm/main.cc)
target_link_libraries(lilium-vm lilium)
target_compile_features(lilium-vm PUBLIC cxx_generic_lambdas)
