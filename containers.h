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

extern struct node_int { int id; struct node_int* next; };
extern struct node_str { char* id; struct node_str* next; };

void push_str (char* c, struct node_str** r, struct node_str** t);
char* pop_str (struct node_str** r, struct node_str** t);
int find_str(char* c, struct node_str* r);
int print_str(struct node_str* r);
void clean_str(struct node_str** r);
void push_int (int i, struct node_int** r, struct node_int** t);
void push_unique_int (int i, struct node_int** r);
int find_int(int c, struct node_int* r);
void clean_int (struct node_int** r);
int pop_int (struct node_int** r, struct node_int** t);
int print_int(struct node_int* r);
void dequeue_int (struct node_int** r);

struct node_var_str { int begin_id; int end_id; int type; char* name; struct node_var_str* next; }; // used for vars in symbol tables
struct node_fun_str { char* name; int type; int arity; struct node_var_str* args; struct node_fun_str* next; };    // used for funs in symbol tables

void push_var_str (int begin_id, int end_id, int type, char* name, struct node_var_str** r, struct node_var_str** t);
struct node_var_str* find_var_str(int loc_id, char* name, struct node_var_str* r);
void clean_var_str(struct node_var_str** r);

void push_fun_str (char* name, int type, int arity, struct node_var_str* args, struct node_fun_str** r, struct node_fun_str** t);
struct node_fun_str* find_fun_str(char* name, struct node_fun_str* r);
void print_fun_str(struct node_fun_str* r);
void clean_fun_str(struct node_fun_str** r);
