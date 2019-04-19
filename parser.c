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
	vars();
	block();
	return;
}

void vars(){
	//check for empty state -- follows of block or stats
	if(tk.id == voidTK || tk.id == scanTK || tk.id == printTK || tk.id == condTK || tk.id == iterTK || tk.id == idTK || tk.id == returnTK){
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
	//check for void tk
	if(tk.id == voidTK){
		consume();
		vars();
		stats();
		if(tk.id == returnTK){
			//reached end of block
			consume();
			return;
		}
	}
	else{
		printf("ERROR: \"%s\" at line %d, expected \"void\" token to begin block\n", tk.str, tk.line);
		exit(1);	
	}
}

void stats(){
	stat();
	if(tk.id == semiTK){
		mStat();
		return;
	}else{
		printf("ERROR: \"%s\" at line %d, expected \";\" to end statement\n", tk.str, tk.line);
		exit(1);	
	}
}

void mStat(){
	//check for empty production
	if(tk.id == returnTK){
		return;
	}else{
		stat();
		if(tk.id == semiTK){
			mStat();
			return;
		}else{
			printf("ERROR: \"%s\" at line %d, expected \";\" to end statement\n", tk.str, tk.line);
			exit(1);	
		}
	}
}

void stat(){
	//check for various initial statement tokens
	if(tk.id == scanTK){
		consume();
		in();
	}else if(tk.id == printTK){
		consume();
		out();
	}else if(tk.id == voidTK){
		consume();
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
		printf("ERROR: \"%s\" at line %d, expected one of(\"scan\",\"print\",\"cond\",\"iter\",\"scan\", or an identifier to begin statement\n", tk.str, tk.line);
		exit(1);
	}

	return;
}

void in(){
	if(tk.id == idTK){
		return;
	}else{
		printf("ERROR: \"%s\" at line %d, expected identifier to follow \"scan\"\n", tk.str, tk.line);
		exit(1);
	}
}

void out(){
	consume();
	expr();
	return;
}


void ifF(){
	if(tk.id == lbTK){
		consume();
		expr();
		RO();
		expr();
		if(tk.id == rbTK){
			consume();			
			stat();
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
	if(tk.id == lbTK){
		consume();
		expr();
		RO();
		expr();
		if(tk.id == rbTK){
			consume();			
			stat();
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
	if(tk.id == eqTK){
		consume();
		expr();
		return;
	}else{
		printf("ERROR: \"%s\" at line %d, expected \"=\" to follow identifier in assignment statement\n", tk.str, tk.line);
		exit(1);
	}
}

void expr(){
	A();
	//expr -> <A> + <expr>
	if(tk.id == plusTK){
		consume();
		expr();
		return;
	//expr -> <A> - <expr>
	}else if(tk.id == minusTK){
		consume();
		expr();
		return;
	//expr -> <A>
	}else{
		return;
	}
}

void RO(){
	if(tk.id == lessTK){
		consume();
		//test for <>
		if(tk.id == greatTK){
			consume();			
			return;
		}
		//just <
		return;
	}else if(tk.id == eqTK){
		consume();
		//test for =< and =>
		if(tk.id == lessTK || tk.id == greatTK){
			consume();
			return;
		}
		//just =
		return;
	}else if(tk.id == greatTK){
		consume();
		return;
	}else{
		printf("ERROR: \"%s\" at line %d, expected reltational operator: \"<\",\"=<\",\">\",\"=>\",\"<>\", or \"=\"\n", tk.str, tk.line);
		exit(1);
	}
}

void A(){
	N();
	if(tk.id == slashTK){
		consume();
		A();
		return;
	}else{
		return;
	}
}

void N(){
	M();
	if(tk.id == starTK){
		consume();
		N();
		return;
	}else{
		return;
	}
}

void M(){
	if(tk.id == percTK){
		consume();
		M();
		return;	
	}else if(tk.id == lpTK || tk.id == idTK || tk.id == intTK){
		R();
		return;
	}else{
		printf("ERROR: \"%s\" at line %d, expected \"%%\", an expression, and identifier, or an integer\n", tk.str, tk.line);
		exit(1);
	}
}

void R(){
	if(tk.id == lpTK){
		consume();
		expr();
		if(tk.id == rpTK){
			consume();
			return;
		}else{
			printf("ERROR: \"%s\" at line %d, expected \")\" to close opening paren\n", tk.str, tk.line);
			exit(1);
		}
	}else if(tk.id == idTK){
		consume();
		return;
	}else if(tk.id == intTK){
		consume();
		return;
	}else{
		printf("ERROR: \"%s\" at line %d, expected an expression, an identifier, or an integer\n", tk.str, tk.line);
		exit(1);
	}
}


void consume(){
	tk = scanner();
	//for debugging
	printf("\nTK:    [%s]\nTKid:  %d\nTKstr: %s\nLine#: %d\n\n", tk.name,tk.id, tk.str, tk.line);
}

