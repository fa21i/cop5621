/**
 * Author:    Grigory Fedyukovich, Subhadeep Bhattacharya
 * Created:   09/30/2020
 *
 * (c) FSU
 **/

#include "cfg.h"
#include "y.tab.h"

void push_istr (int c1, char* c2, struct node_istr** r, struct node_istr** t){
  if (*r == NULL) {                          //If root node is null
    *r = (struct node_istr*)malloc(sizeof(struct node_istr)); //Create a new node
    (*r)->id1 = c1;                           //set the id for the new node
    (*r)->id2 = c2;                           //set the id for the new node
    (*r)->next = NULL;                                      //set next pointer to null
    *t = *r;                                                //Set tail pointer == root pointer
  }
  else {                                    //If root node is not null
    struct node_istr* ptr;
    ptr = (struct node_istr*)malloc(sizeof(struct node_istr));  //Create a temporary node
    ptr->id1 = c1;                                            //Set id for the new node
    ptr->id2 = c2;                                            //Set id for the new node
    ptr->next = NULL;                                         //Set next pointer to NULL
    (*t)->next = ptr;                                         //Set the node after tail
    *t = ptr;                                                 //Set tail as the new pointer
  }
}

char* find_istr(struct node_istr* r, int key){
  while (r != NULL){
    if (r->id1 == key) return r->id2;
    r = r->next;
  }
  return NULL;
}

void push_dstr (char* c1, char* c2, struct node_dstr** r, struct node_dstr** t){
  if (*r == NULL) {                          //If root node is null
    *r = (struct node_dstr*)malloc(sizeof(struct node_dstr)); //Create a new node
    (*r)->id1 = c1;                           //set the id for the new node
    (*r)->id2 = c2;                           //set the id for the new node
    (*r)->next = NULL;                                      //set next pointer to null
    *t = *r;                                                //Set tail pointer == root pointer
  }
  else {                                   //If root node is not null
    struct node_dstr* ptr;
    ptr = (struct node_dstr*)malloc(sizeof(struct node_dstr));  //Create a temporary node
    ptr->id1 = c1;                                            //Set id for the new node
    ptr->id2 = c2;                                            //Set id for the new node
    ptr->next = NULL;                                         //Set next pointer to NULL
    (*t)->next = ptr;                                         //Set the node after tail
    *t = ptr;                                                 //Set tail as the new pointer
  }
}

int find_dstr(char* c1, char* c2, struct node_dstr* r){
  while (r != NULL){
    if (strcmp(c1, r->id1) == 0 && strcmp(c2, r->id2) == 0) return 0;
    r = r->next;
  }
  return 1;
}

struct br_instr* mk_cbr(int id, int cond, int succ1, int succ2){
  struct br_instr* tmp = (struct br_instr*)malloc(sizeof(struct br_instr));
  tmp->id = id;
  tmp->cond = cond;
  tmp->succ1 = succ1;
  tmp->succ2 = succ2;
  tmp->next = NULL;
  return tmp;
}

struct br_instr* mk_ubr(int id, int succ1){
  struct br_instr* tmp = (struct br_instr*)malloc(sizeof(struct br_instr));
  tmp->id = id;
  tmp->cond = 0;
  tmp->succ1 = succ1;
  tmp->next = NULL;
  return tmp;
}

void push_br (struct br_instr* i, struct br_instr** r, struct br_instr** t){
  if (*r == NULL){
    *r = i;
    *t = *r;
  }
  else {
    (*t)->next = i;
    (*t) = i;
  }
}

struct asgn_instr* mk_asgn(int bb, int lhs, int bin, int op1, int op2, int type){
  struct asgn_instr* tmp = (struct asgn_instr*)malloc(sizeof(struct asgn_instr));
  tmp->bb = bb;
  tmp->bin = bin;
  tmp->lhs = lhs;
  tmp->op1 = op1;
  tmp->op2 = op2;
  tmp->type = type;
  tmp->next = NULL;
  return tmp;
}

