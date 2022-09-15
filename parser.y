%{
    #include <ctype.h>
    #include <stdio.h>
    #define YYSTYPE double /* double type for yacc stack */
%}

%token NUMBER, COMPARATOR, OPERATOR, DEFINE, FUNCTION, BOOLCONST, BOOLOP, IF, LET, TYPE, PRINT, NAME, RPAREN, LPAREN
%start program

%%
program :   LPAREN DEFINE fun LPAREN var type RPAREN type expr RPAREN program 
        |   LPAREN PRINT EXPR RPAREN
        ;
type    :   int
        |   bool
        ;
expr    :   term
        |   fla
        ;
term    :   const
        |   var
        |   '(' get-int ')'
        |   '(' '+' term term ')'
        |   '(' '*' term term ')'
        |   '(' '-' term term ')'
        |   '(' div term term ')'
        |   '(' mod term term ')'
        |   '(' if fla term term ')'
        |   '(' fun expr ')'
        |   '(' let '(' var expr ')' term ')'
        ;
fla     :   true 
        |   false
        |   var
        |   '(' get-bool ')'
        |   '(' '=' term term ')'
        |   '(' '<' term term ')'
        |   '(' '<=' term term ')'
        |   '(' '>' term term ')'
        |   '(' '>=' term term ')'
        |   '(' not fla ')'
        |   '(' and fla fla ')'
        |   '(' or fla fla ')'
        |   '(' if fla fla fla ')'
        |   '(' fun expr ')'
        |   '(' let '(' var expr ')' fla ')'
        ;
%%

#include "lex.yy.c"

void yyerror(char * s)
/* yacc error handler */
{  
    fprintf (stderr, "%s\n", s);
}

int main(void)
{
    return yyparse();
}