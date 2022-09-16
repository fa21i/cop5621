%{
	#include <conio.h>
        #include <stdio.h>
        #include "stack.c"
        int yylex();
        FILE *fdot;
	void yyerror(char *s);
        int glob = 0;

%}

%union {int val; char* str;}
%start prog
%token<str> CONST ID 
%token<val> PLUS
%type <val> arg op

%%

prog  : op {glob++;fprintf(fdot, "%d [label=op ordering=\"out\"]\n%d -> %d\n", glob,glob,$1);};
op : arg {glob++;fprintf(fdot, "%d [label=arg ordering=\"out\"]\n%d -> %d\n",glob,glob,$1);$$=glob;};
op : arg PLUS op {glob++;fprintf(fdot, "%d [label=arg ordering=\"out\"]\n%d -> %d\n", glob,glob,$1);
                glob++;fprintf(fdot, "%d [label=\"+\" ordering=\"out\"]\n", glob);
                glob++;fprintf(fdot, "%d [label=op ordering=\"out\"]\n%d -> %d\n", glob,glob,$3);$$=glob;};
arg : ID {glob++;fprintf(fdot, "%d [label=%s ordering=\"out\"]\n", glob, $1);$$=glob;};
arg: CONST {glob++;fprintf(fdot, "%d [label=%s ordering=\"out\"]\n", glob, $1);$$=glob;};

%%

void yyerror(char *s)
{  
        fprintf (stdout, "%s:\n", s);
}

int main()
{
        fdot = fopen("parse_tree.dot", "w+");
        fprintf(fdot, "digraph print {\n");
        yyparse();
        fprintf(fdot, "}\n");
        fclose(fdot);
        return 0;
}