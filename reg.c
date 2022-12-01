#include "cfg.h"
#include "reg.h"
#include "y.tab.h"
struct range* reg[50] = {NULL};

bool register_allocation (struct cfg* t){
  bool found = false;
  struct cfg* e = t;
 
  while (e != NULL){  // loop for all basic blocks
    if (e->valid){    // skip the ones that are not valid
      struct asgn_instr* a = e->asgns;    // start scanning instructions forward
      while (a != NULL){
        if (a->bin == 0) {
          // this is unary instruction
          // TODO: add some code here
          if (a->lhs>0){
                if (reg[a->lhs]==NULL && a->lhs>0){
                    struct range* r = (struct range*)malloc(sizeof(struct range));
                    r->start = e->id;
                    r->end = e->id;
                    reg[a->lhs] = r;
                }
                else if(reg[a->lhs]!=NULL){
                    reg[a->lhs]->end = e->id;
                }
                if(a->type!=INP && a->type!=CONST && reg[a->op1]!=NULL){
                    reg[a->op1]->end = e->id;
                }
            }
        }
        else if (a->bin == 1) {
          // this is binary instruction
          // TODO: add some code here
            if (reg[a->lhs]==NULL && a->lhs>0){
                struct range* r = (struct range*)malloc(sizeof(struct range));
                printf("printing e->id: %d\n",e->id);
                r->end = 12;
                r->end = 23;
                reg[a->lhs] = r;
            }
            else if(reg[a->lhs]!=NULL){
                reg[a->lhs]->end = e->id;
            }
            if (reg[a->op1]!=NULL && reg[a->op2]!=NULL)
            {
              reg[a->op1]->end = e->id;
              reg[a->op2]->end = e->id;
            }

        }
          // this is a call instruction. do nothing here
        else if (a->bin == 2) {
          if (a->lhs != 0 && strcmp(a->fun, "print") != 0){
            printf("lhs: %d %d\n",a->lhs,e->id);
            if (reg[a->lhs]==NULL && a->lhs>0){
              struct range* r = (struct range*)malloc(sizeof(struct range));
              r->start = e->id;
              r->end = e->id;
              reg[a->lhs] = r;
            }
            else{
              reg[a->lhs]->start = e->id;
              reg[a->lhs]->end = e->id;
            }
            
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
  FILE* fp;
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
  for (int i = 0; i < 50; i++)
  {
    for (int j = i; j < 50; j++)
    {
      if (reg[i]!=NULL && reg[j]!=NULL)
      {
        if (i!=j && reg[i]->start>=reg[j]->start && reg[i]->end<=reg[j]->end)
        {
          fprintf(fp, "(assert (not (= x%d x%d)))\n",i,j);
        }
      }
    }
  }
  fprintf(fp,"\n(minimize K)\n(check-sat)\n(get-objectives)\n(get-model)\n");
  fclose(fp);
  // system("dot -Tpdf cfg.dot -o cfg.pdf");

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