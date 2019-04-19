CC = gcc
CFLAGS = -Wall -g
objects := $(patsubst %.c,%.o,$(wildcard *.c))

frontEnd : main.o scanner.o testScanner.o token.o parser.o
	$(CC) $(CFLAGS) -o frontEnd main.o scanner.o testScanner.o token.o parser.o 

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

clean:
	rm frontEnd *.o
