#include "y.tab.h"
#include "containers.h"
#include "symbol.h"
#include "bblock.h"
#include "comp.h"
#include "ssa.h"
#include "calc.h"
#include "optim.h"
struct listToRemove{
  struct statment_s* stat;
  struct listToRemove* next;

};
struct listToRemove* firstRemove;
struct listToRemove* lastRemove;

void insertToRemove( struct statment_s* statment){

  if (firstRemove==NULL){
    firstRemove = lastRemove = calloc(1,sizeof(struct listToRemove));
  }else{
    struct listToRemove* tmp = calloc(1,sizeof(struct listToRemove));
    lastRemove->next = tmp;
    lastRemove = tmp;
  }
  lastRemove->stat = statment;
}
struct FunctionBlock* removeAll(struct FunctionBlock* func){
    int changed = 0;
   if (firstFunction==NULL||firstRemove==NULL) return NULL;
  struct FunctionBlock* functionBlock = firstFunction;
  while(functionBlock!=NULL){
    while(functionBlock!=func){
  functionBlock = functionBlock->next;
    continue;
    }
  struct basic_block* block = functionBlock->firstBlock;
  while(block!=NULL){
    struct statment_s* statment = block->firstStmt;
    while(statment!=NULL){
      if (firstRemove==NULL) break;
    // printf("14 v%d.%d\n",firstRemove->stat->LHS->RGnum,firstRemove->stat->LHS->SSAnum);
      if (statment->LHS->RGnum==firstRemove->stat->LHS->RGnum && statment->LHS->type ==  firstRemove->stat->LHS->type&& statment->LHS->SSAnum ==  firstRemove->stat->LHS->SSAnum)
      {
    // printf("13\n");
        // printf("v%d.%d\n",statment->LHS->RGnum,statment->LHS->SSAnum );
        struct listToRemove* old = firstRemove;
    // printf("12\n");
      firstRemove = firstRemove->next;
      free(old);
      changed=1;
      struct statment_s* tmp =statment;
      statment = statment->next;
    // printf("11\n");
       free_statment(tmp);
    // printf("10\n");
      }
      else{
    // printf("9\n");
      statment = statment->next;
    }
    }
     block = block->next;
  }
    if (changed==1)
    {
      return functionBlock;
    }
  functionBlock = functionBlock->next;
  } return NULL;
}
int replaceRegister(struct FunctionBlock* func,struct statment_s* stat, struct usedChain* statmentChain){
  int changes=0;
  int reached=0;
  struct usedChain* chain= statmentChain;
   if (firstFunction==NULL) return 0;
  struct FunctionBlock* functionBlock = firstFunction;
  while(functionBlock!=NULL){
    if(functionBlock!=func){
  functionBlock = functionBlock->next;
    continue;
    }
  struct basic_block* block = functionBlock->firstBlock;
  while(block!=NULL){
    struct statment_s* statment = block->firstStmt;
    while(statment!=NULL&& chain!=NULL){
      if (statment==chain->statment){
        if (statment->RHS!=NULL&&statment->RHS->RGnum==stat->LHS->RGnum && statment->RHS->type ==  stat->LHS->type&& statment->RHS->SSAnum ==  stat->LHS->SSAnum){
          free(statment->RHS);
          statment->RHS = calloc(1,sizeof(struct statment_value));
           statment->RHS->type = stat->RHS->type;
           statment->RHS->RGnum = stat->RHS->RGnum;
           statment->RHS->title = stat->RHS->title;
           statment->RHS->isNot = stat->RHS->isNot;
           statment->RHS->token = stat->RHS->token;
           statment->RHS->SSAnum = stat->RHS->SSAnum;
           statment->RHS->returnType = stat->RHS->returnType;
           if(stat->RHS->op_values_next!=NULL){
              struct statment_value* op_values =  stat->RHS->op_values_next;
              struct statment_value* first;
              struct statment_value* last;
              first = last = calloc(1,sizeof(struct statment_value));
              first->type = op_values->type;
              first->RGnum = op_values->RGnum;
              first->SSAnum = op_values->SSAnum;
              op_values = op_values->op_values_next;
              while(op_values!=NULL){
                last->op_values_next  = calloc(1,sizeof(struct statment_value));
                last = last->op_values_next;
                last->type = op_values->type;
                last->RGnum = op_values->RGnum;
                last->SSAnum = op_values->SSAnum;
                op_values = op_values->op_values_next;
              }
              statment->RHS->op_values_next = first;
            }
          changes++;
        statment = statment->next;
        }else if(statment->RHS!=NULL&&statment->RHS->op_values_next!=NULL && statment->RHS->op_values_next->op_values_next!= NULL
         && statment->RHS->op_values_next->op_values_next->RGnum==stat->LHS->RGnum
         && statment->RHS->op_values_next->op_values_next->type==stat->LHS->type
         && statment->RHS->op_values_next->op_values_next->SSAnum==stat->LHS->SSAnum){  
      // printf("8 v%d op = %s\n",statment->LHS->RGnum,statment->RHS->title);
          free(statment->RHS->op_values_next->op_values_next);
          statment->RHS->op_values_next->op_values_next = calloc(1,sizeof(struct statment_value));
          statment->RHS->op_values_next->op_values_next->type = stat->RHS->type;
          statment->RHS->op_values_next->op_values_next->title = stat->RHS->title;
          statment->RHS->op_values_next->op_values_next->RGnum = stat->RHS->RGnum;
          statment->RHS->op_values_next->op_values_next->SSAnum = stat->RHS->SSAnum;
          changes++;
        statment = statment->next;
        }else if(statment->RHS!=NULL&&statment->RHS->op_values_next!=NULL && statment->RHS->op_values_next->RGnum==stat->LHS->RGnum 
          && statment->RHS->op_values_next->type==stat->LHS->type
         && statment->RHS->op_values_next->SSAnum==stat->LHS->SSAnum ){
      // printf("9 v%d\n",statment->RHS->op_values_next->RGnum);
          free(statment->RHS->op_values_next);
          struct statment_value* lastOperand = statment->RHS->op_values_next->op_values_next;
          statment->RHS->op_values_next = calloc(1,sizeof(struct statment_value));
          statment->RHS->op_values_next->type = stat->RHS->type;
          statment->RHS->op_values_next->title = stat->RHS->title;
          statment->RHS->op_values_next->RGnum = stat->RHS->RGnum;
          statment->RHS->op_values_next->SSAnum = stat->RHS->SSAnum;
          statment->RHS->op_values_next->op_values_next = lastOperand;
          changes++;
        statment = statment->next;
        }else{
         statment = statment->next;
       }
       chain = chain->next;
      }
      else{
      statment = statment->next;
    }
    }
    //  printf("start\n");
    // if (block->cond!=NULL)
    // { 
    // printf("out v%d.%d type = %d   |   V%d.%d type = %d\n",block->cond->RGnum,block->cond->SSAnum, block->cond->type,stat->LHS->RGnum,stat->LHS->SSAnum, stat->LHS->type );
    
    // }
     if(block->cond!=NULL&&block->cond->RGnum==stat->LHS->RGnum && block->cond->type ==  stat->LHS->type && block->cond->SSAnum ==  stat->LHS->SSAnum){
      // printf("in\n");
          free(block->cond);
          block->cond = calloc(1,sizeof(struct statment_value));
           block->cond->type = stat->RHS->type;
           block->cond->RGnum = stat->RHS->RGnum;
           block->cond->title = stat->RHS->title;
           block->cond->isNot = stat->RHS->isNot;
           block->cond->token = stat->RHS->token;
           block->cond->SSAnum = stat->RHS->SSAnum;
           block->cond->returnType = stat->RHS->returnType;
          changes++;
    }

     block = block->next;
  }
  functionBlock = functionBlock->next;
  }
  return changes;
}
 void combineRedundant (){
  if (firstFunction==NULL) return;
  struct FunctionBlock* functionBlock = firstFunction;
  while(functionBlock!=NULL){
  int changes=0;
  do{
    changes=0;
  struct ssa* item = generateSSA(functionBlock)->firstSSA;
  while(item!=NULL){
    struct ssaItem* subItem = item->firstItem;
    while(subItem!=NULL &&subItem->statment!=NULL&& subItem->statment->RHS!=NULL
     && subItem->statment->RHS->type!=ST_RETURN && subItem->statment->RHS->type!=ST_OP){
      int count=0;
      if (subItem->firstUsed!=NULL){
        count = replaceRegister(functionBlock,subItem->statment,subItem->firstUsed);
      }else if(subItem->statment->block->cond!=NULL && subItem->statment->block->lastStmt ==  subItem->statment ){
        count = replaceRegister(functionBlock,subItem->statment,subItem->firstUsed);
      }
      if (count>0){
        insertToRemove(subItem->statment);
        changes+=count;
      }
      subItem = subItem->next;
    }
    item = item->next;
  }
     removeAll(functionBlock);
  }while(changes>0);
      functionBlock = functionBlock->next;
  }

 }