struct asgn_instr* mk_basgn(int bb, int lhs, int op1, int op2, int type){
  struct asgn_instr* tmp = (struct asgn_instr*)malloc(sizeof(struct asgn_instr));
  tmp->bb = bb;
  tmp->lhs = lhs;
  tmp->bin = 1;
  tmp->op1 = op1;
  tmp->op2 = op2;
  tmp->type = type;
  tmp->next = NULL;
  return tmp;
}

struct asgn_instr* mk_uasgn(int bb, int lhs, int op, int type){
  struct asgn_instr* tmp = (struct asgn_instr*)malloc(sizeof(struct asgn_instr));
  tmp->bb = bb;
  tmp->lhs = lhs;
  tmp->bin = 0;
  tmp->op1 = op;
  tmp->op2 = -1;
  tmp->type = type;
  tmp->next = NULL;
  return tmp;
}

struct asgn_instr* mk_casgn(int bb, int lhs, char* fun){
  struct asgn_instr* tmp = (struct asgn_instr*)malloc(sizeof(struct asgn_instr));
  tmp->bb = bb;
  tmp->lhs = lhs;
  tmp->bin = 2;
  tmp->fun = fun;
  tmp->next = NULL;
  return tmp;
}

void rm_asgn (struct asgn_instr* i, struct asgn_instr** r, struct asgn_instr** t){
  struct asgn_instr* pred = (*r);
  while (pred != NULL && pred->next != i) pred = pred->next;
  if (pred == NULL) return;
  if (i == *t) (*t) = pred;
  else (*pred).next = i->next;
  free(i);
}

//int find_asgn (struct asgn_instr* i, struct asgn_instr* r){
//  while (r != NULL){
//    if (r == i) return 0;
//    else r = r->next;
//  }
//  return 1;
//}

void push_asgn (struct asgn_instr* i, struct asgn_instr** r, struct asgn_instr** t){
  if (*r == NULL){
    *r = i;
    *t = *r;
  }
  else {
    (*t)->next = i;
    (*t) = i;
  }
}

void clean_asgns (struct asgn_instr** r){
  while (*r != NULL){
    struct asgn_instr* tmp = *r;
    (*r) = (*r)->next;
    free(tmp);
  }
}

void clean_bbs (struct br_instr** r){
  while (*r != NULL){
    struct br_instr* tmp = *r;
    (*r) = (*r)->next;
    free(tmp);
  }
}

void clean_istr(struct node_istr** r){
  while (*r != NULL){
    struct node_istr* tmp = *r;
    (*r) = (*r)->next;
    free(tmp);
  }
}

void clean_dstr(struct node_dstr** r){
  while (*r != NULL){
    struct node_dstr* tmp = *r;
    (*r) = (*r)->next;
    free(tmp);
  }
}

void push_ncfg (int src, int dst, bool valid, bool final, struct asgn_instr* asgns, struct br_instr* br, char* fun, struct cfg** r, struct cfg** t)
{
  if (*r == NULL){
    *r = (struct cfg*)malloc(sizeof(struct cfg));
    (*r)->src = src;
    (*r)->dst = dst;
    (*r)->valid = valid;
    (*r)->final = final;
    (*r)->next = NULL;
    (*r)->fun = fun;
    (*r)->br = br;
    (*r)->asgns = asgns;
    *t = *r;
  }
  else{
    struct cfg* ptr;
    ptr = (struct cfg*)malloc(sizeof(struct cfg));
    ptr->src = src;
    ptr->dst = dst;
    ptr->valid = valid;
    ptr->final = final;
    ptr->next = NULL;
    ptr->fun = fun;
    ptr->br = br;
    ptr->asgns = asgns;
    (*t)->next = ptr;
    (*t) = ptr;
  }
}

void push_cfg (int src, int dst, bool valid, bool final, struct asgn_instr* asgns, struct br_instr* br, struct cfg** r, struct cfg** t)
{
  push_ncfg(src, dst, valid, final, asgns, br, (*t)->fun, r, t);
}

void get_next(struct cfg* t, int src, int *n1, int *n2)
{
  *n1 = 0;
  *n2 = 0;
  struct cfg* r = t;
  while (r != NULL)
  {
    if (r->valid && src == r->src)
    {
      if (*n1 == 0) *n1 = r->dst;
      else *n2 = r->dst;
    }
    r = r->next;
  }
}


