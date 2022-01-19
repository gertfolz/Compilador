 /* Gert Willem Folz - 00288550 
 * Trabalho 2 da disciplina de compiladores */
 #include <stdio.h>
 
 #include "hash.h"

 extern FILE *yyin;
 FILE *out;
 int yyparse();
 void initMe();
 int getLineNumber();
 


 int main (int argc, char **argv) 
{
    int token;

    if(argc<3)
    {
        printf("Call ./etapa1 in.txt out.txt");
        exit(1);
    }

    if(!(yyin = fopen(argv[1], "r")))
    {
        fprintf(stderr,"error opening file %s\n", argv[1]);
        exit(2);
    }

      if (!(out = fopen(argv[2],"w"))) 
    {
        fprintf(stderr,"error opening file %s\n", argv[1]);
        exit(2);
    }

    initMe();

    yyparse();
    printf("Fim do arquivo, %d linhas \n", getLineNumber());
    hashPrint();

    fclose(out);

    fprintf(stderr, "deu bom \n"); 
    exit(0);
}