struct statment_value* getContastant(struct ssaList* registersSSA, struct statment_value* value){

  if (value==NULL) return NULL;
  if (value->type==ST_CONST){ 
      return value;
  }else if (value->type==ST_REG){
    struct ssaItem* item = lookupReg(registersSSA,value);
    if (item!=NULL&& item->statment!=NULL && item->statment->RHS!=NULL&&item->statment->RHS->type==ST_CONST){
        return item->statment->RHS;
    }
    return NULL;
  }
  return NULL;
}
void tostring(char str[], int num)
{
    int i, rem, len = 0, n;
    n = num;
    while (n != 0)
    {
        len++;
        n /= 10;
    }
    for (i = 0; i < len; i++)
    {
        rem = num % 10;
        num = num / 10;
        str[len - (i + 1)] = rem + '0';
    }
    str[len] = '\0';
}

 void constantPropagation(){
  if (firstFunction==NULL) return;
  struct FunctionBlock* functionBlock = firstFunction;
  while(functionBlock!=NULL){
  int changes=0;
  do{
    changes=0;
    struct ssa* item = generateSSA(functionBlock)->firstSSA; 
    struct basic_block* block =  getFunctionByTitle(functionBlock->title)->firstBlock;
    while(block!=NULL){
      struct statment_s* statment = block->firstStmt;
      while(statment!=NULL){
        if( statment->RHS!=NULL && statment->RHS->op_values_next!=NULL){
        if (is_fla(statment->RHS->token)){

          struct statment_value* firstValue;
          struct statment_value* secondValue;
          if (statment->RHS->isNot){
            firstValue = getContastant(functionBlock->registersSSA, statment->RHS->op_values_next);

            if (firstValue==NULL){
              statment = statment->next;
              continue;
            }
          }else{
            firstValue = getContastant(functionBlock->registersSSA, statment->RHS->op_values_next);
            secondValue = getContastant(functionBlock->registersSSA, statment->RHS->op_values_next->op_values_next);
          }
          if (firstValue==NULL||secondValue==NULL){
              statment = statment->next;
              continue;
            }
            int result = calculateFla(firstValue, secondValue, statment->RHS->token);
            free_statment_value(statment->RHS);
            statment->RHS = calloc(1,sizeof(struct statment_value));
            statment->RHS->type = ST_CONST;
            if (result){
            statment->RHS->title = "TRUE";
            statment->RHS->token = TRUE;
            }else if (result==0){
            statment->RHS->title = "FALSE";
            statment->RHS->token = FALSE;
            }
            changes++;
        }else if (is_term(statment->RHS->token)){
          // printf("%s\n", );
          struct statment_value* firstValue= getContastant(functionBlock->registersSSA, statment->RHS->op_values_next);
          struct statment_value* secondValue = getContastant(functionBlock->registersSSA, statment->RHS->op_values_next->op_values_next);
          if (firstValue==NULL||secondValue==NULL){
              statment = statment->next;
              continue;
            }
            long result = calculateNum(firstValue, secondValue, statment->RHS->token);
            int length = snprintf( NULL, 0, "%ld", result );
            char* str = malloc( length+1);
            snprintf( str, length + 1, "%ld", result );
            // printf("result = %s\n",str );
            free_statment_value(statment->RHS);
            statment->RHS = calloc(1,sizeof(struct statment_value));
            statment->RHS->type = ST_CONST;
            statment->RHS->title = strdup(str);
            statment->RHS->token = NUMBER;
            changes++;
            free(str);
        }
      }
      statment = statment->next;
      }
        block = block->next;
  }



 }while(changes>0);
      functionBlock = functionBlock->next;

  }
}

