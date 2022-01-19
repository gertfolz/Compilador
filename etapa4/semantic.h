
#ifndef SEMANTIC_HEADER
#define SEMANTIC_HEADER

#include <stdio.h>
#include <stdlib.h>
#include "hash.h"
#include "ast.h"

extern int SemanticErrors;

void checkAndSetDec(AST *node);
void check_undeclared();
void semanticCheck(AST *node);
int checkReturn(AST *node, AST *nodeFunc);
int checkVector(AST* node);
int getSemanticErrors();
void setTypes(AST *node);
void checkOperands(AST *node);
void checkSemantic(AST *node1, AST *node2);
int checkArithmetics(AST *node);

#endif