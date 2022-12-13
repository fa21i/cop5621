/**
 * Author:    Grigory Fedyukovich, Subhadeep Bhattacharya
 * Created:   09/30/2020
 *
 * (c) FSU
 **/

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

struct dstr { char* id1; char* id2; };          // A pair of strings: can be useful whenstoring variables associated to a function

struct node_int { int id; struct node_int* next; };
struct node_str { char* id; struct node_str* next; };
struct node_dstr { char* id1; char* id2; struct node_dstr* next; };

struct ast;
struct ast_child {struct ast* id; struct ast_child* next; }; //Dynamic data structure for the list of children
struct ast {int id; char *token; bool is_leaf; int ntoken; char *fun; struct ast_child* child; struct ast* parent; struct ast* next; int line_number; struct statment_s* reg_statment;}; //Dynamic data structure for the AST. Each node points to a terminal/nonterminal and its children

void push_str (char* c, struct node_str** r, struct node_str** t);
char* pop_str (struct node_str** r);
int find_str(char* c, struct node_str* r);
int print_str(struct node_str* r);

void push_dstr (char* c1, char* c2, struct node_dstr** r, struct node_dstr** t);
int find_dstr(char* c1, char* c2, struct node_dstr* r);

void push_int (int i, struct node_int** r, struct node_int** t);
int pop_int (struct node_int** r);

int pop_ast(struct node_int** r, struct node_int** t);

void insert_child(int val);       // Helper methods to specify child/children for the upcoming node
void insert_children(int, ...);   // Variadic version (expects the # of children to be given in the first place)
                                  // Note that it is not necessary to insert all children at once. You can call either insert_child or insert_children many times -- they all will be aggregated and used during the next call of insert_node/insert_node_tmp
void insert_node_tmp(int val, char * token, bool is_leaf, int ntoken, int line_number);
int insert_node(char* token, int ntoken, int line_number);

struct ast* find_ast_node(int id);
struct ast* get_child(struct ast* ast_node, int id);

int visit_ast(int (*f)(struct ast* ast_node));
void print_ast(); //    run "dot -Tpdf ast.dot -o ast.pdf" to create a PDF. Requires a preinstalled graphviz package (https://graphviz.org/download/)

void free_ast();

// static data structres used in the lab:

static int current_node_id = 0;
static struct ast_child* ast_child_root = NULL;
static struct ast* ast_list_root = NULL;
