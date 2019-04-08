#ifndef SCANNER_H
#define SCANNER_H

#include "token.h"
int lookAhead;
token_t scanner();
void errorMsg(int, int);

#endif
