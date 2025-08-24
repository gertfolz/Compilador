
#include "tacs.h"

TAC *tacCreate(int type, HASH_NODE *res, HASH_NODE *op1, HASH_NODE *op2)
{
    TAC *newTac = 0;
    newTac = (TAC *)calloc(1, sizeof(TAC));
    newTac->type = type;
    newTac->res = res;
    newTac->op1 = op1;
    newTac->op2 = op2;
    newTac->prev = 0;
    newTac->next = 0;

    return newTac;
}

void tacPrint(TAC *tac)
{
    if (!tac)
        return;
    if (tac->type == TAC_SYMBOL)
        return;
    fprintf(stderr, "TAC(");
    switch (tac->type)
    {
    case TAC_SYMBOL:
        fprintf(stderr, "TAC_SYMBOL");
        break;
    case TAC_ADD:
        fprintf(stderr, "TAC_ADD");
        break;
    case TAC_SUB:
        fprintf(stderr, "TAC_SUB");
        break;
    case TAC_ATTR:
        fprintf(stderr, "TAC_ATTR");
        break;
    case TAC_JFALSE:
        fprintf(stderr, "TAC_JFALSE");
        break;
    case TAC_LABEL:
        fprintf(stderr, "TAC_LABEL");
        break;
    case TAC_MULT:
        fprintf(stderr, "TAC_MULT");
        break;
    case TAC_DIV:
        fprintf(stderr, "TAC_DIV");
        break;
    case TAC_JUMP:
        fprintf(stderr, "TAC_JUMP");
        break;
    case TAC_LT:
        fprintf(stderr, "TAC_LESS");
        break;
    case TAC_GT:
        fprintf(stderr, "TAC_GREAT");
        break;
    case TAC_LE:
        fprintf(stderr, "TAC_LE");
        break;
    case TAC_GE:
        fprintf(stderr, "TAC_GE");
        break;
    case TAC_EQ:
        fprintf(stderr, "TAC_EQ");
        break;
    case TAC_DIF:
        fprintf(stderr, "TAC_DIF");
        break;
    case TAC_AND:
        fprintf(stderr, "TAC_AND");
        break;
    case TAC_OR:
        fprintf(stderr, "TAC_OR");
        break;
    case TAC_NOT:
        fprintf(stderr, "TAC_NOT");
        break;
    case TAC_RETURN:
        fprintf(stderr, "TAC_RETURN");
        break;
    case TAC_PRINT:
        fprintf(stderr, "TAC_PRINT");
        break;
    case TAC_READ:
        fprintf(stderr, "TAC_READ");
        break;
    case TAC_BEGINFUNCTION:
        fprintf(stderr, "TAC_BEGINFUNCTION");
        break;
    case TAC_ENDFUNCTION:
        fprintf(stderr, "TAC_ENDFUNCTION");
        break;
    case TAC_PRINTPAR:
        fprintf(stderr, "TAC_PRINTPAR");
        break;
    case TAC_ATTRV:
        fprintf(stderr, "TAC_ATTRV");
        break;
    case TAC_VECTOR:
        fprintf(stderr, "TAC_VECTOR");
        break;
    case TAC_VECIND:
        fprintf(stderr, "TAC_VECIND");
        break;
    case TAC_FUNCARGS:
        fprintf(stderr, "TAC_FUNCARGS");
        break;
    case TAC_UNTIL:
        fprintf(stderr, "TAC_UNTIL");
        break;
    case TAC_COMEFROM:
        fprintf(stderr, "TAC_COMEFROM");
        break;
    case TAC_CHECKPOINT:
        fprintf(stderr, "TAC_CHECKPOINT");
        break;
    case TAC_FUNCPAR:
        fprintf(stderr, "TAC_FUNCPAR");
        break;
    case TAC_FUNC:
        fprintf(stderr, "TAC_FUNC");
        break;
    case TAC_FUNCRES:
        fprintf(stderr, "TAC_FUNCRES");
        break;
    default:
        fprintf(stderr, "TAC_UNKNWON");
        break;
    }

    fprintf(stderr, ",%s", (tac->res) ? tac->res->text : " ");
    fprintf(stderr, ",%s", (tac->op1) ? tac->op1->text : " ");
    fprintf(stderr, ",%s", (tac->op2) ? tac->op2->text : " ");
    fprintf(stderr, ");\n");
}
void tacPrintBackwards(TAC *tac)
{
    if (!tac)
        return;
    else
    {
        tacPrintBackwards(tac->prev);
        tacPrint(tac);
    }
}

