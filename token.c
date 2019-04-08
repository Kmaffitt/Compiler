#include "token.h"

char* tokenNames[31] = {"idTK", "intTK", "iterTK", "voidTK", "varTK", "returnTK", "scanTK", "printTK", "programTK", "condTK", "thenTK", "letTK", "=TK", "<TK", ">TK", ":TK", "+TK", "-TK", "*TK", "/TK", "%TK", ".TK", "(TK", ")TK", ",TK", "{TK", "}TK", ";TK", "[TK", "]TK", "EOFTK"};

char* keywords[11] = {"int", "iter", "void", "var", "return", "scan", "print", "program", "cond", "then", "let"};

char* delimiters[18] = {"=", "<", ">", ":", "+", "-", "*", "/", "%", ".", "(", ")", ",", "{", "}", ";", "[", "]"};
