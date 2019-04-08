CC = gcc
CFLAGS = -Wall -g
objects := $(patsubst %.c,%.o,$(wildcard *.c))

P1 : main.o scanner.o testScanner.o token.o
	$(CC) $(CFLAGS) -o P1 main.o scanner.o testScanner.o token.o

main.o : main.c token.h
	 $(CC) $(CFLAGS) -c main.c

testScanner.o : testScanner.c testScanner.h token.h scanner.h
	$(CC) $(CFLAGS) -c testScanner.c

scanner.o : scanner.c scanner.h token.h 
	$(CC) $(CFLAGS) -c scanner.c

token.o : token.h token.c
	$(CC) $(CFLAGS) -c token.c

clean:
	rm P1 *.o
