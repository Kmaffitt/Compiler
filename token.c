#include "token.h"

char* tokenNames[33] = {"intTK", "idTK", "startTK", "stopTK", "loopTK", "voidTK", "varTK", "returnTK", "scanTK", "outTK", "programTK", "ifTK", "thenTK", "letTK", "=TK", "<TK", ">TK", ":TK", "+TK", "-TK", "*TK", "/TK", "%TK", ".TK", "(TK", ")TK", ",TK", "{TK", "}TK", ";TK", "[TK", "]TK", "EOFTK"};

char* keywords[12] = {"start", "stop", "loop", "void", "var", "return", "scan", "out", "program", "if", "then", "let"};

char* delimiters[18] = {"=", "<", ">", ":", "+", "-", "*", "/", "%", ".", "(", ")", ",", "{", "}", ";", "[", "]"};
