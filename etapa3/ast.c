#include "ast.h"

extern FILE *out;

AST *astCreate(int type, HASH_NODE *symbol, AST* s0, AST* s1, AST* s2, AST* s3)
{
    AST *newnode;
    newnode = (AST*) calloc(1, sizeof(AST));
    newnode->type = type;
    newnode->symbol = symbol;
    newnode->son[0] = s0;
    newnode->son[1] = s1;
    newnode->son[2] = s2;
    newnode->son[3] = s3;
    return newnode;
}

void astPrint(AST *node, int level)
{
    int i;
    if(node == 0)
        return;
    for(i=0;i<level; i++)
        fprintf(stderr, "   ");
    
    fprintf(stderr, "ast(");
    switch (node->type)
    {
    case AST_SYMBOL: fprintf(stderr, "AST_SYMBOL"); break;
    case AST_ADD: fprintf(stderr, "AST_ADD"); break;
    case AST_SUB: fprintf(stderr, "AST_SUB"); break;
    case AST_MULT: fprintf(stderr, "AST_MULT"); break;
    case AST_DIV: fprintf(stderr, "AST_DIV"); break;
    case AST_LT: fprintf(stderr, "AST_LT"); break;
    case AST_GT: fprintf(stderr, "AST_GT"); break;
    case AST_OR: fprintf(stderr, "AST_OR"); break;
    case AST_AND: fprintf(stderr, "AST_AND"); break;
    case AST_NOT: fprintf(stderr, "AST_NOT"); break;
    case AST_LE: fprintf(stderr, "AST_LE"); break;
    case AST_GE: fprintf(stderr, "AST_GE"); break;
    case AST_EQ: fprintf(stderr, "AST_EQ"); break;
    case AST_DIF: fprintf(stderr, "AST_DIF"); break;
    case AST_ATTR: fprintf(stderr, "AST_ATTR"); break;
    case AST_RETURN: fprintf(stderr, "AST_RETURN"); break;
    case AST_PRINT: fprintf(stderr, "AST_PRINT"); break;
    case AST_IF: fprintf(stderr, "AST_IF"); break;
    case AST_IFELSE: fprintf(stderr, "AST_IFELSE"); break;
    case AST_UNTIL: fprintf(stderr, "AST_UNTIL"); break;
    case AST_COMEFROM: fprintf(stderr, "AST_COMEFROM"); break;
    case AST_CHECKPOINT: fprintf(stderr, "AST_CHECKPOINT"); break;
    case AST_LCMD: fprintf(stderr, "AST_LCMD"); break;
    case AST_DATA: fprintf(stderr, "AST_DATA"); break;
    case AST_DEC: fprintf(stderr, "AST_DEC"); break;
    case AST_INT: fprintf(stderr, "AST_INT"); break;
    case AST_CHAR: fprintf(stderr, "AST_CHAR"); break;
    case AST_FLOAT: fprintf(stderr, "AST_FLOAT"); break;
    case AST_DECV: fprintf(stderr, "AST_DECV"); break;
    case AST_VECINIT: fprintf(stderr, "AST_VECINIT"); break;
    case AST_VECS: fprintf(stderr, "AST_VECS"); break;
    case AST_DECDATA: fprintf(stderr, "AST_DECDATA"); break;
    case AST_FUNCLIST: fprintf(stderr, "AST_FUNCLIST"); break;
    case AST_FUNC: fprintf(stderr, "AST_FUNC"); break;
    case AST_ARGS: fprintf(stderr, "AST_ARGS"); break;
    case AST_ARGS2: fprintf(stderr, "AST_ARGS2"); break;
    case AST_BLOCK: fprintf(stderr, "AST_BLOCK"); break;
    case AST_VECTOR: fprintf(stderr, "AST_VECTOR"); break;
    case AST_ARGSRABO: fprintf(stderr, "AST_ARGSRABO"); break;
    case AST_ARGS2RABO: fprintf(stderr, "AST_ARGS2RABO"); break;
    case AST_READ: fprintf(stderr, "AST_READ"); break;
    case AST_FLUX: fprintf(stderr, "AST_FLUX"); break;
    case AST_FUNCAUX: fprintf(stderr, "AST_FUNCAUX"); break;
    case AST_PRINTPAR: fprintf(stderr, "AST_PRINTPAR"); break;
    case AST_PRINTPARTAIL: fprintf(stderr, "AST_PRINTPARTAIL"); break;


   
    default:
        break;
    }
    if(node->symbol!=0)
        fprintf(stderr, ",%s \n", node->symbol->text);
    else
        fprintf(stderr, ",0\n");
    for(i=0; i<MAX_SONS; i++)
        astPrint(node->son[i], level + 1);
}

