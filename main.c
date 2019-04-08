#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>

#include "token.h"
#include "testScanner.h"
#include "scanner.h"



int main(int argc, char *argv[]) {
	
	char filename[50];
	FILE* file;
	
	if ( argc == 2 ){
		strcpy(filename, argv[1]);
		if( access( filename, F_OK ) == -1){
			printf("Input file cannot be read. Exiting.\n");
			exit(1);
		}
	} else {
		printf("Supply exactly one argument, an input file with .sp19 extension.\n");
		exit(1);
	}
	
	file = fopen(filename, "r");
	testScanner();
	fclose(file);
	
	return 0;
}
