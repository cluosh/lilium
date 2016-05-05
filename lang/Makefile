#
# Makefile for parser
#
CC=clang++
LEX=flex
YACC=bison
YFLAGS=-d
CFLAGS+=-std=c++11 -O3 -pedantic -I../

all: lcc

lcc: lex.yy.cpp parser.tab.cpp parser.tab.hpp
	$(CC) $(LDFLAGS) $(CFLAGS) -o $@ lex.yy.cpp parser.tab.cpp

parser.tab.cpp parser.tab.hpp: oxout.y
	$(YACC) $(YFLAGS) -o parser.tab.cpp oxout.y

lex.yy.cpp: oxout.l parser.tab.hpp
	$(LEX) -o $@ oxout.l

oxout.y oxout.l: parser.y scanner.l
	ox parser.y scanner.l

clean:
	rm -f oxout.y oxout.l *.o *.yy.cpp *.tab.cpp *.tab.hpp lcc

.PHONY: clean all
