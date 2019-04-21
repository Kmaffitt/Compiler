#ifndef NODE_H
#define NODE_H
#include "scanner.h"
#include "token.h"
#include "node.h"

typedef struct node_t node_t;
struct node_t{
	char* label;
	node_t* c1, *c2, *c3, *c4;
	token_t token;
};

#endif

