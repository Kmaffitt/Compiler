#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "scanner.h"
#include "token.h"

extern FILE * file;
static int lineCount = 1;
int lookAhead;

//FSA Table: Rows = States, Columns = Alphabet
//{lowercase, uppercase, digits, delimiters, $, WS, EOF}
const int FSATable[20][7] = {
{    1,   -2,    9,   17,   18,    0, 1004},
{    2,    2,    2, 1001, 1001, 1001, 1001},
{    3,    3,    3, 1001, 1001, 1001, 1001},
{    4,    4,    4, 1001, 1001, 1001, 1001},
{    5,    5,    5, 1001, 1001, 1001, 1001},
{    6,    6,    6, 1001, 1001, 1001, 1001},
{    7,    7,    7, 1001, 1001, 1001, 1001},
{    8,    8,    8, 1001, 1001, 1001, 1001},
{   -1,   -1,   -1, 1001, 1001, 1001, 1001},
{ 1002, 1002,   10, 1002, 1002, 1002, 1002},
{ 1002, 1002,   11, 1002, 1002, 1002, 1002},
{ 1002, 1002,   12, 1002, 1002, 1002, 1002},
{ 1002, 1002,   13, 1002, 1002, 1002, 1002},
{ 1002, 1002,   14, 1002, 1002, 1002, 1002},
{ 1002, 1002,   15, 1002, 1002, 1002, 1002},
{ 1002, 1002,   16, 1002, 1002, 1002, 1002},
{ 1002, 1002,   -1, 1002, 1002, 1002, 1002},
{ 1003, 1003, 1003, 1003, 1003, 1003, 1003},
{   18,   18,   18,   18,   19,   18,   18},
{    0,    0,    0,    0,    0,    0,    0}
};

enum alphabet {lowercase, uppercase, digits, delimit, comment, whiteSpace, endOfFile};
	

token_t scanner(){
	int state = 0;
	int inputChar;
	enum alphabet nextChar;
	token_t result;
	char str[] = "";
	int i;
	int flag;
	int commentFlag = 0;
	char tempstr[2];	

	inputChar = lookAhead;
	
	while(state < 1000){
	
		if(inputChar > 96 && inputChar < 123) nextChar = lowercase;
		else if(inputChar > 64 && inputChar < 91) nextChar = uppercase;
		else if(inputChar > 47 && inputChar < 58) nextChar = digits;
		else if(inputChar == 61 || inputChar == 60 || inputChar == 62 
			|| inputChar == 58 || inputChar == 43 || inputChar == 45 
			|| inputChar == 42 || inputChar == 47 || inputChar == 37 
			|| inputChar == 46 || inputChar == 40 || inputChar == 41 
			|| inputChar == 44 || inputChar == 123 || inputChar == 125 
			|| inputChar == 59 || inputChar == 91 || inputChar == 93) nextChar = delimit;
		else if(inputChar == 36) nextChar = comment;
		else if(inputChar == 32 || inputChar == 9 || inputChar == 10) nextChar = whiteSpace;
		else if(inputChar == EOF) nextChar = endOfFile;
		else {
			errorMsg(-3, lineCount);
			exit(1);
		}
	

		

		state = FSATable[state][nextChar];

		if(inputChar == 10 && state < 1000) lineCount += 1;

		if(state < 0){
			errorMsg(state, lineCount);
			exit(1);
		}
		else if(state == 18){
			commentFlag = 1;
		}
		else if(state == 19){
			commentFlag = 0;
		}
		else if(state > 1000){

			flag = 0;
			strcpy(result.str, str);
			if(state == 1001){
	

				for(i=0; i<12; i++){
					if(strcmp(keywords[i], str) == 0){
						result.id = (i+2);
						result.name = tokenNames[result.id];
						flag = 1;
						break;
					}
				}
	

				if(flag == 0) {
					result.id = 1;
					result.name = tokenNames[result.id];
				}
			}
			else if(state == 1002){
				result.id = 0;
				result.name = tokenNames[result.id];
			}
			else if(state == 1003){
				for(i = 0; i < 18; i++){
					if(strcmp(delimiters[i] ,str) == 0){
						result.id = (i+14);
						result.name = tokenNames[result.id];
					}
				}
			}
			else if(state == 1004){
				result.id = 32;
				result.name = tokenNames[result.id];
			}
		}
		else if(commentFlag == 0 && inputChar != 9 && inputChar != 10 && inputChar != 32){
			tempstr[0] = inputChar;
			tempstr[1] = '\0';
			strcat(str, tempstr);
		}

		if(state < 1000) inputChar = fgetc(file);
	}
	
	lookAhead = inputChar;
	result.line = lineCount;
	return result;
}	

void errorMsg(int state, int line){
	if(state == -1) fprintf(stderr, "Token length too long. Line: %d\n", line);
	if(state == -2) fprintf(stderr, "Token may not start with uppercase letter. Line: %d\n", line);
	if(state == -3) fprintf(stderr, "Invalid input. Line: %d\n", line);
}
