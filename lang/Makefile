#
# Makefile for parser
#
CC=clang++
LEX=flex
YACC=bison
YFLAGS=-d
CFLAGS+=-std=c++11 -O3 -pedantic -Wall -I../

OBJ=parser/lex.yy.o parser/parser.tab.o parser/scanner.o parser/controller.o ast/program.o ast/func_def.o ast/expr.o ast/var.o

all: lcc

lcc: $(OBJ) parser/parser.tab.hpp
	$(CC) $(LDFLAGS) -o $@ $(OBJ)

%.o: %.cpp
	$(CC) $(CFLAGS) -o $@ -c $<

%.o: %.cc
	$(CC) $(CFLAGS) -o $@ -c $<

parser/parser.tab.cpp parser/parser.tab.hpp: parser/parser.y
	$(YACC) $(YFLAGS) -o parser/parser.tab.cpp parser/parser.y

parser/lex.yy.cpp: parser/scanner.l parser/parser.tab.hpp
	$(LEX) -o $@ parser/scanner.l

clean:
	rm -f ast/*.o parser/*.hh parser/*.o parser/*.yy.cpp parser/*.tab.cpp parser/*.tab.hpp lcc

.PHONY: clean all
