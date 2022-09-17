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
                $$=glob;
                insert(++glob);fprintf(fdot, "%d [label=\"(\" ordering=\"out\"]\n", glob);
                insert(++glob);fprintf(fdot, "%d [label=DEFINE ordering=\"out\"]\n", glob);
                insert(++glob);fprintf(fdot, "%d [label=NAME ordering=\"out\"]\n", glob);
                insert(++glob);fprintf(fdot, "%d [label=type ordering=\"out\"]\n%d -> %d\n", glob,glob,$4);
                insert(++glob);fprintf(fdot, "%d [label=expr ordering=\"out\"]\n%d -> %d\n", glob,glob,$5);
                insert(++glob);fprintf(fdot, "%d [label=\")\" ordering=\"out\"]\n", glob);
                insert(++glob);fprintf(fdot, "%d [label=program ordering=\"out\"]\n", glob);
                while(!isEmpty()){
                        int x = removeData();
                        if(x!=glob){
                                fprintf(fdot,"%d -> %d\n",glob,x);
                        }
                }
                };
        |   LPAREN DEFINE NAME LPAREN NAME type RPAREN type expr RPAREN program{
                $$=glob;
                insert(++glob);fprintf(fdot, "%d [label=\"(\" ordering=\"out\"]\n", glob);
                insert(++glob);fprintf(fdot, "%d [label=DEFINE ordering=\"out\"]\n", glob);
                insert(++glob);fprintf(fdot, "%d [label=NAME ordering=\"out\"]\n", glob);
                insert(++glob);fprintf(fdot, "%d [label=\"(\" ordering=\"out\"]\n", glob);
                insert(++glob);fprintf(fdot, "%d [label=NAME ordering=\"out\"]\n", glob);
                insert(++glob);fprintf(fdot, "%d [label=type ordering=\"out\"]\n%d -> %d\n", glob,glob,$6);
                insert(++glob);fprintf(fdot, "%d [label=\")\" ordering=\"out\"]\n", glob);
                insert(++glob);fprintf(fdot, "%d [label=type ordering=\"out\"]\n%d -> %d\n", glob,glob,$8);                
                insert(++glob);fprintf(fdot, "%d [label=expr ordering=\"out\"]\n%d -> %d\n", glob,glob,$9);
                insert(++glob);fprintf(fdot, "%d [label=\")\" ordering=\"out\"]\n", glob);
                insert(++glob);fprintf(fdot, "%d [label=program ordering=\"out\"]\n", glob);
                while(!isEmpty()){
                        int x = removeData();
                        if(x!=glob){
                                fprintf(fdot,"%d -> %d\n",glob,x);
                        }
                }
                };

        |   LPAREN DEFINE NAME LPAREN NAME type RPAREN LPAREN NAME type RPAREN type expr RPAREN program{

                $$=glob;
                insert(++glob);fprintf(fdot, "%d [label=\"(\" ordering=\"out\"]\n", glob);
                insert(++glob);fprintf(fdot, "%d [label=DEFINE ordering=\"out\"]\n", glob);
                insert(++glob);fprintf(fdot, "%d [label=NAME ordering=\"out\"]\n", glob);
                insert(++glob);fprintf(fdot, "%d [label=\"(\" ordering=\"out\"]\n", glob);
                insert(++glob);fprintf(fdot, "%d [label=NAME ordering=\"out\"]\n", glob);
                insert(++glob);fprintf(fdot, "%d [label=type ordering=\"out\"]\n%d -> %d\n", glob,glob,$6);
                insert(++glob);fprintf(fdot, "%d [label=\")\" ordering=\"out\"]\n", glob);
                insert(++glob);fprintf(fdot, "%d [label=\"(\" ordering=\"out\"]\n", glob);
                insert(++glob);fprintf(fdot, "%d [label=NAME ordering=\"out\"]\n", glob);
                insert(++glob);fprintf(fdot, "%d [label=type ordering=\"out\"]\n%d -> %d\n", glob,glob,$10);
                insert(++glob);fprintf(fdot, "%d [label=\")\" ordering=\"out\"]\n", glob);
                insert(++glob);fprintf(fdot, "%d [label=type ordering=\"out\"]\n%d -> %d\n", glob,glob,$12);                
                insert(++glob);fprintf(fdot, "%d [label=expr ordering=\"out\"]\n%d -> %d\n", glob,glob,$13);
                insert(++glob);fprintf(fdot, "%d [label=\")\" ordering=\"out\"]\n", glob);
                insert(++glob);fprintf(fdot, "%d [label=program ordering=\"out\"]\n", glob);
                while(!isEmpty()){
                        int x = removeData();
                        if(x!=glob){
                                fprintf(fdot,"%d -> %d\n",glob,x);
                        }
                }
                };
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
        |   fla {
                ++glob;fprintf(fdot, "%d [label=fla ordering=\"out\"]\n%d -> %d\n", glob,glob,$1);$$=glob;
                while(!isEmpty()){
                        int x = removeData();
                        if(x!=glob){
                                fprintf(fdot,"%d -> %d\n",glob,x);
                        }
                }
                };
        ;
