#ifndef STATSEM_H
#define STATSEM_H
#include "token.h"
#include "node.h"

token_t* T;
int size;

void checkStaticSemantics(node_t*);

#endif