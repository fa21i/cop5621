#include "y.tab.h"
#include "ast.h"
#define UNKNOWN_TYPE 999
int yyparse();
struct token{
   char* name;
   int type;
};
struct cfgToken{
   char* name;
   int id;
   int parentNum;
   int childNum;
   struct cfgToken *parent1;
   struct cfgToken *parent2;
   struct cfgToken *child1;
   struct cfgToken *child2;
};
// struct N{
//    struct ast* node;
//    char* type;
// };
struct V{
   int status;
   struct ast* node;
   struct V* next;
   char* label;
};
struct E{
   struct ast *u;
   struct ast *v;
   struct E *next;
};
struct CFG{
   struct V* v;
   struct E* e;
   struct ast* en;
   struct ast* ex;
   struct CFG* next;
};
/*struct Map{
   struct ast* key;
   struct ast* body;
   char* value;
   struct Map* next;
};*/
struct Map* map;
struct CFG* cfg = NULL;
struct token tokens[250];
char* args[20];
int types[20];
int scope[20][2];
char* fun_scope[20];
int id[20];
int arg_c = 0;
int type_c = 0;
struct args {
   char* name;
   int type;
   int s1;
   int s2;
   char* root_fun;
   int id; 
};

bool hasNode(int id){
   struct CFG* temp;
   temp = cfg;
   
   while (temp!=NULL && temp->next!=NULL)
   {
      printf("id: %d,temp id: %d\n",id,temp->en->id);
      if(temp->en->id == id){
        return true; 
      }
      temp=temp->next;
   }
   return false;
}

int is_OP(int t){
   if(t == GT || t == EQ || t == LT || t == GTEQ || t == LTEQ || t == ADDOP
      || t == MINOP || t == MULTOP || t == AND || t == OR)
      return 1;
   return 0;
}

int add_V(struct V* v, struct CFG* c){
   struct V* temp = c->v;
   printf("check v:%s\n",temp->node->token);
   while(temp->next){
      temp = temp->next;
      printf("check v:%s\n",temp->node->token);
   }
   temp->next = v;
   return 0;
}

int add_E(struct E* e, struct CFG* c){
   struct E* temp = c->e;
   while(temp->next)
      temp = temp->next;
   temp->next = e;
   return 0;
}

struct V* find_V(struct CFG* c, int id) {
   struct V *temp = c->v;  
   while(temp)
   {
      if(temp->node->id == id)
         return temp;
      else
         temp = temp->next;
   } 
   return NULL;
}

struct E* find_E(struct CFG* c){
   struct E* temp = c->e;
   while(temp)
   {
      struct V* temp_v = find_V(c, temp->v->id);
      if(temp_v->status == 0)
      {
         return temp;   
      }
      else
         temp = temp->next;
   }
   return NULL;
}

int remove_E(struct CFG* c, struct E* e){
   struct E* temp = c->e;
   struct E* prev;
   if(c->e == e)
   {
      prev = c->e;
      c->e = c->e->next;
      free(prev);
      return 0;
   }
   while(temp)
   {
      if(temp == e)
      {
         prev->next = temp->next;
         free(temp);
         return 0;
      }
      prev = temp;
      temp = temp->next;
   }
}

