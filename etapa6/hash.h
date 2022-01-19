/* Gert Willem Folz - 00288550 
 * Trabalho 5 da disciplina de compiladores */


#ifndef HASH_HEADER
#define HASH_HEADER

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HASH_SIZE 997

#define SYMBOL_VARIABLE 6
#define SYMBOL_LABEL 7
#define SYMBOL_LITINT 275
#define SYMBOL_STRING 278
#define SYMBOL_LITFLOAT 277
#define SYMBOL_LITCHAR 276

#define SYMBOL_IDENTIFIER 274

#define SYMBOL_FUNC 12
#define SYMBOL_VET 13

#define DATATYPE_ERROR 111
#define DATATYPE_INT 9
#define DATATYPE_FLOAT 10
#define DATATYPE_STRING 12
#define DATATYPE_CHAR 11

typedef struct hash_node
{
    int type;
    int datatype;
    char *text;
    struct hash_node *next;
}HASH_NODE;

void initializeHash();
int hashAddress(char *text);
HASH_NODE *hashFind(char *text);
HASH_NODE *hashInsert(char *text, int type);
void hashPrint();
HASH_NODE *makeTemp(void);
HASH_NODE *makeLabel(void);

void printAsm(FILE *fout);
int convertIntegers(char *text);



#endif