void astDecomp(AST *node)
{
    int i;
    if(node == 0)
        return;

    switch (node->type)
    {
    case AST_SYMBOL: 
        fprintf(out, "%s", node->symbol->text); 
        break;
    case AST_ADD:  
        astDecomp(node->son[0]); fprintf(out," + "); astDecomp(node->son[1]);
        break;
    case AST_SUB:  
        astDecomp(node->son[0]); fprintf(out," - "); astDecomp(node->son[1]);
        break;
    case AST_MULT:  
        astDecomp(node->son[0]); fprintf(out," * "); astDecomp(node->son[1]);
        break;
    case AST_DIV:  
        astDecomp(node->son[0]); fprintf(out," / "); astDecomp(node->son[1]);
        break;
    case AST_LT:  
        astDecomp(node->son[0]); fprintf(out," < "); astDecomp(node->son[1]);
        break;
    case AST_GT:  
        astDecomp(node->son[0]); fprintf(out," > "); astDecomp(node->son[1]);
        break;
    case AST_OR:  
        astDecomp(node->son[0]); fprintf(out," | "); astDecomp(node->son[1]);
        break;
    case AST_AND:  
        astDecomp(node->son[0]); fprintf(out," & "); astDecomp(node->son[1]);
        break;
    case AST_NOT:  
        fprintf(out,"!"); astDecomp(node->son[0]);
        break;
    case AST_LE:  
        astDecomp(node->son[0]); fprintf(out," <= "); astDecomp(node->son[1]);
        break;
    case AST_GE:  
        astDecomp(node->son[0]); fprintf(out," >= "); astDecomp(node->son[1]);
        break;
    case AST_EQ:  
        astDecomp(node->son[0]); fprintf(out," == "); astDecomp(node->son[1]);
        break;
    case AST_DIF:  
        astDecomp(node->son[0]); fprintf(out," != "); astDecomp(node->son[1]);
        break;
    case AST_ATTR:
        if(node->symbol)
        {
            fprintf(out, "%s", node->symbol->text); fprintf(out," = "); astDecomp(node->son[0]);
        }
        else
        {
             astDecomp(node->son[0]); fprintf(out," = "); astDecomp(node->son[1]);
        }
        break;
    case AST_RETURN: 
        fprintf(out,"return ("); astDecomp(node->son[0]); fprintf(out,")");
        break;
    case AST_PRINT: 
        fprintf(out,"print "); astDecomp(node->son[0]);
        break;
    case AST_PRINTPAR: 
        if(node->symbol!=0)
        {
            fprintf(out, "%s", node->symbol->text);
            if(node->son[0])
            {
                astDecomp(node->son[0]); 
            }  
        }
         else
        {
             fprintf(out,"(");astDecomp(node->son[0]); fprintf(out,")");astDecomp(node->son[1]); 
        }  
        break;
    case AST_PRINTPARTAIL: 
         if(node->symbol!=0)
         {
              fprintf(out,", ");fprintf(out, "%s", node->symbol->text);
                 if(node->son[0])
                     astDecomp(node->son[0]); 
         }
         else
         {
             fprintf(out,", ("); astDecomp(node->son[0]);  fprintf(out,")");
             if(node->son[1])
                     astDecomp(node->son[1]); 
         }  
         break;
    case AST_IF:
        fprintf(out,"if (");astDecomp(node->son[0]); fprintf(out,") "); astDecomp(node->son[1]);
        break;
    case AST_IFELSE:
        fprintf(out,"if (");astDecomp(node->son[0]); fprintf(out,") "); astDecomp(node->son[1]); fprintf(out,"else \n"); astDecomp(node->son[2]);
        break;
    case AST_UNTIL:
        fprintf(out,"until (");astDecomp(node->son[0]); fprintf(out,") "); astDecomp(node->son[1]);
        break;
    case AST_COMEFROM: 
        fprintf(out,"comefrom: ");astDecomp(node->son[0]);
        break;
    case AST_CHECKPOINT: 
        if(node->symbol!=0)
        {
             fprintf(out, "%s", node->symbol->text);
        }
        break;
    case AST_LCMD:   
        astDecomp(node->son[0]); fprintf(out,";\n");
            if(node->son[1])
            {
                astDecomp(node->son[1]);
            }
        break;
    case AST_DATA: 
        fprintf(out,"data \n{\n"); astDecomp(node->son[0]);  fprintf(out,"}\n");
        break;
    case AST_DEC: 
        astDecomp(node->son[0]); fprintf(out,": "); fprintf(out, "%s", node->symbol->text);  fprintf(out," = ");  astDecomp(node->son[1]); fprintf(out,";\n");
        break;
    case AST_INT: 
        fprintf(out, "int");
        break;
    case AST_CHAR:  
        fprintf(out, "char");
        break;
    case AST_FLOAT:  
        fprintf(out, "float");
        break;
    case AST_DECV:  
        if(!(node->son[3]))
        {
                astDecomp(node->son[0]); fprintf(out,"["); astDecomp(node->son[1]); fprintf(out,"..");astDecomp(node->son[2]);fprintf(out,"]: ");fprintf(out, "%s", node->symbol->text); fprintf(out,";\n");
        }else
        {
                astDecomp(node->son[0]); fprintf(out,"["); astDecomp(node->son[1]); fprintf(out,"..");astDecomp(node->son[2]);fprintf(out,"]: ");fprintf(out, "%s", node->symbol->text);  fprintf(out," = ");  astDecomp(node->son[3]); fprintf(out,";\n");
        }
        break;
    case AST_VECINIT: 
        fprintf(out," "); astDecomp(node->son[0]); 
        if(node->son[1])
             astDecomp(node->son[1]); 
        break;
    case AST_VECS: 
        fprintf(out, "%s", node->symbol->text);
        break;
    case AST_DECDATA: 
        astDecomp(node->son[0]);
        if(node->son[1]);
             astDecomp(node->son[1]);
        break;
    case AST_FUNCLIST:   
        astDecomp(node->son[0]);
        if(node->son[1])
            astDecomp(node->son[1]); 
        break;
    case AST_FUNCAUX:
        astDecomp(node->son[0]); fprintf(out,": "); fprintf(out,"%s", node->symbol->text); fprintf(out,"("); 
        if(node->son[1])
            astDecomp(node->son[1]); 
        fprintf(out,")"); astDecomp(node->son[2]); fprintf(out,"\n\n");
        break;
    case AST_FUNC: 
        fprintf(out,"%s", node->symbol->text);fprintf(out,"("); astDecomp(node->son[0]);fprintf(out,")");
        break;
    case AST_ARGS:
        astDecomp(node->son[0]);fprintf(out,": "); fprintf(out,"%s", node->symbol->text);
        if(node->son[1])
            astDecomp(node->son[1]);
        break;
    case AST_ARGS2: 
        if(node->son[0])
            astDecomp(node->son[0]);
        if(node->son[1])
            astDecomp(node->son[1]);
        break;
    case AST_BLOCK: 
        fprintf(out,"\n{\n"); astDecomp(node->son[0]); fprintf(out,"}");
        break;
    case AST_VECTOR: 
        fprintf(out, "%s", node->symbol->text); fprintf(out,"["); astDecomp(node->son[0]);fprintf(out,"]");
        break;
    case AST_ARGSRABO:
        fprintf(out,", ");astDecomp(node->son[0]);fprintf(out,": "); fprintf(out,"%s", node->symbol->text);
        if(node->son[1])
            astDecomp(node->son[1]);
        break;
    case AST_ARGS2RABO: 
        fprintf(out,", ");astDecomp(node->son[0]);
        if(node->son[1])
            astDecomp(node->son[1]);
         break;
    case AST_READ: 
        fprintf(out,"read");
        break;
    case AST_FLUX: 
        astDecomp(node->son[0]);
        break;
    }
}