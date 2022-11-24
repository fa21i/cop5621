/**
 * Author:    Grigory Fedyukovich, Subhadeep Bhattacharya
 * Created:   09/30/2020
 *
 * (c) FSU
 **/

#include "cfg.h"
#include "y.tab.h"
#define NOVALUE -9999

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
  return tmp;
}

struct br_instr* mk_ubr(int id, int succ1){
  struct br_instr* tmp = (struct br_instr*)malloc(sizeof(struct br_instr));
  tmp->id = id;
  tmp->cond = 0;
  tmp->succ1 = succ1;
  tmp->succ2 = 0;
  return tmp;
}

bool same_binstr (struct br_instr* a1, struct br_instr* a2)
{
  if (a1->cond != a2->cond) return false;
  if (a1->succ1 != a2->succ1) return false;
  if (a1->succ2 != a2->succ2) return false;
  return true;
}


bool same_instr (struct asgn_instr* a1, struct asgn_instr* a2)
{
  if (a1->bin != a2->bin) return false;
  if (a1->lhs != a2->lhs) return false;
  if (a1->op1 != a2->op1) return false;
  if (a1->op2 != a2->op2) return false;
  if (a1->type != a2->type) return false;
  return true;
}

bool is_asgn_to_v(struct asgn_instr* a)
{
  if (a == NULL) return false;
  if (a->bin == 0)
  {
    if (a->type == CONST) return false;
    else if (a->type == NOT) return false;
    else if (a->type == INP) return false; // .. := a
    else if (a->lhs == 0) return false;    // rv :=
    else if (a->lhs < 0) return false;
  }
  return (a->bin <= 1);
}

struct asgn_instr* mk_asgn(int bb, int lhs, int bin, int op1, int op2, int type){
  struct asgn_instr* tmp = (struct asgn_instr*)malloc(sizeof(struct asgn_instr));
  tmp->bb = bb;
  tmp->bin = bin;
  tmp->lhs = lhs;
  tmp->op1 = op1;
  tmp->op2 = op2;
  tmp->type = type;
  tmp->valid = true;
  tmp->prev = NULL;
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
  tmp->valid = true;
  tmp->prev = NULL;
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
  tmp->valid = true;
  tmp->prev = NULL;
  tmp->next = NULL;
  return tmp;
}

struct asgn_instr* mk_casgn(int bb, int lhs, char* fun){
  struct asgn_instr* tmp = (struct asgn_instr*)malloc(sizeof(struct asgn_instr));
  tmp->bb = bb;
  tmp->lhs = lhs;
  tmp->bin = 2;
  tmp->fun = fun;
  tmp->valid = true;
  tmp->prev = NULL;
  tmp->next = NULL;
  return tmp;
}

void rm_asgn (struct asgn_instr* i, struct asgn_instr** r, struct asgn_instr** t){
  struct asgn_instr* pred = (*r);
  while (pred != NULL && pred->next != i) pred = pred->next;
  if (pred == NULL) return;
  if (i == *t) (*t) = pred;
  else pred->next = i->next;
  free(i);
}

//int find_asgn (struct asgn_instr* i, struct asgn_instr* r){
//  while (r != NULL){
//    if (r == i) return 0;
//    else r = r->next;
//  }
//  return 1;
//}

void push_asgn (struct asgn_instr* i, struct asgn_instr* r){
  if (i == NULL) return;
  if (r == NULL)
    r = i;
  else if ((r)->next == NULL){
    r->next = i;
    i->prev = r;
  }
  else
    push_asgn(i, r->next);
}

struct asgn_instr* get_last_asgn (struct asgn_instr* r){
  if (r == NULL)
    return NULL;
  else if (r->next == NULL)
    return r;
  else
    return get_last_asgn(r->next);
}

struct asgn_instr* get_last_val (struct asgn_instr* r, int v, int bin, int ty){
  if (r == NULL)
    return NULL;
  else if (r->valid    && (r->bin == bin || bin == 999) &&
           r->lhs == v && (r->type == ty || ty == 999))
    return r;
  else
    return get_last_val(r->prev, v, bin, ty);
}

