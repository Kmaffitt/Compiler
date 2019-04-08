#ifndef TOKEN_H
#define TOKEN_H


typedef enum TokenType tokenType; 
enum tokenType{intTK, idTK, startTK, stopTK, loopTK, voidTK, varTK, returnTK, scanTK, outTK, programTK, ifTK, thenTK, letTK, eqTK, lessTK, greatTK, colonTK, plusTK, minusTK, starTK, slashTK, percTK, dotTK, lpTK, rpTK, commaTK, lbTK, rbTK, semiTK, lsbTK, rsbTK, EOFTK};


char * tokenNames[33];
char * keywords[12];
char * delimiters[18];

typedef struct token_t token_t;
struct token_t {
	enum tokenType id;
	char * name;
	char str[100];
	int line;
};


#endif
