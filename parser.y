%{
        #include <stdio.h>
        void yyerror(char *s);
        int yylex();
        FILE *fp;
        fp = fpopen("parse_tree.dot", "w");
        fprintf(fp, "diagraph print {\n");

        int glob = 0;
%}

%union {int val; char* str;}
%start program
%token<str> CONST DEFINE FUNCTION BOOLCONST BOOLOP IF LET TYPE PRINT NAME RPAREN LPAREN
%token<val> COMPARATOR OPERATOR

%%
program :       LPAREN DEFINE NAME type expr RPAREN program {glob++;}
        |       LPAREN DEFINE NAME LPAREN NAME type RPAREN type expr RPAREN program
        |       LPAREN DEFINE NAME LPAREN NAME type RPAREN LPAREN NAME type RPAREN type expr RPAREN program
        |       LPAREN PRINT expr RPAREN
        ;
type    :       TYPE {fprintf(fp, "%d [label=%s ordering=\"out\"]\n", glob, $1);}
        ;
expr    :       term
        |       fla
        ;
term    :       CONST   {fprintf(fp, "%d [label=%s ordering=\"out\"]\n", glob, $1);}
        //|       NAME
        |       LPAREN FUNCTION RPAREN
        |       LPAREN OPERATOR term term RPAREN
        |       LPAREN IF fla term term RPAREN
        |       LPAREN NAME RPAREN
        |       LPAREN NAME expr RPAREN
        |       LPAREN NAME expr expr RPAREN
        |       LPAREN LET LPAREN NAME expr RPAREN term RPAREN
        ;
fla     :       BOOLCONST       {fprint(fp, "%d [label=%s ordering=\"out\"]\n", glob, $1);}
        |       NAME            {fprint(fp, "%d [label=%s ordering=\"out\"]\n", glob, $1);}
        |       LPAREN FUNCTION RPAREN  {fprint(fp, "%d [label=%s ordering=\"out\"]\n", glob, $1); fprint(fp, "%d [label=%s ordering=\"out\"]\n", glob, $2); fprint(fp, "%d [label=%s ordering=\"out\"]\n", glob, $3);}
        |       LPAREN COMPARATOR term term RPAREN
        |       LPAREN BOOLOP fla RPAREN
        |       LPAREN BOOLOP fla fla RPAREN
        |       LPAREN IF fla fla fla RPAREN
        |       LPAREN NAME RPAREN      {fprint(fp, "%d [label=%s ordering=\"out\"]\n", glob, $1); fprint(fp, "%d [label=%s ordering=\"out\"]\n", glob, $2); fprint(fp, "%d [label=%s ordering=\"out\"]\n", glob, $3);}
        |       LPAREN NAME expr RPAREN
        |       LPAREN NAME expr expr RPAREN
        |       LPAREN LET LPAREN NAME expr RPAREN fla RPAREN
        ;
%%

#include "lex.yy.c"

void yyerror(char * s)
{  
        fprintf (stderr, "%s\n", s);
}

int main()
{
        yyparse();

        fprintf(fp, "}\n");
        fclose(fp); 
        return 0;
}