term    :   CONST {++glob;fprintf(fdot, "%d [label=%s ordering=\"out\"]\n", glob, $1);$$=glob;};
        |   NAME {++glob;fprintf(fdot, "%d [label=%s ordering=\"out\"]\n", glob, $1);$$=glob;};
        |   LPAREN FUNCTION RPAREN{
                insert(++glob);fprintf(fdot, "%d [label=\"(\" ordering=\"out\"]\n", glob);
                insert(++glob);fprintf(fdot, "%d [label=\"\" ordering=\"out\"]\n", glob);
                insert(++glob);fprintf(fdot, "%d [label=\")\" ordering=\"out\"]\n", glob);$$=glob;
        }
        |   LPAREN ADDOP term term RPAREN {
                insert(++glob);fprintf(fdot, "%d [label=\"(\" ordering=\"out\"]\n", glob);
                insert(++glob);fprintf(fdot, "%d [label=\"+\" ordering=\"out\"]\n", glob);
                insert(++glob);fprintf(fdot, "%d [label=term ordering=\"out\"]\n%d -> %d\n", glob,glob,$3);
                insert(++glob);fprintf(fdot, "%d [label=term ordering=\"out\"]\n%d -> %d\n", glob,glob,$4);
                insert(++glob);fprintf(fdot, "%d [label=\")\" ordering=\"out\"]\n", glob);$$=glob;
                }
        |   LPAREN MULTOP term term RPAREN {
                insert(++glob);fprintf(fdot, "%d [label=\"(\" ordering=\"out\"]\n", glob);
                insert(++glob);fprintf(fdot, "%d [label=\"*|div|mod\" ordering=\"out\"]\n", glob);
                insert(++glob);fprintf(fdot, "%d [label=term ordering=\"out\"]\n%d -> %d\n", glob,glob,$3);
                insert(++glob);fprintf(fdot, "%d [label=term ordering=\"out\"]\n%d -> %d\n", glob,glob,$4);
                insert(++glob);fprintf(fdot, "%d [label=\")\" ordering=\"out\"]\n", glob);$$=glob;
                }
        |   LPAREN IF fla term term RPAREN{
                insert(++glob);fprintf(fdot, "%d [label=\"(\" ordering=\"out\"]\n", glob);
                insert(++glob);fprintf(fdot, "%d [label=IF ordering=\"out\"]\n", glob);
                insert(++glob);fprintf(fdot, "%d [label=fla ordering=\"out\"]\n%d -> %d\n", glob,glob,$3);
                insert(++glob);fprintf(fdot, "%d [label=term ordering=\"out\"]\n%d -> %d\n", glob,glob,$4);
                insert(++glob);fprintf(fdot, "%d [label=term ordering=\"out\"]\n%d -> %d\n", glob,glob,$5);
                insert(++glob);fprintf(fdot, "%d [label=\")\" ordering=\"out\"]\n", glob);$$=glob;
                }
        |   LPAREN NAME RPAREN {
                insert(++glob);fprintf(fdot, "%d [label=\"(\" ordering=\"out\"]\n", glob, $1);
                insert(++glob);fprintf(fdot, "%d [label=NAME ordering=\"out\"]\n",glob);
                insert(++glob);fprintf(fdot, "%d [label=\")\" ordering=\"out\"]\n", glob, $3);$$=glob;
                }
        |   LPAREN NAME expr RPAREN{
                insert(++glob);fprintf(fdot, "%d [label=\"(\" ordering=\"out\"]\n", glob, $1);
                insert(++glob);fprintf(fdot, "%d [label=NAME ordering=\"out\"]\n",glob);
                insert(++glob);fprintf(fdot, "%d [label=expr ordering=\"out\"]\n%d -> %d\n", glob,glob,$3);                
                insert(++glob);fprintf(fdot, "%d [label=\")\" ordering=\"out\"]\n", glob, $3);$$=glob;
                }
        |   LPAREN NAME expr expr RPAREN{
                insert(++glob);fprintf(fdot, "%d [label=\"(\" ordering=\"out\"]\n", glob);
                insert(++glob);fprintf(fdot, "%d [label=NAME ordering=\"out\"]\n",glob);
                insert(++glob);fprintf(fdot, "%d [label=expr ordering=\"out\"]\n%d -> %d\n", glob,glob,$3);   
                insert(++glob);fprintf(fdot, "%d [label=expr ordering=\"out\"]\n%d -> %d\n", glob,glob,$4);                             
                insert(++glob);fprintf(fdot, "%d [label=\")\" ordering=\"out\"]\n", glob, $3);$$=glob;
                
                }
        |   LPAREN LET LPAREN NAME expr RPAREN term RPAREN{
                insert(++glob);fprintf(fdot, "%d [label=\"(\" ordering=\"out\"]\n", glob);
                insert(++glob);fprintf(fdot, "%d [label=LET ordering=\"out\"]\n", glob);
                insert(++glob);fprintf(fdot, "%d [label=\"(\" ordering=\"out\"]\n", glob);
                insert(++glob);fprintf(fdot, "%d [label=NAME ordering=\"out\"]\n",glob);
                insert(++glob);fprintf(fdot, "%d [label=expr ordering=\"out\"]\n%d -> %d\n", glob,glob,$5);
                insert(++glob);fprintf(fdot, "%d [label=\")\" ordering=\"out\"]\n", glob, $3);$$=glob;   
                insert(++glob);fprintf(fdot, "%d [label=term ordering=\"out\"]\n%d -> %d\n", glob,glob,$7);                             
                insert(++glob);fprintf(fdot, "%d [label=\")\" ordering=\"out\"]\n", glob, $3);$$=glob;
                
                }
        ;