//CODE GENERATION

TAC *generateCode(AST *node)
{
    int i;
    TAC *result = 0;
    TAC *code[MAX_SONS];
    HASH_NODE *label;

    if (!node)
        return 0;

    //Process children
    if (node->type == AST_UNTIL)
        label = makeLabel();

    for (i = 0; i < MAX_SONS; i++)
        code[i] = generateCode(node->son[i]);

    //Process this node

    switch (node->type)
    {
    case AST_SYMBOL:
        //fprintf(stderr, "%s", node->symbol->text);
        result = tacCreate(TAC_SYMBOL, node->symbol, 0, 0);
        break;

    case AST_ADD:
        result = binaryOp(code, TAC_ADD);
        break;
    case AST_SUB:
        result = binaryOp(code, TAC_SUB);
        break;
    case AST_MULT:
        result = binaryOp(code, TAC_MULT);
        break;
    case AST_DIV:
        result = binaryOp(code, TAC_DIV);
        break;
    case AST_LT:
        result = binaryOp(code, TAC_LT);
        break;
    case AST_GT:
        result = binaryOp(code, TAC_GT);
        break;
    case AST_OR:
        result = binaryOp(code, TAC_OR);
        break;
    case AST_LE:
        result = binaryOp(code, TAC_LE);
        break;
    case AST_GE:
        result = binaryOp(code, TAC_GE);
        break;
    case AST_EQ:
        result = binaryOp(code, TAC_EQ);
        break;
    case AST_DIF:
        result = binaryOp(code, TAC_DIF);
        break;
    case AST_NOT:
        result = binaryOp(code, TAC_NOT);
        break;

    case AST_ATTR:
        result = attr(code, node);
        break;

    case AST_VECTOR:
        result = tacJoin(code[0], tacCreate(TAC_VECIND, makeTemp(), node->symbol, code[0] ? code[0]->res : 0));
        break;

    case AST_IF:
    case AST_IFELSE:
        result = makeIfThen(code);
        break;
    case AST_UNTIL:
        result = until(code, label);
        break;

    case AST_PRINT:
        result = code[0];
        break;
    case AST_PRINTPAR:
        result = print(code, node);
        break;
    case AST_PRINTPARTAIL:
        result = print(code, node);
        break;

    case AST_FUNCAUX:
        result = makeFunction(code, node);
        break;
    case AST_ARGS:
    case AST_ARGSRABO:
        result = tacJoin(tacJoin(code[1], tacCreate(TAC_FUNCPAR, 0, node->symbol, 0)), code[2]);
        break;
    case AST_FUNC:
        label = makeLabel();
        result = tacJoin(tacJoin(tacJoin(tacCreate(TAC_FUNC, node->symbol, label, 0), code[0]), tacJoin(tacCreate(TAC_JUMP, node->symbol, 0, 0), tacCreate(TAC_LABEL, label, 0, 0))), tacCreate(TAC_FUNCRES, makeTemp(), 0, 0));
        break;
    case AST_ARGS2:
    case AST_ARGS2RABO:
        result = tacJoin(tacJoin(code[0], tacCreate(TAC_FUNCARGS, code[0]->res, 0, 0)), code[1]);
        break;
    case AST_DEC:
        result = tacJoin(code[0], code[1]);
        break;
    case AST_READ:
        result = tacCreate(TAC_READ, makeTemp(), 0, 0);
        break;
    case AST_RETURN:
        result = tacJoin(code[0], tacCreate(TAC_RETURN, code[0] ? code[0]->res : 0, 0, 0));
        break;
    case AST_COMEFROM:
        result = tacCreate(TAC_COMEFROM, makeLabel(), node->son[0]->symbol, 0);
        break;
    case AST_CHECKPOINT:
        result = tacCreate(TAC_JUMP, node->symbol, 0, 0);
        break;
    default:
        result = tacJoin(code[0], tacJoin(code[1], tacJoin(code[2], code[3]))); //return the union of code for all subtrees.
        break;
    }

    return result;
}

