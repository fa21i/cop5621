%{
        #include <stdio.h>
        int yylex();
        void yyerror(char *s);
        FILE *fdot;

        int glob = 0; //id of lefthand side
%}

%union {char* str;}
%start program
%token<str> COMPARATOR MULTOP ADDOP DEFINE FUNCTION BOOLCONST BOOLOP IF LET TYPE PRINT NAME RPAREN LPAREN CONST
%type <str> program type expr term fla

%%
program :       LPAREN DEFINE NAME type expr RPAREN program {glob++;}
        |       LPAREN DEFINE NAME LPAREN NAME type RPAREN type expr RPAREN program
        |       LPAREN DEFINE NAME LPAREN NAME type RPAREN LPAREN NAME type RPAREN type expr RPAREN program
        |       LPAREN PRINT expr RPAREN
        ;
type    :       TYPE {fprintf(fdot, "%d [label=%s ordering=\"out\"]\n", glob, $1);} //WRONG
        ;
expr    :       term
        |       fla
        ;     
term    :       CONST   //{fprintf(fdot, "%d [label=%d ordering=\"out\"]\n", glob, $1); fprintf(num->term)}
        |       NAME //glob++ (add for each terminal)
        |       LPAREN FUNCTION RPAREN //glob += 3
        |       LPAREN ADDOP term term RPAREN //+ - --> glob =+ 3
        |       LPAREN MULTOP term term RPAREN //* /
        |       LPAREN IF fla term term RPAREN
        |       LPAREN NAME RPAREN
        |       LPAREN NAME expr RPAREN
        |       LPAREN NAME expr expr RPAREN
        |       LPAREN LET LPAREN NAME expr RPAREN term RPAREN
        ;
fla     :       BOOLCONST       //{fprintf(fdot, "%d [label=%s ordering=\"out\"]\n", glob, $1);}
        |       NAME            //{fprintf(fdot, "%d [label=%s ordering=\"out\"]\n", glob, $1);}
        |       LPAREN FUNCTION RPAREN  //{fprintf(fdot, "%d [label=%s ordering=\"out\"]\n", glob, $1); fprintf(fdot, "%d [label=%s ordering=\"out\"]\n", glob, $2); fprintf(fdot, "%d [label=%s ordering=\"out\"]\n", glob, $3);}
        |       LPAREN COMPARATOR term term RPAREN
        //|       //**LPAREN BOOLOP fla RPAREN // = ( - fla ) $$ = glob (everywhere), $1 $2 $3 fprintf(fdot, %d->%d.....\n, $$, $1)
        |       LPAREN BOOLOP fla fla RPAREN
        |       LPAREN IF fla fla fla RPAREN
        |       LPAREN NAME RPAREN      //{fprintf(fdot, "%d [label='(' ordering=\"out\"]\n", glob); fprintf(fdot, "%d [label=%s ordering=\"out\"]\n", glob, $2); fprintf(fdot, "%d [label=%s ordering=\"out\"]\n", glob, $3);}
        |       LPAREN NAME expr RPAREN
        |       LPAREN NAME expr expr RPAREN
        |       LPAREN LET LPAREN NAME expr RPAREN fla RPAREN
        ;
%%
        //(+ (* 1 2) 3) --> ( * 1 2 ) glob+=? == 5 $$ == 5 --> 
        //+ glob++ (6) --> (+ term($3) term($4) ) --> $3 == 5 --> $4 will be glob at 3

#include "lex.yy.c"

void yyerror(char * s)
{  
        fprintf (stderr, "%s\n", s);
}

int main(int argc, char* argv[])
{
        fdot = fopen("parse_tree.dot", "w+");
        fprintf(fdot, "diagraph print {\n");
        yyparse();

        fprintf(fdot, "}\n");
        fclose(fdot); 
        return 0;
}
