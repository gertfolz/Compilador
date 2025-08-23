/* Gert Willem Folz - 00288550 
 * Trabalho 2 da disciplina de compiladores */

#include "hash.h"

HASH_NODE *Table[HASH_SIZE];

void hashInit()
{
    for(int i=0; i<HASH_SIZE; i++)
        Table[i]=0;
}

int hashAddress (char *text)
{
    int address=1;
    for (int i=0; i<strlen(text); i++)
        address=(address * text[i])%HASH_SIZE + 1;
    return address-1;
} 

HASH_NODE *hashFind(char *text)
{
    HASH_NODE *node;
    int address=hashAddress(text);
    for (node=Table[address]; node; node=node->next)
        if(strcmp(node->text, text)==0)
            return node;
    return 0;
}

HASH_NODE *hashInsert(char *text, int type)
{
    HASH_NODE *newnode;
    int address = hashAddress(text);

    if((newnode=hashFind(text))!=0)
        return newnode;
    
    newnode=(HASH_NODE*)malloc(sizeof(HASH_NODE));
    newnode->type=type;
    newnode->text=(char*)calloc(strlen(text)+1,sizeof(char));
    strcpy(newnode->text, text);
    newnode->next=Table[address];
    Table[address]=newnode;
    return newnode;
}

void hashPrint()
{
    HASH_NODE *node;
    for(int i=0;i<HASH_SIZE; i++)
        for(node=Table[i]; node; node=node->next)
        {
            printf("Table[%d] has %s ", i, node->text);
            switch(node->type)
            {
                case 274: printf("of TK_IDENTIFIER type\n");break;
                case 275: printf("of LIT_INTEGER type\n");break;
                case 276: printf("of LIT_CHAR type\n");break;
                case 277: printf("of LIT_STRING type\n");break;
            }
        }       
}
