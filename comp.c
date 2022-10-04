#include "y.tab.h"
#include "ast.h"
int yyparse();

int demo(struct ast* node)
{
        //inclass demo function
        printf("visit: %s\n", node->token);
        return 0;
}

int main (int argc, char **argv) {
        int retval = yyparse();
        //TODO: semantic analysis: symbol table + well-form, type check, etc
        //AST -> CFG
        //optims
        //code generation


        if (retval == 0) print_ast();
        free_ast();
        return retval;
}
