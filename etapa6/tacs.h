
#ifndef TACS_HEADER
#define TACS_HEADER

#include "hash.h"
#include "ast.h"

#define TAC_SYMBOL 1
#define TAC_ADD 2
#define TAC_SUB 3
#define TAC_ATTR 4
#define TAC_JFALSE 5
#define TAC_LABEL 6
#define TAC_MULT 7
#define TAC_DIV 8
#define TAC_JUMP 9
#define TAC_LT 10
#define TAC_GT 11
#define TAC_LE 12
#define TAC_GE 13
#define TAC_EQ 14
#define TAC_DIF 15
#define TAC_AND 16
#define TAC_OR 17
#define TAC_NOT 18
#define TAC_RETURN 19
#define TAC_PRINT 20
#define TAC_READ 21
#define TAC_BEGINFUNCTION 22
#define TAC_ENDFUNCTION 23
#define TAC_PRINTPAR 24
#define TAC_ATTRV 25
#define TAC_VECTOR 26
#define TAC_VECIND 27
#define TAC_FUNCPAR 28
#define TAC_UNTIL 29
#define TAC_CHECKPOINT 30
#define TAC_COMEFROM 31
#define TAC_FUNC 32
#define TAC_FUNCARGS 33
#define TAC_FUNCRES 34


typedef struct tac_node
{
    int type;
    HASH_NODE *res;
    HASH_NODE *op1;
    HASH_NODE *op2;
    struct tac_node *prev;
    struct tac_node *next;
} TAC;

TAC* tacCreate(int type, HASH_NODE *res, HASH_NODE *op1, HASH_NODE *op2);
void tacPrint(TAC *tac);
void tacPrintBackwards(TAC *tac); 
TAC *tacJoin(TAC *l1, TAC *l2);
TAC *makeIfThen (TAC* code[]);
TAC *binaryOp(TAC *code[], int type);
TAC *print(TAC *code[], AST *node);
TAC *attr(TAC *code[], AST *node);
TAC *makeFunction(TAC* code[], AST *node);
TAC *until(TAC *code[], HASH_NODE *untilLabel);

//CODE GENERATION

TAC *generateCode(AST *node);

TAC *tacReverse(TAC *tac);
void generateAsm(TAC *first);
int convertIntegers(char *text);


#endif