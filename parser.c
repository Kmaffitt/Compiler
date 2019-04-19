#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "parser.h"
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
	return;
}


void program(){
	printf("in program\n");
	vars();
	block();
	printf("returning program\n");
	return;
}

void vars(){
	printf("in vars\n");
	//check for empty state -- follows of block or stats
	if(tk.id == voidTK || tk.id == scanTK || tk.id == printTK || tk.id == condTK || tk.id == iterTK || tk.id == idTK || tk.id == returnTK){
		printf("returning vars, empty\n");
		return;
	}
	else if(tk.id == varTK){
		consume();
		if(tk.id == idTK){
			consume();
			if(tk.id == colonTK){
				consume();
				if(tk.id == intTK){
					consume();
					vars();
					printf("returning vars\n");
					return;
				}else{
					printf("ERROR: \"%s\" at line %d, expected \"integer\" token to follow \":\"\n", tk.str, tk.line);
					exit(1);				
				}
			}else{
				printf("ERROR: \"%s\" at line %d, expected \":\" token to follow identifier token\n", tk.str, tk.line);
				exit(1);			
			}
		}else{
			printf("ERROR: \"%s\" at line %d, expected \"identifier\" token to follow \"var\"\n", tk.str, tk.line);
			exit(1);
		}
	}else{
		printf("ERROR: \"%s\" at line %d, expected \"var\" token to begin var declaration\n", tk.str, tk.line);
		exit(1);	
	}
}

void block(){
	printf("in block\n");
	//check for void tk
	if(tk.id == voidTK){
		consume();
		vars();
		stats();
		if(tk.id == returnTK){
			//reached end of block
			consume();
			printf("returning block\n");
			return;
		}
	}
	else{
		printf("ERROR: \"%s\" at line %d, expected \"void\" token to begin block\n", tk.str, tk.line);
		exit(1);	
	}
}

void stats(){
	printf("in stats\n");
	stat();
	if(tk.id == semiTK){		
		consume();/////////////////
		mStat();
		printf("returning stats\n");
		return;
	}else{
		printf("ERROR: \"%s\" at line %d, expected \";\" to end statement STATS\n", tk.str, tk.line);
		exit(1);	
	}
}

void mStat(){
	printf("in mStat\n");
	//check for empty production
	if(tk.id == returnTK){
		printf("returning mStat, empty\n");
		return;
	}else{
		stat();
		if(tk.id == semiTK){
			consume();
			mStat();
			printf("returning mStat\n");
			return;
		}else{
			printf("ERROR: \"%s\" at line %d, expected \";\" to end statement MSTAT\n", tk.str, tk.line);
			exit(1);	
		}
	}
}

void stat(){
	printf("in stat\n");
	//check for various initial statement tokens
	if(tk.id == scanTK){
		consume();
		in();
	}else if(tk.id == printTK){
		consume();
		out();
	}else if(tk.id == voidTK){
		//consume();
		block();
	}else if(tk.id == condTK){
		consume();
		ifF();
	}else if(tk.id == iterTK){
		consume();
		loop();
	}else if(tk.id == idTK){
		consume();
		assign();
	}else{
		printf("ERROR: \"%s\" at line %d, expected one of \"scan\",\"print\",\"cond\",\"iter\",\"scan\", or an identifier to begin statement\n", tk.str, tk.line);
		exit(1);
	}
	printf("returning stat\n");
	return;
}

void in(){
	printf("in in\n");
	if(tk.id == idTK){
		consume(); ////////////////
		printf("returning in\n");
		return;
	}else{
		printf("ERROR: \"%s\" at line %d, expected identifier to follow \"scan\"\n", tk.str, tk.line);
		exit(1);
	}
}

void out(){
	printf("in out\n");
	//consume();
	expr();
	printf("returning out\n");
	return;
}


void ifF(){
	printf("in if\n");
	if(tk.id == lsbTK){
		consume();
		expr();
		RO();
		expr();
		if(tk.id == rsbTK){
			consume();			
			stat();
			printf("returning if\n");
			return;
		}else{
			printf("ERROR: \"%s\" at line %d, expected \"]\" to conclude if statement\n", tk.str, tk.line);
			exit(1);
		}
		
	}else{
		printf("ERROR: \"%s\" at line %d, expected \"[\" to follow \"cond\"\n", tk.str, tk.line);
		exit(1);
	}
}

