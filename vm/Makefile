#
# Makefile for bytecode interpreter
#
CC=clang++
CFLAGS+=-std=c++11 -O3 -Wall -pedantic -Wno-gnu-label-as-value

liliumvm: interpreter.o
	$(CC) -o $@ $(LDFLAGS) $^

%.o: %.cc
	$(CC) $(CFLAGS) -o $@ -c $^

clean:
	rm -f *.o liliumvm

.PHONY: clean
