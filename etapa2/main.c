 /* Gert Willem Folz - 00288550 
 * Trabalho 2 da disciplina de compiladores */
 
 int main (int argc, char **argv) 
{
    int token;

    if(argc<2)
    {
        printf("Call ./etapa1 in.txt");
        exit(1);
    }

    if(!(yyin = fopen(argv[1], "r")))
    {
        fprintf(stderr,"error opening file %s\n", argv[1]);
        exit(2);
    }

    initMe();

    yyparse();
    printf("Fim do arquivo, %d linhas \n", getLineNumber());
    hashPrint();
    fprintf(stderr, "deu bom \n"); 
    exit(0);
}
