#include "cfg.h"
#include "reg.h"
#include "y.tab.h"
struct range* reg[50] = {NULL};

void print_all_bb_id(struct range* r, int vid){
  struct range* e = r;
  printf("reg[%d]->bb: ",vid);
  while (e!=NULL)
  {
    printf("%d ",e->bb);
    e = e->next;
  }
  printf("\n\n");
}

void append(struct range* r, int id){
  struct range* t = r;
  while (t->next!=NULL)
  {
    t = t->next;
  }
  struct range* r1 = (struct range*)malloc(sizeof(struct range));
  r1->bb = id;
  r1->next = NULL;
  t->next = r1;
}

bool contains(struct range* r, int bbid){
  struct range* t = r;
  while (t!=NULL)
  {
    // printf("range check: %d compare %d\n",t->bb,bbid);
    if (t->bb == bbid)
    {
      return true;
    }
    t = t->next;
  }
  return false;
}

void add_reg(struct cfg* t, int bb, int id, struct range* rr){
  struct cfg* e = t;
  struct range* r = rr;
  // printf("here\n");
  while(e != NULL){ 
    if(e->valid){
      // printf("here %d\n",e->id);
      if(e->id == bb){
        if (e->br==NULL || id == e->br->cond){
          // printf("here1\n");
          break;
        }
        if(e->asgns!=NULL){
          if (e->asgns->bin==0 && e->asgns->op1==id)
          {
            break;
          }
          if (e->asgns->bin==1 && (e->asgns->op1==id||e->asgns->op2==id))
          {
            break;
          }
        }
        if (e->br!=NULL)
        {
          // printf("bb%d %d %d\n",e->id,e->br->succ1,e->br->succ2);
          if(contains(rr,e->br->succ1)==false){
            append(rr,e->br->succ1);
            add_reg(t,e->br->succ1,id,rr);
          }
          if(e->br->succ2>0 && contains(rr,e->br->succ2)==false){
            append(rr,e->br->succ2);
            add_reg(t,e->br->succ2,id,rr);
          }
          // printf("value of succ2: %d\n",e->br->succ2);
          // if (e->br->succ1>0)
          //   add_reg(t,e->br->succ1,id,rr);
          // if (e->br->succ2>0)
          //   add_reg(t,e->br->succ2,id,rr);        
          
        }
      }
    }
    e = e->next;
  }
}

bool register_allocation (struct cfg* t){
  bool found = false;
  struct cfg* e = t;
 
  while (e != NULL){  // loop for all basic blocks
    if (e->valid){    // skip the ones that are not valid
      // printf("Func name: %s\n",e->fun);
      struct asgn_instr* a = e->asgns;    // start scanning instructions forward      
      while (a != NULL){        
        if (a->lhs > 0)
        {
          struct range* r = (struct range*)malloc(sizeof(struct range));
          if (e->br->cond == a->lhs)
          {
            r->bb = e->id;
            r->next = NULL;
            reg[a->lhs] = r;
          }
          else{
            r->bb = e->br->succ1;
            r->next = NULL;
            reg[a->lhs] = r;
            add_reg(t,e->br->succ1,a->lhs,reg[a->lhs]);
          }
        }
        
        a = a->next;  // go to the next instruction
      }
    }
    e = e->next;    // go to the next basic block
  }
  return found;     // return this boolean value. if anything was optimized in the code above, then `found` should be `true`
}

void print_reg_smt(){
  FILE *fp;
  fp = fopen("reg.smt", "w");
  fprintf(fp, "(declare-fun K () Int)\n");
  for (int i = 0; i < 50; i++)
  {
    if (reg[i]!=NULL)
    {
      fprintf(fp, "(declare-fun x%d () Int)\n",i);
      fprintf(fp, "(assert (> x%d 0))\n",i);
      fprintf(fp, "(assert (<= x%d K))\n",i);
    }
  }
  printf("=======================================================\n");
  for (int i = 0; i < 50; i++)
  {
    if (reg[i]!=NULL) print_all_bb_id(reg[i],i);
    for (int j = i; j < 50; j++)
    {
      if (reg[i]!=NULL && reg[j]!=NULL)
      {
        if (i!=j)
        {
          struct range* r = reg[i];
          while (r!=NULL)
          {
            if (contains(reg[j],r->bb))
            {
              fprintf(fp, "(assert (not (= x%d x%d)))\n",i,j);
              break;
            }
            r = r->next;
          }
        }
      }
    }
  }
  printf("=======================================================\n");
  fprintf(fp,"\n(minimize K)\n(check-sat)\n(get-objectives)\n(get-model)\n");
  fclose(fp);
  system("z3 reg.smt | grep \"define-fun x\" -A 1 | grep -v \"\\-\\-\" > reg.txt");

}

int* traverse_reg_txt(){
  int* reg=(int*) calloc(50, sizeof(char*)); 
  for (int i = 0; i < 50; i++)
  {
    reg[i] = -9999;
  }
  
  FILE* ptr;
    char ch;
    ptr = fopen("reg.txt", "r");
 
    if (NULL == ptr) {
        printf("file can't be opened \n");
    }
 
    printf("content of this file are \n");
    int v=0, g=0;
    while (fscanf(ptr," (define-fun x%d () Int\n %d)\n", &v, &g) == 2) reg[v] = g;
    for (int i = 0; i < 50; i++)
    {
      if(reg[i]!=-9999){
        printf("reg[%d] = %d\n",i,reg[i]);
      }
    }
    
    fclose(ptr);
    return reg;
}