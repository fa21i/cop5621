%{
        #include <stdio.h>
        #include <string.h>
        int yylex();
        FILE *fdot;
	void yyerror(char *s);
        int glob = 0;
%}

%union {int val; char* str;}
%start program
%token <str> CONST NAME TYPE BOOLCONST RPAREN FUNCTION
%token <val> COMPARATOR MULTOP DIVOP ADDOP MINOP DEFINE BOOLOP IF LET PRINT NOT LPAREN
%type <val> program type expr term fla 


%%
program :   LPAREN DEFINE NAME type expr RPAREN program    {glob+=8; fprintf(fdot, "%d [label=program ordering=\"out\"]\n", glob);
                                                        fprintf(fdot, "%d [label=\"(\" ordering=\"out\"]\n", glob-7);
                                                        fprintf(fdot, "%d [label=\"define-fun\" ordering=\"out\"]\n", glob-6);
                                                        fprintf(fdot, "%d [label=%s ordering=\"out\"]\n", glob-5, strtok($3, " "));
                                                        fprintf(fdot, "%d [label=type ordering=\"out\"]\n", glob-4);
                                                        fprintf(fdot, "%d [label=expr ordering=\"out\"]\n", glob-3); 
                                                        fprintf(fdot, "%d [label=\")\" ordering=\"out\"]\n", glob-2); 
                                                        fprintf(fdot, "%d [label=program ordering=\"out\"]\n", glob-1);$$=glob;
                                                        fprintf(fdot, "%d -> %d\n", glob-4, $4);        //link for type
                                                        fprintf(fdot, "%d -> %d\n", glob-3, $5);        //link for expr
                                                        fprintf(fdot, "%d -> %d\n", glob-1, $7);        //link for program
                                                        fprintf(fdot, "%d -> %d\n", $$, glob-7);
                                                        fprintf(fdot, "%d -> %d\n", $$, glob-6);
                                                        fprintf(fdot, "%d -> %d\n", $$, glob-5);
                                                        fprintf(fdot, "%d -> %d\n", $$, glob-4);        
                                                        fprintf(fdot, "%d -> %d\n", $$, glob-3);
                                                        fprintf(fdot, "%d -> %d\n", $$, glob-2);
                                                        fprintf(fdot, "%d -> %d\n", $$, glob-1);}       //works
        |   LPAREN DEFINE NAME LPAREN NAME type RPAREN type expr RPAREN program {glob += 12; 
                                                                                        fprintf(fdot, "%d [label=program ordering=\"out\"]\n", glob); 
                                                                                        fprintf(fdot, "%d [label=\"(\" ordering=\"out\"]\n", glob-11);
                                                                                        fprintf(fdot, "%d [label=\"define-fun\" ordering=\"out\"]\n", glob-10);
                                                                                        fprintf(fdot, "%d [label=%s ordering=\"out\"]\n", glob-9, strtok($3, " ("));
                                                                                        fprintf(fdot, "%d [label=\"(\" ordering=\"out\"]\n", glob-8);
                                                                                        fprintf(fdot, "%d [label=%s ordering=\"out\"]\n", glob-7, strtok($5, " "));
                                                                                        fprintf(fdot, "%d [label=type ordering=\"out\"]\n", glob-6);
                                                                                        fprintf(fdot, "%d [label=\")\" ordering=\"out\"]\n", glob-5);
                                                                                        fprintf(fdot, "%d [label=type ordering=\"out\"]\n", glob-4);
                                                                                        fprintf(fdot, "%d [label=expr ordering=\"out\"]\n", glob-3);
                                                                                        fprintf(fdot, "%d [label=\")\" ordering=\"out\"]\n", glob-2);
                                                                                        fprintf(fdot, "%d [label=program ordering=\"out\"]\n", glob-1);$$ = glob; 
                                                                                        fprintf(fdot, "%d -> %d\n", glob-6, $6);        //for type
                                                                                        fprintf(fdot, "%d -> %d\n", glob-4, $8);         //for type
                                                                                        fprintf(fdot, "%d -> %d\n", glob-3, $9);         //for type
                                                                                        fprintf(fdot, "%d -> %d\n", glob-1, $11);        //for program        
                                                                                        fprintf(fdot, "%d -> %d\n", $$, glob-11);        
                                                                                        fprintf(fdot, "%d -> %d\n", $$, glob-10);
                                                                                        fprintf(fdot, "%d -> %d\n", $$, glob-9);
                                                                                        fprintf(fdot, "%d -> %d\n", $$, glob-8);
                                                                                        fprintf(fdot, "%d -> %d\n", $$, glob-7);
                                                                                        fprintf(fdot, "%d -> %d\n", $$, glob-6);
                                                                                        fprintf(fdot, "%d -> %d\n", $$, glob-5);
                                                                                        fprintf(fdot, "%d -> %d\n", $$, glob-4);        
                                                                                        fprintf(fdot, "%d -> %d\n", $$, glob-3);
                                                                                        fprintf(fdot, "%d -> %d\n", $$, glob-2);
                                                                                        fprintf(fdot, "%d -> %d\n", $$, glob-1);} 
        |   LPAREN DEFINE NAME LPAREN NAME type RPAREN LPAREN NAME type RPAREN type expr RPAREN program {glob+=16;
                                                                                                fprintf(fdot, "%d [label=program ordering=\"out\"]\n", glob);
                                                                                                fprintf(fdot, "%d [label=\"(\" ordering=\"out\"]\n", glob-15);
                                                                                                fprintf(fdot, "%d [label=\"define-fun\" ordering=\"out\"]\n", glob-14); 
                                                                                                fprintf(fdot, "%d [label=%s ordering=\"out\"]\n", glob-13, strtok($3, " ("));
                                                                                                fprintf(fdot, "%d [label=\"(\" ordering=\"out\"]\n", glob-12);
                                                                                                fprintf(fdot, "%d [label=%s ordering=\"out\"]\n", glob-11, strtok($5, " "));
                                                                                                fprintf(fdot, "%d [label=type ordering=\"out\"]\n", glob-10);
                                                                                                fprintf(fdot, "%d [label=\")\" ordering=\"out\"]\n", glob-9);
                                                                                                fprintf(fdot, "%d [label=\"(\" ordering=\"out\"]\n", glob-8);
                                                                                                fprintf(fdot, "%d [label=%s ordering=\"out\"]\n", glob-7, strtok($9, " "));
                                                                                                fprintf(fdot, "%d [label=type ordering=\"out\"]\n", glob-6);
                                                                                                fprintf(fdot, "%d [label=\")\" ordering=\"out\"]\n", glob-5);
                                                                                                fprintf(fdot, "%d [label=type ordering=\"out\"]\n", glob-4);
                                                                                                fprintf(fdot, "%d [label=expr ordering=\"out\"]\n", glob-3);
                                                                                                fprintf(fdot, "%d [label=\")\" ordering=\"out\"]\n", glob-2);
                                                                                                fprintf(fdot, "%d [label=program ordering=\"out\"]\n", glob-1);$$ = glob;
                                                                                                fprintf(fdot, "%d -> %d\n", glob-10, $6);        //type
                                                                                                fprintf(fdot, "%d -> %d\n", glob-6, $10);        //type
                                                                                                fprintf(fdot, "%d -> %d\n", glob-4, $12);        //type
                                                                                                fprintf(fdot, "%d -> %d\n", glob-3, $13);        //expr
                                                                                                fprintf(fdot, "%d -> %d\n", glob-1, $15);        //program
                                                                                                fprintf(fdot, "%d -> %d\n", $$, glob-15);
                                                                                                fprintf(fdot, "%d -> %d\n", $$, glob-14);
                                                                                                fprintf(fdot, "%d -> %d\n", $$, glob-13);
                                                                                                fprintf(fdot, "%d -> %d\n", $$, glob-12);
                                                                                                fprintf(fdot, "%d -> %d\n", $$, glob-11);        
                                                                                                fprintf(fdot, "%d -> %d\n", $$, glob-10);
                                                                                                fprintf(fdot, "%d -> %d\n", $$, glob-9);
                                                                                                fprintf(fdot, "%d -> %d\n", $$, glob-8);
                                                                                                fprintf(fdot, "%d -> %d\n", $$, glob-7);
                                                                                                fprintf(fdot, "%d -> %d\n", $$, glob-6);
                                                                                                fprintf(fdot, "%d -> %d\n", $$, glob-5);
                                                                                                fprintf(fdot, "%d -> %d\n", $$, glob-4);        
                                                                                                fprintf(fdot, "%d -> %d\n", $$, glob-3);
                                                                                                fprintf(fdot, "%d -> %d\n", $$, glob-2);
                                                                                                fprintf(fdot, "%d -> %d\n", $$, glob-1);}
        |   LPAREN PRINT expr RPAREN    {glob+=5; fprintf(fdot, "%d [label=program ordering=\"out\"]\n", glob);
                                                fprintf(fdot, "%d [label=\"(\" ordering=\"out\"]\n", glob-4);
                                                fprintf(fdot, "%d [label=print ordering=\"out\"]\n", glob-3);
                                                fprintf(fdot, "%d [label=expr ordering=\"out\"]\n", glob-2);
                                                fprintf(fdot, "%d [label=\")\" ordering=\"out\"]\n", glob-1); $$=glob;
                                                fprintf(fdot, "%d -> %d\n", glob-2, $3);        //link for expr
                                                fprintf(fdot, "%d -> %d\n", $$, glob-4);
                                                fprintf(fdot, "%d -> %d\n", $$, glob-3);
                                                fprintf(fdot, "%d -> %d\n", $$, glob-2);
                                                fprintf(fdot, "%d -> %d\n", $$, glob-1);}       //works
        ;
