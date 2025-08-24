
#include "hash.h"

HASH_NODE *Table[HASH_SIZE];

void hashInit()
{
    for (int i = 0; i < HASH_SIZE; i++)
        Table[i] = 0;
}

int hashAddress(char *text)
{
    int address = 1;
    for (int i = 0; i < strlen(text); i++)
        address = (address * text[i]) % HASH_SIZE + 1;
    return address - 1;
}

HASH_NODE *hashFind(char *text)
{
    HASH_NODE *node;
    int address = hashAddress(text);
    for (node = Table[address]; node; node = node->next)
        if (strcmp(node->text, text) == 0)
            return node;
    return 0;
}

HASH_NODE *hashInsert(char *text, int type)
{
    HASH_NODE *newnode;
    int address = hashAddress(text);

    if ((newnode = hashFind(text)) != 0)
        return newnode;

    newnode = (HASH_NODE *)malloc(sizeof(HASH_NODE));
    newnode->type = type;
    newnode->text = (char *)calloc(strlen(text) + 1, sizeof(char));
    strcpy(newnode->text, text);
    newnode->next = Table[address];
    Table[address] = newnode;
    return newnode;
}

void hashPrint()
{
    HASH_NODE *node;
    for (int i = 0; i < HASH_SIZE; i++)
        for (node = Table[i]; node; node = node->next)
        {
            printf("Table[%d] has %s ", i, node->text);
            switch (node->type)
            {
            case 274:
                printf("of TK_IDENTIFIER type\n");
                break;
            case 275:
                printf("of LIT_INTEGER type\n");
                break;
            case 276:
                printf("of LIT_CHAR type\n");
                break;
            case 277:
                printf("of LIT_STRING type\n");
                break;
            default:
                printf("\n");
                break;
            }
        }
}

HASH_NODE *makeTemp()
{
    static int serial = 0;
    char buffer[256] = "";
    sprintf(buffer, "aaAaAaaAtemp%d", serial++);
    return hashInsert(buffer, SYMBOL_VARIABLE);
}

HASH_NODE *makeLabel()
{
    static int serial = 0;
    char buffer[256] = "";
    sprintf(buffer, "aaAaAaaaLABEL%d", serial++);
    return hashInsert(buffer, SYMBOL_LABEL);
}

void printAsm(FILE *fout)
{
    int numString = 0;
    HASH_NODE *node;
    for (int i = 0; i < HASH_SIZE; i++)
        for (node = Table[i]; node; node = node->next)
        {
            printf("%d\n", node->type);
            if ((node->type != SYMBOL_FUNC) && (node->type != SYMBOL_LABEL))
                switch (node->type)
                {

                case SYMBOL_LITINT:
                    fprintf(fout, "\n_%s:\t.long\t%d\n", node->text, convertIntegers(node->text));
                    break;
                case SYMBOL_STRING:
                    fprintf(fout, "\n_string%d:\t.string\t%s\n", numString, node->text);
                    break;
                default:
                    fprintf(fout, "\n\t.globl	_%s\n"
                                  "\t.data\n"
                                  "\t.type	_%s, @object\n"
                                  "\t.size	_%s, 4\n"
                                  "_%s:\t.long\t%d\n",
                            node->text, node->text, node->text, node->text, 0);
                    break;
                }
        }
}

int convertIntegers(char *text)
{
    if (strcmp(text, "O") == 0)
        return 0;
    if (strcmp(text, "I") == 0)
        return 1;
    if (strcmp(text, "H") == 0)
        return 2;
    if (strcmp(text, "G") == 0)
        return 3;
    if (strcmp(text, "F") == 0)
        return 4;
    if (strcmp(text, "E") == 0)
        return 5;
    if (strcmp(text, "D") == 0)
        return 6;
    if (strcmp(text, "C") == 0)
        return 7;
    if (strcmp(text, "B") == 0)
        return 8;
    if (strcmp(text, "A") == 0)
        return 9;
    return 0;
}
