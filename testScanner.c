#include <stdio.h>
#include "scanner.h"
#include "testScanner.h"
#include "token.h"

extern int lookAhead;
extern FILE * file;
void testScanner(){
	
	lookAhead = fgetc(file);
	token_t token;
	do{
		token = scanner();
		printf("%s %s %d \n", token.name, token.str, token.line);	
	}while(token.id != EOFTK);
}