int replaceCommonSubExpression(struct FunctionBlock* func,struct statment_s* stat){
  int changes=0;
  int reached=0;
   if (firstFunction==NULL) return 0;
  struct FunctionBlock* functionBlock = firstFunction;
  while(functionBlock!=NULL){
    if(functionBlock!=func){
  functionBlock = functionBlock->next;
    continue;
    }
  struct basic_block* block = functionBlock->firstBlock;
  while(block!=NULL){
    struct statment_s* statment = block->firstStmt;
    while(statment!=NULL){
      if(reached==0){
        reached = statment==stat;
        statment = statment->next;
        continue;
      }
      if (statment->RHS!=NULL && statment->RHS->op_values_next!=NULL 
        && statment->RHS->token == stat->RHS->token
        && statment->RHS->op_values_next->RGnum==stat->RHS->op_values_next->RGnum 
          && statment->RHS->op_values_next->type==stat->RHS->op_values_next->type
         && statment->RHS->op_values_next->SSAnum==stat->RHS->op_values_next->SSAnum )
      {
          if (stat->RHS->op_values_next->op_values_next==NULL)
          {
            free_statment_value(statment->RHS);
            statment->RHS = cpyStatment(stat->LHS);
            changes++;
          }
          else if (statment->RHS->op_values_next->op_values_next!=NULL
           && statment->RHS->op_values_next->op_values_next->RGnum==stat->RHS->op_values_next->op_values_next->RGnum
         && statment->RHS->op_values_next->op_values_next->type==stat->RHS->op_values_next->op_values_next->type
         && statment->RHS->op_values_next->op_values_next->SSAnum==stat->RHS->op_values_next->op_values_next->SSAnum){
            free_statment_value(statment->RHS);
            statment->RHS = cpyStatment(stat->LHS);
            changes++;
          }
      }
      statment = statment->next;
    }
    block = block->next;
  }
  functionBlock = functionBlock->next;
}
  return changes;
}


 void commonSubexpression(){
  if (firstFunction==NULL) return;
  struct FunctionBlock* functionBlock = firstFunction;
  while(functionBlock!=NULL){
  int changes=0;
  do{
    changes=0;
    struct ssa* item = generateSSA(functionBlock)->firstSSA; 
    struct basic_block* block =  getFunctionByTitle(functionBlock->title)->firstBlock;
    while(block!=NULL){
      struct statment_s* statment = block->firstStmt;
      while(statment!=NULL){
        if( statment->RHS!=NULL && statment->RHS->op_values_next!=NULL){
            struct ssaItem* item = lookupReg(firstFunction->registersSSA,statment->LHS);
            if (item!=NULL && item->next==NULL){
              changes+=replaceCommonSubExpression(functionBlock, statment);
            }
          }
      statment = statment->next;
      }
    block = block->next;
  }
 }while(changes>0);
      functionBlock = functionBlock->next;
  }
}

