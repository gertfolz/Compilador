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
        node->symbol->type = SYMBOL_VAR;
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
        node->symbol->type = SYMBOL_VAR;
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
            node->symbol->type = SYMBOL_VAR;
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

void check_undeclared()
{
    SemanticErrors += hashCheckUndeclared();
}

void checkOperands(AST *node)
{
    int i;

    if (node == NULL)
        return;

    for (i = 0; i < MAX_SONS; i++)
        checkOperands(node->son[i]);

    if (node == NULL)
        return;

    switch (node->type)
    {
    case AST_SYMBOL:
        switch (node->symbol->type)
        {
        case SYMBOL_VAR:
            if (node->symbol->datatype == DATATYPE_INT)
            {
                node->datatype = AST_INT;
            }
            else if (node->symbol->datatype == DATATYPE_CHAR)
            {
                node->datatype = AST_CHAR;
            }
            else if (node->symbol->datatype == DATATYPE_FLOAT)
            {
                node->datatype = AST_FLOAT;
            }
        default:
            break;
        }
        break;

    case AST_DECV:
        if (checkVector(node))
            fprintf(stderr, "SEMANTIC ERROR: Invalid range in vector declaration %s. %s %s\n", node->symbol->text, node->son[1]->symbol->text, node->son[2]->symbol->text);
        SemanticErrors++;
        break;
    case AST_FUNCAUX:
        if (checkReturn(node, node))
        {
            fprintf(stderr, "SEMANTIC ERROR: Invalid return type in function %s.\n", node->symbol->text);
            SemanticErrors++;
        }
        break;
    case AST_ATTR:
        if (node != 0)
            if ((node->son[0]->type != AST_VECTOR))
            
            {
                if ((node->symbol->type != SYMBOL_VAR))
                {
                    fprintf(stderr, "SEMANTIC ERROR: Symbol %s is not a scalar.\n", node->symbol->text);
                    SemanticErrors++;
                }
                
            }
         
        // if ((node != 0) && (node->son[0] != 0))
        //     if ((node->symbol->type != SYMBOL_VET) && (node->son[1] != 0))
        //     {
        //         fprintf(stderr, "SEMANTIC ERROR: Symbol %s must be a vector.\n", node->son[0]->symbol->text);
        //         SemanticErrors++;
        //     }
        break;
    case AST_ADD:
    case AST_SUB:
    case AST_MULT:
    case AST_DIV:
        if (checkArithmetics(node))
        {
            fprintf(stderr, "SEMANTIC ERROR: Invalid Operands.\n");
            SemanticErrors++;
        }
        break;
    case AST_VECTOR:
    if (node != 0)
        if (node->symbol->type != SYMBOL_VET)
        {
            fprintf(stderr, "SEMANTIC ERROR: Identifier %s is not a vector.\n", node->symbol->text);
            SemanticErrors++;
        }
    if (node->son[0] != 0)
        if (node->son[0]->datatype != AST_INT)
        {
            fprintf(stderr, "SEMANTIC ERROR: Invalid index type in vector %s, must be integer.\n", node->symbol->text);
            SemanticErrors++;
        }
        break;
    case AST_FUNC:
        break;
    }
}

int checkReturn(AST *node, AST *nodeFunc)
{
    int returnType;
    int i, found = 0;

    if (node->type == AST_RETURN)
    {
        returnType = node->son[0]->symbol->datatype;
        if (nodeFunc->symbol->datatype != returnType)
            return 1;
        else
            return 0;
    }
    else
    {
        for (i = 0; i < MAX_SONS; i++)
        {
            if (node->son[i] != NULL)
            {
                found = checkReturn(node->son[i], nodeFunc);
                if (found != 2)
                    return found;
            }
        }
    }
    return 2;
}

void checkSemantic(AST *node1, AST *node2)
{
    fprintf(stderr, "\n");
    checkAndSetDec(node1);
    checkAndSetDec(node2);
    check_undeclared();
    checkOperands(node1);
    checkOperands(node2);
    fprintf(stderr, "\n");
}

int checkVector(AST *node)
{
    int size = 0, realSize = 0;
    AST *nodeAux;

    if (node->son[3] == 0)
        return 0;
    else
    {
        if (atoi(node->son[1]->symbol->text) >= atoi(node->son[2]->symbol->text))
        {
            size = atoi(node->son[1]->symbol->text) - atoi(node->son[2]->symbol->text);
        }
        if (atoi(node->son[1]->symbol->text) < atoi(node->son[2]->symbol->text))
        {
            size = atoi(node->son[2]->symbol->text) - atoi(node->son[1]->symbol->text);
        }
        nodeAux = node->son[3];
        while (nodeAux->son[0] != 0)
        {

            realSize++;
            if (nodeAux->son[1] != 0)
                nodeAux = nodeAux->son[1];
            else if (realSize != size)
                return 1;
            else
                return 0;
        }
        if (realSize != size)
            return 1;
        else
            return 0;
    }

    return 1;
}

int checkArithmetics(AST *node)
{
    int i = 0, found = 0;

    if (node != 0)
    {
        if (node->son[0] != 0)
        {
            fprintf(stderr, "inferno1\n");
            if (!((node->son[1]->type == AST_ADD || node->son[1]->type == AST_SUB || node->son[1]->type == AST_DIV || node->son[1]->type == AST_MULT || node->son[1]->type == AST_SYMBOL)))
            {
                found = 1;
                return found;
            }

            else
                found = 0;
            fprintf(stderr, "%d\n", found);
        }
        if (node->son[1] != 0)
        {
            fprintf(stderr, "inferno2 %d\n", node->son[1]->type);
            if (!((node->son[1]->type == AST_ADD || node->son[1]->type == AST_SUB || node->son[1]->type == AST_DIV || node->son[1]->type == AST_MULT || node->son[1]->type == AST_SYMBOL)))
            {
                found = 1;
                fprintf(stderr, "%d\n", found);
                return found;
            }
            else
                found = 0;
            fprintf(stderr, "%d\n", found);
        }
        return found;
    }
    for (i = 0; i < MAX_SONS; i++)
    {
        if (node->son[i] != 0)
            found = checkArithmetics(node->son[i]);
        if (found == 1)
            return found;
    }
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

 int getSemanticErrors()
    {
        return SemanticErrors;
    }