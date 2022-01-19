/* Gert Willem Folz - 00288550 
 * Trabalho 2 da disciplina de compiladores */


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

%token TK_IDENTIFIER     

%token LIT_INTEGER       
%token LIT_CHAR          
%token LIT_STRING        

%token TOKEN_ERROR       

%left '|' '&' '~'
%left '<' '>' OPERATOR_LE OPERATOR_GE OPERATOR_EQ OPERATOR_DIF 
%left '+' '-'
%left '*' '/'

%start programa 

%%

programa: dataList funcList
    ;

dataList: KW_DATA '{' decList '}' 
    ;

decList: dec decList
    | 
    ;

dec: dataType ':' TK_IDENTIFIER '=' literalsInit ';'
    | dataType  '[' LIT_INTEGER OPERATOR_RANGE LIT_INTEGER ']' ':' TK_IDENTIFIER '=' vectorInit ';'
    | dataType  '[' LIT_INTEGER OPERATOR_RANGE LIT_INTEGER ']' ':' TK_IDENTIFIER ';'
    ;

vectorInit: literalsInit vectorInit
    |
    ;

vector: TK_IDENTIFIER '[' expr ']'
    ;

funcList: funcAux funcList 
    |
    ;

funcAux: dataType ':' TK_IDENTIFIER '(' args ')' block 
    ;

func: TK_IDENTIFIER '(' args2 ')'
    ;

args2: expr args2rabo
    |
    ;

args2rabo: ',' expr args2rabo
    |
    ;

args: dataType ':' TK_IDENTIFIER argsrabo  
    |
    ;

argsrabo: ',' dataType ':' TK_IDENTIFIER argsrabo   
    |                                              
    ;

dataType: KW_INT
    | KW_CHAR
    | KW_FLOAT
    ; 

literalsInit: LIT_INTEGER
    | LIT_CHAR
    ;

block: '{' lcmd '}'
    ;

lcmd: cmd ';' lcmd
    |
    ;

cmd: atrib 
    | return 
    | print 
    | fluxCrl
    | checkpoint 
    | comefrom 
    | block 
    |
    ;

atrib: TK_IDENTIFIER '=' expr
    | vector '=' expr
    ;

expr: LIT_INTEGER
    | TK_IDENTIFIER
    | LIT_CHAR
    | expr '+' expr
    | expr '-' expr
    | expr '*' expr
    | expr '/' expr
    | expr '<' expr
    | expr '>' expr
    | expr '|' expr
    | expr '&' expr
    | '~' expr 
    | expr OPERATOR_LE expr
    | expr OPERATOR_GE expr
    | expr OPERATOR_EQ expr
    | expr OPERATOR_DIF expr
    | '(' expr ')'
    | vector
    | KW_READ
    | func
    ;

return: KW_RETURN expr
    ;

print: KW_PRINT printList
    ;

printList: LIT_STRING printTail
    | expr printTail
    ;

printTail: ',' expr printTail 
    |',' LIT_STRING printTail 
    | 
    ;

fluxCrl: KW_IF '(' expr ')' fluxCmd
    | KW_IF '(' expr ')' fluxCmd KW_ELSE fluxCmd
    | KW_UNTIL '(' expr ')' fluxCmd
    ;

fluxCmd: cmd
    ;

comefrom: KW_COMEFROM ':' checkpoint
    ;

checkpoint: TK_IDENTIFIER
    ;

%%

int yyerror ()
{
    fprintf(stderr, "syntax error at line %d\n", getLineNumber());
    exit(3);
}
