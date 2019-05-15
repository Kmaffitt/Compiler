#ifndef CODEGEN_H
#define CODEGEN_H
#include "token.h"
#include "node.h"

void gen(node_t*);
token_t* makeTemp();
char* makeLabel();

#endif