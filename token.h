#ifndef TOKEN_H
#define TOKEN_H


typedef enum TokenType tokenType; 
enum tokenType{idTK, intTK, iterTK, voidTK, varTK, returnTK, scanTK, printTK, programTK, condTK, thenTK, letTK, eqTK, lessTK, greatTK, colonTK, plusTK, minusTK, starTK, slashTK, percTK, dotTK, lpTK, rpTK, commaTK, lbTK, rbTK, semiTK, lsbTK, rsbTK, EOFTK};


char* tokenNames[31];
char* keywords[11];
char* delimiters[18];

typedef struct token_t token_t;
struct token_t {
	enum tokenType id;
	char* name;
	char str[100];
	int line;
};


#endif