void get_prev(struct cfg* t, int dst, struct cfg** c1, struct cfg** c2)
{
  *c1 = NULL;
  *c2 = NULL;
  struct cfg* r = t;
  while (r != NULL)
  {
    if (r->valid && dst == r->dst)
    {
      if (*c1 == NULL) *c1 = r;
      else *c2 = r;
    }
    r = r->next;
  }
}

struct node_int* printed_r = NULL;
struct node_int* printed_t = NULL;

void print_cfg_ir(struct cfg* t, int sz, struct node_fun_str* fun_r) {
  while (fun_r != NULL) {
    fprintf (stderr, "\nfunction %s\n", fun_r->name);
    for (int i = 0; i <= sz; i++) {
      struct cfg *r = t;
      while (r != NULL) {
        struct cfg* c1; struct cfg* c2;
        get_prev(t, r->src, &c1, &c2);
        if (r->valid && (r->dst == i || (c1 == NULL && c2 == NULL))
            && strcmp (fun_r->name, r->fun) == 0) {
          if (1 == find_int(r->dst, printed_r)) {
            push_int(r->dst, &printed_r, &printed_t);
            fprintf(stderr, "bb%d:\n", r->dst);

            if (r->asgns != NULL) {
              if (r->asgns->bin == 0) {
                if (r->asgns->type == CONST)
                  fprintf(stderr, "  v%d := %d\n", r->asgns->lhs, r->asgns->op1);
                else if (r->asgns->type == NOT)
                  fprintf(stderr, "  v%d := not v%d\n", r->asgns->lhs, r->asgns->op1);
                else if (r->asgns->type == INP)
                  fprintf(stderr, "  v%d := a%d\n", r->asgns->lhs, -r->asgns->op1);
                else if (r->asgns->lhs == 0)
                  fprintf(stderr, "  rv := v%d\n", r->asgns->op1);
                else if (r->asgns->lhs < 0)
                  fprintf(stderr, "  a%d := v%d\n", -r->asgns->lhs, r->asgns->op1);
                else
                  fprintf(stderr, "  v%d := v%d\n", r->asgns->lhs, r->asgns->op1);
              } else if (r->asgns->bin == 1) {
                if (r->asgns->type == EQ)
                  fprintf(stderr, "  v%d := v%d = v%d\n", r->asgns->lhs, r->asgns->op1, r->asgns->op2);
                else if (r->asgns->type == LT)
                  fprintf(stderr, "  v%d := v%d < v%d\n", r->asgns->lhs, r->asgns->op1, r->asgns->op2);
                else if (r->asgns->type == PLUS)
                  fprintf(stderr, "  v%d := v%d + v%d\n", r->asgns->lhs, r->asgns->op1, r->asgns->op2);
                else if (r->asgns->type == MINUS)
                  fprintf(stderr, "  v%d := v%d - v%d\n", r->asgns->lhs, r->asgns->op1, r->asgns->op2);
                else if (r->asgns->type == AND)
                  fprintf(stderr, "  v%d := v%d and v%d\n", r->asgns->lhs, r->asgns->op1, r->asgns->op2);
                else if (r->asgns->type == LE)
                  fprintf(stderr, "  v%d := v%d <= v%d\n", r->asgns->lhs, r->asgns->op1, r->asgns->op2);
                else if (r->asgns->type == MULT)
                  fprintf(stderr, "  v%d := v%d * v%d\n", r->asgns->lhs, r->asgns->op1, r->asgns->op2);
                else if (r->asgns->type == DIV)
                  fprintf(stderr, "  v%d := v%d div v%d\n", r->asgns->lhs, r->asgns->op1, r->asgns->op2);
                else if (r->asgns->type == MOD)
                  fprintf(stderr, "  v%d := v%d mod v%d\n", r->asgns->lhs, r->asgns->op1, r->asgns->op2);
                else if (r->asgns->type == GT)
                  fprintf(stderr, "  v%d := v%d > v%d\n", r->asgns->lhs, r->asgns->op1, r->asgns->op2);
                else if (r->asgns->type == GE)
                  fprintf(stderr, "  v%d := v%d >= v%d\n", r->asgns->lhs, r->asgns->op1, r->asgns->op2);
                else if (r->asgns->type == OR)
                  fprintf(stderr, "  v%d := v%d or v%d\n", r->asgns->lhs, r->asgns->op1, r->asgns->op2);
              } else if (r->asgns->bin == 2) {
                if (r->asgns->lhs == 0)
                  fprintf(stderr, "  call %s \n", r->asgns->fun);
                else if (strcmp(r->asgns->fun, "print") != 0)
                  fprintf(stderr, "  v%d := rv\n", r->asgns->lhs);
              }
            }
            if (r->br != NULL) {
              if (r->br->succ1 != 0) {
                if (r->br->cond == 0)
                  fprintf(stderr, "  br bb%d\n", r->br->succ1);
                else
                  fprintf(stderr, "  br v%d bb%d bb%d\n", r->br->cond, r->br->succ1, r->br->succ2);
              }
            }
          }
        }
        r = r->next;
      }
    }
    fun_r = fun_r->next;
  }
}

