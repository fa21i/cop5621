%{
    #include <ctype.h>
    #include <stdio.h>
    #define YYSTYPE double /* double type for yacc stack */
%}

%token NUMBER, COMPARATOR, OPERATOR, DEFINE, FUNCTION, BOOLCONST, BOOLOP, IF, LET, TYPE, PRINT, NAME, RPAREN, LPAREN

%%
program :   LPAREN define-fun fun '(' var type ')' type expr ')' program 
        |   (print expr)
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




Lines :  Lines S '\n' { printf("OK \n"); }
    |  S '\n’
    |  error '\n' {yyerror("Error: reenter last line:");
                    yyerrok; };
S     :  '(' S ')’
    |  '[' S ']’
    |   /* empty */    ;
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