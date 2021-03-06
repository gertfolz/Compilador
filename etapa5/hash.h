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

typedef struct hash_node
{
    int type;
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


#endif

