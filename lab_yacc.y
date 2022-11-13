%{
void yyerror (char *s);
int yylex();
#include "ast.h"
#include "containers.h"

struct node_int* tmp_r;
struct node_int* tmp_t;
%}

%union {int val; char* str;}
%start program
%token PLUS MINUS DIV MOD MULT EQ LT GT GE LE NOT OR AND IF LET LPAR RPAR GETINT GETBOOL DEFFUN TRUE FALSE INT
BOOL ERR PRINT EVAL CALL FUNID REGID INP
%token<str> ID CONST BOOLID INTID LETID VARID
%type<val> expr funid letid intid boolid type vars exprs

%%
program : LPAR funid expr RPAR {
  insert_children (2, $2, $3);
  insert_node("ENTRY", PRINT);}
| LPAR DEFFUN funid vars type expr RPAR program {
  insert_child($3);
  for (int i = 0; i < $4; i++) insert_child(pop_int(&tmp_r, &tmp_t));
  insert_children (2, $5, $6);
  insert_node("DEF-FUN", DEFFUN);
};

type : INT { $$ = insert_node("ret INT", INT);}
| BOOL { $$ = insert_node("ret BOOL", BOOL);};

funid : ID { $$ = insert_node($1, FUNID);}
| PRINT { $$ = insert_node("main", FUNID);}

letid : ID { $$ = insert_node($1, LETID);};
intid : ID { $$ = insert_node($1, INTID);};
boolid : ID { $$ = insert_node($1, BOOLID);};

vars : LPAR intid INT RPAR vars {push_int ($2, &tmp_r, &tmp_t); $$ = $5 + 1;}
| LPAR boolid BOOL RPAR vars { push_int ($2, &tmp_r, &tmp_t); $$ = $5 + 1;}
| {$$ = 0;};

exprs : expr exprs { push_int ($1, &tmp_r, &tmp_t); $$ = $2 + 1;}
| {$$ = 0;};

expr : CONST { $$ = insert_node ($1, CONST);}
| TRUE{ $$ = insert_node("TRUE", TRUE);}
| FALSE { $$ = insert_node("FALSE", FALSE);}
| ID { $$ = insert_node($1, VARID);}
| LPAR PLUS expr expr exprs RPAR {
  insert_children (2, $3, $4);
  for (int i = 0; i < $5; i++) insert_child(pop_int(&tmp_r, &tmp_t));
  $$ = insert_node("PLUS", PLUS);}
| LPAR MINUS expr expr RPAR {
  insert_children (2, $3, $4);
  $$ = insert_node("MINUS", MINUS);}
| LPAR MULT expr expr exprs RPAR {
  insert_children (2, $3, $4);
  for (int i = 0; i < $5; i++) insert_child(pop_int(&tmp_r, &tmp_t));
  $$ = insert_node("MULT", MULT);}
| LPAR DIV expr expr RPAR {
  insert_children (2, $3, $4);
  $$ = insert_node("DIV", DIV);}
| LPAR MOD expr expr RPAR {
  insert_children (2, $3, $4);
  $$ = insert_node("MOD", MOD);}
| LPAR EQ expr expr RPAR {
  insert_children (2, $3, $4);
  $$ = insert_node("EQ", EQ);}
| LPAR LT expr expr RPAR {
  insert_children (2, $3, $4);
  $$ = insert_node("LT", LT);}
| LPAR LE expr expr RPAR {
  insert_children (2, $3, $4);
  $$ = insert_node("LE", LE);}
| LPAR GE expr expr RPAR {
  insert_children (2, $3, $4);
  $$ = insert_node("GE", GE);}
| LPAR GT expr expr RPAR {
  insert_children (2, $3, $4);
  $$ = insert_node("GT", GT);}
| LPAR AND expr expr exprs RPAR {
  insert_children (2, $3, $4);
  for (int i = 0; i < $5; i++) insert_child(pop_int(&tmp_r, &tmp_t));
  $$ = insert_node("AND", AND);}
| LPAR OR expr expr exprs RPAR {
  insert_children (2, $3, $4);
  for (int i = 0; i < $5; i++) insert_child(pop_int(&tmp_r, &tmp_t));
  $$ = insert_node("OR", OR);}
| LPAR NOT expr RPAR {
  insert_child ($3);
  $$ = insert_node("NOT", NOT);}
| LPAR IF expr expr expr RPAR {
  insert_children (3, $3, $4, $5);
  $$ = insert_node("IF", IF);}
| LPAR LET LPAR letid expr RPAR expr RPAR {
  insert_children (3, $4, $5, $7);
  $$ = insert_node("LET", LET);}
| LPAR ID exprs RPAR {
  for (int i = 0; i < $3; i++) insert_child(pop_int(&tmp_r, &tmp_t));
  $$ = insert_node($2, CALL);}
| LPAR GETINT RPAR {
  $$ = insert_node("GET-INT", CALL);}
| LPAR GETBOOL RPAR {
  $$ = insert_node("GET-BOOL", CALL);}
;
%%

void yyerror (char *s) {fprintf (stderr, "%s\n", s);}             /* if any error occurs print it */
