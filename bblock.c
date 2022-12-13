// #include "y.tab.h"
// #include "symbol.h"
// #include "cfg2.h"
// #include "cfg.h"
// #include "containers.h"
// #include "comp.h"
// #include "ssa.h"
// #include "bblock.h"

#include "y.tab.h"
#include "containers.h"
#include "symbol.h"
#include "bblock.h"
#include "comp.h"
#include "ssa.h"

  int reg_num = 0;
  int arg_num = 0;
  struct FunctionBlock* createFunctionBlock(struct basic_block* block, enum basics type, char* title){
  struct FunctionBlock* function_block = calloc(1,sizeof(struct FunctionBlock));
  function_block->title = title;
  function_block->type = type;
  function_block->registersSSA = createSSAlist(ST_REG);
  if (firstFunction==NULL){
    firstFunction = lastFunction = function_block;
  }else{
        function_block->prev = lastFunction;
        lastFunction->next = function_block;
        lastFunction = function_block;
    }
  AddBlockToFunction(block);
  return function_block;
 }

  struct FunctionBlock* creatEntry(int id, char* title, enum basics type, int startingNode, int endingNode, int returnType){
   struct basic_block* block  = calloc(1,sizeof(struct basic_block));
   block->index = id;
   block->startingNode = startingNode;
   block->endingNode = endingNode;
   block->parent = NULL;
   block->bbNum = 0;
   block->type = ENTRY;
   block->returnType = returnType;
   return createFunctionBlock(block, type, title);
  }

  // struct basic_block* creatExit(struct FunctionBlock* function_block, struct basic_block* parentBlock,int id){
  //  struct basic_block* block  = creatSubBlock(parentBlock, id,0, 0, 0);
  //  block->type = EXIT;
  //  return block;
  // }

  struct basic_block* creatSubBlock(struct basic_block* parentBlock,int id,int bbNum, int startingNode, int endingNode){
   struct basic_block* block  = calloc(1,sizeof(struct basic_block));
   block->index = id;
   block->startingNode = startingNode;
   block->endingNode = endingNode;
   block->parent = parentBlock;
   block->bbNum = bbNum;
   return block;
  }
  struct statment_s*  cpyS(struct basic_block* block, struct statment_s*  cpyStat){
     struct statment_s* statment  = calloc(1,sizeof(struct statment_s));
   statment->LHS = cpyStatment(cpyStat->LHS);
   statment->RHS =  cpyStatment(cpyStat->RHS);
   statment->index = cpyStat->index;
   statment->block = block;
   return statment;
  }

  struct statment_value* cpyStatment(struct statment_value* oldStat){

    if (oldStat==NULL) return NULL;
   struct statment_value* statment = calloc(1,sizeof(struct statment_value));
   statment->type = oldStat->type;
   statment->RGnum = oldStat->RGnum;
   statment->title = oldStat->title;
   statment->isNot = oldStat->isNot;
   statment->token = oldStat->token;
   statment->SSAnum = oldStat->SSAnum;
   statment->returnType = oldStat->returnType;

   if(oldStat->op_values_next!=NULL){
    struct statment_value* op_values =  oldStat->op_values_next;
    struct statment_value* first;
    struct statment_value* last;
    first = last = calloc(1,sizeof(struct statment_value));
    while(op_values!=NULL){
        last->type = op_values->type;
        last->title = op_values->title;
        last->RGnum = op_values->RGnum;
        last->SSAnum = op_values->SSAnum;
      op_values = op_values->op_values_next;
      if (op_values!=NULL){
        last->op_values_next  = calloc(1,sizeof(struct statment_value));
        last = last->op_values_next;
      }
    }
    statment->op_values_next = first;
  }
  return statment;
  }

  void addOperand(struct statment_value* operand, struct ast* node){
    if (operand==NULL){
      operand = calloc(1,sizeof(struct statment_value));
    }
    if ( node!=NULL&&  node->ntoken==LET && get_child(node,3)->reg_statment!=NULL &&get_child(node, 3)->reg_statment->LHS!=NULL ){
        operand->type = ST_REG;
        operand->title = get_child(node, 3)->reg_statment->LHS->title;
        operand->RGnum = get_child(node, 3)->reg_statment->LHS->RGnum;
        operand->SSAnum = get_child(node, 3)->reg_statment->LHS->SSAnum;
      }else if ( node!=NULL&&node->reg_statment!=NULL&&node->reg_statment->LHS!=NULL){
        operand->type = ST_REG;
        operand->title = node->reg_statment->LHS->title;
        operand->RGnum = node->reg_statment->LHS->RGnum;
        operand->SSAnum = node->reg_statment->LHS->SSAnum;
      }
  }

  struct statment_value* createStatment(struct basic_block* block, enum StatementType type , struct ast* node, int noRegNumUpdate){

   struct statment_value* statment = calloc(1,sizeof(struct statment_value));
   statment->type = type;
   statment->title = node->token;
   statment->token = node->ntoken;
   statment->returnType = checkResultType(node);
   if (type==ST_PARAM){
    if (node->ntoken==EVAL){
      statment->RGnum = 1;
    }else{
      arg_num++;
      statment->RGnum = arg_num;
      struct symbol* var = lookup(node->token, node->id);
      if (var!=NULL&&var->ArgValue==NULL){
        statment->returnType = var->returnType;
        var->ArgValue = statment;
      }
    }
  }else if (type == ST_REG ){
    if (noRegNumUpdate){
      return statment;
    }else{
     reg_num++;
     statment->RGnum =reg_num;
   }
   struct symbol* var = lookup(node->token, node->id);
   if (var!=NULL && var->RegValue==NULL){
    var->RegValue = statment;
  }
  }else if (type == ST_CALL ||type == ST_CONST){
   statment->title = node->token;
  }else if (type == ST_RETURN ){
   statment->RGnum =0;
   statment->title = "rv";
  }else if (type == ST_PRINT ){
   statment->title = "print";
  }else if (type == ST_EVAL ){
   statment->title = "EVAL ";
  }else if (type==ST_EXIT){
    if (node->child!=NULL){
      struct symbol* fun = lookup(get_child(node,1)->token, node->id);
      if (fun!=NULL &&block->lastStmt->LHS->type==ST_RETURN){
        fun->RegValue = block->lastStmt->LHS;
      }
    }
   statment->RGnum = reg_num-1;
   arg_num = 0;
   reg_num=0;
  }else if (type==ST_OP && !(node->ntoken == MULT||node->ntoken == PLUS||node->ntoken == AND ||node->ntoken == OR)){
    struct ast_child* t = node->child;
    if (node->ntoken==NOT){
      statment->isNot = 1;
    }
    struct statment_value* first;
    struct statment_value* last;
    first = last = calloc(1,sizeof(struct statment_value));
    while(t!=NULL){
      addOperand(last,t->id);
      t =t->next;
      if (t!=NULL){
        last->op_values_next  = calloc(1,sizeof(struct statment_value));
        last = last->op_values_next;
      }
    }
    statment->op_values_next = first;
  }
  return statment;
  }

  

  struct statment_s*  AddStatement(struct FunctionBlock* funcBlock, struct basic_block* block, struct statment_value* LHS, struct statment_value* RHS, struct ast* node, int noRegNumUpdated){
   struct statment_s* statment  = calloc(1,sizeof(struct statment_s));
   statment->LHS = LHS;
   statment->RHS = RHS;
   statment->index = node->id;
   statment->block = block;
   if (RHS!=NULL){
    LHS->returnType = RHS->returnType;
   }
   insertStatement(block, statment);
   node->reg_statment = statment;
   return statment;
  }

  void insertStatement(struct basic_block* block,struct statment_s* statment){
   if (block->firstStmt==NULL){
    block->firstStmt = block->lastStmt = statment;
  }else{
    statment->prev = block->lastStmt;
    block->lastStmt->next = statment;
    block->lastStmt = statment;
  }
  }

  int getIndex(struct basic_block* block){
    return block->index+reg_num;
  }

  void AddIFBranching(struct basic_block* block, struct basic_block* block1, struct basic_block* block2, struct basic_block* closureBlock ){
    if (block!=NULL)
    {
      block->firstBB = block1;
      block->secondBB = block2;
      block->closureBB = closureBlock;
    }
  }

  void AddBranching(struct basic_block* block, struct basic_block* branching){
   if (block!=NULL){
    block->closureBB = branching;
  }
  }
  void updateRegNum(struct basic_block* block){
    if (block!=NULL&&block->closureBB!=NULL&&block->closureBB->firstStmt!=NULL)
    {
      struct basic_block* second = block->secondBB;
      while(second->cond!=NULL){
        second = second->closureBB;
      }
      struct basic_block* first = block->firstBB;
      while(first->cond!=NULL){
        first = first->closureBB;
      }

      if (second->lastStmt->LHS->RGnum==0||second->lastStmt->LHS->SSAnum==0)
      {
      second->lastStmt->LHS = cpyStatment(block->closureBB->firstStmt->RHS);
      first->lastStmt->LHS = cpyStatment(block->closureBB->firstStmt->RHS);
      }
      return;
    }
  }

  void resetRegCount(){
    programRegistersSSA = createSSAlist(ST_REG);
    argumentsSSA = createSSAlist(ST_PARAM);
    returnsSSA = createSSAlist(ST_RETURN);
    arg_num = 0;
    reg_num = 0;
  }
