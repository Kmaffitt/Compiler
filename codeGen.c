#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include "node.h"
#include "token.h"
#include "codeGen.h"

extern FILE* outFile;
extern token_t* T;
token_t Temp[100];
int numTemps = 0;

void gen(node_t* node){

    token_t* gg = makeTemp();
    printf("%s\n",gg->str);
    token_t* ff = makeTemp();
    printf("%s\n",ff->str);
    return;
   
    //fprintf(outFile, "Testing\n");

    char* label = node->label;

    if(node == NULL){
        return;
    }else{
        if(strcmp(label, "<program>")){
            //vars
            gen(node->c1);
            //block
            gen(node->c2);
            fprintf(outFile, "STOP\n");
            //print out variables

        }else if(strcmp(label, "<vars>")){
            //ID
            gen(node->c1);
            //vars
            gen(node->c3);

        }else if(strcmp(label, "<block>")){
            //vars
            gen(node->c1);
            //stats
            gen(node->c2);

        }else if(strcmp(label, "<stats>")){
            //stat
            gen(node->c1);
            //mStat
            gen(node->c2);

        }else if(strcmp(label, "<mStat>")){
            //stat
            gen(node->c1);
            //mStat
            gen(node->c2);

        }else if(strcmp(label, "<stat>")){
            //<in> | <out> | <block> | <if> | <loop> | <assign>
            gen(node->c1);

        }else if(strcmp(label, "<in>")){
            fprintf(outFile, "READ %s\n", node->token.str);

        }else if(strcmp(label, "<out>")){
            //expr
            gen(node->c1);
            
            //TODO
            //temp to save expr's value

            fprintf(outFile, "WRITE %s\n", node->token.str);

        }else if(strcmp(label, "<if>")){
            //expr
        }
    }
    
    return;
}

token_t* makeTemp(){
    token_t* tempID = NULL;
	tempID = malloc(sizeof(token_t));
	if(tempID == NULL){
		return NULL;
	}
    char num[3];

    sprintf(num, "%d", numTemps);
    char stub[]= "T";
    strcpy(tempID->str, strcat(stub, num));
    Temp[numTemps++] = *tempID;
    
    return tempID;
}