int getNumberOfUsage(struct FunctionBlock* functionBlock, struct basic_block* checkBlock){

  int count=0;
 struct basic_block* block =  functionBlock->firstBlock;
    while(block!=NULL){
      if (block->firstBB!=NULL&&block->firstBB->bbNum==checkBlock->bbNum){
        count+=2;
      }
      if (block->secondBB!=NULL&&block->secondBB->bbNum==checkBlock->bbNum){
        count+=2;
      }
      if (block->closureBB!=NULL&&block->closureBB->bbNum==checkBlock->bbNum){
        count++;
      }
    block = block->next;
    }
    return count;

}
void simplifyCFG(){
if (firstFunction==NULL) return;
  struct FunctionBlock* functionBlock = firstFunction;
  while(functionBlock!=NULL){
  int changes=0;
  do{
    changes=0;
    struct ssa* item = generateSSA(functionBlock)->firstSSA; 
    struct basic_block* block =  getFunctionByTitle(functionBlock->title)->firstBlock;
    while(block!=NULL){
      if(block->cond!=NULL && block->cond->token==TRUE){
        free_block(functionBlock, block->secondBB);
        free(block->cond);
        block->cond = NULL;
        block->closureBB =  block->firstBB;
        block->firstBB = NULL;
        block->secondBB = NULL;
      }else if(block->cond!=NULL && block->cond->token==FALSE){
        free_block(functionBlock, block->firstBB);
        free(block->cond);
        block->cond = NULL;
        block->closureBB =  block->secondBB;
        block->firstBB = NULL;
        block->secondBB = NULL;
      }
      if (block->parent!=NULL && block->parent->closureBB==block && getNumberOfUsage(functionBlock, block)==1){
        struct statment_s* statment = block->firstStmt;
        while(statment!=NULL){
          insertStatement(block->parent, cpyS(block->parent,statment));
          struct statment_s* tmp =statment;
          statment = statment->next;
          free_statment(tmp);
        }
        block->parent->cond = block->cond;
        if (block->firstBB!=NULL){
        block->parent->firstBB =  block->firstBB;
        block->firstBB->parent = block->parent;
        }
        if (block->secondBB!=NULL){
        block->parent->secondBB =  block->secondBB;
        block->secondBB->parent = block->parent;
        }
        block->parent->closureBB = block->closureBB;
        if (block->closureBB!=NULL){
        block->closureBB->parent = block->parent;
        }
        block->parent->next = block->next;
        if (block->next!=NULL){
          block->next->prev = block->parent;
        }
        if(block->prev==NULL&& block->next==NULL){
      functionBlock->firstBlock = functionBlock->lastBlock  = NULL;
      }else if(block->prev==NULL){
        functionBlock->firstBlock = block->next;
        block->next->prev = NULL;
      }else if(block->next==NULL) {
        functionBlock->lastBlock =block->prev;
        block->prev->next = NULL;
      }
      else{
        block->next->prev = block->prev;
        block->prev->next = block->next;
      }
      free(block);
      }
    block = block->next;
  }
 }while(changes>0);
      functionBlock = functionBlock->next;
  }

} 

