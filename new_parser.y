%{
        #include <stdio.h>
        int yylex();
        FILE *fdot;
	void yyerror(char *s);
        int glob = 0;
        int a[250];
        int i;
        extern FILE *yyin;
        void general_loop(int glob, int id){
                for(i=0;i<a[id];i++){
                        fprintf(fdot,"%d -> %d\n",glob,i+id);
                }
        }
%}

%union {int val; char* str;}
%start program
%token <str> NAME CONST 
%token <val> COMPARATOR ADDOP MINOP DEFINE GETINT GETBOOL AND OR IF LET INTTYPE BOOLTYPE PRINT RPAREN LPAREN NOT MULTOP TRUECONST FALSECONST
%type <val> program type expr term fla 


%%
program :   LPAREN DEFINE NAME type expr RPAREN program  {
                a[glob+1]=6;$$=glob+1;
                ++glob;fprintf(fdot, "%d [label=\"(\" ordering=\"out\"]\n", glob);
                ++glob;fprintf(fdot, "%d [label=DEFINE ordering=\"out\"]\n", glob);
                ++glob;fprintf(fdot, "%d [label=NAME ordering=\"out\"]\n", glob);
                ++glob;fprintf(fdot, "%d [label=type ordering=\"out\"]\n",glob);
                general_loop(glob,$4);
                ++glob;fprintf(fdot, "%d [label=expr ordering=\"out\"]\n",glob);
                general_loop(glob,$5);
                ++glob;fprintf(fdot, "%d [label=\")\" ordering=\"out\"]\n", glob);
                ++glob;fprintf(fdot, "%d [label=program ordering=\"out\"]\n", glob);
                general_loop(glob,$$);
                fprintf(fdot,"%d -> %d\n",glob,$$-1);
                };
        |   LPAREN DEFINE NAME LPAREN NAME type RPAREN type expr RPAREN program{
                a[glob+1]=10;$$=glob+1;
                ++glob;fprintf(fdot, "%d [label=\"(\" ordering=\"out\"]\n", glob);
                ++glob;fprintf(fdot, "%d [label=DEFINE ordering=\"out\"]\n", glob);
                ++glob;fprintf(fdot, "%d [label=NAME ordering=\"out\"]\n", glob);
                ++glob;fprintf(fdot, "%d [label=\"(\" ordering=\"out\"]\n", glob);
                ++glob;fprintf(fdot, "%d [label=NAME ordering=\"out\"]\n", glob);
                ++glob;fprintf(fdot, "%d [label=type ordering=\"out\"]\n",glob);
                general_loop(glob,$6);
                ++glob;fprintf(fdot, "%d [label=\")\" ordering=\"out\"]\n", glob);
                ++glob;fprintf(fdot, "%d [label=type ordering=\"out\"]\n",glob); 
                general_loop(glob,$8);               
                ++glob;fprintf(fdot, "%d [label=expr ordering=\"out\"]\n",glob);
                general_loop(glob,$9);
                ++glob;fprintf(fdot, "%d [label=\")\" ordering=\"out\"]\n", glob);
                ++glob;fprintf(fdot, "%d [label=program ordering=\"out\"]\n", glob);
                general_loop(glob,$$);
                fprintf(fdot,"%d -> %d\n",glob,$$-1);
                };

        |   LPAREN DEFINE NAME LPAREN NAME type RPAREN LPAREN NAME type RPAREN type expr RPAREN program{
                a[glob+1]=14;$$=glob+1;
                ++glob;fprintf(fdot, "%d [label=\"(\" ordering=\"out\"]\n", glob);
                ++glob;fprintf(fdot, "%d [label=DEFINE ordering=\"out\"]\n", glob);
                ++glob;fprintf(fdot, "%d [label=NAME ordering=\"out\"]\n", glob);
                ++glob;fprintf(fdot, "%d [label=\"(\" ordering=\"out\"]\n", glob);
                ++glob;fprintf(fdot, "%d [label=NAME ordering=\"out\"]\n", glob);
                ++glob;fprintf(fdot, "%d [label=type ordering=\"out\"]\n", glob);
                general_loop(glob,$6);
                ++glob;fprintf(fdot, "%d [label=\")\" ordering=\"out\"]\n", glob);
                ++glob;fprintf(fdot, "%d [label=\"(\" ordering=\"out\"]\n", glob);
                ++glob;fprintf(fdot, "%d [label=NAME ordering=\"out\"]\n", glob);
                ++glob;fprintf(fdot, "%d [label=type ordering=\"out\"]\n", glob);
                general_loop(glob,$10);
                ++glob;fprintf(fdot, "%d [label=\")\" ordering=\"out\"]\n", glob);
                ++glob;fprintf(fdot, "%d [label=type ordering=\"out\"]\n",glob);  
                general_loop(glob,$12);              
                ++glob;fprintf(fdot, "%d [label=expr ordering=\"out\"]\n",glob);
                general_loop(glob,$13);
                ++glob;fprintf(fdot, "%d [label=\")\" ordering=\"out\"]\n", glob);
                ++glob;fprintf(fdot, "%d [label=program ordering=\"out\"]\n", glob);
                general_loop(glob,$$);
                fprintf(fdot,"%d -> %d\n",glob,$$-1);
                };
        |   LPAREN PRINT expr RPAREN {
                a[glob+1]=4;$$=glob+1;
                ++glob;fprintf(fdot, "%d [label=\"(\" ordering=\"out\"]\n", glob);
                ++glob;fprintf(fdot, "%d [label=PRINT ordering=\"out\"]\n", glob);
                ++glob;fprintf(fdot, "%d [label=expr ordering=\"out\"]\n",glob);
                general_loop(glob,$3);
                ++glob;fprintf(fdot, "%d [label=\")\" ordering=\"out\"]\n", glob);
                ++glob;fprintf(fdot, "%d [label=program ordering=\"out\"]\n", glob);
                general_loop(glob,$$);
                };
        ;
