#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include "node.h"
#include "token.h"
#include "codeGen.h"

extern FILE* outFile;
extern token_t T[];
extern int size;
token_t Temp[100];
int numTemps = 0;
int numLabels = 0;

void gen(node_t* node){

    //fprintf(outFile, "Testing\n");
    char* label = node->label;
    printf("%s\n", label);

    if(node == NULL){
        printf("Null node\n");
        return;
    }else{
        if(!strcmp(label, "<program>")){
            printf("program\n");
            //vars
            //gen(node->c1);
            //block
            gen(node->c2);
            fprintf(outFile, "STOP\n");
            //print out variables
            int i;
            for(i=0; i<size; i++){
                fprintf(outFile, "%s 0\n", T[i].str);
            }

            for(i=0; i<numTemps; i++){
                fprintf(outFile, "%s 0\n", Temp[i].str);
            }

        }else if(!strcmp(label, "<vars>")){
            printf("vars\n");
            //ID
            //gen(node->c1);
            //vars
            //gen(node->c3);

        }else if(!strcmp(label, "<block>")){
            printf("block\n");
            //vars
            //gen(node->c1);
            //stats
            gen(node->c2);

        }else if(!strcmp(label, "<stats>")){
            printf("stats\n");
            //stat
            gen(node->c1);
            //mStat
            gen(node->c2);

        }else if(!strcmp(label, "<mStat>")){
            printf("mStat\n");
            if(node->c1 != NULL){
                //stat
                gen(node->c1);
                //mStat
                gen(node->c2);
            }

        }else if(!strcmp(label, "<stat>")){
            printf("stat\n");
            //<in> | <out> | <block> | <if> | <loop> | <assign>
            gen(node->c1);

        }else if(!strcmp(label, "<in>")){
            printf("in\n");
            fprintf(outFile, "READ %s\n", node->token.str);

        }else if(!strcmp(label, "<out>")){
            printf("out\n");
            //expr
            gen(node->c1);
            //temp Identifier to store expr's result
            token_t* tp = makeTemp();
            fprintf(outFile, "STORE %s\n", tp->str);
            fprintf(outFile, "WRITE %s\n", tp->str);

        }else if(!strcmp(label, "<if>")){
            printf("if\n");
            //store RO
            char* RO = node->c2->c1->label;
            //rightmost expr
            gen(node->c3);
            //store result
            token_t* tp = makeTemp();
            fprintf(outFile, "STORE %s\n", tp->str);
            //leftmost expr
            gen(node->c1);
            fprintf(outFile, "SUB %s\n", tp->str);
            //result is in acc, now determine which BR command is appropriate for our RO
            //new label for jump
            char* label = makeLabel();
            if(!strcmp(RO, "<>")){
                fprintf(outFile, "BRZERO %s\n", label);
            }else if(!strcmp(RO, "<")){
                fprintf(outFile, "BRZPOS %s\n", label);
            }else if(!strcmp(RO, "=<")){
                fprintf(outFile, "BRPOS %s\n", label);
            }else if(!strcmp(RO, "=>")){
                fprintf(outFile, "BRNEG %s\n", label);
            }else if(!strcmp(RO, ">")){
                fprintf(outFile, "BRZNEG %s\n", label);
            }else if(!strcmp(RO, "=")){
                fprintf(outFile, "BRPOS %s\n", label);
                fprintf(outFile, "BRNEG %s\n", label);
            }
            //<stat>
            gen(node->c4);
            //if cond is not true, do nothing
            fprintf(outFile, "%s: NOOP\n", label);

        }else if(!strcmp(label, "<loop>")){
            printf("loop\n");
            //store RO
            char* RO = node->c2->c1->label;
            //temp var for evaluating expr
            token_t* tp = makeTemp();
            //labels for loop
            char* startLabel = makeLabel();
            char* endLabel = makeLabel();
            fprintf(outFile, "%s: ", startLabel);
            //rightmost expr
            gen(node->c3);
            //store result
            fprintf(outFile, "STORE %s\n", tp->str);
            //leftmost expr
            gen(node->c1);
            fprintf(outFile, "SUB %s\n", tp->str);
            //result is in acc, now determine which BR command is appropriate for our RO
            if(!strcmp(RO, "<>")){
                fprintf(outFile, "BRZERO %s\n", endLabel);
            }else if(!strcmp(RO, "<")){
                fprintf(outFile, "BRZPOS %s\n", endLabel);
            }else if(!strcmp(RO, "=<")){
                fprintf(outFile, "BRPOS %s\n", endLabel);
            }else if(!strcmp(RO, "=>")){
                fprintf(outFile, "BRNEG %s\n", endLabel);
            }else if(!strcmp(RO, ">")){
                fprintf(outFile, "BRZNEG %s\n", endLabel);
            }else if(!strcmp(RO, "=")){
                fprintf(outFile, "BRPOS %s\n", endLabel);
                fprintf(outFile, "BRNEG %s\n", endLabel);
            }
            //<stat>
            gen(node->c4);
            //do it again
            fprintf(outFile, "BR %s\n", startLabel);
            //or stop
            fprintf(outFile, "%s: NOOP\n", endLabel);

        }else if(!strcmp(label, "<assign>")){
            printf("assign\n");
            //<expr>
            gen(node->c2);
            fprintf(outFile, "STORE %s\n", node->token.str);

        }else if(!strcmp(label, "<expr>")){
            printf("expr\n");
            if(node->c2 == NULL){
                //<A>
                gen(node->c1);
            }else{
                gen(node->c3);
                token_t* tp = makeTemp();
                fprintf(outFile, "STORE %s\n", tp->str);
                gen(node->c1);
                if(!strcmp(node->c2->label, "+")){
                    fprintf(outFile, "ADD %s\n", tp->str);
                }else{
                    fprintf(outFile, "SUB %s\n", tp->str);
                }
            }

        }else if(!strcmp(label, "<A>")){
            printf("A\n");
            if(node->c2 == NULL){
                gen(node->c1);
            }
            else{
                gen(node->c3);
                token_t* tp = makeTemp();
                fprintf(outFile, "STORE %s\n", tp->str);
                gen(node->c1);
                fprintf(outFile, "DIV %s\n", tp->str);
            }

        }else if(!strcmp(label, "<N>")){
            printf("N\n");
            if(node->c2 == NULL){
                gen(node->c1);
            }else{
                gen(node->c3);
                token_t* tp = makeTemp();
                fprintf(outFile, "STORE %s\n", tp->str);
                gen(node->c1);
                fprintf(outFile, "MULT %s\n", tp->str);
            }

        }else if(!strcmp(label, "<M>")){
            printf("M\n");
            if(!strcmp(node->c1->label, "%")){
                gen(node->c2);
                fprintf(outFile, "MULT -1\n");
            }else{
                gen(node->c1);
            }

        }else if(!strcmp(label, "<R>")){
            printf("R\n");
            if(node->c1 == NULL){
                fprintf(outFile, "LOAD %s\n", node->token.str);
            }else{
                gen(node->c1);
            }

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

char* makeLabel(){
    char* string;
    string = malloc(sizeof(char)*4);
    
    char num[3];
    sprintf(num, "%d", numLabels++);

    char stub[]= "L";
    strcat(string, stub);
    strcat(string, num);

    return string;
}