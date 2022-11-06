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
(>) 	{yylval.str = yytext; return GT;}
(=) 	{yylval.str = yytext; return EQ;}
(<) 	{yylval.str = yytext; return LT;}
(<=) 	{yylval.str = yytext; return LTEQ;}
(>=) 	{yylval.str = yytext; return GTEQ;}
\+ 	{yylval.str = yytext; return ADDOP;}
\- 	{yylval.str = yytext; return MINOP;}
\* 	{yylval.str = yytext; return MULTOP;}
div      {yylval.str = yytext; return DIVOP;}
mod      {yylval.str = yytext; return MODOP;}
define-fun	{yylval.str = yytext; return DEFINE;}
get-int     {yylval.str = yytext; return GETINT;}
get-bool 	{yylval.str = yytext; return GETBOOL;}
true        {yylval.str = yytext; return TRUECONST;}
false 	{yylval.str = yytext; return FALSECONST;}
and     {yylval.str = yytext; return AND;}
or 	{yylval.str = yytext; return OR;}
not 	{yylval.str = yytext; return NOT;}
if	{yylval.str = yytext; return IF;}
let 	{yylval.str = yytext; return LET;}
int     {yylval.str = yytext; return INTTYPE;}
bool 	{yylval.str = yytext; return BOOLTYPE;}
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
