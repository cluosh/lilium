#
# Makefile for bytecode assembler/disassembler
#
CC=clang
LEX=flex

bas: scan.o
	$(CC) -o $@ $(LDFLAGS) $^

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $^

scan.c: scan.l
	$(LEX) -o $@ $^

clean:
	rm -f *.o scan.c bas