struct statment_value* createNotReg( struct statment_value* reg){
 struct statment_value* statment = calloc(1,sizeof(struct statment_value));
   statment->type = ST_OP;
   statment->isNot = 1;
 struct statment_value* operand = calloc(1,sizeof(struct statment_value));
    operand->type = ST_REG;
    operand->title = reg->title;
    operand->RGnum = reg->RGnum;
    operand->SSAnum = reg->SSAnum;


statment->op_values_next = operand;
return statment;


}
void addStatmentValueToChain(struct condition_chain* chain, struct statment_value* reg){
  struct condition_value* condVal = calloc(1,sizeof(struct condition_value));
  if (chain->first==NULL){
    chain->first = chain->last =condVal;
  }else{
    condVal->prev =  chain->last;
    chain->last->next = condVal;
    chain->last = condVal;
  }
    chain->last->value = reg;
    chain->count++;
}

void generatePath(struct basic_block* block){
freeConditionChain( block);
  if (block->type==ENTRY ||(block->parent!=NULL&&block->parent->parent==NULL && block->bbNum==block->parent->closureBB->bbNum)){
    block->conditionChain= calloc(1,sizeof(struct condition_chain));
    block->conditionChain->count = 0;
  }else if (block->parent!=NULL && block->parent->cond!=NULL){
        block->conditionChain= calloc(1,sizeof(struct condition_chain));
        struct condition_chain* chain = block->conditionChain;
        struct basic_block* tmpParent = block->parent;
        struct basic_block* tmpcurrentParent = tmpParent->parent;
        while(tmpcurrentParent!=NULL&& tmpcurrentParent->cond!=NULL){
            if (tmpParent->bbNum == tmpcurrentParent->firstBB->bbNum){
            addStatmentValueToChain(chain, cpyStatment(tmpcurrentParent->cond));
            }else if (tmpParent->bbNum == tmpcurrentParent->secondBB->bbNum){
            addStatmentValueToChain(chain, createNotReg(tmpcurrentParent->cond));
            }
            tmpParent = tmpcurrentParent;
            tmpcurrentParent= tmpcurrentParent->parent;
        }
        if (block->bbNum == block->parent->firstBB->bbNum){
            addStatmentValueToChain(chain, cpyStatment(block->parent->cond));
        }else if (block->bbNum == block->parent->secondBB->bbNum){
            addStatmentValueToChain(chain, createNotReg(block->parent->cond));
        }
  }
}


 struct ssaList* generateSSA(struct FunctionBlock* functionBlock){
    clearList(functionBlock->registersSSA);
    struct ssaList* registersSSA =createSSAlist(ST_REG);;
  struct basic_block* block = getFunctionByTitle(functionBlock->title)->firstBlock;
  while(block!=NULL){
    struct statment_s* statment = block->firstStmt;
    while(statment!=NULL){
      if (statment->LHS->type==ST_REG
        &&(block->parent!=NULL&& block->parent->cond!=NULL &&block->parent->closureBB==block && block->firstStmt==statment)) {
          struct ssaItem* item =lookupInsertReg(registersSSA, statment, statment->RHS);
          updateRegNum(block->parent);
       }
       
       if (statment->LHS->type==ST_REG
        &&(!(block->parent!=NULL&& block->parent->cond!=NULL && block->cond==NULL &&(block->parent->firstBB != block||block->parent->secondBB != block)&& block->lastStmt==statment))
        ) {
        struct ssaItem* item = lookupInsertReg(registersSSA, statment, statment->LHS);
            // printf("subitem v%d.%d\n", item->reg->RGnum, item->reg->SSAnum);
        checkUsedInStatments(item, block, statment->next);
       }
       statment = statment->next;
     }
     block = block->next;
  }
  functionBlock->registersSSA = registersSSA;
  return registersSSA;
}


