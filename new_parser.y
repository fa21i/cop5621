%{
        #include <stdio.h>
        #include <string.h>
        #include <stdlib.h>
        #include "ast.h"
        int yylex();
        FILE *fdot;
	void yyerror(char *s);
        int glob = 0;
        int a[250];
        int i;
        char s[3];
        extern FILE *yyin;
        char * getStr(char* a){
                // free(s);
                // strncpy(s,a,2);
                // s[2] = '\0';
                // fprintf(fdot,"GetSTR: %s -> %s\n",a,strtok(a," "));
                return strdup(strtok(a," "));
                // return s;
        }
%}

%union {int val; char* str;}
%start program
%token <str> NAME CONST 
%token <val> LT GT EQ LTEQ GTEQ ADDOP MINOP DEFINE GETINT GETBOOL AND OR IF LET INTTYPE BOOLTYPE PRINT RPAREN LPAREN NOT MULTOP TRUECONST FALSECONST
%type <val> program type expr term fla 


%%
program :   LPAREN DEFINE NAME type expr RPAREN program  {
                insert_child(insert_node(getStr($3),1));
                insert_child($4);
                insert_child($5);
                // insert_child($7);
                $$ = insert_node("define-fun",DEFINE);
                };
        |   LPAREN DEFINE NAME LPAREN NAME type RPAREN type expr RPAREN program{
                int name1 = insert_node(getStr($3),1);
                int name2 = insert_node(getStr($5),1);
                insert_child(name1);
                insert_child(name2);
                insert_child($6);
                insert_child($8);
                insert_child($9);
                // insert_child($11);
                $$ = insert_node("define-fun",DEFINE);
                };

        |   LPAREN DEFINE NAME LPAREN NAME type RPAREN LPAREN NAME type RPAREN type expr RPAREN program{
                int name1 = insert_node(getStr($3),1);
                int name2 = insert_node(getStr($5),1);
                int name3 = insert_node(getStr($9),1);
                insert_child(name1);
                insert_child(name2);
                insert_child(name3);
                insert_child($13);
                // insert_child($15);
                $$ = insert_node("define-fun",DEFINE);
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
                $$ = $1;
                // $$ = insert_node($1,1);
                
                };
        |   fla {
                $$ = $1;
                // insert_child($1);
                // $$ = insert_node("fla",1);
                };
        ;
term    :   CONST {
                $$ = insert_node(getStr($1),1);
                // fprintf(fdot, "$$: %d ------------> $1: %s\n",$$,strtok($1, " "));
                };
        |   NAME {
                $$ = insert_node(getStr($1),1);
                // fprintf(fdot, "$$: %d ------------> $1: %s\n",$$,strtok($1, " "));

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
                $$ = insert_node(getStr($2),1);
                };
        |   LPAREN NAME expr RPAREN{
                insert_child($3);
                $$ = insert_node(getStr($2),1);
                };
        |   LPAREN NAME expr expr RPAREN{
                insert_child($3);
                insert_child($4);
                $$ = insert_node(getStr($2),1);
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
                $$ = insert_node("True",1);
                };
        |   FALSECONST {
                $$ = insert_node("False",1);
                };
        ;
        |   LPAREN GETBOOL RPAREN{
                $$ = insert_node("get-bool",GETINT);
                };
        |   LPAREN LT expr expr RPAREN{
                insert_child($3);
                insert_child($4);
                $$ = insert_node("LT",1);
                
                };
        |   LPAREN GT expr expr RPAREN{
                insert_child($3);
                insert_child($4);
                $$ = insert_node("GT",1);
                
                };
        |   LPAREN EQ expr expr RPAREN{
                insert_child($3);
                insert_child($4);
                $$ = insert_node("EQ",1);
                
                };
        |   LPAREN LTEQ expr expr RPAREN{
                insert_child($3);
                insert_child($4);
                $$ = insert_node("LTEQ",1);
                
                };
        |   LPAREN GTEQ expr expr RPAREN{
                insert_child($3);
                insert_child($4);
                $$ = insert_node("GT EQ",1);
                
                };
        |   LPAREN NOT expr RPAREN{
                insert_child($3);
                $$ = insert_node("Not",1);
                };
        |   LPAREN AND expr expr RPAREN{
                insert_child($3);
                insert_child($4);
                $$ = insert_node("AND",1);
        };
        |   LPAREN OR expr expr RPAREN{
                insert_child($3);
                insert_child($4);
                $$ = insert_node("OR",1);
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
        yyin=fopen("tests/correct programs/sample4.txt ","r+");
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
