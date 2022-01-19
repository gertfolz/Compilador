#include "generateASM.h"

void addData(FILE* fout, AST* node)
{
    static int LC = 2;

    if(!node) return;
	if(node->type == AST_DEC) {   
    	fprintf(fout, "\t.globl	_%s\n"
                     "\t.data\n"
                     "\t.type	_%s, @object\n"
                     "\t.size	_%s, 4\n"
                     "_%s:\n", node->symbol->text, node->symbol->text, node->symbol->text, node->symbol->text);
        char *conversion = convertIntegers(node->son[1]->symbol->text);
		if(node->son[0]->type == AST_FLOAT) {
			fprintf(fout, "\t.float	%s\n", conversion);
		}
		else {
			fprintf(fout, "\t.long	%s\n", conversion);
		}
	}
    else if (node->type == AST_ARGS){  // Params are global variables!
    	fprintf(fout, "\t.globl	_%s\n"
                     "\t.data\n"
                     "\t.type	_%s, @object\n"
                     "\t.size	_%s, 4\n"
                     "_%s:\n", node->symbol->text, node->symbol->text, node->symbol->text, node->symbol->text);
        if(node->son[0]->type == AST_FLOAT) {
			fprintf(fout, "\t.float	0\n");
		}
		else {
			fprintf(fout, "\t.long	0\n");
		}
    }
	else if(node->type == AST_DECV){
        char *conversion = calloc(strlen(node->son[1]->symbol->text)+1, sizeof(char));
        johannNumberConversion(node->son[1]->symbol->text, conversion);
        fprintf(fout, "\t.globl	_%s\n"
                     "\t.data\n"
                     "\t.type	_%s, @object\n"
                     "\t.size	_%s, %d\n"
                     "_%s:\n", node->symbol->text, node->symbol->text, node->symbol->text, 4*atoi(conversion), node->symbol->text);
        free(conversion);
        for(AST* aux = node->son[2]; aux; aux = aux->son[1]) {
            char *conversion = calloc(strlen(aux->son[0]->symbol->text)+1, sizeof(char));
            johannNumberConversion(aux->son[0]->symbol->text, conversion);
            if(node->son[0]->type == AST_FLOAT){
                fprintf(fout, "\t.float	%s\n", conversion);
            }
            else{
                fprintf(fout, "\t.long	%s\n", conversion);
            }
            free(conversion);
        }
	}
    else if(node->type == AST_SYMBOL){
        if(node->symbol->type == SYMBOL_STRING){
            fprintf(fout, "\t.section\t .rodata\n.LC%d:\n\t.string %s \n", LC, node->symbol->text);
            LC++;          
        }
    }
    
	for(int i = 0; i < MAX_SONS; i++){
		addData(fout, node->son[i]);
	}
}

