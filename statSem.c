#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include "node.h"
#include "token.h"

int size = 0;
token_t T[100];

void checkStaticSemantics(node_t* node){

    int i;
    int varDefined = 0;
    
    if(node == NULL){
		printf("Fatal error processing parse tree\n");
		exit(1);
	}
    //declaring var
    if(!strcmp(node->label,"Identifier")){
        //check if ID is already stored
        for(i = 0; i < size; i++){
            if(!strcmp(T[i].str,node->token.str)){
                //already have ID
                printf("SEMANTIC ERROR: Declared variable %s on line %d, already defined on line%d\n", node->token.str, node->token.line, T[i].line);
                exit(1);
            }
        }
        //haven't defined var yet, add it to T
        T[size++] = node->token;
    //using var-- R, in, or assign
    }else if(!strcmp(node->label,"<R>") || !strcmp(node->label,"<in>") || !strcmp(node->label,"<assign>")){
        //check if ID has been defined yet
        for(i = 0; i < size; i++){
            if(strcmp(T[i].str,node->token.str)){
                //already have ID
                varDefined = 1;
                break;
            }    
        }
        if(!varDefined){
            printf("SEMANTIC ERROR: Used undeclared variable %s on line %d\n", node->token.str, node->token.line);
            exit(1);
        }
    }

    if(node -> c1 != NULL){
		checkStaticSemantics(node -> c1);
	}if(node -> c2 != NULL){
		checkStaticSemantics(node -> c2);
	}if(node -> c3 != NULL){
		checkStaticSemantics(node -> c3);
	}if(node -> c4 != NULL){
		checkStaticSemantics(node -> c4);
	} 
}