void generateProgramSSA(){
  clearList(programRegistersSSA);
  clearList(argumentsSSA);
  clearList(returnsSSA);
struct FunctionBlock* functionBlock = firstFunction;
while(functionBlock!=NULL){

  struct basic_block* block = functionBlock->firstBlock;
  while(block!=NULL){
    struct statment_s* statment = block->firstStmt;
    while(statment!=NULL){
      if ((statment->RHS!=NULL && statment->RHS->type==ST_PARAM))
      {
         lookupInsertReg(argumentsSSA, statment, statment->RHS);
      }
      if (statment->LHS->type==ST_REG
        &&(block->parent!=NULL&& block->parent->cond!=NULL &&block->parent->closureBB==block && block->firstStmt==statment)) {
         
          struct ssaItem* item =lookupInsertReg(programRegistersSSA, statment, statment->RHS);
          updateRegNum(block->parent);
       }
       
       if (statment->LHS->type==ST_REG
        &&(!(block->parent!=NULL&& block->parent->cond!=NULL && block->cond==NULL &&(block->parent->firstBB != block||block->parent->secondBB != block)&& block->lastStmt==statment))
        ) {
        struct ssaItem* item = lookupInsertReg(programRegistersSSA, statment, statment->LHS);
        checkUsedInStatments(item, block, statment->next);
       }

      if ((statment->LHS!=NULL && statment->LHS->type==ST_RETURN))
      {
         lookupInsertReg(returnsSSA, statment, statment->LHS);
         struct symbol* fun = lookup(functionBlock->title, block->index);
        if (fun!=NULL){
          fun->RegValue =  statment->LHS;
        }
      }
       statment = statment->next;
     }
     block = block->next;
  }
  functionBlock = functionBlock->next;
  }
}

