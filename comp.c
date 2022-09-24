#include "new_parser.tab.h"
#include "ast.h"
int yyparse();
extern FILE *yyin;

int main (int argc, char **argv) {
  char filename[50];
  int retval;

  yyin=fopen("tests/correct programs/sample3.txt ","r+");
  if(yyin==NULL)
  {
          free_ast();
          return 0;
  }
  else 
  {
          retval = yyparse();
          print_ast();
          free_ast();
  }
  printf("retval: %d",retval);
  return retval;

}