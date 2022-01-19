
#ifndef SEMANTIC_HEADER
#define SEMANTIC_HEADER

#include <stdio.h>
#include <stdlib.h>
#include "hash.h"
#include "ast.h"

extern int SemanticErrors;

void checkAndSetDec(AST *node);

void setTypes(AST *node);

#endif