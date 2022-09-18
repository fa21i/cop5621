%{
int count = 1;
int token = 0;
int token_not_defined=0;
#include <stdio.h>
#include "new_parser.tab.h"
int yylex();
int yywrap(){return 1;}
%}

%%
;.* {;}
[-]?[0-9]+ 	{yylval.str = yytext; return CONST;}
(<|>|<=|>=|=) 	{yylval.str = yytext; return COMPARATOR;}
\+ 	{yylval.str = yytext; return ADDOP;}
\- 	{yylval.str = yytext; return MINOP;}
\*|mod|div 	{yylval.str = yytext; return MULTOP;}
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