void freeConditionChain(struct basic_block* block){

if (block->conditionChain!=NULL)
{
  struct condition_value* condVal;
  while(block->conditionChain->first!=NULL){

    condVal= block->conditionChain->first;
    block->conditionChain->first = block->conditionChain->first->next;
    free(condVal);
  }
  free(block->conditionChain);
  }
}

void AddBlockToFunction(struct basic_block* block){
    if (lastFunction->firstBlock==NULL){
        lastFunction->firstBlock = lastFunction->lastBlock = block;
    }
    else{
        block->prev = lastFunction->lastBlock;
        lastFunction->lastBlock->next = block;
        lastFunction->lastBlock = block;
    }
}

int visit_cfg(int (*f)(struct FunctionBlock* FunctionBlock)){
  struct FunctionBlock* temp_root = firstFunction;
    while(temp_root!=NULL){
      if (f(temp_root) == 1) return 1;
      // struct BlockNodes* temp_root = func->firstBlock;
      // while(temp_root != NULL){
      //   if (f(temp_root) == 1) return 1;
      //   temp_root = temp_root -> next;
      // }
      temp_root = temp_root->next;
    }
return 0;
}

void free_statment_value(struct statment_value* statment_val){
    struct statment_value* statment_op = statment_val->op_values_next;
    while(statment_op!=NULL){
        struct statment_value* tmpV = statment_op;
        statment_op = statment_op->op_values_next;
        free(tmpV);
    }
    free(statment_val);
}
void free_statment(struct statment_s* statment){
   if (statment->LHS!=NULL){
       free_statment_value(statment->LHS);
   }
   if (statment->RHS!=NULL){
       free_statment_value(statment->RHS);
   }
    if(statment->prev==NULL&& statment->next==NULL){
      statment->block->firstStmt = statment->block->lastStmt = NULL;
      }else if(statment->prev==NULL){
        statment->block->firstStmt = statment->next;
        statment->block->firstStmt->prev = NULL;
      }else if(statment->next==NULL) {
        statment->block->lastStmt =statment->prev;
        statment->prev->next = NULL;
      }
      else{
        statment->next->prev = statment->prev;
        statment->prev->next = statment->next;
      }
      free(statment);

}

