#include "semantic.h"

int SemanticErrors = 0;

void checkAndSetDec(AST *node)
{
    int i;
    AST* nodePai = node;
    if (node == 0)
        return;

    switch (node->type)
    {
    case AST_DEC:
        if (node->symbol)
            if (node->symbol->type != SYMBOL_IDENTIFIER)
            {
                if (node->symbol->text)
                {
                    fprintf(stderr, "Semantic ERROR: variable %s already declared\n", node->symbol->text);
                    SemanticErrors++;
                }
            }
        node->symbol->type = SYMBOL_VARIABLE;
        setTypes(node);
        break;
    case AST_DECV:
        if (node->symbol)
            if (node->symbol->type != SYMBOL_IDENTIFIER)
            {
                if (node->symbol->text)
                {
                    fprintf(stderr, "Semantic ERROR: variable %s already declared\n", node->symbol->text);
                    SemanticErrors++;
                }
            }
        node->symbol->type = SYMBOL_VET;
        setTypes(node);
        break;
    case AST_FUNCAUX:
        if (node->symbol)
            if (node->symbol->type != SYMBOL_IDENTIFIER)
            {
                if (node->symbol->text)
                {
                    fprintf(stderr, "Semantic ERROR: function %s already declared\n", node->symbol->text);
                    SemanticErrors++;
                }
            }
        node->symbol->type = SYMBOL_FUNC;
        setTypes(node);
        break;
    case AST_ARGS:
        if (node->symbol)
            if (node->symbol->type != SYMBOL_IDENTIFIER)
            {
                fprintf(stderr, "SEMANTIC ERROR. Parameter %s already declared\n", node->symbol->text);
                SemanticErrors++;
            }
        node->symbol->type = SYMBOL_VARIABLE;
        setTypes(node);
        if (node->son[1] != NULL)
            node = node->son[1];
        else
            break;
        while (node->symbol != NULL)
        {
            if (node->symbol->type != SYMBOL_IDENTIFIER)
            {
                fprintf(stderr, "SEMANTIC ERROR. Parameter %s already declared\n", node->symbol->text);
                SemanticErrors++;
            }
            node->symbol->type = SYMBOL_VARIABLE;
            setTypes(node);
            if (node->son[1] != NULL)
            {
                node = node->son[1];
            }
            else
                break;
        }
        break;
    }
    for (i = 0; i < MAX_SONS; i++)
        checkAndSetDec(node->son[i]);
}


void setTypes(AST *node)
{
    if (node->son[0] != NULL)
    {
        switch (node->son[0]->type)
        {
        case AST_INT:
            node->son[0]->datatype = AST_INT;
            node->symbol->datatype = DATATYPE_INT;
            node->datatype = AST_INT;
            break;
        case AST_FLOAT:
            node->son[0]->datatype = AST_FLOAT;
            node->symbol->datatype = DATATYPE_FLOAT;
            node->datatype = AST_FLOAT;
            break;
        case AST_CHAR:
            node->son[0]->datatype = AST_CHAR;
            node->symbol->datatype = DATATYPE_CHAR;
            node->datatype = AST_CHAR;
            break;
        default:
            break;
        }
    }

}
