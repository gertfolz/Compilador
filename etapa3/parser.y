/* Gert Willem Folz - 00288550 
 * Trabalho 2 da disciplina de compiladores */

%{
    #include "hash.h"
    #include "ast.h"

    int getLineNumber();
    int yyerror();
    int yylex();  
%}

%union
{
    HASH_NODE *symbol;
    AST *ast;
}

%token KW_CHAR           
%token KW_INT            
%token KW_FLOAT          
%token KW_DATA           

%token KW_IF             
%token KW_ELSE            
%token KW_UNTIL          
%token KW_COMEFROM       
%token KW_READ           
%token KW_PRINT          
%token KW_RETURN         

%token OPERATOR_LE       
%token OPERATOR_GE       
%token OPERATOR_EQ       
%token OPERATOR_DIF      
%token OPERATOR_RANGE    

%token<symbol> TK_IDENTIFIER     

%token<symbol> LIT_INTEGER       
%token<symbol> LIT_CHAR          
%token<symbol> LIT_STRING        
 
%token TOKEN_ERROR   

%type<ast> expr
%type<ast> cmd
%type<ast> lcmd
%type<ast> atrib
%type<ast> return
%type<ast> printList
%type<ast> printTail
%type<ast> literalsInit
%type<ast> print
%type<ast> dataList
%type<ast> decList
%type<ast> dec
%type<ast> vectorInit
%type<ast> dataType
%type<ast> args
%type<ast> args2
%type<ast> funcList
%type<ast> block
%type<ast> args2rabo
%type<ast> vector
%type<ast> argsrabo
%type<ast> func
%type<ast> fluxCmd
%type<ast> fluxCrl
%type<ast> comefrom
%type<ast> checkpoint
%type<ast> funcAux





%left '|' '&' '~'
%left '<' '>' OPERATOR_LE OPERATOR_GE OPERATOR_EQ OPERATOR_DIF 
%left '+' '-'
%left '*' '/'

%start programa

%%

programa: dataList funcList         {astPrint($1, 0);astDecomp($1);astPrint($2, 0);astDecomp($2);}
    ;

dataList: KW_DATA '{' decList '}'   {$$ = astCreate(AST_DATA, 0, $3, 0, 0, 0);}
    ;

decList: dec decList                {$$ = astCreate(AST_DECDATA, 0, $1, $2, 0, 0);}
    |                               {$$ = 0;}
    ;

dec: dataType ':' TK_IDENTIFIER '=' literalsInit ';'         {$$ = astCreate(AST_DEC, $3, $1, $5, 0, 0);}
    | dataType  '[' LIT_INTEGER OPERATOR_RANGE LIT_INTEGER ']' ':' TK_IDENTIFIER '=' vectorInit ';' {$$ = astCreate(AST_DECV, $8, $1, astCreate(AST_VECS, $3, 0, 0, 0, 0), astCreate(AST_VECS, $5, 0, 0, 0, 0), $10);}
    | dataType  '[' LIT_INTEGER OPERATOR_RANGE LIT_INTEGER ']' ':' TK_IDENTIFIER ';'                {$$ = astCreate(AST_DECV, $8, $1, astCreate(AST_VECS, $3, 0, 0, 0, 0), astCreate(AST_VECS, $5, 0, 0, 0, 0), 0);}
    ;


vectorInit: literalsInit vectorInit      {$$ = astCreate(AST_VECINIT, 0, $1, $2, 0, 0);}
    |                                    {$$ = 0;}
    ;

vector: TK_IDENTIFIER '[' expr ']'       {$$ = astCreate(AST_VECTOR, $1, $3, 0, 0, 0);}
    ;

funcList: funcAux funcList     {$$ = astCreate(AST_FUNCLIST, 0, $1, $2, 0, 0);}
    |                                                                {$$ = 0;}
    ;

funcAux: dataType ':' TK_IDENTIFIER '(' args ')' block   {$$ = astCreate(AST_FUNCAUX, $3, $1, $5, $7, 0);}
    ;

func: TK_IDENTIFIER '(' args2 ')'                                       {$$ = astCreate(AST_FUNC, $1, $3, 0, 0, 0);}
    ;

args2: expr args2rabo           {$$ = astCreate(AST_ARGS2, 0, $1, $2, 0, 0);} 
    |                           {$$ = 0;}  
    ;

args2rabo: ',' expr args2rabo   {$$ = astCreate(AST_ARGS2RABO, 0, $2, $3, 0, 0);}
    |                           {$$ = 0;} 
    ;

args: dataType ':' TK_IDENTIFIER argsrabo       {$$ = astCreate(AST_ARGS, $3, $1, $4, 0, 0);} 
    |                                           {$$ = 0;}
    ;

argsrabo: ',' dataType ':' TK_IDENTIFIER argsrabo   {$$ = astCreate(AST_ARGSRABO, $4, $2, $5, 0, 0);} 
    |                                               {$$ = 0;}
    ;

dataType: KW_INT                 {$$ = astCreate(AST_INT, 0, 0, 0, 0, 0);}
    | KW_CHAR                    {$$ = astCreate(AST_CHAR, 0, 0, 0, 0, 0);}
    | KW_FLOAT                   {$$ = astCreate(AST_FLOAT, 0, 0, 0, 0, 0);}
    ;       

