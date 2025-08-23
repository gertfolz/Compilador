#include <stdio.h>
#include <stdlib.h>
#include "hash.h"

extern FILE *yyin;
FILE *out;
extern int yyparse();
void initMe();
int getLineNumber();
int isRunning();
extern int yylex();
extern char* yytext;


int main (void) {
    FILE *test = fopen("test.txt", "r");
    yyin = test;

    initMe();

    int token;
    while (isRunning())
    {
        token = yylex();
        if (isRunning() != 0)
        {
            switch (token)
            {
                case 256: printf("KW_CHAR\n");                   break;
                case 257: printf("KW_INT\n");                    break;
                case 258: printf("KW_FLOAT\n");                  break;
                case 259: printf("KW_BOOL\n");                   break;

                case 261: printf("KW_IF\n");                     break;
                case 263: printf("KW_ELSE\n");                   break;
                case 264: printf("KW_WHILE\n");                  break;
                case 266: printf("KW_READ\n");                   break;
                case 267: printf("KW_PRINT\n");                  break;
                case 268: printf("KW_RETURN\n");                 break;

                case 270: printf("OPERATOR_LE\n");               break;
                case 271: printf("OPERATOR_GE\n");               break;
                case 272: printf("OPERATOR_EQ\n");               break;
                case 273: printf("OPERATOR_DIF\n");              break;
                case 274: printf("OPERATOR_RANGE\n");            break;

                case 280: printf("TK_IDENTIFIER: %s\n", yytext); break;

                case 281: printf("LIT_INTEGER: %s\n", yytext);   break;
                case 285: printf("LIT_CHAR: %s\n", yytext);      break;
                case 286: printf("LIT_STRING: %s\n", yytext);    break;

                case 290: printf("TOKEN_ERROR: %s\n", yytext);   break;
                default:  printf("SPECIAL_CHAR: %s\n", yytext);
            }
        }
    }
    // fclose(test);
    printf("\nline_count: %d\n", getLineNumber());

    /* impressão da tabela de símbolos */
    hashPrint();
    return 0;
}