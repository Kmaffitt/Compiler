#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include "node.h"
#include "token.h"

void printTree(node_t* node, int level){

	int i;

	if(node == NULL){
		printf("Fatal error printing tree\n");
		exit(1);
	}else{
		for(i = 0; i < level; i++){
			printf("  ");
		}		
		level++;
		printf("%s ", node->label);
		if(node->token.id != -1){
			printf(": TK[%s] = %s", node->token.name, node->token.str);
		}
		printf("\n");
		if(node -> c1 != NULL){
			printTree(node -> c1, level);
		}if(node -> c2 != NULL){
			printTree(node -> c2, level);
		}if(node -> c3 != NULL){
			printTree(node -> c3, level);
		}if(node -> c4 != NULL){
			printTree(node -> c4, level);
		} 
	}
}
