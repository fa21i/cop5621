%{
int count = 1;
int token = 0;
int token_not_defined=0;
#include <stdio.h>
#include "y.tab.h"
int yylex();
int yywrap(){return 1;}
%}

%%
;.* {;}
[-]?[0-9]+ 	{yylval.str = yytext; return CONST;}
[<|>|<=|>=|=] 	{yylval.str = yytext; return COMPARATOR;}
\+ 	{return ADDOP;}
\- 	{return MINOP;}
\*|mod|div 	{yylval.str = yytext; return MULTOP;}
define-fun	{yylval.str = yytext; return DEFINE;}
get-int|get-bool 	{yylval.str = yytext; return FUNCTION;}
true|false 	{yylval.str = yytext; return BOOLCONST;}
and|or 	{yylval.str = yytext; return BOOLOP;}
not 	{yylval.str = yytext; return NOT;}
if	{yylval.str = yytext; return IF;}
let 	{yylval.str = yytext; return LET;}
int|bool 	{yylval.str = yytext; return TYPE;}
print 	{yylval.str = yytext; return PRINT;}
[a-zA-Z][a-zA-Z0-9]*	{yylval.str = yytext; return NAME;}
[)]   	{yylval.str = yytext; return RPAREN;}
[(]   	{yylval.str = yytext; return LPAREN;}
\n    	{count++;}
" "	{;}
\"	{;}
\t	{;}
. 	{;}
%%
