%{
        #include <stdio.h>
        #define _SVID_SOURCE
        #define _POSIX_C_SOURCE 200809L
        #include <string.h>
        #include <stdlib.h>
        #include "ast.h"
        int yylex();
        FILE *fdot;
	void yyerror(char *s);
        int glob = 0;
        int a[250];
        int i;
        char* s;
        extern FILE *yyin;
        char * getStr(char* a){
                return strtok(a, " ");
        }
%}

%union {int val; char* str;}
%start program
%token <str> NAME CONST 
%token <val> COMPARATOR ADDOP MINOP DEFINE GETINT GETBOOL AND OR IF LET INTTYPE BOOLTYPE PRINT RPAREN LPAREN NOT MULTOP TRUECONST FALSECONST
%type <val> program type expr term fla 


%%
program :   LPAREN DEFINE NAME type expr RPAREN program  {
                
                };
        |   LPAREN DEFINE NAME LPAREN NAME type RPAREN type expr RPAREN program{
                insert_child($6);
                insert_child($8);
                insert_child($9);
                insert_child($10);
                $$ = insert_node("Define",DEFINE);
                };

        |   LPAREN DEFINE NAME LPAREN NAME type RPAREN LPAREN NAME type RPAREN type expr RPAREN program{
                };
        |   LPAREN PRINT expr RPAREN {
                int main_loc = insert_node("main",1);
                insert_child(main_loc);
                insert_child($3);
                $$ = insert_node("ENTRY",2);
                };
        ;
type    :   INTTYPE {
                $$ = insert_node("int",INTTYPE);
                };
                
        |   BOOLTYPE {
                $$ = insert_node("bool",BOOLTYPE);
                };
        ;
expr    :   term {
                // insert_child($1);
                // $$ = insert_node("term",1);
                
                };
        |   fla {
                // insert_child($1);
                // $$ = insert_node("fla",1);
                };
        ;
term    :   CONST {
                $$ = insert_node(getStr($1),CONST);

                };
        |   NAME {
                $$ = insert_node(getStr($1),NAME);

                };
        |   LPAREN GETINT RPAREN{
                $$ = insert_node("get-int",GETINT);
        };
        |   LPAREN ADDOP expr expr RPAREN {
                insert_child($3);
                insert_child($4);
                $$ = insert_node("PLUS",ADDOP);
                };
        |   LPAREN MINOP expr expr RPAREN {
                insert_child($3);
                insert_child($4);
                $$ = insert_node("-",1);
                };
        |   LPAREN MULTOP expr expr RPAREN {
                insert_child($3);
                insert_child($4);
                $$ = insert_node("*",1);
                };
        |   LPAREN IF expr expr expr RPAREN{
                insert_child($3);
                insert_child($4);
                insert_child($5);
                $$ = insert_node("IF",1) ;
                };
        |   LPAREN NAME RPAREN {
                $$ = insert_node($2,1);
                };
        |   LPAREN NAME expr RPAREN{
                insert_child($3);
                $$ = insert_node("NAME",1);
                };
        |   LPAREN NAME expr expr RPAREN{
                insert_child($3);
                insert_child($4);
                $$ = insert_node("NAME",1);
                };
        |   LPAREN LET LPAREN NAME expr RPAREN expr RPAREN{
                int loc = insert_node(getStr($4),1);
                insert_child(loc);
                insert_child($5);                
                insert_child($7);
                $$ = insert_node("LET",1);
                };
        ;
fla     :   TRUECONST {
                };
        |   FALSECONST {
                };
        ;
        |   LPAREN GETBOOL RPAREN{
                };
        |   LPAREN COMPARATOR expr expr RPAREN{
                };
        |   LPAREN NOT expr RPAREN{
                };
        |   LPAREN AND expr expr RPAREN{
        };
        |   LPAREN OR expr expr RPAREN{
        };
        ;
%%

/*arg : ID {glob ++; printf("id: %s, %d\n", $1, glob); $$ = glob; }
| CONST {glob ++; printf("const: %s, %d\n", $1, glob); $$ = glob; };
*/

void yyerror(char *s)
{  
        fprintf (stdout, "%s\n", s);
}

int main(int argc, char* argv[])
{
        char filename[50];

        fdot = fopen("parse_tree.dot", "w+");
        fprintf(fdot, "digraph print {\n");     
        yyin=fopen("tests/correct programs/sample1.txt ","r+");
        if(yyin==NULL)
        {
                printf("Failed");
                print_ast();
                return 0;
        }
        else 
        {
                yyparse();
                print_ast();
                free_ast();
        }
        fprintf(fdot, "}\n");
        fclose(fdot); 
        return 0;
        
}
