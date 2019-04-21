#ifndef PARSER_H
#define PARSER_H

#include "token.h"
#include "node.h"

node_t* parse();
node_t* program();
node_t* vars();
node_t* block();
node_t* stats();
node_t* mStat();
node_t* stat();
node_t* in();
node_t* out();
node_t* ifF();
node_t* loop();
node_t* assign();
node_t* expr();
node_t* RO();
node_t* A();
node_t* N();
node_t* M();
node_t* R();
void consume();
node_t* create_node(char*);


#endif