TAC *tacJoin(TAC *l1, TAC *l2)
{
    TAC *point;

    if (!l1)
        return l2;
    if (!l2)
        return l1;

    for (point = l2; point->prev != 0; point = point->prev)
        ;
    point->prev = l1;
    return l2;
}

TAC *makeIfThen(TAC *code[])
{
    TAC *jumpTac = 0, *jumpTacElse = 0;
    TAC *labelTac = 0, *labelTacElse = 0;
    HASH_NODE *newLabel = 0, *labelElse = 0;

    newLabel = makeLabel();
    jumpTac = tacJoin(code[0], tacCreate(TAC_JFALSE, newLabel, code[0] ? code[0]->res : 0, 0));
    labelTac = tacCreate(TAC_LABEL, newLabel, 0, 0);

    if (code[2])
    {
        labelElse = makeLabel();
        labelTacElse = tacCreate(TAC_LABEL, labelElse, 0, 0);
        jumpTacElse = tacCreate(TAC_JUMP, labelElse, 0, 0);
        return tacJoin(tacJoin(tacJoin(tacJoin(tacJoin(jumpTac, code[1]), jumpTacElse), labelTac), code[2]), labelTacElse); //tacJoin(tacJoin(tacJoin(tacJoin(tacJoin(jumpTac, code[1]), jumpTacElse), labelTacElse), code[2]), labelTacElse);
    }
    return tacJoin(tacJoin(jumpTac, code[1]), labelTac);
}

TAC *binaryOp(TAC *code[], int type)
{
    return tacJoin(tacJoin(code[0], code[1]), tacCreate(type, makeTemp(), code[0] ? code[0]->res : 0, code[1] ? code[1]->res : 0));
}

TAC *print(TAC *code[], AST *node)
{
    if (code[1])
    {
        return tacJoin(tacJoin(code[0], tacCreate(TAC_PRINT, code[0] ? code[0]->res : 0, 0, 0)), code[1]);
    }
    else if (node->symbol)
    {
        return tacJoin(tacCreate(TAC_PRINT, node->symbol, 0, 0), code[0]);
    }
    else
        return tacJoin(code[0], tacCreate(TAC_PRINT, code[0] ? code[0]->res : 0, 0, 0));
}

TAC *attr(TAC *code[], AST *node)
{
    if (code[1])
    {
        return tacJoin(tacJoin(code[0], code[1]), tacCreate(TAC_ATTRV, code[0] ? code[0]->res : 0, code[1] ? code[1]->res : 0, 0));
    }
    else
    {
        return tacJoin(code[0], tacCreate(TAC_ATTR, node->symbol, code[0] ? code[0]->res : 0, 0));
    }
}

TAC *makeFunction(TAC *code[], AST *node)
{
    TAC *symbol = tacCreate(TAC_SYMBOL, node->symbol, 0, 0);
    return tacJoin(tacJoin(tacJoin(tacCreate(TAC_BEGINFUNCTION, symbol->res, 0, 0), code[1]), code[2]), tacCreate(TAC_ENDFUNCTION, symbol->res, 0, 0));
}

TAC *until(TAC *code[], HASH_NODE *untilLabel)
{
    TAC *jumpTac = 0, *untilTac = 0;
    TAC *labelJumpTac = 0, *labelUntilTac = 0;
    HASH_NODE *newLabel = 0;

    newLabel = makeLabel();
    untilTac = tacCreate(TAC_UNTIL, newLabel, code[0] ? code[0]->res : 0, 0);
    jumpTac = tacCreate(TAC_JUMP, untilLabel, 0, 0);
    labelUntilTac = tacCreate(TAC_LABEL, untilLabel, 0, 0);
    labelJumpTac = tacCreate(TAC_LABEL, newLabel, 0, 0);

    return tacJoin(tacJoin(tacJoin(tacJoin(tacJoin(labelUntilTac, code[0]), untilTac), code[1]), jumpTac), labelJumpTac);
}

TAC *tacReverse(TAC *tac)
{
    TAC* t = tac;

    if (t)
    {
        for (t; t->prev; t = t->prev)
            t->prev->next = t;
        return t;
    }
      
}