fla     :   BOOLCONST {++glob;fprintf(fdot, "%d [label=const ordering=\"out\"]\n", glob);$$=glob;};
        |   NAME {++glob;fprintf(fdot, "%d [label=NAME ordering=\"out\"]\n", glob);$$=glob;};
        |   LPAREN FUNCTION RPAREN{
                insert(++glob);fprintf(fdot, "%d [label=\"(\" ordering=\"out\"]\n", glob, $1);
                insert(++glob);fprintf(fdot, "%d [label=FUNCTION ordering=\"out\"]\n",glob);
                insert(++glob);fprintf(fdot, "%d [label=\")\" ordering=\"out\"]\n", glob);$$=glob;
                }
        |   LPAREN COMPARATOR term term RPAREN{
                insert(++glob);fprintf(fdot, "%d [label=\"(\" ordering=\"out\"]\n", glob, $1);
                insert(++glob);fprintf(fdot, "%d [label=COMPARATOR ordering=\"out\"]\n",glob);
                insert(++glob);fprintf(fdot, "%d [label=term ordering=\"out\"]\n%d -> %d\n", glob,glob,$3);                             
                insert(++glob);fprintf(fdot, "%d [label=term ordering=\"out\"]\n%d -> %d\n", glob,glob,$4);                             
                insert(++glob);fprintf(fdot, "%d [label=\")\" ordering=\"out\"]\n", glob);$$=glob;
                }
        |   LPAREN NOT fla RPAREN{
                insert(++glob);fprintf(fdot, "%d [label=\"(\" ordering=\"out\"]\n", glob, $1);
                insert(++glob);fprintf(fdot, "%d [label=NOT ordering=\"out\"]\n",glob);
                insert(++glob);fprintf(fdot, "%d [label=fla ordering=\"out\"]\n%d -> %d\n", glob,glob,$3);                             
                insert(++glob);fprintf(fdot, "%d [label=\")\" ordering=\"out\"]\n", glob);$$=glob;
                }
        |   LPAREN BOOLOP term term RPAREN{
                insert(++glob);fprintf(fdot, "%d [label=\"(\" ordering=\"out\"]\n", glob, $1);
                insert(++glob);fprintf(fdot, "%d [label=BOOLOP ordering=\"out\"]\n",glob);
                insert(++glob);fprintf(fdot, "%d [label=term ordering=\"out\"]\n%d -> %d\n", glob,glob,$3);
                insert(++glob);fprintf(fdot, "%d [label=term ordering=\"out\"]\n%d -> %d\n", glob,glob,$4);                            
                insert(++glob);fprintf(fdot, "%d [label=\")\" ordering=\"out\"]\n", glob);$$=glob;
        }
        |   LPAREN IF fla fla fla RPAREN{
                insert(++glob);fprintf(fdot, "%d [label=\"(\" ordering=\"out\"]\n", glob, $1);
                insert(++glob);fprintf(fdot, "%d [label=IF ordering=\"out\"]\n",glob);
                insert(++glob);fprintf(fdot, "%d [label=fla ordering=\"out\"]\n%d -> %d\n", glob,glob,$3);
                insert(++glob);fprintf(fdot, "%d [label=fla ordering=\"out\"]\n%d -> %d\n", glob,glob,$4);    
                insert(++glob);fprintf(fdot, "%d [label=fla ordering=\"out\"]\n%d -> %d\n", glob,glob,$5);                                    
                insert(++glob);fprintf(fdot, "%d [label=\")\" ordering=\"out\"]\n", glob);$$=glob;
  
        }
        |   LPAREN NAME RPAREN{
                insert(++glob);fprintf(fdot, "%d [label=\"(\" ordering=\"out\"]\n", glob, $1);
                insert(++glob);fprintf(fdot, "%d [label=NAME ordering=\"out\"]\n",glob);
                insert(++glob);fprintf(fdot, "%d [label=\")\" ordering=\"out\"]\n", glob, $3);$$=glob;
                }
        |   LPAREN NAME expr RPAREN{
                insert(++glob);fprintf(fdot, "%d [label=\"(\" ordering=\"out\"]\n", glob, $1);
                insert(++glob);fprintf(fdot, "%d [label=NAME ordering=\"out\"]\n",glob);
                insert(++glob);fprintf(fdot, "%d [label=expr ordering=\"out\"]\n%d -> %d\n", glob,glob,$3);
                insert(++glob);fprintf(fdot, "%d [label=\")\" ordering=\"out\"]\n", glob, $3);$$=glob;
                }
        |   LPAREN NAME expr expr RPAREN{
                insert(++glob);fprintf(fdot, "%d [label=\"(\" ordering=\"out\"]\n", glob);
                insert(++glob);fprintf(fdot, "%d [label=NAME ordering=\"out\"]\n",glob);
                insert(++glob);fprintf(fdot, "%d [label=expr ordering=\"out\"]\n%d -> %d\n", glob,glob,$3);
                insert(++glob);fprintf(fdot, "%d [label=expr ordering=\"out\"]\n%d -> %d\n", glob,glob,$4);
                insert(++glob);fprintf(fdot, "%d [label=\")\" ordering=\"out\"]\n", glob);$$=glob;
                }
        |   LPAREN LET LPAREN NAME expr RPAREN fla RPAREN{
                insert(++glob);fprintf(fdot, "%d [label=\"(\" ordering=\"out\"]\n", glob);
                insert(++glob);fprintf(fdot, "%d [label=LET ordering=\"out\"]\n", glob);
                insert(++glob);fprintf(fdot, "%d [label=\"(\" ordering=\"out\"]\n", glob);
                insert(++glob);fprintf(fdot, "%d [label=\"NAME\" ordering=\"out\"]\n",glob);
                insert(++glob);fprintf(fdot, "%d [label=expr ordering=\"out\"]\n%d -> %d\n", glob,glob,$5);
                insert(++glob);fprintf(fdot, "%d [label=\")\" ordering=\"out\"]\n", glob);$$=glob;    
                insert(++glob);fprintf(fdot, "%d [label=fla ordering=\"out\"]\n%d -> %d\n", glob,glob,$7);                                    
                insert(++glob);fprintf(fdot, "%d [label=\")\" ordering=\"out\"]\n", glob);$$=glob;
                }
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