void free_block(struct FunctionBlock* functionBlock, struct basic_block* block){
   while(block->firstStmt!=NULL){
            free_statment(block->firstStmt);
    }
     if (block->cond!=NULL){
             free_statment_value(block->cond);
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
struct FunctionBlock* getFunctionByTitle(char* title){
  struct FunctionBlock* temp_root = firstFunction;
    while(temp_root!=NULL&& strcmp(temp_root->title, title)){
      temp_root = temp_root->next;
    }
    return temp_root;

  
};
void clearAvailableAsgn(struct FunctionBlock* functionBlock){
  if (functionBlock==NULL) return;
   struct available_regs* tmp= functionBlock->firstAvailableReg;
  while(tmp!=NULL){
      removeAvailableAsgn(functionBlock, tmp->asgn);
    tmp = tmp->next;
  }
}
void removeAvailableAsgn(struct FunctionBlock* functionBlock,struct statment_value* value){
  if (functionBlock==NULL||value==NULL) return;
   struct available_regs* tmp= functionBlock->firstAvailableReg;
  while(tmp!=NULL){
    if(tmp->asgn==value){
      if (tmp->prev==NULL&& tmp->next==NULL){
      functionBlock->firstAvailableReg = functionBlock->lastAvailableReg  = NULL;
      }else if(tmp->prev==NULL){
        functionBlock->firstAvailableReg = tmp->next;
        tmp->next->prev = NULL;
      }else if(tmp->next==NULL){
        functionBlock->lastAvailableReg =tmp->prev;
        tmp->prev->next = NULL;
      }
      else{
        tmp->next->prev = tmp->prev;
        tmp->prev->next = tmp->next;
      }
      free(tmp);
    }
    tmp = tmp->next;
  }
    
}
void inserAsgn(struct FunctionBlock* functionBlock, struct statment_value* value){
  if (functionBlock->firstAvailableReg==NULL){
    functionBlock->firstAvailableReg = functionBlock->lastAvailableReg = calloc(1, sizeof(struct available_regs));
  }else{
    struct available_regs* tmp =calloc(1, sizeof(struct available_regs));
    tmp->prev = functionBlock->lastAvailableReg;
    functionBlock->lastAvailableReg->next = tmp;
    functionBlock->lastAvailableReg = tmp;
  }
  functionBlock->lastAvailableReg->asgn = value;
}

void lookupInsertAsgn(struct FunctionBlock* functionBlock, struct statment_value* value){

  if (functionBlock==NULL||value==NULL) return;
   struct available_regs* tmp= functionBlock->firstAvailableReg;

  while(tmp!=NULL){
    if(tmp->asgn->RGnum == value->RGnum ){
      return;
    }
    tmp = tmp->next;
  }
  inserAsgn(functionBlock, value);
}

void createAvailableAsgnList(){

struct FunctionBlock* functionBlock = firstFunction;
while(functionBlock!=NULL){

  struct basic_block* block = functionBlock->firstBlock;
  while(block!=NULL){
    struct statment_s* statment = block->firstStmt;
    while(statment!=NULL){
       statment = statment->next;
     }
     block = block->next;
    }
    functionBlock = functionBlock->next;
  }
}

