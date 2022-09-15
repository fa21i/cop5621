%{
        #include <stdio.h>
        void yyerror(char *s);
        int yylex();

        int glob = 0;
%}

%union {int val; char* str;}
%start program
%token<str> CONST DEFINE FUNCTION BOOLCONST BOOLOP IF LET TYPE PRINT NAME RPAREN LPAREN
%token<val> COMPARATOR MULTOP ADDOP

%%
program :       LPAREN DEFINE NAME type expr RPAREN program {glob++;}
        |       LPAREN DEFINE NAME LPAREN NAME type RPAREN type expr RPAREN program
        |       LPAREN DEFINE NAME LPAREN NAME type RPAREN LPAREN NAME type RPAREN type expr RPAREN program
        |       LPAREN PRINT expr RPAREN
        ;
type    :       TYPE {fprintf(fdot, "%d [label=%s ordering=\"out\"]\n", glob, $1);}
        ;
expr    :       term
        |       fla
        ;
term    :       CONST   {fprintf(fdot, "%d [label=%s ordering=\"out\"]\n", glob, $1);}
        //|       NAME
        |       LPAREN FUNCTION RPAREN
        |       LPAREN ADDOP term term RPAREN
        |       LPAREN MULTOP term term RPAREN
        |       LPAREN IF fla term term RPAREN
        |       LPAREN NAME RPAREN
        |       LPAREN NAME expr RPAREN
        |       LPAREN NAME expr expr RPAREN
        |       LPAREN LET LPAREN NAME expr RPAREN term RPAREN
        ;
fla     :       BOOLCONST       {fprintf(fdot, "%d [label=%s ordering=\"out\"]\n", glob, $1);}
        |       NAME            {fprintf(fdot, "%d [label=%s ordering=\"out\"]\n", glob, $1);}
        |       LPAREN FUNCTION RPAREN  {fprintf(fdot, "%d [label=%s ordering=\"out\"]\n", glob, $1); fprintf(fdot, "%d [label=%s ordering=\"out\"]\n", glob, $2); fprintf(fdot, "%d [label=%s ordering=\"out\"]\n", glob, $3);}
        |       LPAREN COMPARATOR term term RPAREN
        |       LPAREN BOOLOP fla RPAREN
        |       LPAREN BOOLOP fla fla RPAREN
        |       LPAREN IF fla fla fla RPAREN
        |       LPAREN NAME RPAREN      {fprintf(fdot, "%d [label=%s ordering=\"out\"]\n", glob, $1); fprintf(fdot, "%d [label=%s ordering=\"out\"]\n", glob, $2); fprintf(fdot, "%d [label=%s ordering=\"out\"]\n", glob, $3);}
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

int main(int argc, char* argv[])
{

        FILE *fdot = fopen("parse_tree.dot", "w");
        fprintf(fdot, "diagraph print {\n");

        if(argc > 1)
	{
		FILE *fp = fopen(argv[1], "r");
		if(fp)
			yyin = fp;
	}
	yylex();

	if(token_not_defined == 1)
	{
		printf("Scanner aborted, invalid input");
	}

        

        yyparse();

        //fprintf(fp, "}\n");
        fclose(fdot); 
        return 0;
}