int reg_reassign(struct FunctionBlock* func, struct basic_block* b,struct statment_s* mainStatment,  struct statment_value regReplaced, struct statment_value* reg){
  int reached=0;
  if (b==NULL) return 0;
  struct basic_block* block = b;
  while(block!=NULL){
    struct statment_s* statment = block->firstStmt;
    while(statment!=NULL){
      if(reached==0){
        reached = statment==mainStatment;
         if(reached==0){
        statment = statment->next;
        continue;
        }
      }
      if (statment->LHS->type==regReplaced.type && statment->LHS->RGnum==regReplaced.RGnum){
          free(statment->LHS);
          statment->LHS = cpyStatment(reg);
      }
      if (statment->RHS!=NULL&&statment->RHS->type == ST_REG && statment->RHS->type==regReplaced.type && statment->RHS->RGnum==regReplaced.RGnum){
          free(statment->RHS);
          statment->RHS = cpyStatment(reg);
      }
      if (statment->RHS!=NULL&&statment->RHS->type == ST_OP && statment->RHS->op_values_next!=NULL 
          && statment->RHS->op_values_next->type==ST_REG
          && statment->RHS->op_values_next->RGnum==regReplaced.RGnum){
          statment->RHS->op_values_next->RGnum = reg->RGnum;
      }
      if (statment->RHS!=NULL&&statment->RHS->type == ST_OP && statment->RHS->op_values_next!=NULL 
          && statment->RHS->op_values_next->op_values_next!=NULL 
          && statment->RHS->op_values_next->op_values_next->type==ST_REG 
          && statment->RHS->op_values_next->op_values_next->RGnum==regReplaced.RGnum){

          statment->RHS->op_values_next->op_values_next->RGnum = reg->RGnum;
      }
      
        statment = statment->next;
    }
      if (block->cond!=NULL && block->cond->type==ST_REG && block->cond->RGnum==regReplaced.RGnum){
          free(block->cond);
          block->cond = cpyStatment(reg);
      }
     block = block->next;
  }
  return 1;
}

int is_reg_available(struct FunctionBlock* func, struct basic_block* b,struct statment_s* mainStatment, struct statment_value regReplaced){
 int reached=0;
 if (b==NULL) return 0;
  struct basic_block* block = b;
  while(block!=NULL){
    struct statment_s* statment = block->firstStmt;
    while(statment!=NULL){
      if(reached==0){
        reached = statment == mainStatment;
        statment = statment->next;
        continue;
      }
      if (statment->LHS->type == regReplaced.type && statment->LHS->RGnum==regReplaced.RGnum){
        return 0;
      } 
       if (statment->RHS!=NULL&&statment->RHS->type == ST_REG && statment->RHS->type==regReplaced.type && statment->RHS->RGnum==regReplaced.RGnum){
        return 0;
      }
        if (statment->RHS!=NULL&&statment->RHS->type == ST_OP && statment->RHS->op_values_next!=NULL 
          && statment->RHS->op_values_next->type==ST_REG
          && statment->RHS->op_values_next->type==regReplaced.type
          && statment->RHS->op_values_next->RGnum==regReplaced.RGnum){
        return 0;
      }
        if (statment->RHS!=NULL&&statment->RHS->type == ST_OP && statment->RHS->op_values_next!=NULL 
          && statment->RHS->op_values_next->op_values_next!=NULL 
          && statment->RHS->op_values_next->op_values_next->type==ST_REG 
          && statment->RHS->op_values_next->op_values_next->type==regReplaced.type
          && statment->RHS->op_values_next->op_values_next->RGnum==regReplaced.RGnum){
        return 0;
      }
        statment = statment->next;
    }
     block = block->next;
  }
  return 1;
}

