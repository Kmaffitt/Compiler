#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "parser.h"
#include "scanner.h"
#include "token.h"
#include "node.h"


extern int lookAhead;
extern FILE* file;

token_t tk;

/********************************
Parser for BNF--

<program>     ->      <vars> <block>
<block>       ->      void <vars> <stats> return
<vars>        ->      empty | var Identifier : Integer <vars> 
<expr>        ->      <A> + <expr> | <A> - <expr> | <A>
<A>           ->      <N> / <A> | <N>
<N>           ->      <M> * <N> | <M>
<M>           ->      % <M> |  <R>
<R>           ->      ( <expr> ) | Identifier | Integer
<stats>       ->      <stat> ; <mStat>
<mStat>       ->      empty |  <stat>  ;  <mStat>
<stat>        ->      <in> | <out> | <block> | <if> | <loop> | <assign>
<in>          ->      scan  Identifier 
<out>         ->      print <expr>
<if>          ->      cond [ <expr> <RO> <expr> ] <stat>
<loop>        ->      iter [ <expr> <RO> <expr> ] <stat>
<assign>      ->      Identifier  = <expr>  
<RO>          ->      < | = <  | >  | = > | < >   |   =            

********************************/

node_t* parse(){
	
	//open file and get first token
	lookAhead = fgetc(file);
	consume();	

	node_t* root;

	root = program();

	//printf("%s\n", root -> token.str);
	
	//check for overrun
	if(tk.id == EOFTK){
		printf("CLEAN PARSE\n\n");
		return root;
	}else{
		printf("ERROR: \"%s\" at line %d, extra input beyond last block of program\n", tk.str, tk.line);
		exit(1);
	}
	
}

/***************
<program>  ->     <vars> <block>
***************/
node_t* program(){
	printf("in program\n");

	node_t* node = 	create_node("<program>");
	
	node -> c1 = vars();
	node -> c2 = block();
	printf("returning program\n");
	return node;
}