type    :   INTTYPE {
                a[glob+1]=1;$$=glob+1;
                ++glob;fprintf(fdot, "%d [label=int ordering=\"out\"]\n", glob,$1);};
        |   BOOLTYPE {
                a[glob+1]=1;$$=glob+1;
                ++glob;fprintf(fdot, "%d [label=bool ordering=\"out\"]\n", glob,$1);};
        ;
expr    :   term {
                a[glob+1]=1;$$=glob+1;
                ++glob;fprintf(fdot, "%d [label=term ordering=\"out\"]\n", glob);
                general_loop(glob,$1);
                };
        |   fla {
                a[glob+1]=1;$$=glob+1;
                ++glob;fprintf(fdot, "%d [label=fla ordering=\"out\"]\n", glob);
                general_loop(glob,$1);
                };
        ;
term    :   CONST {
                a[glob+1]=1;$$=glob+1;
                ++glob;fprintf(fdot, "%d [label=%s ordering=\"out\"]\n", glob,$1);
                };
        |   NAME {
                a[glob+1]=1;$$=glob+1;
                ++glob;fprintf(fdot, "%d [label=%s ordering=\"out\"]\n", glob,$1);
                };
        |   LPAREN GETINT RPAREN{
                a[glob+1]=3;$$=glob+1;
                ++glob;fprintf(fdot, "%d [label=\"(\" ordering=\"out\"]\n", glob);
                ++glob;fprintf(fdot, "%d [label=\"getInt\" ordering=\"out\"]\n", glob);
                ++glob;fprintf(fdot, "%d [label=\")\" ordering=\"out\"]\n", glob); 
        };
        |   LPAREN ADDOP term term RPAREN {
                a[glob+1]=5;$$=glob+1;
                ++glob;fprintf(fdot, "%d [label=\"(\" ordering=\"out\"]\n", glob);
                ++glob;fprintf(fdot, "%d [label=\"+\" ordering=\"out\"]\n", glob);
                ++glob;fprintf(fdot, "%d [label=term ordering=\"out\"]\n", glob);
                general_loop(glob,$3);
                ++glob;fprintf(fdot, "%d [label=term ordering=\"out\"]\n", glob);
                general_loop(glob,$4);
                ++glob;fprintf(fdot, "%d [label=\")\" ordering=\"out\"]\n", glob);
                };
        |   LPAREN MINOP term term RPAREN {
                a[glob+1]=5;$$=glob+1;
                ++glob;fprintf(fdot, "%d [label=\"(\" ordering=\"out\"]\n", glob);
                ++glob;fprintf(fdot, "%d [label=\"-\" ordering=\"out\"]\n", glob);
                ++glob;fprintf(fdot, "%d [label=term ordering=\"out\"]\n", glob);
                general_loop(glob,$3);
                ++glob;fprintf(fdot, "%d [label=term ordering=\"out\"]\n", glob);
                general_loop(glob,$4);
                ++glob;fprintf(fdot, "%d [label=\")\" ordering=\"out\"]\n", glob);
                };
        |   LPAREN MULTOP term term RPAREN {
                a[glob+1]=5;$$=glob+1;
                ++glob;fprintf(fdot, "%d [label=\"(\" ordering=\"out\"]\n", glob);
                ++glob;fprintf(fdot, "%d [label=\"*|div|mod\" ordering=\"out\"]\n", glob);
                ++glob;fprintf(fdot, "%d [label=term ordering=\"out\"]\n", glob);
                general_loop(glob,$3);
                ++glob;fprintf(fdot, "%d [label=term ordering=\"out\"]\n", glob);
                general_loop(glob,$4);
                ++glob;fprintf(fdot, "%d [label=\")\" ordering=\"out\"]\n", glob);
                };
        |   LPAREN IF fla term term RPAREN{
                a[glob+1]=6;$$=glob+1;
                ++glob;fprintf(fdot, "%d [label=\"(\" ordering=\"out\"]\n", glob);
                ++glob;fprintf(fdot, "%d [label=IF ordering=\"out\"]\n", glob);
                ++glob;fprintf(fdot, "%d [label=fla ordering=\"out\"]\n", glob);
                general_loop(glob,$3);
                ++glob;fprintf(fdot, "%d [label=term ordering=\"out\"]\n", glob);
                general_loop(glob,$4);
                ++glob;fprintf(fdot, "%d [label=term ordering=\"out\"]\n", glob);
                general_loop(glob,$5);
                ++glob;fprintf(fdot, "%d [label=\")\" ordering=\"out\"]\n", glob);
                };
        |   LPAREN NAME RPAREN {
                a[glob+1]=3;$$=glob+1;
                ++glob;fprintf(fdot, "%d [label=\"(\" ordering=\"out\"]\n", glob);
                ++glob;fprintf(fdot, "%d [label=NAME ordering=\"out\"]\n",glob);
                ++glob;fprintf(fdot, "%d [label=\")\" ordering=\"out\"]\n", glob);
                }
        |   LPAREN NAME expr RPAREN{
                a[glob+1]=4;$$=glob+1;
                ++glob;fprintf(fdot, "%d [label=\"(\" ordering=\"out\"]\n", glob);
                ++glob;fprintf(fdot, "%d [label=NAME ordering=\"out\"]\n",glob);
                ++glob;fprintf(fdot, "%d [label=expr ordering=\"out\"]\n", glob);  
                general_loop(glob,$3);              
                ++glob;fprintf(fdot, "%d [label=\")\" ordering=\"out\"]\n", glob);
                
                };
        |   LPAREN NAME expr expr RPAREN{
                a[glob+1]=5;$$=glob+1;
                ++glob;fprintf(fdot, "%d [label=\"(\" ordering=\"out\"]\n", glob);
                ++glob;fprintf(fdot, "%d [label=NAME ordering=\"out\"]\n",glob);
                ++glob;fprintf(fdot, "%d [label=expr ordering=\"out\"]\n",glob);  
                general_loop(glob,$4); 
                ++glob;fprintf(fdot, "%d [label=expr ordering=\"out\"]\n",glob);  
                general_loop(glob,$5);                           
                ++glob;fprintf(fdot, "%d [label=\")\" ordering=\"out\"]\n", glob);
                
                };
        |   LPAREN LET LPAREN NAME expr RPAREN term RPAREN{
                a[glob+1]=8;$$=glob+1;
                ++glob;fprintf(fdot, "%d [label=\"(\" ordering=\"out\"]\n", glob);
                ++glob;fprintf(fdot, "%d [label=LET ordering=\"out\"]\n", glob);
                ++glob;fprintf(fdot, "%d [label=\"(\" ordering=\"out\"]\n", glob);
                ++glob;fprintf(fdot, "%d [label=NAME ordering=\"out\"]\n",glob);
                ++glob;fprintf(fdot, "%d [label=expr ordering=\"out\"]\n",glob);
                general_loop(glob,$5);
                ++glob;fprintf(fdot, "%d [label=\")\" ordering=\"out\"]\n", glob); 
                ++glob;fprintf(fdot, "%d [label=term ordering=\"out\"]\n",glob); 
                general_loop(glob,$7);                            
                ++glob;fprintf(fdot, "%d [label=\")\" ordering=\"out\"]\n", glob);
                
                };
        ;
