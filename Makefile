CC = gcc
CFLAGS = -Wall -g
objects := $(patsubst %.c,%.o,$(wildcard *.c))

comp : main.o scanner.o testScanner.o token.o parser.o testTree.o statSem.o codeGen.o
	$(CC) $(CFLAGS) -o comp main.o scanner.o testScanner.o token.o parser.o testTree.o statSem.o codeGen.o

main.o : main.c token.h
	 $(CC) $(CFLAGS) -c main.c

testScanner.o : testScanner.c testScanner.h token.h scanner.h
	$(CC) $(CFLAGS) -c testScanner.c

scanner.o : scanner.c scanner.h token.h 
	$(CC) $(CFLAGS) -c scanner.c

token.o : token.h token.c
	$(CC) $(CFLAGS) -c token.c

parser.o : parser.h parser.c
	$(CC) $(CFLAGS) -c parser.c

testTree.o : node.h token.c token.h
	$(CC) $(CFLAGS) -c testTree.c

statSem.o : node.h token.c token.h
	$(CC) $(CFLAGS) -c statSem.c

codeGen.o : node.h token.c token.h
	$(CC) $(CFLAGS) -c codeGen.c

clean:
	rm comp *.o *.asm
	clear