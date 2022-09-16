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
[-]?[0-9]+ 	{/*printf("line %d: arithmetic const:\t\t%s\n", count,yytext); token++;*/ yylval.str = yytext; return CONST;}
[<|>|<=|>=|=] 	{/*printf("line %d: arithmetic comparison:\t\t%s\n", count,yytext);token++;*/yylval.str = yytext; return COMPARATOR;}
[\+|\-] 	{/*printf("line %d: arithmetic operation:\t\t%s\n", count,yytext);token++;*/yylval.str = yytext; return ADDOP;}
\*|mod|div 	{/*printf("line %d: arithmetic operation:\t\t%s\n", count,yytext);token++;*/yylval.str = yytext; return MULTOP;}
define-fun	{/*printf("line %d: function definition keyword:\t%s\n", count,yytext);token++;*/yylval.str = yytext; return DEFINE;}
get-int|get-bool 	{/*printf("line %d: predefined functions:\t\t%s\n", count,yytext);token++;*/yylval.str = yytext; return FUNCTION;}
true|false 	{/*printf("line %d: boolean constants:\t\t\t%s\n", count,yytext);token++;*/yylval.str = yytext; return BOOLCONST;}
and|or 	{/*printf("line %d: boolean operator:\t\t%s\n", count,yytext);token++;*/yylval.str = yytext; return BOOLOP;}
not 	{/*printf("line %d: boolean operator:\t\t%s\n", count,yytext);token++;*/yylval.str = yytext; return NOT;}
if	{/*printf("line %d: conditional operator:\t\t%s\n", count,yytext);token++;*/yylval.str = yytext; return IF;}
let 	{/*printf("line %d: local variable declaration:\t\t%s\n", count,yytext);token++;*/yylval.str = yytext; return LET;}
int|bool 	{/*printf("line %d: variable/function types:\t%s\n", count,yytext);token++;*/yylval.str = yytext; return TYPE;}
print 	{/*printf("line %d: program entry:\t\t\t%s\n", count,yytext);token++;*/yylval.str = yytext; return PRINT;}
[a-zA-Z][a-zA-Z0-9]*	{/*printf("line %d: variable/function name:\t\t%s\n", count,yytext);token++;*/yylval.str = yytext; return NAME;}
[)]   	{/*printf("line %d: right paranthesis:\t\t%s\n", count,yytext);token++;*/yylval.str = yytext; return RPAREN;}
[(]   	{/*printf("line %d: left paranthesis:\t\t%s\n", count,yytext);token++;*/yylval.str = yytext; return LPAREN;}
\n    	{count++;}
" "	{;}
\"	{;}
\t	{;}
. 	{;}
%%
