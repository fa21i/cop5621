%{
        #include <stdio.h>
        #include "queue.c"
        int yylex();
        FILE *fdot;
	void yyerror(char *s);
        int glob = 0;
        int arr[50];
        extern FILE *yyin;
%}

%union {int val; char* str;}
%start program
%token <str> NAME
%token <val> COMPARATOR MULTOP ADDOP DEFINE FUNCTION BOOLCONST BOOLOP IF LET TYPE PRINT RPAREN LPAREN NOT CONST
%type <val> program type expr term fla 


%%
program :   LPAREN DEFINE NAME type expr RPAREN program  {
                insert(++glob);fprintf(fdot, "%d [label=program ordering=\"out\"]\n", glob);
                insert(++glob);fprintf(fdot, "%d [label=define-fun ordering=\"out\"]\n", glob);
                insert(++glob);fprintf(fdot, "%d [label=print ordering=\"out\"]\n", glob);
                insert(++glob);fprintf(fdot, "%d [label=expr ordering=\"out\"]\n", glob);
                insert(++glob);fprintf(fdot, "%d [label=\")\" ordering=\"out\"]\n", glob);
                };  
        |   LPAREN DEFINE NAME LPAREN NAME type RPAREN type expr RPAREN program
        |   LPAREN DEFINE NAME LPAREN NAME type RPAREN LPAREN NAME type RPAREN type expr RPAREN program
        |   LPAREN PRINT expr RPAREN {
                
                $$=glob;
                insert(++glob);fprintf(fdot, "%d [label=\"(\" ordering=\"out\"]\n", glob);
                insert(++glob);fprintf(fdot, "%d [label=PRINT ordering=\"out\"]\n", glob);
                insert(++glob);fprintf(fdot, "%d [label=expr ordering=\"out\"]\n%d -> %d\n", glob,glob,$3);
                insert(++glob);fprintf(fdot, "%d [label=\")\" ordering=\"out\"]\n", glob);
                insert(++glob);fprintf(fdot, "%d [label=program ordering=\"out\"]\n", glob);
                while(!isEmpty()){
                        int x = removeData();
                        if(x!=glob){
                                fprintf(fdot,"%d -> %d\n",glob,x);
                        }
                        
                }
                };
        ;
type    :   TYPE {++glob;fprintf(fdot, "%d [label=type ordering=\"out\"]\n", glob);$$=glob;};
        ;
expr    :   term {
                insert(++glob);fprintf(fdot, "%d [label=term ordering=\"out\"]\n", glob);$$=glob;
                while(!isEmpty()){
                        int x = removeData();
                        if(x!=glob){
                                fprintf(fdot,"%d -> %d\n",glob,x);
                        }
                        
                }
                };
        |   fla {++glob;fprintf(fdot, "%d [label=fla ordering=\"out\"]\n%d -> %d\n", glob,glob,$1);$$=glob;};
        ;
term    :   CONST {++glob;fprintf(fdot, "%d [label=%s ordering=\"out\"]\n", glob, $1);$$=glob;};
        |   NAME {++glob;fprintf(fdot, "%d [label=%s ordering=\"out\"]\n", glob, $1);$$=glob;};
        |   LPAREN FUNCTION RPAREN
        |   LPAREN ADDOP term term RPAREN {
                insert(++glob);fprintf(fdot, "%d [label=\"(\" ordering=\"out\"]\n", glob);
                insert(++glob);fprintf(fdot, "%d [label=\"+\" ordering=\"out\"]\n", glob);
                insert(++glob);fprintf(fdot, "%d [label=term ordering=\"out\"]\n%d -> %d\n", glob,glob,$3);
                insert(++glob);fprintf(fdot, "%d [label=term ordering=\"out\"]\n%d -> %d\n", glob,glob,$4);
                insert(++glob);fprintf(fdot, "%d [label=\")\" ordering=\"out\"]\n", glob);$$=glob;
                }
        |   LPAREN MULTOP term term RPAREN
        |   LPAREN IF fla term term RPAREN
        |   LPAREN NAME RPAREN {
                insert(++glob);fprintf(fdot, "%d [label=\"(\" ordering=\"out\"]\n", glob, $1);
                insert(++glob);fprintf(fdot, "%d [label=\"%s\" ordering=\"out\"]\n", glob, strtok($2, " )"));
                insert(++glob);fprintf(fdot, "%d [label=\")\" ordering=\"out\"]\n", glob, $3);$$=glob;
                }
        |   LPAREN NAME expr RPAREN
        |   LPAREN NAME expr expr RPAREN
        |   LPAREN LET LPAREN NAME expr RPAREN term RPAREN
        ;
fla     :   BOOLCONST {++glob;fprintf(fdot, "%d [label=const ordering=\"out\"]\n", glob);$$=glob;};
        |   NAME {++glob;fprintf(fdot, "%d [label=const ordering=\"out\"]\n", glob);$$=glob;};
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
void yyerror(char * s)
{  
        fprintf (stderr, "%s\n", s);
}

int main(int argc, char* argv[])
{
        FILE *fp;
        char filename[50];

        fdot = fopen("parse_tree.dot", "w+");
        fprintf(fdot, "digraph print {\n");
 
        yyin=fopen("sample.txt","r+");
        if(yyin==NULL)
        {
                return 0;
        }
        else 
        {
                yyparse();
        }
        fprintf(fdot, "}\n");
        fclose(fdot); 
        fclose(fp);
        return 0;
        
}