void generateAsm(TAC* first, AST* ast)
{
    int LC = 2;
    int BL = 0;

    TAC *tac;
    FILE *fout;
    fout = fopen ("out.s", "w");

    //INIT

    fprintf(fout, "## FIXED INIT\n"
	            "\t.section	.rodata\n\n");

    //HASH TABLE
    printAsm(fout);
    
    fprintf(fout, "\n# STRING\n"
                    ".printIntStr:\n"
                    "\t.string \"%%d\\n\" \n"
                    ".printString:\n"
                    "\t.string \"%%s\\n\" \n"
                    "\t.text\n\n");
    addData(fout, ast);
    //EACH TAC
    for (tac = first; tac; tac = tac->next) 
    {
        switch(tac->type)
        {
            case TAC_BEGINFUNCTION:
                fprintf(fout,   "# TAC BEGIN_FUNCTION\n"
                                "\t.globl	%s\n"
                                "\t.type	%s, @function\n" 
                                "%s:\n"
                                "\tpushq	%%rbp\n"
                                "\tmovq	%%rsp, %%rbp\n\n",
                                tac->res->text,tac->res->text,tac->res->text);
                break;
            case TAC_ENDFUNCTION:
                fprintf(fout, "# TAC END_FUNCTION\n"
                                "\tpopq	%%rbp\n"
                                "\tret\n");
                break;
            case TAC_PRINT: 
                fprintf(fout, "# PRINT\n"
                                "\tmovl	_%s(%%rip), %%eax\n"
                                "\tmovl	%%eax, %%esi\n"
                                "\tleaq	.printIntStr(%%rip), %%rdi\n"
                                "\tmovl	$0, %%eax\n"
                                "\tcall	printf@PLT\n\n",
                                tac->res?tac->res->text:"");
                break;
            
            case TAC_ADD:
              fprintf(fout, "\tmovl _%s(%%rip), %%eax\n"
                           "\tmovl _%s(%%rip), %%edx\n"
                           "\taddl %%eax, %%edx\n"
                           "\tmovl %%edx, _%s(%%rip)\n", tac->op1->text, tac->op2->text, tac->res->text);
              break;

            case TAC_SUB:
               fprintf(fout, "\tmovl _%s(%%rip), %%eax\n"
                            "\tmovl _%s(%%rip), %%edx\n"
                            "\tsubl %%eax, %%edx\n"
                            "\tmovl %%edx, _%s(%%rip)\n", tac->op2->text, tac->op1->text, tac->res->text);
               break;

            case TAC_MULT:
               fprintf(fout, "\tmovl _%s(%%rip), %%eax\n"
                            "\tmovl _%s(%%rip), %%edx\n"
                            "\timull %%eax, %%edx\n"
                            "\tmovl %%edx, _%s(%%rip)\n", tac->op1->text, tac->op2->text, tac->res->text);
               break;

            case TAC_DIV:
              fprintf(fout, "\tmovl _%s(%%rip), %%eax\n"
                           "\tmovl _%s(%%rip), %%ecx\n"
                           "\tcltd\n"
                           "\tidivl %%ecx\n"
                           "\tmovl %%eax, _%s(%%rip)\n", tac->op1->text, tac->op2->text, tac->res->text);
              break;

            case TAC_GT:
              fprintf(fout, "\tmovl _%s(%%rip), %%eax\n"
                      "\tmovl _%s(%%rip), %%edx\n"
                      "\tcmpl %%eax, %%edx\n"
                      "\tjg .BL%d\n"
                      "\tmovl $0, %%eax\n"
                      "\tjmp .BL%d\n"
                      ".BL%d:\n"
                      "\tmovl $1, %%eax\n"
                      ".BL%d:\n"
                      "\tmovl %%eax, _%s(%%rip)\n", tac->op2->text, tac->op1->text, BL, BL+1, BL, BL+1, tac->res->text);
              BL+=2;
              break;

            case TAC_LT:
              fprintf(fout, "\tmovl _%s(%%rip), %%eax\n"
                      "\tmovl _%s(%%rip), %%edx\n"
                      "\tcmpl %%eax, %%edx\n"
                      "\tjl .BL%d\n"
                      "\tmovl $0, %%eax\n"
                      "\tjmp .BL%d\n"
                      ".BL%d:\n"
                      "\tmovl $1, %%eax\n"
                      ".BL%d:\n"
                      "\tmovl %%eax, _%s(%%rip)\n", tac->op2->text, tac->op1->text, BL, BL+1, BL, BL+1, tac->res->text);
              BL+=2;
              break;

            case TAC_GE:
              fprintf(fout, "\tmovl _%s(%%rip), %%eax\n"
                           "\tmovl _%s(%%rip), %%edx\n"
                           "\tcmpl %%eax, %%edx\n"
                           "\tjge .BL%d\n"
                           "\tmovl $0, %%eax\n"
                           "\tjmp .BL%d\n"
                           ".BL%d:\n"
                           "\tmovl $1, %%eax\n"
                           ".BL%d:\n"
                           "\tmovl %%eax, _%s(%%rip)\n", tac->op2->text, tac->op1->text, BL, BL+1, BL, BL+1, tac->res->text);
              BL+=2;
              break;

            case TAC_LE:
              fprintf(fout, "\tmovl _%s(%%rip), %%eax\n"
                           "\tmovl _%s(%%rip), %%edx\n"
                           "\tcmpl %%eax, %%edx\n"
                           "\tjle .BL%d\n"
                           "\tmovl $0, %%eax\n"
                           "\tjmp .BL%d\n"
                           ".BL%d:\n"
                           "\tmovl $1, %%eax\n"
                           ".BL%d:\n"
                           "\tmovl %%eax, _%s(%%rip)\n", tac->op2->text, tac->op1->text, BL, BL+1, BL, BL+1, tac->res->text);
                BL+=2;
                break;

            case TAC_EQ:
              fprintf(fout, "\tmovl _%s(%%rip), %%eax\n"
                      "\tmovl _%s(%%rip), %%edx\n"
                      "\tcmpl %%eax, %%edx\n"
                      "\tje .BL%d\n"
                      "\tmovl $0, %%eax\n"
                      "\tjmp .BL%d\n"
                      ".BL%d:\n"
                      "\tmovl $1, %%eax\n"
                      ".BL%d:\n"
                      "\tmovl %%eax, _%s(%%rip)\n", tac->op2->text, tac->op1->text, BL, BL+1, BL, BL+1, tac->res->text);
              BL+=2;
              break;

            case TAC_DIF:
              fprintf(fout, "\tmovl _%s(%%rip), %%eax\n"
                      "\tmovl _%s(%%rip), %%edx\n"
                      "\tcmpl %%eax, %%edx\n"
                      "\tjne .BL%d\n"
                      "\tmovl $0, %%eax\n"
                      "\tjmp .BL%d\n"
                      ".BL%d:\n"
                      "\tmovl $1, %%eax\n"
                      ".BL%d:\n"
                      "\tmovl %%eax, _%s(%%rip)\n", tac->op2->text, tac->op1->text, BL, BL+1, BL, BL+1, tac->res->text);
              BL+=2;
              break;

            case TAC_AND:
                fprintf(fout, "\tmovl _%s(%%rip), %%eax\n"
                             "\tmovl _%s(%%rip), %%edx\n"
                             "\tandl %%eax, %%edx\n"
                             "\tjz .BL%d\n"
                             "\tmovl $1, %%eax\n"
                             "\tjmp .BL%d\n"
                             ".BL%d:\n"
                             "\tmovl $0, %%eax\n"
                             ".BL%d:\n"
                             "\tmovl %%eax, _%s(%%rip)\n", tac->op1->text, tac->op2->text, BL, BL+1, BL, BL+1, tac->res->text);
                BL+=2;
                break;

            case TAC_OR:
              fprintf(fout, "\tmovl _%s(%%rip), %%eax\n"
                      "\tmovl _%s(%%rip), %%edx\n"
                      "\torl %%eax, %%edx\n"
                      "\tjz .BL%d\n"
                      "\tmovl $1, %%eax\n"
                      "\tjmp .BL%d\n"
                      ".BL%d:\n"
                      "\tmovl $0, %%eax\n"
                      ".BL%d:\n"
                      "\tmovl %%eax, _%s(%%rip)\n", tac->op1->text, tac->op2->text, BL, BL+1, BL, BL+1, tac->res->text);
              BL+=2;
              break;

            case TAC_NOT:
              fprintf(fout, "\tmovl _%s(%%rip), %%eax\n"
                      "\tmovl $1, %%edx\n"
                      "\tandl %%eax, %%edx\n"
                      "\tjz .BL%d\n"
                      "\tmovl $0, %%eax\n"
                      "\tjmp .BL%d\n"
                      ".BL%d:\n"
                      "\tmovl $1, %%eax\n"
                      ".BL%d:\n"
                      "\tmovl %%eax, _%s(%%rip)\n", tac->op1->text, BL, BL+1, BL, BL+1, tac->res->text);
              BL+=2;
              break;
            case TAC_ATTR:
              fprintf(fout, "\tmovl _%s(%%rip), %%eax\n"
                           "\tmovl %%eax, _%s(%%rip)\n", tac->op1->text, tac->res->text);
              break;

            default: break;
        }
        
   
    } 
    
    fclose(fout);
}

char convertIntegers(char *text)
{
    if(strcmp (text, "O") == 0)
        return '0';
    if(strcmp (text, "I") == 0)
        return '1';
    if(strcmp (text, "H") == 0)
        return '2';
    if(strcmp (text, "G") == 0)
        return '3';
    if(strcmp (text, "F") == 0)
        return '4';
    if(strcmp (text, "E") == 0)
        return '5';
    if(strcmp (text, "D") == 0)
        return '6';
    if(strcmp (text, "C") == 0)
        return '7';
    if(strcmp (text, "B") == 0)
        return '8';
    if(strcmp (text, "A") == 0)
        return '9';
    return 0;
}