fla     :   TRUECONST {
                a[glob+1]=1;$$=glob+1;
                ++glob;fprintf(fdot, "%d [label=true ordering=\"out\"]\n", glob,$1);
                };
        |   FALSECONST {
                a[glob+1]=1;$$=glob+1;
                ++glob;fprintf(fdot, "%d [label=false ordering=\"out\"]\n", glob,$1);
                };
        |   NAME {
                a[glob+1]=1;$$=glob+1;
                ++glob;fprintf(fdot, "%d [label=NAME ordering=\"out\"]\n", glob,$1);
                };
        |   LPAREN GETBOOL RPAREN{
                a[glob+1]=3;$$=glob+1;
                ++glob;fprintf(fdot, "%d [label=\"(\" ordering=\"out\"]\n", glob);
                ++glob;fprintf(fdot, "%d [label=getBool ordering=\"out\"]\n",glob);
                ++glob;fprintf(fdot, "%d [label=\")\" ordering=\"out\"]\n", glob);
                };
        |   LPAREN COMPARATOR term term RPAREN{
                a[glob+1]=5;$$=glob+1;
                ++glob;fprintf(fdot, "%d [label=\"(\" ordering=\"out\"]\n", glob);
                ++glob;fprintf(fdot, "%d [label=COMPARATOR ordering=\"out\"]\n",glob);
                ++glob;fprintf(fdot, "%d [label=term ordering=\"out\"]\n", glob);  
                general_loop(glob,$3);                          
                ++glob;fprintf(fdot, "%d [label=term ordering=\"out\"]\n", glob);  
                general_loop(glob,$4);                        
                ++glob;fprintf(fdot, "%d [label=\")\" ordering=\"out\"]\n", glob);
                };
        |   LPAREN NOT fla RPAREN{
                a[glob+1]=4;$$=glob+1;
                ++glob;fprintf(fdot, "%d [label=\"(\" ordering=\"out\"]\n", glob);
                ++glob;fprintf(fdot, "%d [label=NOT ordering=\"out\"]\n",glob);
                ++glob;fprintf(fdot, "%d [label=fla ordering=\"out\"]\n", glob); 
                general_loop(glob,$3);                            
                ++glob;fprintf(fdot, "%d [label=\")\" ordering=\"out\"]\n", glob);
                };
        |   LPAREN AND term term RPAREN{
                a[glob+1]=5;$$=glob+1;
                ++glob;fprintf(fdot, "%d [label=\"(\" ordering=\"out\"]\n", glob);
                ++glob;fprintf(fdot, "%d [label=AND ordering=\"out\"]\n",glob);
                ++glob;fprintf(fdot, "%d [label=term ordering=\"out\"]\n",glob);
                general_loop(glob,$3);
                ++glob;fprintf(fdot, "%d [label=term ordering=\"out\"]\n",glob); 
                general_loop(glob,$4);                           
                ++glob;fprintf(fdot, "%d [label=\")\" ordering=\"out\"]\n", glob);
        };
        |   LPAREN OR term term RPAREN{
                a[glob+1]=5;$$=glob+1;
                ++glob;fprintf(fdot, "%d [label=\"(\" ordering=\"out\"]\n", glob);
                ++glob;fprintf(fdot, "%d [label=OR ordering=\"out\"]\n",glob);
                ++glob;fprintf(fdot, "%d [label=term ordering=\"out\"]\n",glob);
                general_loop(glob,$3);
                ++glob;fprintf(fdot, "%d [label=term ordering=\"out\"]\n",glob); 
                general_loop(glob,$4);                           
                ++glob;fprintf(fdot, "%d [label=\")\" ordering=\"out\"]\n", glob);
        };
        |   LPAREN IF fla fla fla RPAREN{
                a[glob+1]=6;$$=glob+1;
                ++glob;fprintf(fdot, "%d [label=\"(\" ordering=\"out\"]\n", glob);
                ++glob;fprintf(fdot, "%d [label=IF ordering=\"out\"]\n",glob);
                ++glob;fprintf(fdot, "%d [label=fla ordering=\"out\"]\n", glob);
                general_loop(glob,$3);
                ++glob;fprintf(fdot, "%d [label=fla ordering=\"out\"]\n", glob); 
                general_loop(glob,$4);  
                ++glob;fprintf(fdot, "%d [label=fla ordering=\"out\"]\n", glob);
                general_loop(glob,$5);                                    
                ++glob;fprintf(fdot, "%d [label=\")\" ordering=\"out\"]\n", glob);
  
        };
        |   LPAREN NAME RPAREN{
                a[glob+1]=3;$$=glob+1;
                ++glob;fprintf(fdot, "%d [label=\"(\" ordering=\"out\"]\n", glob);
                ++glob;fprintf(fdot, "%d [label=NAME ordering=\"out\"]\n",glob);
                ++glob;fprintf(fdot, "%d [label=\")\" ordering=\"out\"]\n", glob);
                }
        |   LPAREN NAME expr RPAREN{
                a[glob+1]=4;$$=glob+1;
                ++glob;fprintf(fdot, "%d [label=\"(\" ordering=\"out\"]\n", glob);
                ++glob;fprintf(fdot, "%d [label=NAME ordering=\"out\"]\n",glob);
                ++glob;fprintf(fdot, "%d [label=expr ordering=\"out\"]\n",glob);
                general_loop(glob,$3);
                ++glob;fprintf(fdot, "%d [label=\")\" ordering=\"out\"]\n", glob);
                }
        |   LPAREN NAME expr expr RPAREN{
                a[glob+1]=5;$$=glob+1;
                ++glob;fprintf(fdot, "%d [label=\"(\" ordering=\"out\"]\n", glob);
                ++glob;fprintf(fdot, "%d [label=NAME ordering=\"out\"]\n",glob);
                ++glob;fprintf(fdot, "%d [label=expr ordering=\"out\"]\n",glob);
                general_loop(glob,$3);
                ++glob;fprintf(fdot, "%d [label=expr ordering=\"out\"]\n",glob);
                general_loop(glob,$4);
                ++glob;fprintf(fdot, "%d [label=\")\" ordering=\"out\"]\n", glob);
                };
        |   LPAREN LET LPAREN NAME expr RPAREN fla RPAREN{
                a[glob+1]=8;$$=glob+1;
                ++glob;fprintf(fdot, "%d [label=\"(\" ordering=\"out\"]\n", glob);
                ++glob;fprintf(fdot, "%d [label=LET ordering=\"out\"]\n", glob);
                ++glob;fprintf(fdot, "%d [label=\"(\" ordering=\"out\"]\n", glob);
                ++glob;fprintf(fdot, "%d [label=\"NAME\" ordering=\"out\"]\n",glob);
                ++glob;fprintf(fdot, "%d [label=expr ordering=\"out\"]\n",glob);
                general_loop(glob,$5);
                ++glob;fprintf(fdot, "%d [label=\")\" ordering=\"out\"]\n", glob);    
                ++glob;fprintf(fdot, "%d [label=fla ordering=\"out\"]\n",glob);   
                general_loop(glob,$7);                                 
                ++glob;fprintf(fdot, "%d [label=\")\" ordering=\"out\"]\n", glob);
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
