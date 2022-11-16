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
#include "containers.h"

struct dstr { char* id1; char* id2; };          // A pair of strings: can be useful when storing variables associated to a function

struct node_dstr { char* id1; char* id2; struct node_dstr* next; };
struct node_istr { int id1; char* id2; struct node_istr* next; };

void push_istr (int c1, char* c2, struct node_istr** r, struct node_istr** t);
char* find_istr(struct node_istr* r, int key);
void clean_istr(struct node_istr** r);

struct br_instr { int id; int cond; int succ1; int succ2; struct br_instr* next;};
struct br_instr* mk_cbr(int id, int cond, int succ1, int succ2);
struct br_instr* mk_ubr(int id, int succ1);
void push_br (struct br_instr* i, struct br_instr** r, struct br_instr** t);
void clean_bbs (struct br_instr** r);

struct asgn_instr {int bb; int lhs; int bin; int op1; int op2; int type; char* fun; struct asgn_instr* next; };
struct asgn_instr* mk_asgn(int bb, int lhs, int bin, int op1, int op2, int type);
struct asgn_instr* mk_basgn(int bb, int lhs, int op1, int op2, int type);
struct asgn_instr* mk_uasgn(int bb, int lhs, int op, int type);
struct asgn_instr* mk_casgn(int bb, int lhs, char* fun);
void push_asgn (struct asgn_instr* i, struct asgn_instr** r, struct asgn_instr** t);
//int find_asgn (struct asgn_instr* i, struct asgn_instr* r);
void rm_asgn (struct asgn_instr* i, struct asgn_instr** r, struct asgn_instr** t);
void clean_asgns (struct asgn_instr** r);

struct cfg {int src; int dst; bool valid; bool final; struct cfg* next; struct asgn_instr* asgns; struct br_instr* br; char* fun; };

void get_next(struct cfg* t, int src, int *n1, int *n2);
void get_prev(struct cfg* t, int dst, struct cfg** c1, struct cfg** c2);
void push_cfg (int src, int dst, bool valid, bool final, struct asgn_instr* asgns, struct br_instr* br, struct cfg** r, struct cfg** t);
void push_ncfg (int src, int dst, bool valid, bool final, struct asgn_instr* asgns, struct br_instr* br, char* fun, struct cfg** r, struct cfg** t);
void print_cfg_ir(struct cfg* r, int sz, struct node_fun_str* fun_r);

void optim(struct cfg* r, int sz, struct node_fun_str* fun_r);