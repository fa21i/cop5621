#include "y.tab.h"
#include "ast.h"
int yyparse();
char* args[20];
int types[20];
int scope[20][2];
char* fun_scope[20];
int arg_c = 0;
int type_c = 0;

int print_array(int a[]){
   for (int i = 0; i < type_c; i++)
   {
      printf("index: %d, ",a[i]);
   }  
   printf("\n");
   return 0;
}
int print_char_array(char* a[]){
   
   for (int i = 0; i < arg_c; i++)
   {
      printf("index: %s, ",a[i]);
   }  
   printf("\n");
   return 0;
}
int print_scope(int a[][2], int index){
   for (int i = 0; i < arg_c; i++)
   {
      printf("scope %d: %d, ",index,a[i][index]);
   }  
   printf("\n");
   return 0;
}
int return_index(char* a[], char* str){
   for(int i=0;i<arg_c;i++){
      if(strcmp(a[i],str)==0){
         return i;
      }
   }
   return -1;
}

int demo(struct ast* node)
{
   if(node->ntoken == DEFINE){
      printf("visit fun: %s\n", node->token);
      int how_many_children = get_child_num(node);
      printf("how_many_children: %d\n", how_many_children);
      int id_fun_body = get_child(node, how_many_children)->id;
      fun_scope[arg_c] = get_child(node,1)->token ;
      scope[arg_c][0] = get_child(node,1)->id;
      scope[arg_c][1] = id_fun_body;
      args[arg_c++] = get_child(node,1)->token;
      types[type_c++] = get_child(node, how_many_children-1)->ntoken;
      for(int i = 0; i < how_many_children-3; i++)
      {
         struct ast *arg = get_child(node, i+2);
         if(arg->ntoken==NAME){
            if(return_index(args,arg->token)!=-1){
               printf("%s is already declared\n",arg->token);
               return 1;
            }
            fun_scope[arg_c] = get_child(node,1)->token;
            scope[arg_c][0] = arg->id;
            scope[arg_c][1] = id_fun_body;
            args[arg_c++] = arg->token;
         }
         if(arg->ntoken==INTTYPE || arg->ntoken==BOOLTYPE){
            types[type_c++] = arg->ntoken;
         }
         printf("   declare its arg: %s of type %d, valid in %d -- %d\n",
            arg->token, 
            arg->ntoken,
            arg->id, id_fun_body);
      }
   }
   if(node->ntoken == LET){
      printf("visit fun: %s\n", node->token);
      struct ast *let_child = get_child(node,2);
      int let_expr = let_child->ntoken;
      printf("   declare its arg: %s of type %d, valid in %d -- %d\n",
         get_child(node,1)->token,
         let_expr,
         get_child(node,1)->id,
         get_child(node,3)->id);

      if(let_expr==ADDOP||let_expr==MINOP||let_expr==MULTOP ||
         let_expr==LT || let_expr==GT || let_expr == EQ || 
         let_expr == LTEQ || let_expr == GTEQ ||
         let_expr==GETINT || let_expr==INTTYPE || let_expr==CONST)
         types[type_c++] = INTTYPE;
      else if(let_expr==TRUECONST || let_expr==FALSECONST ||
         let_expr==OR || let_expr==NOT || let_expr==AND ||
         let_expr==GETBOOL ||let_expr==BOOLTYPE)
         types[type_c++] = BOOLTYPE;
      else{
         printf("Invalid Code\n");
         return 1;
      }

      fun_scope[arg_c] = node->token;
      scope[arg_c][0] = get_child(node,1)->id;
      scope[arg_c][1] = get_child(node,3)->id;
      args[arg_c++] = get_child(node,1)->token;         
   }
   return 0;
}

int demo1(struct ast* node){
   if(node->ntoken==NAME){
      int r_index = return_index(args,node->token); 
      printf("var: %s, fun-scope: %s, scope: %d -- %d, ID: %d\n",
         node->token,
         fun_scope[r_index],
         scope[r_index][0],
         scope[r_index][1],
         node->id);
      if(r_index==-1){
         printf("%s not initialized\n",node->token);
         return 1;
      }
      else if(get_child(get_root(node),1)->ntoken!=MAIN){
         if(strcmp(fun_scope[r_index],"LET")==0){
            if(scope[r_index][0]!=node->id && scope[r_index][1]<node->id){
               printf("%s is not in scope\n",node->token);
               // return 1;
            }
            return 0;
         }
         else if( strcmp(fun_scope[r_index],get_child(get_root(node),1)->token)!=0){
            printf("%s is not in scope\n",node->token);
            return 1;
         }
         
         else{
            return 0;
         }
      }
      else if(get_child(get_root(node),1)->ntoken==MAIN && strcmp(fun_scope[r_index],"LET")==0){
         printf("%s in not in scope\n", node->token);
         return 1;
      }
   }
   return 0;
}

int main (int argc, char **argv) {

   int retval = yyparse();

   if (retval == 0) {
      int a = visit_ast(demo);
      if(a!=0){
         return 1;
      }
      a = visit_ast(demo1);
      if(a!=0){
         return 1;
      }
      print_char_array(args);
      print_array(types);
      print_scope(scope,0);
      print_scope(scope,1);
      print_ast();
   }

   free_ast();
   return retval;
}
