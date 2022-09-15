%{
int count = 1;
int token = 0;
int token_not_defined=0;
#include <stdio.h>
#include 'y.tab.h'
%}

%%
;.* {;}
[-]?[0-9]+ 	{printf("line %d: arithmetic const:\t\t%s\n", count,yytext); token++; return NUMBER;}
[<|>|<=|>=|=] 	{printf("line %d: arithmetic comparison:\t\t%s\n", count,yytext);token++; return COMPARATOR;}
[\+|\-|\*]|mod|div 	{printf("line %d: arithmetic operation:\t\t%s\n", count,yytext);token++; return OPERATOR;}
define-fun	{printf("line %d: function definition keyword:\t%s\n", count,yytext);token++; return DEFINE;}
get-int|get-bool 	{printf("line %d: predefined functions:\t\t%s\n", count,yytext);token++; return FUNCTION;}
true|false 	{printf("line %d: boolean constants:\t\t\t%s\n", count,yytext);token++; return BOOLCONST;}
and|or|not 	{printf("line %d: boolean operator:\t\t%s\n", count,yytext);token++; return BOOLOP;}
if	{printf("line %d: conditional operator:\t\t%s\n", count,yytext);token++; return IF;}
let 	{printf("line %d: local variable declaration:\t\t%s\n", count,yytext);token++; return LET;}
int|bool 	{printf("line %d: variable/function types:\t%s\n", count,yytext);token++; return TYPE;}
print 	{printf("line %d: program entry:\t\t\t%s\n", count,yytext);token++; return PRINT;}
[a-zA-Z][a-zA-Z0-9]*	{printf("line %d: variable/function name:\t\t%s\n", count,yytext);token++; return NAME;}
[)]   	{printf("line %d: right paranthesis:\t\t%s\n", count,yytext);token++; return RPAREN;}
[(]   	{printf("line %d: left paranthesis:\t\t%s\n", count,yytext);token++; return LPAREN;}
\n    	{count++;}
" "	{;}
\"	{;}
\t	{;}
. 	{token_not_defined = 1; return 0;}
%%

int yywrap(){}
int main(){

FILE *fp;
char filename[50];

fp = fopen("sample6.txt","r");
yyin = fp;

yylex();
if(token_not_defined==1){
	printf("\nError message: Token cannot be recognized\n");
}
if(token==0){
	printf("\nError message: No token can be recognized\n");
}

return 0;
}
