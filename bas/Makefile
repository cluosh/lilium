#
# Makefile for bytecode assembler/disassembler
#
CC=clang++
LEX=flex
YACC=bison
YFLAGS=-d
CFLAGS+=-std=c++11 -O3 -Wall -pedantic -I../

bas: lex.yy.cpp parser.tab.cpp parser.tab.hpp
	$(CC) $(LDFLAGS) $(CFLAGS) -o $@ lex.yy.cpp parser.tab.cpp

parser.tab.cpp parser.tab.hpp: parser.y
	$(YACC) $(YFLAGS) -o parser.tab.cpp parser.y

lex.yy.cpp: scanner.l parser.tab.hpp
	$(LEX) -o $@ scanner.l

clean:
	rm -f *.o *.yy.cpp *.tab.cpp *.tab.hpp bas

.PHONY: clean
