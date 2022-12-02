%{
  #include "y.tab.h"
  int yylex();
%}
%%

\n {yylineno++;}
[ \t] {}
";".+$ ;
[-]?([1-9][0-9]*|0) { /*fprintf(stderr,"line %d: integer const: \t\t%s\n", yylineno, yytext);*/ yylval.str = strdup(yytext); return CONST;}
"=" {/*fprintf(stderr,"line %d: equality: \t\t\t%s\n", yylineno, yytext);*/ return EQ;}
"<" {/*fprintf(stderr,"line %d: comparison: \t\t%s\n", yylineno, yytext);*/ return LT;}
">" {/*fprintf(stderr,"line %d: comparison: \t\t%s\n", yylineno, yytext);*/ return GT;}
"<=" {/*fprintf(stderr,"line %d: comparison: \t\t%s\n", yylineno, yytext);*/ return LE;}
">=" {/*fprintf(stderr,"line %d: comparison: \t\t%s\n", yylineno, yytext);*/ return GE;}
"+" {/*fprintf(stderr,"line %d: plus: \t\t\t%s\n", yylineno, yytext);*/ return PLUS;}
"-" {/*fprintf(stderr,"line %d: minus: \t\t\t\t%s\n", yylineno, yytext);*/ return MINUS;}
"*" {/*fprintf(stderr,"line %d: multiply: \t\t\t%s\n", yylineno, yytext);*/ return MULT;}
"div" {/*fprintf(stderr,"line %d: division: \t\t%s\n", yylineno, yytext);*/ return DIV;}
"mod" {/*fprintf(stderr,"line %d: modulus: \t\t%s\n", yylineno, yytext);*/ return MOD;}
"define-fun" {/*fprintf(stderr,"line %d: function definition: %s\n", yylineno, yytext);*/ return DEFFUN;}
"get-int" {/*fprintf(stderr,"line %d: function call: \t\t%s\n", yylineno, yytext);*/ return GETINT;}
"get-bool" {/*fprintf(stderr,"line %d: function call: \t\t%s\n", yylineno, yytext);*/ return GETBOOL;}
"true" {/*fprintf(stderr,"line %d: boolean const: \t\t%s\n", yylineno, yytext);*/ return TRUE;}
"false" {/*fprintf(stderr,"line %d: boolean const: \t\t%s\n", yylineno, yytext);*/ return FALSE;}
"and" {/*fprintf(stderr,"line %d: conjunction: \t\t%s\n", yylineno, yytext);*/ return AND;}
"or" {/*fprintf(stderr,"line %d: disjunction: \t\t%s\n", yylineno, yytext);*/ return OR;}
"not" {/*fprintf(stderr,"line %d: negation: \t\t\t%s\n", yylineno, yytext);*/ return NOT;}
"if" {/*fprintf(stderr,"line %d: if-then-else: \t\t%s\n", yylineno, yytext);*/ return IF;}
"let" {/*fprintf(stderr,"line %d: local variable: \t%s\n", yylineno, yytext);*/ return LET;}
"int" {/*fprintf(stderr,"line %d: type declaration: \t%s\n", yylineno, yytext);*/ return INT;}
"bool" {/*fprintf(stderr,"line %d: type declaration: \t%s\n", yylineno, yytext);*/ return BOOL;}
"print" {/*fprintf(stderr,"line %d: program entry: \t\t%s\n", yylineno, yytext);*/ return PRINT;}
"eval" {/*fprintf(stderr,"line %d: main: \t%s\n", yylineno, yytext);*/ return EVAL;}
[a-zA-Z][a-zA-Z0-9]* { yylval.str = strdup(yytext); /*fprintf(stderr,"line %d: id: \t\t\t\t%s\n", yylineno, yylval.str);*/ return ID;}
[(] {/*fprintf(stderr,"line %d: left parenthesis: \t%s\n", yylineno, yytext);*/ return LPAR;}
[)] {/*fprintf(stderr,"line %d: right parenthesis: \t%s\n", yylineno, yytext);*/ return RPAR;}
. { /*fprintf(stderr,"line %d: Lex error: \t%s\n", yylineno, yytext);*/ return ERR;}
%%

int yywrap(void) {return 1;}
