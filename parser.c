#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "scanner.h"
#include "token.h"

extern int lookAhead;
extern FILE* file;

token_t tk;

void parse(){
	//get file
	lookAhead = fgetc(file);
	//get first token
	consume();
	
	program();

	printf("CLEAN PARSE\n\n");
	return 0;
}


void program(){
	vars();
	block();
	return;
}

void vars(){
	//check for empty state -- follows of block or stats
	if(tk.name == voidTK || tk.name == scanTK || tk.name == printTK || tk.name == condTK || tk.name == iterTK || tk.name == idTK){
		return;
	}
	else if(tk.name == varTK){
		consume();
		if(tk.name == idTK){
			consume();
			if(tk.name == semiTK){
				consume();
				if(tk.name == intTK){
					consume();
					vars();
					return;
				}else{
					printf("ERROR: \"%s\" at line %d, expected \"integer\" token to follow \":\"\n", tk.str, tk.line);
				}
			}else{
				printf("ERROR: \"%s\" at line %d, expected \":\" token to follow identifier token\n", tk.str, tk.line);
			}
		}else{
			printf("ERROR: \"%s\" at line %d, expected \"identifier\" token to follow \"var\"\n", tk.str, tk.line);
		}
	}else{
		printf("ERROR: \"%s\" at line %d, expected \"var\" token to begin var declaration\n", tk.str, tk.line);
	}
}

void block(){
	//check for void tk
	if(tk.name == voidTK){
		consume();
		vars();
		stats();
		if(tk.name == retunTK){
			//reached end of block
			consume();
			return;
		}
	}
	else{
		printf("ERROR: \"%s\" at line %d, expected \"void\" token to begin block\n", tk.str, tk.line);
	}
}

void consume(){
	tk = scanner();
}