int construct_cfg(struct ast* node){
   //printf("node: %s\n",node->token);
   if((node->ntoken==DEFINE || node->ntoken==PRINT) 
     /* && !hasNode(get_child(node,1)->id)*/){
      
      struct CFG* new_cfg = (struct CFG*) malloc(sizeof(struct CFG));
      struct ast* en = get_child(node,1);
      struct ast* ex = get_child(node,get_child_num(node));
      //printf("reached\n");
      //printf("  node: %s\n",en->token);
      new_cfg->ex = ex;
      //printf("  reached\n");
      new_cfg->en = en;
      
      struct V* v1 = (struct V*) malloc(sizeof(struct V));
      v1->node = en;
      v1->status = 0;
      v1->next = NULL;
      struct V* v2 = (struct V*) malloc(sizeof(struct V));
      v2->node = ex;
      v2->status = 0;
      v2->next = NULL;
      v1->next = v2;
      new_cfg->v = v1;
      struct E* e1 = (struct E*) malloc(sizeof(struct E));
      e1->u = en;
      e1->v = ex;
      e1->next = NULL;
      new_cfg->e = e1;
      new_cfg->next = NULL;
     
      //printf("are you here\n");
      struct E* temp_E;
      while(temp_E = find_E(new_cfg))
      {
         struct V *u = find_V(new_cfg, temp_E->u->id);
         struct V *v = find_V(new_cfg, temp_E->v->id);
         
         v->status = 1;
         printf("e found\n");

         if(v->node->is_leaf)
            continue; 
         if(is_OP(v->node->ntoken))
         {
            printf("op?\n");
            struct V* v1 = (struct V*) malloc(sizeof(struct V));
            struct V* v2 = (struct V*) malloc(sizeof(struct V));
            v1->status = 0;
            v2->status = 0;
            v1->node = get_child(v->node,1);
            v2->node = get_child(v->node,2);
            v1->next = v2;
            v2->next = NULL;
            struct E* e1 = (struct E*) malloc(sizeof(struct E));
            struct E* e2 = (struct E*) malloc(sizeof(struct E));
            struct E* e3 = (struct E*) malloc(sizeof(struct E));
            e1->u = u->node;
            e1->v = v1->node;
            e2->u = v1->node;
            e2->v = v2->node;
            e3->u = v2->node;
            e3->v = v->node;
            e1->next = e2;
            e2->next = e3;
            e3->next = NULL;

            add_V(v1, new_cfg);
            add_E(e1, new_cfg);

            remove_E(new_cfg, temp_E);  
            /* create label for v*/
            if(v1->node->is_leaf)
            {
               v1->label = (char*) malloc(12*sizeof(char));
               v1->label = v1->node->token;
            }
            if(v2->node->is_leaf)
            {
               v2->label = (char*) malloc(12*sizeof(char));
               v2->label = v2->node->token;
            }

            v->label = (char*) malloc(12*sizeof(char));
            v->label = v->node->token;
            
         }
         else if(v->node->ntoken == LET)
         {
            printf("let\n");
            struct V* v1 = (struct V*) malloc(sizeof(struct V));
            struct V* v2 = (struct V*) malloc(sizeof(struct V));
            struct V* v3 = (struct V*) malloc(sizeof(struct V));
            v1->status = 0;
            v2->status = 0;
            v3->status = 0;
            v1->node = get_child(v->node,1);
            v2->node = get_child(v->node,2);
            v3->node = get_child(v->node,3);
            v1->next = v2;
            v2->next = v3;
            v3->next = NULL;
            struct E* e1 = (struct E*) malloc(sizeof(struct E));
            struct E* e2 = (struct E*) malloc(sizeof(struct E));
            struct E* e3 = (struct E*) malloc(sizeof(struct E));
            struct E* e4 = (struct E*) malloc(sizeof(struct E));
            e1->u = u->node;
            e1->v = v2->node;
            e2->u = v2->node;
            e2->v = v1->node;
            e3->u = v1->node;
            e3->v = v3->node;
            e4->u = v3->node;
            e4->v = v->node;
            e1->next = e2;
            e2->next = e3;
            e3->next = e4;
            e4->next = NULL;

            add_V(v1, new_cfg);
            add_E(e1, new_cfg);

            remove_E(new_cfg, temp_E);

            /* create label for v*/
           if(v1->node->is_leaf)
            {
               v1->label = (char*) malloc(12*sizeof(char));
               v1->label = v1->node->token;
            }
            if(v2->node->is_leaf)
            {
               v2->label = (char*) malloc(12*sizeof(char));
               v2->label = v2->node->token;
            }
            if(v3->node->is_leaf)
            {
               v3->label = (char*) malloc(12*sizeof(char));
               v3->label = v2->node->token;
            }

            v->label = (char*) malloc(50*sizeof(char));
            //sprintf(v->label, "%s := v%d", v1->node->token, v2->node->id);
            v->label = "Let goes here";
         }          
         else if(v->node->ntoken == IF)
         {
            printf("if?\n");
            struct V* v1 = (struct V*) malloc(sizeof(struct V));
            struct V* v2 = (struct V*) malloc(sizeof(struct V));
            struct V* v3 = (struct V*) malloc(sizeof(struct V));
            v1->status = 0;
            v2->status = 0;
            v3->status = 0;
            v1->node = get_child(v->node,1);
            v2->node = get_child(v->node,2);
            v3->node = get_child(v->node,3);
            v1->next = v2;
            v2->next = v3;
            v3->next = NULL;
            struct E* e1 = (struct E*) malloc(sizeof(struct E));
            struct E* e2 = (struct E*) malloc(sizeof(struct E));
            struct E* e3 = (struct E*) malloc(sizeof(struct E));
            struct E* e4 = (struct E*) malloc(sizeof(struct E));
            struct E* e5 = (struct E*) malloc(sizeof(struct E));
            e1->u = u->node;
            e1->v = v1->node;
            e2->u = v1->node;
            e2->v = v2->node;
            e3->u = v1->node;
            e3->v = v3->node;
            e4->u = v2->node;
            e4->v = v->node;
            e5->u = v3->node;
            e5->v = v->node;
            e1->next = e2;
            e2->next = e3;
            e3->next = e4;
            e4->next = e5;
            e5->next = NULL;

            add_V(v1, new_cfg);
            add_E(e1, new_cfg);

            remove_E(new_cfg, temp_E);

            /* create label for v*/
            if(v1->node->is_leaf)
            {
               v1->label = (char*) malloc(12*sizeof(char));
               v1->label = v1->node->token;
               printf("v1 %s\n", v1->label);
            }
            if(v2->node->is_leaf)
            {
               v2->label = (char*) malloc(12*sizeof(char));
               v2->label = v2->node->token;
               printf("v2 %s\n", v2->label);
            }
            if(v3->node->is_leaf)
            {
               v3->label = (char*) malloc(12*sizeof(char));
               v3->label = v2->node->token;
               printf("v3 %s\n", v3->label);
            }
            
            v->label = (char*) malloc(100*sizeof(char));
            //v->label = "If " + v1->node->id + " is true, then " +v->node->id + " := " + v2->node->id + ", else " + v->node->id + " := " + v3->node->id;
            sprintf(v->label,"can yoou see me..." /*"If v%d is true, then v%d := v%d, else v%d := v%d", v1->node->id, v->node->id, v2->node->id, v->node->id, v3->node->id*/);
            printf("vv %s\n", v->label);
         }
         else if(v->node->ntoken == NOT)
         {
            struct V* v1 = (struct V*) malloc(sizeof(struct V));
            v1->status = 0;
            v1->node = get_child(v->node,1);
            v1->next = NULL;
            struct E* e1 = (struct E*) malloc(sizeof(struct E));
            struct E* e2 = (struct E*) malloc(sizeof(struct E));
            e1->u = u->node;
            e1->v = v1->node;
            e2->u = v1->node;
            e2->v = v->node;
            e1->next = e2;
            e2->next = NULL;
  
            add_V(v1, new_cfg);
            add_E(e1, new_cfg);

            remove_E(new_cfg, temp_E);
            /* create label for v*/
            if(v1->node->is_leaf)
            {
               v1->label = (char*) malloc(12*sizeof(char));
               v1->label = v1->node->token;
            }
            
            v->label = (char*) malloc(20*sizeof(char));
            sprintf(v->label, "NOT v%d", v1->node->id);
         }
      }
      new_cfg->next = NULL;
      if(cfg){
         struct CFG* temp = cfg;
         while (temp->next)
         {
            temp = temp->next;
         }
         temp->next = new_cfg;
      }
      else{
         cfg = new_cfg;
      }
   }
   return 0;
}

