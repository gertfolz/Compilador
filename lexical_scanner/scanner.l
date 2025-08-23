 /* Gert Willem Folz - 00288550 
 * Trabalho 2 da disciplina de compiladores */

%{
    #include "hash.h"
    #include "ast.h"
    #include "y.tab.h" 
    int running=1;
    int lineNumber=1;
    void hashInit();
%}

%x COMMENT

%%          

data                             {return KW_DATA;}
char                             {return KW_CHAR;}
int                              {return KW_INT;}
float                            {return KW_FLOAT;}
if                               {return KW_IF;}
else                             {return KW_ELSE;}
until                            {return KW_UNTIL;}
comefrom                         {return KW_COMEFROM;}
read                             {return KW_READ;}
print                            {return KW_PRINT;}
return                           {return KW_RETURN;}

[-,;:()\[\]{}+*/<>|&~=]          {return yytext[0];}

"<="                             {return OPERATOR_LE;}
">="                             {return OPERATOR_GE;}
"=="                             {return OPERATOR_EQ;}
"!="                             {return OPERATOR_DIF;}
".."                             {return OPERATOR_RANGE;}

[a-zA-Z][ a-zA-Z0-9]*[a-zA-Z0-9] {yylval.symbol = hashInsert(yytext, TK_IDENTIFIER); return TK_IDENTIFIER;}
[a-zA-Z]                         {yylval.symbol = hashInsert(yytext, TK_IDENTIFIER); return TK_IDENTIFIER;}

[0-9]+                           {yylval.symbol = hashInsert(yytext, LIT_INTEGER); return LIT_INTEGER;}  

"'"[ -~]"'"                      {yylval.symbol = hashInsert(yytext, LIT_CHAR); return LIT_CHAR;}

"\""([^"\n]|"\\\"")*"\""         {yylval.symbol = hashInsert(yytext, LIT_STRING); return LIT_STRING;}



[ \t]
"\n"                             lineNumber++; 
"##".*
"#/"                             {BEGIN(COMMENT);}

<COMMENT>"/#"                    {BEGIN(INITIAL);}
<COMMENT>"\n"                    {++lineNumber;}
<COMMENT>.

.                                {return TOKEN_ERROR;}
%%


int yywrap()
{
    running=0;
    return 1;
}

int isRunning()
{
    return running;
}

int getLineNumber()
{
    return lineNumber;
}

void initMe()
{
    hashInit();
}
