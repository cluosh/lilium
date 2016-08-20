# Lilium

Lilium is a simple register machine, designed to execute a language resembling Common Lisp. The machine consists of 
256 registers and uses 32-bit bytecode instructions, which are parsed by a token-threaded interpreter. All of the tools
are written in C++14 with the help of Flex/Bison as parser generators.

The repository contains a set of basic tools:
 * **lilium-as**: A bytecode assembler
 * **lilium-objdump**: A bytecode disassembler
 * **lilium-cc**: A bytecode compiler for a Lisp-like language
 * **lilium-vm**: The actual register machine