void clean_asgns (struct asgn_instr** r){
  while (*r != NULL){
    struct asgn_instr* tmp = *r;
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
//  printf("push_ncfg: [%d -> %d]\n", src, dst );
  if (*r == NULL){
    *r = (struct cfg*)malloc(sizeof(struct cfg));
    (*r)->id = dst;
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
    ptr->id = dst;
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

void find_nodes(struct cfg* t, int src, struct cfg** c1, struct cfg** c2)
{
  *c1 = NULL;
  *c2 = NULL;
  struct cfg* r = t;
  while (r != NULL)
  {
    if (r->valid && src == r->src)
    {
      if (*c1 == NULL) *c1 = r;
      else *c2 = r;
    }
    r = r->next;
  }
}

void get_bb_ids(struct cfg* t){
  struct cfg* r = t;
  while (r != NULL){
    if (1 == find_int(r->id, bbs_r)) {
      push_int(r->id, &bbs_r, &bbs_t);
    }
    r = r->next;
  }
}

bool is_transit (int bb_id, struct cfg* t, struct cfg** s, struct cfg** d){
  struct cfg* e = t;
  int num_src = 0;
  int num_dst = 0;
  while (e != NULL){
    if (e->valid){
      if (e->src == bb_id){
        num_dst++;
        *d = e;
      }
      if (e->dst == bb_id){
        num_src++;
        *s = e;
      }
    }
    e = e->next;
  }
  return (num_src == 1 && num_dst == 1 && (*s)->br->cond == 0);
}

bool is_fork (int bb_id, struct cfg* t, struct cfg** s, struct cfg** d1, struct cfg** d2){
  struct cfg* e = t;
  int num_src = 0;
  int num_dst = 0;
  while (e != NULL){
    if (e->valid){
      if (e->src == bb_id){
        num_dst++;
        if (*d1 == NULL) *d1 = e;
        else *d2 = e;
      }
      if (e->dst == bb_id){
        num_src++;
        *s = e;
      }
    }
    e = e->next;
  }
  return (num_src == 1 && num_dst == 2);
}

bool cfg_compact (struct cfg* t){
  bool found = false;
  struct node_int* r = bbs_r;
  while (r != NULL){
    // check if bb with r->id can be removed:
    struct cfg* s;
    struct cfg* d = NULL;
    if (is_transit (r->id, t, &s, &d)){
      found = 0;
      printf("compaction: [%d -> %d] -> [%d -> %d]", s->src, s->dst, d->src, d->dst);
      d->valid = false;
      push_asgn(d->asgns, (s->asgns));
      s->br = d->br;
      s->dst = d->dst;
      printf("  ->  [%d -> %d]\n", s->src, s->dst);
    }
    r = r->next;
  }
  return found;
}

bool cfg_unreach (struct cfg* t){
  bool found = false;
  struct node_int* r = bbs_r;
  while (r != NULL){
    struct cfg* s;
    struct cfg* d1 = NULL;
    struct cfg* d2 = NULL;
    if (is_fork (r->id, t, &s, &d1, &d2)){
      // found conditional:
      if (s->br->succ1 != d1->id){
        struct cfg* d3 = d1;
        d1 = d2;
        d2 = d3;
      }
      struct asgn_instr* a = get_last_asgn (s->asgns);
      if (a != NULL){
        if (a->lhs == s->br->cond && a->type == CONST){
          if (a->op1 == 0){
            printf("unreach first: [%d -> %d] + [%d -> %d] \n", d1->src, d1->dst, d2->src, d2->dst);
            d1->valid = 0;
            s->br->cond = 0;
            s->br->succ1 = s->br->succ2;
            s->br->succ2 = 0;
            found = true;
          }
          else if (a->op1 == 1) {
            printf("unreach second: [%d -> %d] + [%d -> %d] \n", d1->src, d1->dst, d2->src, d2->dst);
            d2->valid = 0;
            s->br->cond = 0;
            s->br->succ2 = 0;
            found = true;
          }
        }
      }
    }
    r = r->next;
  }
  return found;
}

bool cfg_dupl (struct cfg* t){
  bool found = false;
  struct node_int* r = bbs_r;
  while (r != NULL){
    struct cfg* s;
    struct cfg* d1 = NULL;
    struct cfg* d2 = NULL;
    if (is_fork (r->id, t, &s, &d1, &d2)){
      struct asgn_instr* a1 = d1->asgns;
      struct asgn_instr* a2 = d2->asgns;
      while (a1 != NULL || a2 != NULL){
        if (!a1->valid) {a1 = a1->next; continue;}
        if (!a2->valid) {a2 = a2->next; continue;}
        if (same_instr(a1, a2)) {
          a1 = a1->next;
          a2 = a2->next;
        }
        else break;
      }
      if (a1 == NULL && a2 == NULL){
        if (same_binstr(d1->br, d2->br)){
          printf("dupl second: [%d -> %d] + [%d -> %d] \n", d1->src, d1->dst, d2->src, d2->dst);
          d2->valid = 0;
          d2->valid = 0;
          s->br->cond = 0;
          s->br->succ2 = 0;
          found = true;
        }
      }
    }
    r = r->next;
  }
  return found;
}

bool opt_cp (struct cfg* t,int register_values[]){
  bool found = false;

  struct cfg* e = t;
  while (e != NULL){  // loop for all basic blocks
    if (e->valid){    // skip the ones that are not valid
      // struct asgn_instr* a = get_last_asgn (e->asgns);    // start scanning instructions backwards
      struct asgn_instr* a = e->asgns;    // start scanning instructions forward
      fprintf(stderr,"func: %s\n", e->fun);
      while (a != NULL){
        if (a->bin == 0) {
          // this is unary instruction
          // TODO: add some code here
          if (a->type == CONST){
            if (a->lhs==0){
              fprintf(stderr, "  rv := %d\n", a->op1); 
            }
            else if(a->lhs<0){
              fprintf(stderr, "  a%d := %d\n",-a->lhs, a->op1); 
            }
            else{
              fprintf(stderr, "  v%d := %d\n", a->lhs, a->op1);       // assigning a numeric constant to virtual register
              register_values[a->lhs] = a->op1;
            }
          }
          else if (a->type == NOT){
            fprintf(stderr, "  v%d := not v%d\n", a->lhs, a->op1);  // applying boolean negation
            if(a->op1)                                              
              a->op1 = false;
            else
              a->op1 = true;
            a->type = CONST;
          }
          else if (a->type == INP){
            fprintf(stderr, "  v%d := a%d\n", a->lhs, -a->op1);     // assigning a value from input register to a virtual register            
            int x = register_values[-a->op1];
            if (x != NOVALUE)
            {
              a->op1 = x;
              a->type = CONST;
              register_values[a->lhs] = x;
            }
          }
          else if (a->lhs == 0){
            fprintf(stderr, "  rv := v%d\n", a->op1);               // assigning a value from virtual register to output register
            int x = register_values[a->op1];
            if (x != NOVALUE){
              a->op1 = x;
              a->type = CONST;
            }
          }
          else if (a->lhs < 0){
            fprintf(stderr, "  a%d := v%d\n", -a->lhs, a->op1);     // assigning a value from virtual register to input register
            int x = register_values[a->op1];
            if (x != NOVALUE)
            {
              a->op1 = x;
              a->type = CONST;
              register_values[-a->lhs] = x;
            }
          }
          else{
            fprintf(stderr, "  v%d := v%d\n", a->lhs, a->op1);      // assigning a value from virtual register to another virtual register
            int x = register_values[a->op1];
            if (x != NOVALUE)
            {
              a->op1 = x;
              a->type = CONST;
              register_values[a->lhs] = x;
            }
          }
        }
        else if (a->bin == 1) {
          // this is binary instruction
          // TODO: add some code here
          if (a->type == EQ){
            fprintf(stderr, "  v%d := v%d = v%d\n", a->lhs, a->op1, a->op2);
            int x = register_values[a->op1];
            int y = register_values[a->op2];
            if (x!=NOVALUE && y!=NOVALUE){
              if (x == y)
                a->op1 = true;
              else
                a->op1=false;
              register_values[a->lhs] = a->op1;
              a->type = CONST;
              a->bin = 0;
            }
          }
          else if (a->type == LT){
            fprintf(stderr, "  v%d := v%d < v%d\n", a->lhs, a->op1, a->op2);
            int x = register_values[a->op1];
            int y = register_values[a->op2];
            if (x!=NOVALUE && y!=NOVALUE){
              if (x <= y)
                a->op1 = true;
              else
                a->op1=false;
              register_values[a->lhs] = a->op1;
              a->type = CONST;
              a->bin = 0;
            }
          }
          else if (a->type == PLUS){
            fprintf(stderr, "  v%d := v%d + v%d\n", a->lhs, a->op1, a->op2);
            int x = register_values[a->op1];
            int y = register_values[a->op2];
            if (x!=NOVALUE && y!=NOVALUE){
              
              a->op1 = x + y;
              register_values[a->lhs] = a->op1;
              a->bin = 0;
              a->type = CONST;
            }
          }
          else if (a->type == MINUS){
            fprintf(stderr, "  v%d := v%d - v%d\n", a->lhs, a->op1, a->op2);
            int x = register_values[a->op1];
            int y = register_values[a->op2];
            if (x!=NOVALUE && y!=NOVALUE){
              a->op1 = x-y;
              register_values[a->lhs] = a->op1;
              a->type = CONST;
              a->bin = 0;
            }
          }
          else if (a->type == AND){
            fprintf(stderr, "  v%d := v%d and v%d\n", a->lhs, a->op1, a->op2);
            int x = register_values[a->op1];
            int y = register_values[a->op2];
            if (x!=NOVALUE && y!=NOVALUE){
              if (x && y)
                a->op1 = true;
              else
                a->op1=false;
              register_values[a->lhs] = a->op1;
              a->type = CONST;
              a->bin = 0;
            }
          }
          else if (a->type == LE){
            fprintf(stderr, "  v%d := v%d <= v%d\n", a->lhs, a->op1, a->op2);
            int x = register_values[a->op1];
            int y = register_values[a->op2];
            if (x!=NOVALUE && y!=NOVALUE){
              if (x <= y)
                a->op1 = true;
              else
                a->op1=false;
              register_values[a->lhs] = a->op1;
              a->type = CONST;
              a->bin = 0;
            }
          }
          else if (a->type == MULT){
            fprintf(stderr, "  v%d := v%d * v%d\n", a->lhs, a->op1, a->op2);
            int x = register_values[a->op1];
            int y = register_values[a->op2];
            if (x!=NOVALUE && y!=NOVALUE){
              a->op1 = x*y;
              register_values[a->lhs] = a->op1;
              a->type = CONST;
              a->bin = 0;
              a->bin = 0;
            }
          }
          else if (a->type == DIV){
            fprintf(stderr, "  v%d := v%d div v%d\n", a->lhs, a->op1, a->op2);
            int x = register_values[a->op1];
            int y = register_values[a->op2];
            if (x!=NOVALUE && y!=NOVALUE){
              a->op1 = x / y;
              register_values[a->lhs] = a->op1;
              a->type = CONST;
              a->bin = 0;
            }
          }
          else if (a->type == MOD){
            fprintf(stderr, "  v%d := v%d mod v%d\n", a->lhs, a->op1, a->op2);
            int x = register_values[a->op1];
            int y = register_values[a->op2];
            if (x!=NOVALUE && y!=NOVALUE){
              a->op1 = x % y;
              register_values[a->lhs] = a->op1;
              a->type = CONST;
              a->bin = 0;
            }
            
          }
          else if (a->type == GT){
            fprintf(stderr, "  v%d := v%d > v%d\n", a->lhs, a->op1, a->op2);
            int x = register_values[a->op1];
            int y = register_values[a->op2];
            if (x!=NOVALUE && y!=NOVALUE){
              if (x > y)
                a->op1 = true;
              else
                a->op1=false;
              register_values[a->lhs] = a->op1;
              a->type = CONST;
              a->bin = 0;
            }
          }
          else if (a->type == GE){
            fprintf(stderr, "  v%d := v%d >= v%d\n", a->lhs, a->op1, a->op2);
            int x = register_values[a->op1];
            int y = register_values[a->op2];
            if (x!=NOVALUE && y!=NOVALUE){
              if (x >= y)
                a->op1 = true;
              else
                a->op1=false;
              register_values[a->lhs] = a->op1;
              a->type = CONST;
              a->bin = 0;
            }
          }
          else if (a->type == OR){
            fprintf(stderr, "  v%d := v%d or v%d\n", a->lhs, a->op1, a->op2);
            int x = register_values[a->op1];
            int y = register_values[a->op2];
            if (x!=NOVALUE && y!=NOVALUE){
              if (x || y)
                a->op1 = true;
              else
                a->op1=false;
              register_values[a->lhs] = a->op1;
              a->type = CONST;
              a->bin = 0;
            }
          }
        }
        else {
          // this is a call instruction. do nothing here
          printf("func: %s, v%d := v%d v%d\n",e->fun, a->lhs, a->op1 ,a->op2);

        }
        a = a->next;  // go to the next instruction
      }
    }
    e = e->next;    // go to the next basic block
  }
  printf("-----------------------------------------------------\n");
  return found;     // return this boolean value. if anything was optimized in the code above, then `found` should be `true`
}

bool opt_arithm (struct cfg* t, int register_values[]){
  bool found = false;
  struct cfg* e = t;
  struct asgn_instr* char_v[50]={NULL};
  // for (int i = 0; i < 50; i++)
  // {
  //   struct asgn_instr* a = NULL;
  //   char_v[i] = a;
  // }
   
  while (e != NULL){  // loop for all basic blocks
    if (e->valid){    // skip the ones that are not valid
      struct asgn_instr* a = e->asgns;    // start scanning instructions forward
      
      while (a != NULL){
        if (a->bin == 0) {
          // this is unary instruction
          // TODO: add some code here
          if (a->type == CONST){
            if (a->lhs==0){
              fprintf(stderr, "  rv := %d\n", a->op1); 
            }
            else if(a->lhs<0){
              fprintf(stderr, "  a%d := %d\n",-a->lhs, a->op1); 
            }
            else{
              fprintf(stderr, "  v%d := %d\n", a->lhs, a->op1);       // assigning a numeric constant to virtual register
              char_v[a->lhs] = a;
            }
          }
          else if (a->type == NOT){
            fprintf(stderr, "  v%d := not v%d\n", a->lhs, a->op1);  // applying boolean negation
            char_v[a->lhs] = a;
          }
          else if (a->type == INP){
            fprintf(stderr, "  v%d := a%d\n", a->lhs, -a->op1);     // assigning a value from input register to a virtual register
          }
          else if (a->lhs == 0){
            fprintf(stderr, "  rv := v%d\n", a->op1);               // assigning a value from virtual register to output register
          }
          else if (a->lhs < 0){
            fprintf(stderr, "  a%d := v%d\n", -a->lhs, a->op1);     // assigning a value from virtual register to input register
          }
          else{
            fprintf(stderr, "  v%d := v%d\n", a->lhs, a->op1);      // assigning a value from virtual register to another virtual register
            char_v[a->lhs] = a;
          }  
        }
        else if (a->bin == 1) {
          // this is binary instruction
          // TODO: add some code here
          if (a->type == PLUS){
            fprintf(stderr, "  v%d := v%d + v%d\n", a->lhs, a->op1, a->op2);
            int x = register_values[a->op1];
            int y = register_values[a->op2];
            if (x == 0){
              a->op1 = a->op2;
              a->bin = 0;
              a->type = 0;
            }
            else if(y == 0){
              a->bin = 0;
              a->type = 0;
            }
          }
          else if (a->type == MINUS){
            fprintf(stderr, "  v%d := v%d - v%d\n", a->lhs, a->op1, a->op2);
            int x = register_values[a->op1];
            int y = register_values[a->op2];
            if(y==0){
              a->bin = 0;
              a->type = 0;
            }
            else if(char_v[a->op1]->op1 == char_v[a->op2]->op1){
              a->op1 = 0;
              register_values[a->lhs] = 0;
              a->bin = 0;
              a->type = CONST;
            }
          }
          else if (a->type == MULT){
            fprintf(stderr, "  v%d := v%d * v%d\n", a->lhs, a->op1, a->op2);
            int x = register_values[a->op1];
            int y = register_values[a->op2];
            if(x==1){
              a->op1 = a->op2;
              a->bin = 0;
              a->type = 0;
            }else if(y==1){
              a->bin = 0;
              a->type = 0;
            }
            else if(x==0|| y==0){
              register_values[a->lhs] = 0;
              a->op1 = 0;
              a->bin = 0;
              a->type = CONST;
            }
          }
          else if (a->type == DIV){
            fprintf(stderr, "  v%d := v%d div v%d\n", a->lhs, a->op1, a->op2);
            int x = register_values[a->op1];
            int y = register_values[a->op2];
            if(y==1){
              a->bin = 0;
              a->type = 0;
            }
            else if(x==0){
              register_values[a->lhs] = 0;
              a->op1 = 0;
              a->bin = 0;
              a->type = CONST;
            }
          }
          else if (a->type == AND){
            fprintf(stderr, "  v%d := v%d and v%d\n", a->lhs, a->op1, a->op2);
            int x = register_values[a->op1];
            int y = register_values[a->op2];
            if(y==0 || x==0){
              register_values[a->lhs] = false;
              a->op1 = false;
              a->bin = 0;
              a->type = CONST;
            }
            else if(x==1){
              a->op1 = a->op2;
              a->bin = 0;
              a->type = 0;
            }
            else if(y==1){
              a->bin = 0;
              a->type = 0;
            }else if(char_v[a->op1]->op1 == char_v[a->op2]->op1 && char_v[a->op1]->bin ==0 && char_v[a->op2]->bin == 0){
              a->op1 = char_v[a->op1]->op1;
              a->bin = 0;
              a->type = 0;
            }else if (char_v[a->op1]->type==NOT && char_v[a->op1]->op1==a->op2){
              a->op1 = 0;
              a->type = CONST;
            }else if (char_v[a->op2]->type==NOT && char_v[a->op2]->op1==a->op1){
              a->op1 = 0;
              a->type = CONST;
            }
            
            
          }
          else if (a->type == OR){
            fprintf(stderr, "  v%d := v%d or v%d\n", a->lhs, a->op1, a->op2);
            int x = register_values[a->op1];
            int y = register_values[a->op2];
            if(y==1 || x==1){
              register_values[a->lhs] = true;
              a->op1 = true;
              a->bin = 0;
              a->type = CONST;
            }
            else if(x==0){
              a->op1 = a->op2;
              a->bin = 0;
              a->type = 0;
            }
            else if(y==0){
              a->bin = 0;
              a->type = 0;
            }
            else if(a->op1 == a->op2){
              a->bin = 0;
              a->type = 0;
            }else if(char_v[a->op1]->op1 == char_v[a->op2]->op1 && char_v[a->op1]->bin ==0 && char_v[a->op2]->bin == 0){
              a->op1 = char_v[a->op1]->op1;
              a->bin = 0;
              a->type = 0;
            }
            
          }
        }
        else {
          // this is a call instruction. do nothing here
        }
        a = a->next;  // go to the next instruction
      }
    }
    e = e->next;    // go to the next basic block
  }
  return found;     // return this boolean value. if anything was optimized in the code above, then `found` should be `true`
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
        if (r->valid && (r->id == i || (c1 == NULL && c2 == NULL))
            && strcmp (fun_r->name, r->fun) == 0) {
          if (1 == find_int(r->id, printed_r)) {
            // don't print the same basic block twice
            push_int(r->id, &printed_r, &printed_t);
            fprintf(stderr, "bb%d:\n", r->id);

            struct asgn_instr* a = r->asgns;
            while (a != NULL) {
              if (a->bin == 0) {
                if (a->type == CONST){
                  if (a->lhs==0){
                    fprintf(stderr, "  rv := %d\n", a->op1); 
                  }
                  else if(a->lhs<0){
                    fprintf(stderr, "  a%d := %d\n",-a->lhs, a->op1); 
                  }
                  else
                    fprintf(stderr, "  v%d := %d\n", a->lhs, a->op1);       // assigning a numeric constant to virtual register
                }
                else if (a->type == NOT)
                  fprintf(stderr, "  v%d := not v%d\n", a->lhs, a->op1);  // applying boolean negation
                else if (a->type == INP)
                  fprintf(stderr, "  v%d := a%d\n", a->lhs, -a->op1);     // assigning a value from input register to a virtual register
                else if (a->lhs == 0)
                  fprintf(stderr, "  rv := v%d\n", a->op1);               // assigning a value from virtual register to output register
                else if (a->lhs < 0)
                  fprintf(stderr, "  a%d := v%d\n", -a->lhs, a->op1);     // assigning a value from virtual register to input register
                else
                  fprintf(stderr, "  v%d := v%d\n", a->lhs, a->op1);      // assigning a value from virtual register to another virtual register
              }
              else if (a->bin == 1) {                                     // binary instructions, all have the same structure
                if (a->type == EQ)
                  fprintf(stderr, "  v%d := v%d = v%d\n", a->lhs, a->op1, a->op2);
                else if (a->type == LT)
                  fprintf(stderr, "  v%d := v%d < v%d\n", a->lhs, a->op1, a->op2);
                else if (a->type == PLUS)
                  fprintf(stderr, "  v%d := v%d + v%d\n", a->lhs, a->op1, a->op2);
                else if (a->type == MINUS)
                  fprintf(stderr, "  v%d := v%d - v%d\n", a->lhs, a->op1, a->op2);
                else if (a->type == AND)
                  fprintf(stderr, "  v%d := v%d and v%d\n", a->lhs, a->op1, a->op2);
                else if (a->type == LE)
                  fprintf(stderr, "  v%d := v%d <= v%d\n", a->lhs, a->op1, a->op2);
                else if (a->type == MULT)
                  fprintf(stderr, "  v%d := v%d * v%d\n", a->lhs, a->op1, a->op2);
                else if (a->type == DIV)
                  fprintf(stderr, "  v%d := v%d div v%d\n", a->lhs, a->op1, a->op2);
                else if (a->type == MOD)
                  fprintf(stderr, "  v%d := v%d mod v%d\n", a->lhs, a->op1, a->op2);
                else if (a->type == GT)
                  fprintf(stderr, "  v%d := v%d > v%d\n", a->lhs, a->op1, a->op2);
                else if (a->type == GE)
                  fprintf(stderr, "  v%d := v%d >= v%d\n", a->lhs, a->op1, a->op2);
                else if (a->type == OR)
                  fprintf(stderr, "  v%d := v%d or v%d\n", a->lhs, a->op1, a->op2);
              }
              else if (a->bin == 2) {
              if (a->lhs == 0)
                fprintf(stderr, "  call %s \n", a->fun);
              else if (strcmp(a->fun, "print") != 0)
                fprintf(stderr, "  v%d := rv\n", a->lhs);
              }
              a = a->next;
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