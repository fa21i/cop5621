

%{
        #include <stdio.h>
        #include <string.h>
        #include <stdlib.h>
        #include "ast.h"
        int yylex();
        
	void yyerror(char *s);
        int glob = 0;
        int a[250];
        int i;
        char s[3];
        char * getStr(char* a){
                // free(s);
                // strncpy(s,a,2);
                // s[2] = '\0';
                // fprintf(fdot,"GetSTR: %s -> %s\n",a,strtok(a," "));
                return strdup(strtok(strtok(a," "),")"));
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
                insert_child(insert_node(getStr($3),NAME));
                insert_child($4);
                insert_child($5);
                // insert_child($7);
                $$ = insert_node("define-fun",DEFINE);
                };
        |   LPAREN DEFINE NAME LPAREN NAME type RPAREN type expr RPAREN program{
                int name1 = insert_node(getStr($3),NAME);
                int name2 = insert_node(getStr($5),NAME);
                insert_child(name1);
                insert_child(name2);
                insert_child($6);
                insert_child($8);
                insert_child($9);
                // insert_child($11);
                $$ = insert_node("define-fun",DEFINE);
                };

        |   LPAREN DEFINE NAME LPAREN NAME type RPAREN LPAREN NAME type RPAREN type expr RPAREN program{
                int name1 = insert_node(getStr($3),NAME);
                int name2 = insert_node(getStr($5),NAME);
                int name3 = insert_node(getStr($9),NAME);
                insert_child(name1);
                insert_child(name2);
                insert_child(name3);
                insert_child($6);
                insert_child($10);
                insert_child($12);
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
                $$ = insert_node(getStr($1),CONST);
                // fprintf(fdot, "$$: %d ------------> $1: %s\n",$$,strtok($1, " "));
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
                $$ = insert_node("-",MINOP);
                };
        |   LPAREN MULTOP expr expr RPAREN {
                insert_child($3);
                insert_child($4);
                $$ = insert_node("*",MULTOP);
                };
        |   LPAREN IF expr expr expr RPAREN{
                insert_child($3);
                insert_child($4);
                insert_child($5);
                $$ = insert_node("IF",IF) ;
                };
        |   LPAREN NAME RPAREN {
                $$ = insert_node(getStr($2),NAME);
                };
        |   LPAREN NAME expr RPAREN{
                insert_child($3);
                $$ = insert_node(getStr($2),NAME);
                };
        |   LPAREN NAME expr expr RPAREN{
                insert_child($3);
                insert_child($4);
                $$ = insert_node(getStr($2),NAME);
                };
        |   LPAREN LET LPAREN NAME expr RPAREN expr RPAREN{
                int loc = insert_node(getStr($4),NAME);
                insert_child(loc);
                insert_child($5);                
                insert_child($7);
                $$ = insert_node("LET",LET);
                };
        ;
fla     :   TRUECONST {
                $$ = insert_node("True",TRUECONST);
                };
        |   FALSECONST {
                $$ = insert_node("False",FALSECONST);
                };
        ;
        |   LPAREN GETBOOL RPAREN{
                $$ = insert_node("get-bool",GETBOOL);
                };
        |   LPAREN LT expr expr RPAREN{
                insert_child($3);
                insert_child($4);
                $$ = insert_node("LT",LT);
                
                };
        |   LPAREN GT expr expr RPAREN{
                insert_child($3);
                insert_child($4);
                $$ = insert_node("GT",GT);
                
                };
        |   LPAREN EQ expr expr RPAREN{
                insert_child($3);
                insert_child($4);
                $$ = insert_node("EQ",EQ);
                
                };
        |   LPAREN LTEQ expr expr RPAREN{
                insert_child($3);
                insert_child($4);
                $$ = insert_node("LTEQ",LTEQ);
                
                };
        |   LPAREN GTEQ expr expr RPAREN{
                insert_child($3);
                insert_child($4);
                $$ = insert_node("GTEQ",GTEQ);
                
                };
        |   LPAREN NOT expr RPAREN{
                insert_child($3);
                $$ = insert_node("Not",NOT);
                };
        |   LPAREN AND expr expr RPAREN{
                insert_child($3);
                insert_child($4);
                $$ = insert_node("AND",AND);
        };
        |   LPAREN OR expr expr RPAREN{
                insert_child($3);
                insert_child($4);
                $$ = insert_node("OR",OR);
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


<<<<<<< HEAD
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



/*
prog : sum {
        int a1 = insert_node(token: "1", ntoken: 0);
        int a2 = insert_node("x", 0);
        int a3 = insert_node("y", 0);
        insert_children(3, a1, a2, a3);
        insert_node("+", 0);
}


sum: arg {
                insert_child($1);
                $$ = $1 //$$=insert_node("+", PLUS);
        }
        |
        sum PLUS arg {
                inert_child($1);
                isnert_child($3);
                OR
                insert_children(2, $1, $3);
                $$ = insert_node("+", PLUS);
        }

arg: ID {
        $$ = insert_node(strdup($1), ID);
}
}*/
=======
>>>>>>> main
