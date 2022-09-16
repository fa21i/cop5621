%{
	#include <conio.h>
        #include <stdio.h>
        #include "stack.c"
        int yylex();
        FILE *fdot;
	void yyerror(char *s);
        int glob = 0;
        int chars=0;

%}

%union {int val; char* str;}
%start prog
%token<str> CONST ID PLUS
%type <str> arg op

%%

prog  : op {fprintf(fdot, "%d [label=prog ordering=\"out\"]\n%d [label=op ordering=\"out\"]\n", chars++,chars++);};
op : arg {fprintf(fdot, "%d [label=arg ordering=\"out\"]\n", chars++);};
op : arg PLUS op {fprintf(fdot, "%d [label=op ordering=\"out\"]\n", chars++, $1); 
                fprintf(fdot, "%d [label=\"+\" ordering=\"out\"]\n", chars++, $1);
                fprintf(fdot, "%d [label=arg ordering=\"out\"]\n", chars++, $1);
                $$=glob;};
arg : ID {fprintf(fdot, "%d [label=%s ordering=\"out\"]\n", chars++, $1); $$=glob;};
arg: CONST {fprintf(fdot, "%d [label=%s ordering=\"out\"]\n", chars++, $1);$$=glob;};

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