%{
#include <stdio.h>
#include "yacc.tab.h"
int yylex();
int yywrap(){return 1;}
%}

%%
[0-9]+ 	{yylval.str = yytext; return CONST;}
"+" 	{yylval.str = yytext; return PLUS;}
[a-z]+ {return ID;}
. 	{;}
%%
