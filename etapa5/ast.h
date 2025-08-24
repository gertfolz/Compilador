
//AST - Abstract syntax tree
#include <stdio.h>
#include <stdlib.h>
#include "hash.h"

#ifndef AST_HEADER
#define AST_HEADER

#define MAX_SONS 4

#define AST_SYMBOL 1
#define AST_ADD 2
#define AST_SUB 3
#define AST_MULT 4
#define AST_DIV 5
#define AST_LT 6
#define AST_GT 7
#define AST_OR 8
#define AST_AND 9
#define AST_NOT 10
#define AST_LE 11
#define AST_GE 12
#define AST_EQ 13
#define AST_DIF 14
#define AST_ATTR 15
#define AST_RETURN 16
#define AST_PRINT 17
#define AST_IF 18
#define AST_IFELSE 19
#define AST_UNTIL 20
#define AST_COMEFROM 21
#define AST_CHECKPOINT 22
#define AST_LCMD 23
#define AST_DATA 24
#define AST_DEC 25
#define AST_INT 26
#define AST_CHAR 27
#define AST_FLOAT 28
#define AST_DECV 29
#define AST_VECINIT 30
#define AST_VECS 31
#define AST_DECDATA 32
#define AST_FUNCLIST 33
#define AST_FUNC 34
#define AST_ARGS 35
#define AST_ARGS2 36
#define AST_BLOCK 37
#define AST_VECTOR 38
#define AST_ARGSRABO 39
#define AST_ARGS2RABO 40
#define AST_READ 41
#define AST_FLUX 42
#define AST_FUNCAUX 43
#define AST_PRINTPAR 44
#define AST_PRINTPARTAIL 45





typedef struct astnode
{
    int type;
    HASH_NODE *symbol;
    struct astnode *son[MAX_SONS];
} AST;

AST *astCreate(int type, HASH_NODE *symbol, AST* s0, AST* s1, AST* s2, AST* s3);
void astPrint(AST *node, int level);
void astDecomp(AST *node);

#endif 