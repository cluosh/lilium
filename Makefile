#
# Makefile for bytecode interpreter
#
CC=clang++
CFLAGS+=-std=c++11

liliumvm: interpreter.o
	$(CC) -o $@ $(LDFLAGS) $^

%.o: %.cpp
	$(CC) $(CFLAGS) -o $@ -c $^

clean:
	rm -f *.o liliumvm

.PHONY: clean