int isRegFoundBefore(struct basic_block* tmp_block,struct statment_s* mainStatment,  struct statment_value* reg){
      while(tmp_block!=NULL){
         struct statment_s* tmp_stat =tmp_block->firstStmt;
        while(tmp_stat!=NULL){
          if (tmp_stat==mainStatment){
          return 0;
          }
          if (tmp_stat->LHS->type == reg->type && tmp_stat->LHS->RGnum == reg->RGnum)
              return 1;
          tmp_stat = tmp_stat->next;
        }
        tmp_block = tmp_block->next;
      }
      return 0;
}

void remove_redundant(struct FunctionBlock* func){
  int reached=0;
  if (func==NULL) return ;
  struct basic_block* block = func->firstBlock;
  while(block!=NULL){
    struct statment_s* statment = block->firstStmt;
    while(statment!=NULL){
      if (statment->RHS!=NULL && statment->LHS->type == statment->RHS->type && statment->LHS->RGnum == statment->RHS->RGnum){
        struct statment_s* tmp_stat = statment; 
        statment = statment->next;
        free_statment(tmp_stat);
        continue;
      }
      else
       statment = statment->next;
    }
     block = block->next;
  }
}


void AllocateRegisters(){

  struct FunctionBlock* functionBlock = firstFunction;
  while(functionBlock!=NULL){
 struct basic_block* block = functionBlock->firstBlock;
    struct basic_block* root_block = block;
  while(block!=NULL){
    struct statment_s* statment = block->firstStmt;
    while(statment!=NULL){
       
      if (statment->RHS!=NULL){

        if (statment->RHS!=NULL&&statment->RHS->type == ST_REG && is_reg_available(functionBlock, block, statment, *statment->RHS)){
          lookupInsertAsgn(functionBlock,statment->RHS );
          //add statment->RHS to available list
        }
         if (statment->RHS!=NULL&&statment->RHS->type == ST_OP && statment->RHS->op_values_next!=NULL 
          && statment->RHS->op_values_next->op_values_next!=NULL 
          && statment->RHS->op_values_next->op_values_next->type==ST_REG
          && is_reg_available(functionBlock, block, statment, *statment->RHS->op_values_next->op_values_next)){
          lookupInsertAsgn(functionBlock, statment->RHS->op_values_next->op_values_next);
          //add statment->RHS->op_values_next->op_values_next to available list
        }
        if (statment->RHS!=NULL&&statment->RHS->type == ST_OP && statment->RHS->op_values_next!=NULL 
          && statment->RHS->op_values_next->type==ST_REG
          && is_reg_available(functionBlock, block, statment, *statment->RHS->op_values_next)){
          lookupInsertAsgn(functionBlock, statment->RHS->op_values_next);
          //add statment->RHS->op_values_next to available list
        }
        if (isRegFoundBefore(functionBlock->firstBlock,statment,  statment->LHS)==0 &&statment->LHS->type==ST_REG){
          // use available List to replace LHS if any register available 
            struct available_regs* available_reg = functionBlock->lastAvailableReg;
          while(available_reg!=NULL){
          if (is_reg_available(functionBlock, block, statment, *available_reg->asgn)){
              reg_reassign(functionBlock, block, statment, *statment->LHS, available_reg->asgn);
            removeAvailableAsgn(functionBlock,  available_reg->asgn);
              break;
            }
            struct statment_value* tmp_available = available_reg->asgn;
            available_reg = available_reg->prev;
            removeAvailableAsgn(functionBlock, tmp_available);
          }

        }

          if (is_reg_available(functionBlock, block, statment, *statment->LHS) &&statment->LHS->type==ST_REG){

             lookupInsertAsgn(functionBlock, statment->LHS);
          }
      }
       statment = statment->next;
     }
     block = block->next;
    }
    remove_redundant(functionBlock);
  functionBlock = functionBlock->next;
  }

}