void basic_optim(struct cfg* t, int sz, struct node_fun_str* fun_r){
  //non branching blocks
  //check successor -- check that successor doesn't have more than one parent -- merge
  while (fun_r != NULL) {
    fprintf (stderr, "\nfunction %s\n", fun_r->name);
    for (int i = 0; i <= sz; i++) {
      struct cfg *r = t;
      while (r != NULL) {
        struct cfg* c1; struct cfg* c2;
        get_prev(t, r->src, &c1, &c2);
        if (r->valid && (r->dst == i || (c1 == NULL && c2 == NULL))
            && strcmp (fun_r->name, r->fun) == 0) {
          if (1 == find_int(r->dst, printed_r)) {
            push_int(r->dst, &printed_r, &printed_t);
            if(i==0){
              printf(stderr, "bb%d:\n", r->dst);
            }

            if(c1 != NULL && c2 == NULL)
            {
              //add instr of r to instr of c1, c1 br
              
            }

            if (r->asgns != NULL) 
            {
              if (r->asgns->bin == 0) 
              { //if const, not, inp, lhs == 0, lhs < 0)  
                if (r->asgns->type == CONST)
                  fprintf(stderr, "  v%d := %d\n", r->asgns->lhs, r->asgns->op1);
                else if (r->asgns->type == NOT)
                  fprintf(stderr, "  v%d := not v%d\n", r->asgns->lhs, r->asgns->op1);
                else if (r->asgns->type == INP)
                  fprintf(stderr, "  v%d := a%d\n", r->asgns->lhs, -r->asgns->op1);
                else if (r->asgns->lhs == 0)
                  fprintf(stderr, "  rv := v%d\n", r->asgns->op1);
                else if (r->asgns->lhs < 0)
                  fprintf(stderr, "  a%d := v%d\n", -r->asgns->lhs, r->asgns->op1);
                else
                  fprintf(stderr, "  v%d := v%d\n", r->asgns->lhs, r->asgns->op1);
              } 
              else if (r->asgns->bin == 1) 
              { //if is binary operation
              } 
              else if (r->asgns->bin == 2) 
              { //call, print, rv
              } 
            }
            if (r->br != NULL) {
              if (r->br->succ1 != 0) {
                if (r->br->cond == 0)
                  fprintf(stderr, "  br bb%d\n", r->br->succ1);
                else
                  fprintf(stderr, "  br v%d bb%d bb%d\n", r->br->cond, r->br->succ1, r->br->succ2);
              }  
            }
          }
        }
        r=r->next;
      }       
    }
    fun_r = fun_r->next;
  }
}

void dup_optim(){
  //two blocks have save set of instructions
  //check through blocks for same instr
}

void unr_optim(){
  //branching br with always true/false
  //check for br with guard -- check through instr for conditionality
  // -- find where block meets back up -- delete unreachable code
}
