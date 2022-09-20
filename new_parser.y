%{
        #include <stdio.h>
        #include "queue.c"
        #include "stack.c"
        int yylex();
        FILE *fdot;
	void yyerror(char *s);
        int glob = 0;
        int arr[50];
        extern FILE *yyin;
%}

%union {int val; char* str;}
%start program
%token <str> NAME CONST 
%token <val> COMPARATOR ADDOP MINOP DEFINE GETINT GETBOOL AND OR IF LET INTTYPE BOOLTYPE PRINT RPAREN LPAREN NOT MULTOP TRUECONST FALSECONST

%type <val> program type expr term fla 


%%
program :   LPAREN DEFINE NAME type expr RPAREN program  {
                $$=glob;
                int prog_pos = glob;
                insert(++glob);fprintf(fdot, "%d [label=\"(\" ordering=\"out\"]\n", glob);
                insert(++glob);fprintf(fdot, "%d [label=DEFINE ordering=\"out\"]\n", glob);
                insert(++glob);fprintf(fdot, "%d [label=NAME ordering=\"out\"]\n", glob);
                insert(++glob);fprintf(fdot, "%d [label=type ordering=\"out\"]\n%d -> %d\n", glob,glob,$4);
                insert(++glob);fprintf(fdot, "%d [label=expr ordering=\"out\"]\n%d -> %d\n", glob,glob,$5);
                insert(++glob);fprintf(fdot, "%d [label=\")\" ordering=\"out\"]\n", glob);
                insert(prog_pos);
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
                int prog_pos = glob;
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
                insert(prog_pos);
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
                int prog_pos = glob;
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
                insert(prog_pos);
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
type    :       INTTYPE {++glob;fprintf(fdot, "%d [label=int ordering=\"out\"]\n", glob);$$=glob;};
        |       BOOLTYPE {++glob;fprintf(fdot, "%d [label=bool ordering=\"out\"]\n", glob);$$=glob;};
        ;
expr    :   term {
                insert(++glob);fprintf(fdot, "%d [label=term ordering=\"out\"]\n", glob,$1);$$=glob;
                while(!isEmpty()){
                        int x = removeData();
                        if(x!=glob){
                                fprintf(fdot,"%d -> %d\n",glob,x);
                        }
                }
                };
        |   fla {
                insert(++glob);fprintf(fdot, "%d [label=fla2 ordering=\"out\"]\n", glob,$1);$$=glob;
                while(!isEmpty()){
                        int x = removeData();
                        if(x!=glob){
                                fprintf(fdot,"%d -> %d\n",glob,x);
                        }
                }
                };
        ;
term    :   CONST {insert(++glob);fprintf(fdot, "%d [label=%s ordering=\"out\"]\n", glob, $1);$$=glob;};
        |   NAME {insert(++glob);fprintf(fdot, "%d [label=%s ordering=\"out\"]\n", glob,$1);$$=glob;};
        |   LPAREN GETINT RPAREN{
                $$=glob;
                insert(++glob);fprintf(fdot, "%d [label=\"(\" ordering=\"out\"]\n", glob);
                insert(++glob);fprintf(fdot, "%d [label=get_int ordering=\"out\"]\n", glob);
                insert(++glob);fprintf(fdot, "%d [label=\")\" ordering=\"out\"]\n", glob);
        };
        |   LPAREN ADDOP term term RPAREN {
                insert(++glob);fprintf(fdot, "%d [label=\"(\" ordering=\"out\"]\n", glob);
                insert(++glob);fprintf(fdot, "%d [label=\"+\" ordering=\"out\"]\n", glob);
                insert(++glob);fprintf(fdot, "%d [label=term2 ordering=\"out\"]\n", glob,$3);
                int x;
                do{
                        x = removeData();
                        fprintf(fdot,"%d -> %d\n",glob,x);
                }while(x<$3&&x!=0);
                insert(++glob);fprintf(fdot, "%d [label=term2 ordering=\"out\"]\n", glob,$4);
                do{
                        x = removeData();
                        fprintf(fdot,"%d -> %d\n",glob,x);
                }while(x<$4&&x!=0);
                insert(++glob);fprintf(fdot, "%d [label=\")\" ordering=\"out\"]\n", glob);$$=glob;
                };
        |   LPAREN MINOP term term RPAREN {
                insert(++glob);fprintf(fdot, "%d [label=\"(\" ordering=\"out\"]\n", glob);
                insert(++glob);fprintf(fdot, "%d [label=\"-\" ordering=\"out\"]\n", glob);
                insert(++glob);fprintf(fdot, "%d [label=term ordering=\"out\"]\n", glob,$3);
                int x;
                do{
                        x = removeData();
                        fprintf(fdot,"%d -> %d\n",glob,x);
                }while(x!=$3&&x!=0);
                insert(++glob);fprintf(fdot, "%d [label=term ordering=\"out\"]\n", glob,$4);
                do{
                        x = removeData();
                        fprintf(fdot,"%d -> %d\n",glob,x);
                }while(x!=$4&&x!=0);
                insert(++glob);fprintf(fdot, "%d [label=\")\" ordering=\"out\"]\n", glob);$$=glob;
                };
        |   LPAREN MULTOP term term RPAREN {
                $$=glob;
                insert(++glob);fprintf(fdot, "%d [label=\"(\" ordering=\"out\"]\n", glob);
                insert(++glob);fprintf(fdot, "%d [label=\"*|div|mod\" ordering=\"out\"]\n", glob);
                insert(++glob);fprintf(fdot, "%d [label=term ordering=\"out\"]\n", glob,$3);
                int x;
                do{
                        x = removeData();
                        fprintf(fdot,"%d -> %d\n",glob,x);
                }while(x!=$3&&x!=0);
                insert(++glob);fprintf(fdot, "%d [label=term ordering=\"out\"]\n", glob,$4);
                do{
                        x = removeData();
                        fprintf(fdot,"%d -> %d\n",glob,x);
                }while(x!=$4&&x!=0);
                insert(++glob);fprintf(fdot, "%d [label=\")\" ordering=\"out\"]\n", glob);
                };
        |   LPAREN IF fla term term RPAREN{
                $$=glob;
                insert(++glob);fprintf(fdot, "%d [label=\"(\" ordering=\"out\"]\n", glob);
                insert(++glob);fprintf(fdot, "%d [label=IF ordering=\"out\"]\n", glob);
                insert(++glob);fprintf(fdot, "%d [label=fla ordering=\"out\"]\n", glob,$3);
                int x;
                do{
                        x = removeData();
                        fprintf(fdot,"%d -> %d\n",glob,x);
                }while(x!=$3&&x!=0);
                insert(++glob);fprintf(fdot, "%d [label=term ordering=\"out\"]\n", glob,$4);
                do{
                        x = removeData();
                        fprintf(fdot,"%d -> %d\n",glob,x);
                }while(x!=$4&&x!=0);
                insert(++glob);fprintf(fdot, "%d [label=term ordering=\"out\"]\n", glob,$5);
                do{
                        x = removeData();
                        fprintf(fdot,"%d -> %d\n",glob,x);
                }while(x!=$5&&x!=0);
                insert(++glob);fprintf(fdot, "%d [label=\")\" ordering=\"out\"]\n", glob);
                };
        |   LPAREN NAME RPAREN {
                insert(++glob);fprintf(fdot, "%d [label=\"(\" ordering=\"out\"]\n", glob);
                insert(++glob);fprintf(fdot, "%d [label=NAME ordering=\"out\"]\n",glob);
                insert(++glob);fprintf(fdot, "%d [label=\")\" ordering=\"out\"]\n", glob);$$=glob;
                }
        |   LPAREN NAME expr RPAREN{
                insert(++glob);fprintf(fdot, "%d [label=\"(\" ordering=\"out\"]\n", glob);
                insert(++glob);fprintf(fdot, "%d [label=NAME ordering=\"out\"]\n",glob);
                insert(++glob);fprintf(fdot, "%d [label=expr ordering=\"out\"]\n%d -> %d\n", glob,glob,$3);
                insert(++glob);fprintf(fdot, "%d [label=\")\" ordering=\"out\"]\n", glob);$$=glob;
                
                };
        |   LPAREN NAME expr expr RPAREN{
                insert(++glob);fprintf(fdot, "%d [label=\"(\" ordering=\"out\"]\n", glob);
                insert(++glob);fprintf(fdot, "%d [label=NAME ordering=\"out\"]\n",glob);
                insert(++glob);fprintf(fdot, "%d [label=expr ordering=\"out\"]\n%d -> %d\n", glob,glob,$3);
                insert(++glob);fprintf(fdot, "%d [label=expr ordering=\"out\"]\n%d -> %d\n", glob,glob,$4);                           
                insert(++glob);fprintf(fdot, "%d [label=\")\" ordering=\"out\"]\n", glob);$$=glob;
                
                };
        |   LPAREN LET LPAREN NAME expr RPAREN term RPAREN{
                insert(++glob);fprintf(fdot, "%d [label=\"(\" ordering=\"out\"]\n", glob);
                insert(++glob);fprintf(fdot, "%d [label=LET ordering=\"out\"]\n", glob);
                insert(++glob);fprintf(fdot, "%d [label=\"(\" ordering=\"out\"]\n", glob);
                insert(++glob);fprintf(fdot, "%d [label=NAME ordering=\"out\"]\n",glob);
                insert(++glob);fprintf(fdot, "%d [label=expr ordering=\"out\"]\n%d -> %d\n", glob,glob,$5);
                insert(++glob);fprintf(fdot, "%d [label=\")\" ordering=\"out\"]\n", glob);$$=glob;   
                insert(++glob);fprintf(fdot, "%d [label=term1 ordering=\"out\"]\n", glob,glob,$7);
                int x;
                do{
                        x = removeData();
                        fprintf(fdot,"%d -> %d\n",glob,x);
                }while(x!=$7&&x!=0);                      
                insert(++glob);fprintf(fdot, "%d [label=\")\" ordering=\"out\"]\n", glob);$$=glob;
                
                };
        ;
fla     :   TRUECONST {insert(++glob);fprintf(fdot, "%d [label=true ordering=\"out\"]\n", glob,$1);$$=glob;};
        |   FALSECONST {insert(++glob);fprintf(fdot, "%d [label=false ordering=\"out\"]\n", glob,$1);$$=glob;};
        |   NAME {insert(++glob);fprintf(fdot, "%d [label=\"%s\" ordering=\"out\"]\n", glob,$1);$$=glob;};
        |   LPAREN GETBOOL RPAREN{
                insert(++glob);fprintf(fdot, "%d [label=\"(\" ordering=\"out\"]\n", glob);
                insert(++glob);fprintf(fdot, "%d [label=get-bool ordering=\"out\"]\n",glob);
                insert(++glob);fprintf(fdot, "%d [label=\")\" ordering=\"out\"]\n", glob);$$=glob;
                };
        |   LPAREN COMPARATOR term term RPAREN{
                insert(++glob);fprintf(fdot, "%d [label=\"(\" ordering=\"out\"]\n", glob);
                insert(++glob);fprintf(fdot, "%d [label=COMPARATOR ordering=\"out\"]\n",glob);
                insert(++glob);fprintf(fdot, "%d [label=term ordering=\"out\"]\n", glob,$3);
                int x;
                do{
                        x = removeData();
                        fprintf(fdot,"%d -> %d\n",glob,x);
                }while(x!=$3&&x!=0);                            
                insert(++glob);fprintf(fdot, "%d [label=term ordering=\"out\"]\n", glob,$4); 
                do{
                        x = removeData();
                        fprintf(fdot,"%d -> %d\n",glob,x);
                }while(x!=$4&&x!=0);                            
                insert(++glob);fprintf(fdot, "%d [label=\")\" ordering=\"out\"]\n", glob);$$=glob;
                };
        |   LPAREN NOT fla RPAREN{
                insert(++glob);fprintf(fdot, "%d [label=\"(\" ordering=\"out\"]\n", glob);
                insert(++glob);fprintf(fdot, "%d [label=NOT ordering=\"out\"]\n",glob);
                insert(++glob);fprintf(fdot, "%d [label=fla ordering=\"out\"]\n", glob,$3); 
                int x;
                do{
                        x = removeData();
                        fprintf(fdot,"%d -> %d\n",glob,x);
                }while(x!=$3&&x!=0);                             
                insert(++glob);fprintf(fdot, "%d [label=\")\" ordering=\"out\"]\n", glob);$$=glob;
                };
        |   LPAREN AND fla fla RPAREN{
                insert(++glob);fprintf(fdot, "%d [label=\"(\" ordering=\"out\"]\n", glob);
                insert(++glob);fprintf(fdot, "%d [label=and ordering=\"out\"]\n",glob);
                insert(++glob);fprintf(fdot, "%d [label=fla ordering=\"out\"]\n", glob);
                int x;
                do{
                        x = removeData();
                        fprintf(fdot,"%d -> %d\n",glob,x);
                }while(x!=$3&&x!=0);
                insert(++glob);fprintf(fdot, "%d [label=fla ordering=\"out\"]\n", glob); 
                do{
                        x = removeData();
                        fprintf(fdot,"%d -> %d\n",glob,x);
                }while(x!=$4&&x!=0);                           
                insert(++glob);fprintf(fdot, "%d [label=\")\" ordering=\"out\"]\n", glob);$$=glob;
        };
        |   LPAREN OR fla fla RPAREN{
                insert(++glob);fprintf(fdot, "%d [label=\"(\" ordering=\"out\"]\n", glob);
                insert(++glob);fprintf(fdot, "%d [label=or ordering=\"out\"]\n",glob);
                insert(++glob);fprintf(fdot, "%d [label=fla ordering=\"out\"]\n", glob);
                int x;
                do{
                        x = removeData();
                        fprintf(fdot,"%d -> %d\n",glob,x);
                }while(x!=$3&&x!=0);
                insert(++glob);fprintf(fdot, "%d [label=fla ordering=\"out\"]\n", glob); 
                do{
                        x = removeData();
                        fprintf(fdot,"%d -> %d\n",glob,x);
                }while(x!=$4&&x!=0);                           
                insert(++glob);fprintf(fdot, "%d [label=\")\" ordering=\"out\"]\n", glob);$$=glob;
        };
        |   LPAREN IF fla fla fla RPAREN{
                insert(++glob);fprintf(fdot, "%d [label=\"(\" ordering=\"out\"]\n", glob);
                insert(++glob);fprintf(fdot, "%d [label=IF ordering=\"out\"]\n",glob);
                insert(++glob);fprintf(fdot, "%d [label=fla1 ordering=\"out\"]\n", glob,$3);
                int x;
                do{
                        x = removeData();
                        fprintf(fdot,"%d -> %d\n",glob,x);
                }while(x!=$3&&x!=0);
                insert(++glob);fprintf(fdot, "%d [label=fla1 ordering=\"out\"]\n", glob,$4); 
                do{
                        x = removeData();
                        fprintf(fdot,"%d -> %d\n",glob,x);
                }while(x!=$4&&x!=0);   
                insert(++glob);fprintf(fdot, "%d [label=fla1 ordering=\"out\"]\n", glob,$5); 
                do{
                        x = removeData();
                        fprintf(fdot,"%d -> %d\n",glob,x);
                }while(x!=$5&&x!=0);                                   
                insert(++glob);fprintf(fdot, "%d [label=\")\" ordering=\"out\"]\n", glob);$$=glob;
  
        };
        |   LPAREN NAME RPAREN{
                insert(++glob);fprintf(fdot, "%d [label=\"(\" ordering=\"out\"]\n", glob);
                insert(++glob);fprintf(fdot, "%d [label=NAME ordering=\"out\"]\n",glob);
                insert(++glob);fprintf(fdot, "%d [label=\")\" ordering=\"out\"]\n", glob);$$=glob;
                }
        |   LPAREN NAME expr RPAREN{
                insert(++glob);fprintf(fdot, "%d [label=\"(\" ordering=\"out\"]\n", glob);
                insert(++glob);fprintf(fdot, "%d [label=NAME ordering=\"out\"]\n",glob);
                insert(++glob);fprintf(fdot, "%d [label=expr ordering=\"out\"]\n%d -> %d\n", glob,glob,$3);
                insert(++glob);fprintf(fdot, "%d [label=\")\" ordering=\"out\"]\n", glob);$$=glob;
                }
        |   LPAREN NAME expr expr RPAREN{
                insert(++glob);fprintf(fdot, "%d [label=\"(\" ordering=\"out\"]\n", glob);
                insert(++glob);fprintf(fdot, "%d [label=NAME ordering=\"out\"]\n",glob);
                insert(++glob);fprintf(fdot, "%d [label=expr ordering=\"out\"]\n%d -> %d\n", glob,glob,$3);
                insert(++glob);fprintf(fdot, "%d [label=expr ordering=\"out\"]\n%d -> %d\n", glob,glob,$4);
                insert(++glob);fprintf(fdot, "%d [label=\")\" ordering=\"out\"]\n", glob);$$=glob;
                };
        |   LPAREN LET LPAREN NAME expr RPAREN fla RPAREN{
                insert(++glob);fprintf(fdot, "%d [label=\"(\" ordering=\"out\"]\n", glob);
                insert(++glob);fprintf(fdot, "%d [label=LET ordering=\"out\"]\n", glob);
                insert(++glob);fprintf(fdot, "%d [label=\"(\" ordering=\"out\"]\n", glob);
                insert(++glob);fprintf(fdot, "%d [label=\"NAME\" ordering=\"out\"]\n",glob);
                insert(++glob);fprintf(fdot, "%d [label=expr ordering=\"out\"]\n%d -> %d\n", glob,glob,$5);
                insert(++glob);fprintf(fdot, "%d [label=\")\" ordering=\"out\"]\n", glob);$$=glob;    
                insert(++glob);fprintf(fdot, "%d [label=fla ordering=\"out\"]\n%d -> %d\n", glob,glob,$7);
                int x; 
                do{
                        x = removeData();
                        fprintf(fdot,"%d -> %d\n",glob,x);
                }while(x!=$7&&x!=0);                                     
                insert(++glob);fprintf(fdot, "%d [label=\")\" ordering=\"out\"]\n", glob);$$=glob;
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
        yyin=fopen(argv[1],"r+");
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
        return 0;
        
}
