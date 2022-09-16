%{
        #include <stdio.h>
        int yylex();
        FILE *fdot;
	void yyerror(char *s);
        int glob = 0;
%}

%union {int val; char* str;}
%start program
%token <str> NAME
%token <val> COMPARATOR MULTOP ADDOP DEFINE FUNCTION BOOLCONST BOOLOP IF LET TYPE PRINT RPAREN LPAREN NOT CONST
%type <val> program type expr term fla 


%%
program :   LPAREN NAME type expr RPAREN program    {glob += 3; fprintf(fdot, "%d [label=program ordering=\"out\"]\n", glob);
                                                        fprintf(fdot, "%d [label=\"(\" ordering=\"out\"]\n", glob); 
                                                        fprintf(fdot, "%d [label=%s ordering=\"out\"]\n", glob, $2);
                                                        fprintf(fdot, "%d [label=type ordering=\"out\"]\n", glob);
                                                        fprintf(fdot, "%d [label=expr ordering=\"out\"]\n",glob);
                                                        fprintf(fdot, "%d [label=\")\" ordering=\"out\"]\n", glob);
                                                        fprintf(fdot, "%d [label=program ordering=\"out\"]\n", glob); $$ = glob;}
        |   LPAREN DEFINE NAME LPAREN NAME type RPAREN type expr RPAREN program {glob += 7; $$ = glob; }
        |   LPAREN DEFINE NAME type RPAREN LPAREN NAME type RPAREN type expr RPAREN program
        |   LPAREN PRINT expr RPAREN
        ;
type    :   TYPE
        ;
expr    :   term
        |   fla
        ;
term    :   CONST
        |   NAME
        |   LPAREN FUNCTION RPAREN
        |   LPAREN ADDOP term term RPAREN
        |   LPAREN MULTOP term term RPAREN
        |   LPAREN IF fla term term RPAREN
        |   LPAREN NAME RPAREN
        |   LPAREN NAME expr RPAREN
        |   LPAREN NAME expr expr RPAREN
        |   LPAREN LET LPAREN NAME expr RPAREN term RPAREN
        ;
fla     :   BOOLCONST
        |   NAME        {glob++; fprintf(fdot, "%d [label=%s ordering=\"out\"]\n", glob, $1); $$ = glob;}
        |   LPAREN FUNCTION RPAREN
        |   LPAREN COMPARATOR term term RPAREN
        |   LPAREN NOT fla RPAREN
        |   LPAREN BOOLOP term term RPAREN
        |   LPAREN IF fla fla fla RPAREN
        |   LPAREN NAME RPAREN
        |   LPAREN NAME expr RPAREN
        |   LPAREN NAME expr expr RPAREN
        |   LPAREN LET LPAREN NAME expr RPAREN fla RPAREN
        ;
%%

/*arg : ID {glob ++; printf("id: %s, %d\n", $1, glob); $$ = glob; }
| CONST {glob ++; printf("const: %s, %d\n", $1, glob); $$ = glob; };
*/
#include "lex.yy.c"

void yyerror(char * s)
{  
        fprintf (stderr, "%s\n", s);
}

int main(int argc, char* argv[])
{
        fdot = fopen("parse_tree.dot", "w+");
        fprintf(fdot, "digraph print {\n");
        yyparse();

        fprintf(fdot, "}\n");
        fclose(fdot); 
        return 0;
}
