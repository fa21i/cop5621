#include "y.tab.h"
#include "ast.h"
int yyparse();

int demo(struct ast* node)
{
        
       /*Semantic checking
           each variable v can be used only in two cases

              in the body of a function if v is declared as one of the function's arguements:
                 e.g., (define-fun f (v int) bool (= v 1)) -- allwed
                       (define-fun g bool (= v 1)) -- not allowed (semantic error)
           
              in the body of a let-binder if v is declared in the let-s first argumen,
                 e.g., (+ (let (v 1) (= v 1)) v) -- not allowed 

           each variable name can be declared exactly once:
              per each function,
                 e.g., (define-fun f (v int) (v bool) ...) --  not allowed
              a sequence of nested let-biinders,
                 e.g., (let (x ...) (let (x ...) ...)) --  not allowed (cant redefine)
              or both,
                 e.g., (define-fun f (v int) ... (let (v ...) ...)) -- not allowed
              but... (+ (let (x ...) x) (let (x ...) x)) -- allowed (stay within their scopes, not the same "x")

           functions can be called only if thye are previously defined (except get-int and get-bool)
              (define-fun f (v int) bool (g))
              (define-fun g bool (get-bool)) -- not allowed (g is used before it's declared, get-bool is allowed, should be switched)
           
           each function name can be used in exactly one define-fun and cannot be an input/local variable name of the defined function
              (define-fun f (f int) bool true)
              (define-fun f bool true)

              (define-fun g (v int) bool (let (g true) g))
       
        (define-fun f1 (i int) bool (= i 1))
        (define-fun f2 (i bool) (j int) int (if i j 0))
        (print (let (x (f2 (get-bool) (get-int))) x))
        
        vsit fin: f1
           visit its arg: i of type INT, valid in 2 -- 6
          user var (of unknown type): i in location 4
        visit fun: f2
           declare its arg: i of type BOOL, valid in 8 -- 14 (scope)
           declare its arg: j of type INT, valid in 9 -- 14
          use var (of unknown type): i in location 11
          use var (of unknown type): j in location 12
        visit fun: main
          declare LET var: x, scope 18 -- 18
          use var (of unknown type) x in location 17 (bug/semantic error)
          use var (of unknown type) x in location 18
        */
        
        
        //inclass demo function
        //printf("visit: %s\n", node->token);

        if(node->token == FUNID)
        {
                printf("visit fun: %s\n", node->token);
                struct ast* parent = node->parent;
                int how_many_children = get_children_num(parent):
                int id_fun_body = get_child(parent, how_many_children)->id;
                for(int i = 0; i < how_many_children - 3; i++)
                {
                        struct ast *arg = get_child(parent, i + 2);
                        printf("   declare its arg: %s of type %s, valid in %d -- %d\n",
                         arg->token, (arg->token == INTID ? "INT" : "BOOL"), arg->id, id_fun_body);
                }
        }
        if(node->token == VARID)
           printf("  use var (of unknown type): %s in location %d\n", node->token, node->id);
        if(node->token == LETID){
           struct ast* parent = node->parent;
           struct ast* first_sib = get_child(parent, 2);
           struct ast* second_sib = get_child(parent, 3);
           printf("  declare LET var: %s, scope %d -- %d\n",
           node->token, first_sib->id + 1, second_sib->id);
        }


        /*if(node->token == INTID)
                printf("visit int var: %s\n", node->token);
        if(node->token == BOOLID)
                printf("visit bool var: %s\n", node->token);*/
        return 0;
}

int main (int argc, char **argv) {
        int retval = yyparse();
        //TODO: semantic analysis: symbol table + well-form, type check, etc
        //AST -> CFG
        //optims
        //code generation
        visit_ast(demo);


        if (retval == 0) print_ast();
        free_ast();
        return retval;
}