literalsInit: LIT_INTEGER   {$$ = astCreate(AST_SYMBOL, $1, 0, 0, 0, 0);}
    | LIT_CHAR              {$$ = astCreate(AST_SYMBOL, $1, 0, 0, 0, 0);}
    ;

block: '{' lcmd '}'         {$$ = astCreate(AST_BLOCK, 0, $2, 0, 0, 0);}  
    ;

lcmd: cmd ';' lcmd      {$$ = astCreate(AST_LCMD, 0, $1, $3, 0, 0);}
    |                   {$$ = 0; }
    ;

cmd: atrib 
    | return 
    | print 
    | fluxCrl
    | checkpoint 
    | comefrom 
    | block 
    |               {$$ = 0; }
    ;

atrib: TK_IDENTIFIER '=' expr   {$$ = astCreate(AST_ATTR, $1, $3, 0, 0, 0);}
    | vector '=' expr           {$$ = astCreate(AST_ATTR, 0, $1, $3, 0, 0);}
    ;

expr: LIT_INTEGER                   {$$ = astCreate(AST_SYMBOL, $1, 0, 0, 0, 0);}
    | TK_IDENTIFIER                 {$$ = astCreate(AST_SYMBOL, $1, 0, 0, 0, 0);} 
    | LIT_CHAR                      {$$ = astCreate(AST_SYMBOL, $1, 0, 0, 0, 0);}     
    | expr '+' expr                 {$$ = astCreate(AST_ADD, 0, $1, $3, 0, 0);}
    | expr '-' expr                 {$$ = astCreate(AST_SUB, 0, $1, $3, 0, 0);}
    | expr '*' expr                 {$$ = astCreate(AST_MULT, 0, $1, $3, 0, 0);}
    | expr '/' expr                 {$$ = astCreate(AST_DIV, 0, $1, $3, 0, 0);}
    | expr '<' expr                 {$$ = astCreate(AST_LT, 0, $1, $3, 0, 0);}
    | expr '>' expr                 {$$ = astCreate(AST_GT, 0, $1, $3, 0, 0);}
    | expr '|' expr                 {$$ = astCreate(AST_OR, 0, $1, $3, 0, 0);}
    | expr '&' expr                 {$$ = astCreate(AST_AND, 0, $1, $3, 0, 0);}
    | '~' expr                      {$$ = astCreate(AST_NOT, 0, $2, 0, 0, 0);}  
    | expr OPERATOR_LE expr         {$$ = astCreate(AST_LE, 0, $1, $3, 0, 0);}
    | expr OPERATOR_GE expr         {$$ = astCreate(AST_GE, 0, $1, $3, 0, 0);}
    | expr OPERATOR_EQ expr         {$$ = astCreate(AST_EQ, 0, $1, $3, 0, 0);}
    | expr OPERATOR_DIF expr        {$$ = astCreate(AST_DIF, 0, $1, $3, 0, 0);}
    | '(' expr ')'                  {$$ = $2;}    
    | vector                        {$$ = $1;}          
    | KW_READ                       {$$ = astCreate(AST_READ, 0, 0, 0, 0, 0);}       
    | func                          {$$ = $1;}       
    ;       

return: KW_RETURN expr              {$$ = astCreate(AST_RETURN, 0, $2, 0, 0, 0);}
    ;

print: KW_PRINT printList           {$$ = astCreate(AST_PRINT, 0, $2, 0, 0, 0);}
    ;

printList: LIT_STRING printTail     {$$ = astCreate(AST_PRINTPAR, $1, $2, 0, 0, 0);}
    | expr printTail                {$$ = astCreate(AST_PRINTPAR, 0, $1, $2, 0, 0);}
    ;

printTail: ',' expr printTail       {$$ = astCreate(AST_PRINTPARTAIL, 0, $2, $3, 0, 0);}
    |',' LIT_STRING printTail       {$$ = astCreate(AST_PRINTPARTAIL, $2, $3, 0, 0, 0);}
    |                               {$$ = 0;}  
    ;

fluxCrl: KW_IF '(' expr ')' fluxCmd                  {$$ = astCreate(AST_IF, 0, $3, $5, 0, 0);}
    | KW_IF '(' expr ')' fluxCmd KW_ELSE fluxCmd     {$$ = astCreate(AST_IFELSE, 0, $3, $5, $7, 0);}
    | KW_UNTIL '(' expr ')' fluxCmd                  {$$ = astCreate(AST_UNTIL, 0, $3, $5, 0, 0);}
    ;

fluxCmd: cmd                                         {$$ = astCreate(AST_FLUX, 0, $1, 0, 0, 0);}
    ;

comefrom: KW_COMEFROM ':' checkpoint                 {$$ = astCreate(AST_COMEFROM, 0, $3, 0, 0, 0);}     
    ;

checkpoint: TK_IDENTIFIER                            {$$ = astCreate(AST_CHECKPOINT, $1, 0, 0, 0, 0);} 
    ;

%%

int yyerror ()
{
    fprintf(stderr, "syntax error at line %d\n", getLineNumber());
    exit(3);
}