void loop(){
	printf("in loop\n");
	if(tk.id == lsbTK){
		consume();
		expr();
		RO();
		expr();
		if(tk.id == rsbTK){
			consume();			
			stat();
			printf("returning loop\n");
			return;
		}else{
			printf("ERROR: \"%s\" at line %d, expected \"]\" to conclude iter statement\n", tk.str, tk.line);
			exit(1);
		}
		
	}else{
		printf("ERROR: \"%s\" at line %d, expected \"[\" to follow \"iter\"\n", tk.str, tk.line);
		exit(1);
	}
}

void assign(){
	printf("in assign\n");
	if(tk.id == eqTK){
		consume();
		expr();
		printf("returning assign\n");
		return;
	}else{
		printf("ERROR: \"%s\" at line %d, expected \"=\" to follow identifier in assignment statement\n", tk.str, tk.line);
		exit(1);
	}
}

void expr(){
	printf("in expr\n");
	A();
	//expr -> <A> + <expr>
	if(tk.id == plusTK){
		consume();
		expr();
		printf("returning expr +\n");
		return;
	//expr -> <A> - <expr>
	}else if(tk.id == minusTK){
		consume();
		expr();
		printf("returning expr -\n");
		return;
	//expr -> <A>
	}else{
		printf("returning expr\n");
		return;
	}
}

void RO(){
	printf("in RO\n");
	if(tk.id == lessTK){
		consume();
		//test for <>
		if(tk.id == greatTK){
			consume();
			printf("returning RO <>\n");			
			return;
		}
		//just <
		printf("returning RO <\n");
		return;
	}else if(tk.id == eqTK){
		consume();
		//test for =< and =>
		if(tk.id == lessTK || tk.id == greatTK){
			consume();
			printf("returning RO =< or =>\n");
			return;
		}
		//just =
		printf("returning =\n");
		return;
	}else if(tk.id == greatTK){
		consume();
		printf("returning >\n");
		return;
	}else{
		printf("ERROR: \"%s\" at line %d, expected reltational operator: \"<\",\"=<\",\">\",\"=>\",\"<>\", or \"=\"\n", tk.str, tk.line);
		exit(1);
	}
}

void A(){
	printf("in A\n");
	N();
	if(tk.id == slashTK){
		consume();
		A();
		printf("returning A, N/A\n");
		return;
	}else{
		printf("returning A, N\n");
		return;
	}
}

void N(){
	printf("in N\n");
	M();
	if(tk.id == starTK){
		consume();
		N();
		printf("returning N, M*N\n");
		return;
	}else{
		printf("returning N, M\n");
		return;
	}
}

void M(){
	printf("In M\n");
	if(tk.id == percTK){
		consume();
		M();
		printf("returning M, %%M\n");
		return;	
	}else if(tk.id == lpTK || tk.id == idTK || tk.id == intTK){
		R();
		printf("returning M, R\n");
		return;
	}else{
		printf("ERROR: \"%s\" at line %d, expected \"%%\", an expression, an identifier, or an integer\n", tk.str, tk.line);
		exit(1);
	}
}

void R(){
	printf("In R\n");
	if(tk.id == lpTK){
		consume();
		expr();
		if(tk.id == rpTK){
			consume();
			printf("returning R, expr\n");
			return;
		}else{
			printf("ERROR: \"%s\" at line %d, expected \")\" to close opening paren\n", tk.str, tk.line);
			exit(1);
		}
	}else if(tk.id == idTK){
		consume();
		printf("returning R, id\n");
		return;
	}else if(tk.id == intTK){
		consume();
		printf("returning R, int\n");
		return;
	}else{
		printf("ERROR: \"%s\" at line %d, expected an expression, an identifier, or an integer\n", tk.str, tk.line);
		exit(1);
	}
}


void consume(){
	tk = scanner();
	//for debugging
	printf("\nNEXT TOKEN\nTK:    [%s]\nTKid:  %d\nTKstr: %s\nLine#: %d\n\n", tk.name,tk.id, tk.str, tk.line);
}