type    :   TYPE        {glob++; fprintf(fdot, "%d [label=%s ordering=\"out\"]\n", glob, $1); $$ = glob;}       //works
        ;
expr    :   term        {glob++; fprintf(fdot, "%d [label=term ordering=\"out\"]\n", glob); $$ = glob;}  //work
        |   fla         {glob++; fprintf(fdot, "%d [label=fla ordering=\"out\"]\n", glob); $$ = glob;}
        ;
term    :   CONST       {glob++; fprintf(fdot, "%d [label=%s ordering=\"out\"]\n", glob, $1); $$ = glob;
                                fprintf(fdot, "%d -> %d\n", $$+1, glob);}       //works
        |   NAME        {glob++; fprintf(fdot, "%d [label=%s ordering=\"out\"]\n", glob, $1); $$ = glob;
                                fprintf(fdot, "%d -> %d\n", $$+1, glob);}       //works
        |   LPAREN FUNCTION RPAREN     {glob+=3;
                                                fprintf(fdot, "%d [label=\"(\" ordering=\"out\"]\n", glob-2); 
                                                fprintf(fdot, "%d [label=\"%s\" ordering=\"out\"]\n", glob-1, strtok($2, " )"));
                                                fprintf(fdot, "%d [label=\")\" ordering=\"out\"]\n", glob); $$ = glob;
                                                fprintf(fdot, "%d -> %d\n", $$+1, glob-2);
                                                fprintf(fdot, "%d -> %d\n", $$+1, glob-1);
                                                fprintf(fdot, "%d -> %d\n", $$+1, glob);}
        |   LPAREN ADDOP term term RPAREN       {glob+=5;
                                                        fprintf(fdot, "%d [label=\"(\" ordering=\"out\"]\n", glob-4); 
                                                        fprintf(fdot, "%d [label=\"+\" ordering=\"out\"]\n", glob-3); 
                                                        fprintf(fdot, "%d [label=term ordering=\"out\"]\n", glob-2); 
                                                        fprintf(fdot, "%d [label=term ordering=\"out\"]\n", glob-1); 
                                                        fprintf(fdot, "%d [label=\")\" ordering=\"out\"]\n", glob); $$ = glob;
                                                        fprintf(fdot, "%d -> %d\n", $$+1, glob-4);
                                                        fprintf(fdot, "%d -> %d\n", $$+1, glob-3);
                                                        fprintf(fdot, "%d -> %d\n", $$+1, glob-2);
                                                        fprintf(fdot, "%d -> %d\n", $$+1, glob-1);
                                                        fprintf(fdot, "%d -> %d\n", $$+1, glob);}
        |   LPAREN MINOP term term RPAREN       /*{glob+=5;
                                                        fprintf(fdot, "%d [label=\"(\" ordering=\"out\"]\n", glob-4); 
                                                        fprintf(fdot, "%d [label=\"-\" ordering=\"out\"]\n", glob-3); 
                                                        fprintf(fdot, "%d [label=term ordering=\"out\"]\n", glob-2); 
                                                        fprintf(fdot, "%d [label=term ordering=\"out\"]\n", glob-1); 
                                                        fprintf(fdot, "%d [label=\")\" ordering=\"out\"]\n", glob); $$ = glob;
                                                        fprintf(fdot, "%d -> %d\n", $$+1, glob-5);
                                                        fprintf(fdot, "%d -> %d\n", $$+1, glob-4);
                                                        fprintf(fdot, "%d -> %d\n", $$+1, glob-3);
                                                        fprintf(fdot, "%d -> %d\n", $$+1, glob-2);
                                                        fprintf(fdot, "%d -> %d\n", $$+1, glob-1);}*/
        |   LPAREN MULTOP term term RPAREN       /*{glob+=5;
                                                        fprintf(fdot, "%d [label=\"(\" ordering=\"out\"]\n", glob-5); 
                                                        fprintf(fdot, "%d [label=\"*\" ordering=\"out\"]\n", glob-4); 
                                                        fprintf(fdot, "%d [label=term ordering=\"out\"]\n", glob-3); 
                                                        fprintf(fdot, "%d [label=term ordering=\"out\"]\n", glob-2); 
                                                        fprintf(fdot, "%d [label=\")\" ordering=\"out\"]\n", glob-1); $$ = glob;
                                                        fprintf(fdot, "%d -> %d\n", $$+1, glob-5);
                                                        fprintf(fdot, "%d -> %d\n", $$+1, glob-4);
                                                        fprintf(fdot, "%d -> %d\n", $$+1, glob-3);
                                                        fprintf(fdot, "%d -> %d\n", $$+1, glob-2);
                                                        fprintf(fdot, "%d -> %d\n", $$+1, glob-1);}*/
        |   LPAREN DIVOP term term RPAREN       /*{glob+=5;
                                                        fprintf(fdot, "%d [label=\"(\" ordering=\"out\"]\n", glob-5); 
                                                        fprintf(fdot, "%d [label=\"+\" ordering=\"out\"]\n", glob-4); 
                                                        fprintf(fdot, "%d [label=term ordering=\"out\"]\n", glob-3); 
                                                        fprintf(fdot, "%d [label=term ordering=\"out\"]\n", glob-2); 
                                                        fprintf(fdot, "%d [label=\")\" ordering=\"out\"]\n", glob-1); $$ = glob;
                                                        fprintf(fdot, "%d -> %d\n", $$+1, glob-5);
                                                        fprintf(fdot, "%d -> %d\n", $$+1, glob-4);
                                                        fprintf(fdot, "%d -> %d\n", $$+1, glob-3);
                                                        fprintf(fdot, "%d -> %d\n", $$+1, glob-2);
                                                        fprintf(fdot, "%d -> %d\n", $$+1, glob-1);}*/
        |   LPAREN IF fla term term RPAREN
        |   LPAREN NAME RPAREN
        |   LPAREN NAME expr RPAREN
        |   LPAREN NAME expr expr RPAREN
        |   LPAREN LET LPAREN NAME expr RPAREN term RPAREN
        ;
fla     :   BOOLCONST   {glob++; fprintf(fdot, "%d [label=%s ordering=\"out\"]\n", glob, $1); $$ = glob;
                                fprintf(fdot, "%d -> %d\n", $$+1, glob);}       //works
        |   NAME        {glob++; fprintf(fdot, "%d [label=%s ordering=\"out\"]\n", glob, $1); $$ = glob;
                                fprintf(fdot, "%d -> %d\n", $$+1, glob);}       //works
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
