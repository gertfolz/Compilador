#ifndef ASMGEN_HEADER
#define ASMGEN_HEADER

#include <stdio.h>
#include <string.h>
#include "semantic.h"
#include "ast.h"
#include "hash.h"
#include "tacs.h"

void generateAsm(TAC* first, AST* ast);
char convertIntegers(char *text);
void addData(FILE* fout, AST* node);
#endif