int print_cfg(){
   struct CFG* temp = cfg;

   
   FILE *fp;
   fp = fopen("cfg.dot", "w");
   fprintf(fp, "digraph print {\n");

   int i = 0;
   while(temp)
   {
      struct V* temp_V = temp->v;
      struct E* temp_E = temp->e;
      //printf("c\n");
      while(temp_V)
      {

         //if let or if
         if(temp_V->node->ntoken == LET)
         {
            fprintf(fp, "%d [label=\"%s\", fontname=\"monospace\"]\n", temp_V->node->id,  temp_V->label);   
         }
         else if(temp_V->node->ntoken == IF)
         {
            fprintf(fp, "%d [label=\"IF v%d = true, then v%d := v%d, else v%d := v%d\", fontname=\"monospace\"]\n", temp_V->node->id,get_child(temp_V->node,1)->id, temp_V->node->id,get_child(temp_V->node,2)->id,temp_V->node->id,get_child(temp_V->node,3)->id);
         }
         else if(is_OP(temp_V->node->ntoken)!=0)
         {
            fprintf(fp, "%d [label=\"v%d := v%d %s v%d\", fontname=\"monospace\"]\n", temp_V->node->id,  temp_V->node->id, get_child(temp_V->node,1)->id, temp_V->node->token, get_child(temp_V->node,2)->id);    //replace with temp_V->label
         }
         else{
            fprintf(fp, "%d [label=\"v%d := %s\", fontname=\"monospace\"]\n", temp_V->node->id, temp_V->node->id, temp_V->node->token);    //replace with temp_V->label
         }
         
         temp_V = temp_V->next;
      }
    
      while(temp_E)
      {
         //printf("e\n");
         fprintf(fp, "%d->%d\n", temp_E->u->id, temp_E->v->id);     //<uID->vID>
         temp_E = temp_E->next;
      }
      
      temp = temp->next;
      printf("%d\n",i++);
   }
   printf("out\n");
   fprintf(fp, "}\n");
   fclose(fp);
   system("dot -Tpdf cfg.dot -o cfg.pdf");

  /* while (temp!=NULL && temp->next!=NULL){
      printf("Entry: %s, Exit: %s, Next: %s\n",temp->en->token,temp->ex->token,temp->next->en->token);
      temp=temp->next;
   }
   printf("Entry: %s, Exit: %s, Next: NULL\n",temp->en->token,temp->ex->token);*/
   return 0;
}

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
int print_tokens(){
   for (int i = 1; tokens[i].name!=NULL||tokens[i].type!=0 ; i++){
      printf("id: %d,name: %s,type : %d\n",i,tokens[i].name,tokens[i].type);
   }  
   return 0;
}
int get_count(char *a[],char *str){
   int count=0;
   for(int i=0;i<arg_c;i++){
      if(strcmp(a[i],str)==0){
         count++;
      }
   }
   return count;
}
int* get_all_index(char* a[], char* str){
   static int b[10];
   int c=0;
   for(int i=0;i<arg_c;i++){
      if(strcmp(a[i],str)==0){
         b[c++] = i;
      }
   }
   return b;
}
int return_index(char* a[], char* str){
   for(int i=0;i<arg_c;i++){
      if(strcmp(a[i],str)==0){
         return i;
      }
   }
   return -1;
}
int scope_index(int num){
   for(int i=0;i<arg_c;i++){
      printf("\\\\%d scope_index: %d\n",num,scope[i][0]);
      if(scope[i][0]==num){
         return i;
      }
   }
   return -1;
}
int count_vars(char* fun_name){
   int c=0;
   for(int i=0;i<arg_c;i++){
      if(strcmp(fun_name,fun_scope[i])==0){
         c++;
      }
   }
   return c;
}
int declarations(struct ast* node)
{
   if(node->ntoken == DEFINE){
      // printf("visit fun: %s\n", node->token);
      for (int i = 0; i < arg_c; i++)
      {
         if((strcmp(fun_scope[i],"DEFINE")==0 || strcmp(fun_scope[i],"LET")==0) && 
            strcmp(args[i],get_child(node,1)->token)==0){
               printf("function %s again cannot be declared twice\n",args[i]);
               return 1;
            }
      }
      int how_many_children = get_child_num(node);
      // printf("how_many_children: %d\n", how_many_children);
      int id_fun_body = get_child(node, how_many_children)->id;
      fun_scope[arg_c] = "DEFINE" ;
      scope[arg_c][0] = get_child(node,1)->id;
      scope[arg_c][1] = id_fun_body;
      args[arg_c++] = get_child(node,1)->token;
      types[type_c++] = get_child(node, how_many_children-1)->ntoken;
      for(int i = 0; i < how_many_children-3; i++)
      {
         struct ast *arg = get_child(node, i+2);
         if(arg->ntoken==NAME){
            int r_index = return_index(args,arg->token);
            if(r_index!=-1 && fun_scope[r_index]==get_child(node,1)->token){
               printf("%s is already declared \n",arg->token);
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
         // printf("   declare its arg: %s of type %d, valid in %d -- %d\n",
         //    arg->token, 
         //    arg->ntoken,
         //    arg->id, id_fun_body);
      }
      
      
   }
   if(node->ntoken == LET){
      // printf("visit fun: %s\n", node->token);
      for (int i = 0; i < arg_c; i++)
      {
         if(strcmp(fun_scope[i],"DEFINE")==0 && 
            strcmp(args[i],get_child(node,1)->token)==0){
               printf("Cannot define function %s again",args[i]);
               return 1;
            }
      }
      
      struct ast *let_child = get_child(node,2);
      int let_expr = let_child->ntoken;
      // printf("   declare its arg: %s of type %d, valid in %d -- %d\n",
         // get_child(node,1)->token,
         // let_expr,
         // get_child(node,1)->id,
         // get_child(node,3)->id);

      if(let_expr==ADDOP||let_expr==MINOP||let_expr==MULTOP ||
         let_expr==GETINT || let_expr==INTTYPE || let_expr==CONST)
         types[type_c++] = INTTYPE;
      else if(let_expr==TRUECONST || let_expr==FALSECONST ||
         let_expr==OR || let_expr==NOT || let_expr==AND ||
         let_expr==GETBOOL ||let_expr==BOOLTYPE || let_expr==LT || let_expr==GT || let_expr == EQ || 
         let_expr == LTEQ || let_expr == GTEQ )
         types[type_c++] = BOOLTYPE;
      else if(let_expr==LET || let_expr==IF || let_expr==NAME){
         types[type_c++] = UNKNOWN_TYPE;
      }
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
int scope_checking(struct ast* node){
   if(node->ntoken==NAME){
      int r_index = return_index(args,node->token); 
      // printf("var: %s, fun-scope: %s, scope: %d -- %d, ID: %d\n",
      //    node->token,
      //    fun_scope[r_index],
      //    scope[r_index][0],
      //    scope[r_index][1],
      //    node->id);
      if(r_index==-1){
         printf("%s not initialized\n",node->token);
         return 1;
      }
      // else if(get_child(get_root(node),1)->ntoken!=MAIN){
      if(strcmp(fun_scope[r_index],"LET")==0){
         for (int i = 0; i < arg_c; i++)
         {
            if(strcmp(fun_scope[i],"LET")==0){
               struct ast* temp = find_ast_node(scope[i][0])->parent;
               struct ast* t_parent = find_parent(node,temp);
               if(t_parent!=NULL && node->parent!=temp && temp!=t_parent &&
                  strcmp(get_child(temp,1)->token,node->token)==0){
                  printf("%s is already declared in %d-%d scope\n",node->token,temp->id,t_parent->id);
                  return 1;
               }
            }
         }
         bool flag = false;
         for (int i = 0; i < arg_c; i++){
            if(strcmp(fun_scope[i],"LET")==0 && strcmp(node->token,args[i])==0){
               struct ast* temp = find_ast_node(scope[i][0])->parent;
               struct ast* t_parent = find_parent(node,temp);
               if (t_parent!=NULL)
               {
                  flag = true;
                  break;
               }
            }
         }
         if(!flag){
            printf("%s is not in scope1\n",node->token);
            return 1;
         }
         return 0;
      }
      else if(get_child(get_root(node),1)->ntoken!=MAIN && strcmp(fun_scope[r_index],"DEFINE")!=0){ 
         //    strcmp(fun_scope[r_index],get_child(get_root(node),1)->token)!=0){
         // printf("%s is not in scope\n",node->token);
         // return 1;

         bool flag = false;
         for (int i = 0; i < arg_c; i++){
            if(strcmp(fun_scope[i],get_child(get_root(node),1)->token)==0){
                  flag = true;
            }
         }
         if(!flag){
            printf("%s is not in scopee\n",node->token);
            return 1;
         }
      }
      //    return 0;
      // }
      else if(get_child(get_root(node),1)->ntoken==MAIN){
         // if(strcmp(fun_scope[r_index],"LET")==0){
         //    printf("%s in not in scope\n", node->token);
         //    return 1;
         // }
         for (int i = 0; i < arg_c; i++)
         {
            if(strcmp(args[i],node->token)==0
                 && strcmp(fun_scope[i],"DEFINE")!=0 && strcmp(fun_scope[i],"LET")!=0){
               printf("Variable %s in not in scope\n",node->token);
               return 1;
            }
         }
         
         
      }
   }
   return 0;
}
int type_checking(struct ast* node){
   tokens[node->id].name = node->token;
   
   if(node->ntoken==INTTYPE)
      tokens[node->id].type = INTTYPE;
   else if(node->ntoken==BOOLTYPE)
      tokens[node->id].type = BOOLTYPE;
   else if(node->ntoken==CONST)
      tokens[node->id].type = INTTYPE;
   else if(node->ntoken==TRUECONST)
      tokens[node->id].type = BOOLTYPE;
   else if(node->ntoken==FALSECONST)
      tokens[node->id].type = BOOLTYPE;
   else if(node->ntoken==GETINT)
      tokens[node->id].type = INTTYPE;
   else if(node->ntoken==GETBOOL)
      tokens[node->id].type = BOOLTYPE;
   else if(node->ntoken==NAME){
      // printf("variable: %s-id: %d\n",node->token,node->id);
      for (int i = 0; i < arg_c; i++)
      {
         // printf("\\\\\\\\function root: %s of %s\n",get_child(get_root(node),1)->token,args[i]);
         if(strcmp(fun_scope[i],"LET")!=0 && strcmp(fun_scope[i],get_child(get_root(node),1)->token)==0 && strcmp(args[i],node->token)==0){
            tokens[node->id].type = types[i];
            break;
         }
         // else if(fun_scope[i]=="LET" &&  && (scope[i][1]>=node->id || scope[i][0]==node->id)){
         //    if(types[i]==999){
         //       printf("/////////////Type of %s is %d\n",node->token,get_child(find_ast_node(scope[i][0])->parent,3)->id);
         //       types[i] = tokens[get_child(find_ast_node(scope[i][0])->parent,2)->id].type; 
         //    }  find_parent
         //    tokens[node->id].type = types[i];
            
         //    break;
         // }
         else if(strcmp(fun_scope[i],"LET")==0 
            && strcmp(args[i],node->token)==0 && scope[i][0]==node->id){
            if(types[i]==999){
               // printf("/////////////Type of %s is %d\n",node->token,get_child(find_ast_node(scope[i][0])->parent,3)->id);
               types[i] = tokens[get_child(find_ast_node(scope[i][0])->parent,2)->id].type; 
            }
            tokens[node->id].type = types[i];
            break;
         }
         else if(strcmp(fun_scope[i],"LET")==0 && strcmp(args[i],node->token)==0){
            
            struct ast* temp = find_ast_node(scope[i][0])->parent;
            struct ast* t_parent = find_parent(node,temp);
            if(t_parent!=NULL && t_parent==temp){
               if(types[i]==999){
                  // printf("/////////////Type of %s is %d\n",node->token,get_child(find_ast_node(scope[i][0])->parent,3)->id);
                  types[i] = tokens[get_child(find_ast_node(scope[i][0])->parent,2)->id].type; 
               }
               tokens[node->id].type = types[i];
               break;
            }
            
         }
      }
      int r_index = return_index(args,node->token);
      if(r_index!=-1 && strcmp(fun_scope[r_index],"DEFINE")==0 && strcmp(node->parent->token,"DEFINE-FUN")!=0 ){
         // printf("heere %s\n",node->token);
         int child_num = get_child_num(node);
         if(count_vars(args[r_index])==child_num){
            if(child_num>0){
               int *b = get_all_index(fun_scope,node->token);
               for (int i = 0; i < child_num; i++)
               {
                 if(tokens[get_child(node,i+1)->id].type!=types[b[i]]){
                     printf("Argument #%d  of Function %s is of wrong type\n",i+1,node->token);
                     return 1;
                 }
               }
            }
            tokens[node->id].type = types[r_index];
         }
         else{
            printf("Number of arguments of %s are not correct\n",node->token);
            return 1;
         }
      }
      else if(r_index!=-1 && strcmp(fun_scope[r_index],"DEFINE")==0 && get_child(get_root(node),1)->ntoken!=MAIN){
         tokens[node->id].type = types[r_index];
      }
   }
   else if (node->ntoken == ADDOP 
      || node->ntoken == MINOP 
      || node->ntoken == MULTOP
      ){
   
      struct ast *child1 = get_child(node,1); 
      struct ast *child2 = get_child(node,2); 
      if(tokens[child1->id].type != INTTYPE){
         printf("First operator of %s is not of type INT\n",node->token);
         return 1;
      } 
      if(tokens[child2->id].type != INTTYPE){
         printf("Second operator of %s is not of type INT\n",node->token);
         return 1;
      }
      tokens[node->id].type = INTTYPE;
   }
   else if ( 
         node->ntoken == AND
      || node->ntoken == OR
      
      ){
   
      struct ast *child1 = get_child(node,1); 
      struct ast *child2 = get_child(node,2); 
      if(tokens[child1->id].type != BOOLTYPE){
         printf("First operator of %s is not of type BOOL\n",node->token);
         return 1;
      } 
      if(tokens[child2->id].type != BOOLTYPE){
         printf("Second operator of %s is not of type BOOL\n",node->token);
         return 1;
      }
      tokens[node->id].type = BOOLTYPE;    
   }
   else if(
         node->ntoken == EQ 
      || node->ntoken == LT 
      || node->ntoken == GT
      || node->ntoken == GTEQ
      || node->ntoken == LTEQ
   ){
      struct ast *child1 = get_child(node,1); 
      struct ast *child2 = get_child(node,2); 
      if(tokens[child1->id].type != tokens[child2->id].type){
         printf("First and second operator of %s is not of same type\n",node->token);
         return 1;
      } 
      tokens[node->id].type = BOOLTYPE; 
   }
   else if(node->ntoken == NOT){
      struct ast *child1 = get_child(node,1);
      if(tokens[child1->id].type != BOOLTYPE){
         printf("\'NOT\' argument %s is not of type BOOL\n",node->token);
         return 1;
      } 
      tokens[node->id].type = BOOLTYPE;   
   }

   else if(node->ntoken==LET){
      // struct ast* root = get_root(node->parent);
      // int child_num = get_child_num(root);
      // printf("%d",root->id);
      // int ret_type = get_child(node,child_num-1)->ntoken;
      // int type = tokens[get_child(node,child_num)->id].type;
      // // printf("%s> %d--%d--%d\n",get_child(node,1)->token,child_num,ret_type,type);
      // if(ret_type != type){
      //    printf("Type of the body of a function %s doesn't match with return type\n",get_child(node,1)->token);
      //    return 1;
      // }
      tokens[node->id].type = tokens[get_child(node,3)->id].type;
   }

   else if(node->ntoken==IF){
      struct ast *child1 = get_child(node,1); 
      struct ast *child2 = get_child(node,2); 
      struct ast *child3 = get_child(node,3);
      if(tokens[child1->id].type != BOOLTYPE){
         printf("First operator of %s is not of type BOOL\n",node->token);
         return 1;
      } 
      if(tokens[child2->id].type != tokens[child3->id].type){
         printf("Second and third operator (%s,%s) of %s are not same\n",tokens[child2->id].name,tokens[child3->id].name,node->token);
         return 1;
      }
      tokens[node->id].type = tokens[get_child(node,3)->id].type;
   }
   else if(strcmp(node->token,"DEFINE-FUN")==0){
      int child_num = get_child_num(node);
      
      int ret_type = get_child(node,child_num-1)->ntoken;
      int type = tokens[get_child(node,child_num)->id].type;
      // printf("%s> %d--%d--%d\n",get_child(node,1)->token,child_num,ret_type,type);
      if(ret_type != type){
         printf("Type of the body of function %s doesn't match with return type\n",get_child(node,1)->token);
         return 1;
      }
   }
   return 0;      
}

int main (int argc, char **argv) {

   int retval = yyparse();

   if (retval == 0) {
      print_ast();
      // int a = visit_ast(declarations);
      // if(a!=0){
      //    return 1;
      // }
      // a = visit_ast(scope_checking);
      // if(a!=0){
      //    return 1;
      // }
      // a = visit_ast(type_checking);
      // if(a!=0){
      //    return 1;
      // }
      visit_ast(construct_cfg);
      print_cfg();
      
      // print_char_array(args);
      // print_array(types);
      // print_scope(scope,0);
      // print_scope(scope,1);
      // print_char_array(fun_scope);
      // printf("\n");
      // print_tokens();
      printf("Success!\n");
   }

   free_ast();
   return retval;
}
