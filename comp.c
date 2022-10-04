#include "y.tab.h"
#include "ast.h"
int yyparse();
int main (int argc, char **argv) {
        int retval = yyparse();
        if (retval == 0) print_ast();
        free_ast();
        return retval;
}