/***************
<vars>         ->      empty | var Identifier : Integer <vars> 
***************/
node_t* vars(){
	printf("in vars\n");

	node_t* node = 	create_node("<vars>");

	//check for empty state -- follows of block or stats
	if(tk.id == voidTK || tk.id == scanTK || tk.id == printTK || tk.id == condTK || tk.id == iterTK || tk.id == idTK || tk.id == returnTK){
		printf("returning vars, empty\n");
		return node;
	}
	else if(tk.id == varTK){
		consume();
		if(tk.id == idTK){
			//save idTK
			node -> c1 = create_node("Identifier");
			node -> c1 -> token = tk;
			consume();
			if(tk.id == colonTK){
				consume();
				if(tk.id == intTK){
					//save id value
					node -> c2 = create_node("Integer");
					node -> c2 -> token = tk;
					consume();
					node -> c3 = vars();
					printf("returning vars\n");
					return node;
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

/***************
<block>       ->      void <vars> <stats> return
***************/
node_t* block(){
	printf("in block\n");
	node_t* node = 	create_node("<block>");
	//check for void tk
	if(tk.id == voidTK){
		consume();
		node -> c1 = vars();
		node -> c2 = stats();
		if(tk.id == returnTK){
			//reached end of block
			consume();
			printf("returning block\n");
			return node;
		}else{
			printf("ERROR: \"%s\" at line %d, expected \"return\" token to end block\n", tk.str, tk.line);
			exit(1);	
		}
	}
	else{
		printf("ERROR: \"%s\" at line %d, expected \"void\" token to begin block\n", tk.str, tk.line);
		exit(1);	
	}
}
/***************
<stats>         ->      <stat> ; <mStat>
***************/
node_t* stats(){
	printf("in stats\n");
	node_t* node = 	create_node("<stats>");
	node -> c1 = stat();
	if(tk.id == semiTK){		
		consume();
		node -> c2 = mStat();
		printf("returning stats\n");
		return node;
	}else{
		printf("ERROR: \"%s\" at line %d, expected \";\" to end statement STATS\n", tk.str, tk.line);
		exit(1);	
	}
}

/***************
<mStat>       ->      empty |  <stat>  ;  <mStat>
***************/
node_t* mStat(){
	printf("in mStat\n");
	node_t* node = 	create_node("<mStat>");
	//check for empty production
	if(tk.id == returnTK){
		printf("returning mStat, empty\n");
		return node;
	}else{
		node -> c1 = stat();
		if(tk.id == semiTK){
			consume();
			node -> c2 = mStat();
			printf("returning mStat\n");
			return node;
		}else{
			printf("ERROR: \"%s\" at line %d, expected \";\" to end statement MSTAT\n", tk.str, tk.line);
			exit(1);	
		}
	}
}

/***************
<stat>           ->      <in> | <out> | <block> | <if> | <loop> | <assign>
***************/
node_t* stat(){
	printf("in stat\n");
	node_t* node = 	create_node("<stat>");
	//check for various initial statement tokens
	if(tk.id == scanTK){
		consume();
		node -> c1 = in();
	}else if(tk.id == printTK){
		consume();
		node -> c1 = out();
	}else if(tk.id == voidTK){
		//consume();
		node -> c1 = block();
	}else if(tk.id == condTK){
		consume();
		node -> c1 = ifF();
	}else if(tk.id == iterTK){
		consume();
		node -> c1 = loop();
	}else if(tk.id == idTK){
		//consume();
		node -> c1 = assign();
	}else{
		printf("ERROR: \"%s\" at line %d, expected one of \"scan\",\"print\",\"cond\",\"iter\",\"scan\", or an identifier to begin statement\n", tk.str, tk.line);
		exit(1);
	}
	printf("returning stat\n");
	return node;
}

/***************
<in>              ->      scan  Identifier 
***************/
node_t* in(){
	printf("in in\n");
	node_t* node = 	create_node("<in>");
	if(tk.id == idTK){
		//save Identifier		
		node -> c1 = create_node("Identifier");
		node -> c1 -> token = tk;
		consume();
		printf("returning in\n");
		return node;
	}else{
		printf("ERROR: \"%s\" at line %d, expected identifier to follow \"scan\"\n", tk.str, tk.line);
		exit(1);
	}
}

/***************
<out>            ->      print <expr>
***************/
node_t* out(){
	printf("in out\n");
	node_t* node = 	create_node("<out>");
	node -> c1 = expr();
	printf("returning out\n");
	return node;
}

/***************
<if>               ->      cond [ <expr> <RO> <expr> ] <stat>
***************/
node_t* ifF(){
	printf("in if\n");
	node_t* node = 	create_node("<if>");
	if(tk.id == lsbTK){
		consume();
		node -> c1 = expr();
		node -> c2 = RO();
		node -> c3 = expr();
		if(tk.id == rsbTK){
			consume();			
			node -> c4 = stat();
			printf("returning if\n");
			return node;
		}else{
			printf("ERROR: \"%s\" at line %d, expected \"]\" to conclude if statement\n", tk.str, tk.line);
			exit(1);
		}
		
	}else{
		printf("ERROR: \"%s\" at line %d, expected \"[\" to follow \"cond\"\n", tk.str, tk.line);
		exit(1);
	}
}

/***************
<loop>          ->      iter [ <expr> <RO> <expr> ] <stat>
***************/
node_t* loop(){
	printf("in loop\n");
	node_t* node = 	create_node("<loop>");
	if(tk.id == lsbTK){
		consume();
		node -> c1 = expr();
		node -> c2 = RO();
		node -> c3 = expr();
		if(tk.id == rsbTK){
			consume();			
			node -> c4 = stat();
			printf("returning loop\n");
			return node;
		}else{
			printf("ERROR: \"%s\" at line %d, expected \"]\" to conclude iter statement\n", tk.str, tk.line);
			exit(1);
		}
		
	}else{
		printf("ERROR: \"%s\" at line %d, expected \"[\" to follow \"iter\"\n", tk.str, tk.line);
		exit(1);
	}
}

/***************
<assign>       ->      Identifier  = <expr>  
***************/
node_t* assign(){
	printf("in assign\n");
	node_t* node = 	create_node("<assign>");
	//save id
	node -> c1 = create_node("Identifier");
	node -> c1 -> token = tk;
	consume();
	if(tk.id == eqTK){
		consume();
		node -> c2 = expr();
		printf("returning assign\n");
		return node;
	}else{
		printf("ERROR: \"%s\" at line %d, expected \"=\" to follow identifier in assignment statement\n", tk.str, tk.line);
		exit(1);
	}
}

/***************
<expr>        ->      <A> + <expr> | <A> - <expr> | <A>
***************/
node_t* expr(){
	printf("in expr\n");
	node_t* node = 	create_node("<expr>");
	node -> c1 = A();
	//expr -> <A> + <expr>
	if(tk.id == plusTK){
		//save +
		node -> c2 = create_node("+");
		node -> c2 -> token = tk;
		consume();
		node -> c3 = expr();
		printf("returning expr +\n");
		return node;
	//expr -> <A> - <expr>
	}else if(tk.id == minusTK){
		//save -
		node -> c2 = create_node("-");
		node -> c2 -> token = tk;		
		consume();
		node -> c3 = expr();
		printf("returning expr -\n");
		return node;
	//expr -> <A>
	}else{
		printf("returning expr\n");
		return node;
	}
}

/***************
<RO>            ->      < | = <  | >  | = > | < >   |   =    
***************/
node_t* RO(){
	printf("in RO\n");
	node_t* node = 	create_node("<R0>");
	if(tk.id == lessTK){
		consume();
		//test for <>
		if(tk.id == greatTK){
			consume();
			node -> c1 = create_node("<>");
			printf("returning RO <>\n");			
			return node;
		}
		//just <
		node -> c1 = create_node("<");
		printf("returning RO <\n");
		return node;
	}else if(tk.id == eqTK){
		consume();
		//test for =< and =>
		if(tk.id == lessTK){
			node -> c1 = create_node("=<");
			consume();
			printf("returning RO =<\n");
			return node;
		}else if(tk.id == greatTK){
			node -> c1 = create_node("=>");
			consume();
			printf("returning RO =>\n");
			return node;
		}else{
			//just =
			node -> c1 = create_node("=");
			node -> c1 -> token = tk;
			printf("returning =\n");
			return node;
		}
	}else if(tk.id == greatTK){
		node -> c1 = create_node(">");
		node -> c1 -> token = tk;
		consume();
		printf("returning >\n");
		return node;
	}else{
		printf("ERROR: \"%s\" at line %d, expected reltational operator: \"<\",\"=<\",\">\",\"=>\",\"<>\", or \"=\"\n", tk.str, tk.line);
		exit(1);
	}
}

/***************
<A>             ->        <N> / <A> | <N>
***************/
node_t* A(){
	printf("in A\n");
	node_t* node = 	create_node("<A>");
	node -> c1 = N();
	if(tk.id == slashTK){
		//save /
		node -> c2 = create_node("/");
		node -> c2 -> token = tk;
		consume();
		node -> c3 = A();
		printf("returning A, N/A\n");
		return node;
	}else{
		printf("returning A, N\n");
		return node;
	}
}

/***************
<N>             ->       <M> * <N> | <M>
***************/
node_t* N(){
	printf("in N\n");
	node_t* node = 	create_node("<N>");
	node -> c1 = M();
	if(tk.id == starTK){
		//save *
		node -> c2 = create_node("*");
		node -> c2 -> token = tk;		
		consume();
		node -> c3 = N();
		printf("returning N, M*N\n");
		return node;
	}else{
		printf("returning N, M\n");
		return node;
	}
}

/***************
<M>              ->     % <M> |  <R>
***************/
node_t* M(){
	printf("In M\n");
	node_t* node = 	create_node("<M>");
	if(tk.id == percTK){
		//save %
		node -> c1 = create_node("%");
		node -> c1 -> token = tk;
		consume();
		node -> c2 = M();
		printf("returning M, %%M\n");
		return node;	
	//First(R)
	}else if(tk.id == lpTK || tk.id == idTK || tk.id == intTK){
		node -> c1 = R();
		printf("returning M, R\n");
		return node;
	}else{
		printf("ERROR: \"%s\" at line %d, expected \"%%\", an expression, an identifier, or an integer\n", tk.str, tk.line);
		exit(1);
	}
}

/***************
<R>              ->      ( <expr> ) | Identifier | Integer
***************/
node_t* R(){
	printf("In R\n");
	node_t* node = 	create_node("<R>");
	if(tk.id == lpTK){
		consume();
		node -> c1 = expr();
		if(tk.id == rpTK){
			consume();
			printf("returning R, expr\n");
			return node;
		}else{
			printf("ERROR: \"%s\" at line %d, expected \")\" to close opening paren\n", tk.str, tk.line);
			exit(1);
		}
	}else if(tk.id == idTK){
		//save id
		node -> c1 = create_node("Identifier");
		node -> c1 -> token = tk;
		consume();
		printf("returning R, id\n");
		return node;
	}else if(tk.id == intTK){
		//save int
		node -> c1 = create_node("Integer");
		node -> c1 -> token = tk;
		consume();
		printf("returning R, int\n");
		return node;
	}else{
		printf("ERROR: \"%s\" at line %d, expected an expression, an identifier, or an integer\n", tk.str, tk.line);
		exit(1);
	}
}

//returns next token in file
void consume(){
	tk = scanner();
	//for debugging
	printf("\nNEXT TOKEN\nTK:    [%s]\nTKstr: %s\nLine#: %d\n\n", tk.name, tk.str, tk.line);
}

node_t* create_node(char* label){
	node_t* node = NULL;
	node = malloc(sizeof(node_t));
	if(node == NULL){
		return NULL;
	}
	node -> label = label;
	node -> c1 = NULL;
	node -> c2 = NULL;
	node -> c3 = NULL;
	node -> c4 = NULL;

	//will get replaced if token needs to be stored. This way can test before printing token when printing nodes
	token_t dummyToken;
	dummyToken.id = -1;
	node -> token = dummyToken;

	return node;
}
