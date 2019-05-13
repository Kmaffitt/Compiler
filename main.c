#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>

#include "testTree.h"
#include "testScanner.h"
#include "scanner.h"
#include "parser.h"
#include "node.h"
#include "token.h"
#include "statSem.h"
#include "codeGen.h"


FILE* file;
FILE* outFile;


int main(int argc, char *argv[]) {
	
	char filename[50];

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
	//testScanner();
	node_t* tree = parse();
	//printf("\n\nPRINTING TREE\n\n");
	//printTree(tree, 0);
	//printf("\n\n");
	printf("\n\nCHECKING STATIC SEMANTICS\n\n");
	checkStaticSemantics(tree);
    printf("PARSE TREE STATIC SEMANTICS VALIDATED\n");


	strcat(filename, ".asm");
    outFile = fopen(filename, "w");
	gen(tree);

	fclose(file);
	fclose(outFile);
	return 0;
}


