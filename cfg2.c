#include "y.tab.h" 
#include "containers.h"
#include "bblock.h"
#include "symbol.h"
#include "comp.h"
#include "ssa.h"
#include "ast.h"
#include "cfg2.h"

struct FunctionBlock* currentFunction;
struct basic_block* currentBlock;
int descent_recursion(struct ast* node, struct basic_block* block );
void AddBranch(struct ast* node, struct basic_block* block );


void printBlocks(){
    struct FunctionBlock* func = firstFunction;
    while(func!=NULL){
    struct basic_block* block = func->firstBlock;
    while(block!=NULL){
        printf("%d starting = %d ending = %d\n",block->index, block->startingNode, block->endingNode );
        block=block->next;
    }
    func = func->next;
    }
}

int bbNum = 0;

void AddBranch(struct ast* node, struct basic_block* block ){
        block->endingNode = get_child(node, 1)->id;
        struct basic_block* block1 = creatSubBlock(block,get_child(node, 1)->id,++bbNum,get_child(node, 1)->id, get_child(node, 2)->id);
        struct basic_block* block2 = creatSubBlock(block,get_child(node, 2)->id,++bbNum, get_child(node, 2)->id,  get_child(node, 3)->id );
        struct basic_block* closure = creatSubBlock(block,get_child(node, 3)->id,++bbNum, get_child(node, 3)->id ,node->id);
        AddBranching(block1, closure);
        AddBlockToFunction(block1);
        descent_recursion(get_child(node, 2), block1);
        AddBranching(block2,closure);
        AddBlockToFunction(block2);
        descent_recursion(get_child(node, 3), block2);
        AddBranching(closure,block->closureBB);
        AddBlockToFunction(closure);
        AddIFBranching(block, block1, block2, closure);
}
int descent_recursion(struct ast* node, struct basic_block* block ){

    if (node->ntoken==IF ){
      descent_recursion(get_child(node, 1), block);
      AddBranch(node, lastFunction->lastBlock);
    }

    else{ 
     struct ast_child* tmp = node->child;
        while(tmp!=NULL){
            descent_recursion(tmp->id, block);
            tmp = tmp->next;
        }
    }
    return 0;
}


int create_blocks(struct ast* tmp){

    if (isFunction(tmp->ntoken)){
        struct ast_child* t = tmp->child;
        struct ast* ident = t->id;
        t = t->next;
        while(t!=NULL && isParameter(t->id->ntoken)){
            t = t->next;
        }
        struct FunctionBlock* createdFunction = creatEntry( ident->id, ident->token, B_FUNCTION, ident->id, tmp->id, checkResultType(t->id));
        descent_recursion(t->id, createdFunction->lastBlock);
    }else if(tmp->ntoken==PRINT){
        int startNode = 1;
        struct ast_child* t = tmp->child;
        while(t!=NULL&&t->id!=NULL && get_child(t->id, 1)!=NULL){
            t = t->id->child;
        }
         if(lastFunction!=NULL&& lastFunction->lastBlock!=NULL){
             lastFunction->lastBlock->endingNode=t->id->id-1;
        }
        struct FunctionBlock* createdFunction =creatEntry(t->id->id-1, tmp->token, B_PRINT,t->id->id-1, tmp->id,  checkResultType(t->id));
        descent_recursion(tmp->child->id, createdFunction->lastBlock);
    }else if(tmp->ntoken==EVAL){
        int startNode = 1;
        struct ast_child* t = tmp->child;
        while(t!=NULL&&t->id!=NULL && get_child(t->id, 1)!=NULL){
            t = t->id->child;
        }
         if(lastFunction!=NULL&& lastFunction->lastBlock!=NULL){
             lastFunction->lastBlock->endingNode=t->id->id-1;
        }
        struct FunctionBlock* createdFunction =creatEntry(t->id->id-1, tmp->token, B_EVAL,t->id->id-1, tmp->id,  checkResultType(t->id));
        descent_recursion(tmp->child->id,  createdFunction->lastBlock);
    }

    return 0;
}

void initCFG(){
    // printBlocks();
    currentFunction = firstFunction;
    currentBlock = firstFunction->firstBlock;
    resetRegCount();
}

int shouldNotUpdateReg(struct ast* node){
    if(node->ntoken != IF && node->parent!=NULL && node->parent->ntoken == IF && ((get_child(node->parent, 2)==node)||(get_child(node->parent, 3)==node))){
        return 1;
    }
    return 0;
}

int create_cfg(struct ast* tmp){
    while(currentBlock!=NULL&& currentBlock->next!=NULL&&currentBlock->next->startingNode<tmp->id ){
        currentBlock = currentBlock->next;
    }
    // if (currentSubBlock!=NULL&&currentSubBlock->block!=NULL&&currentSubBlock->block->startingNode<tmp->id && currentSubBlock->block->endingNode>=tmp->id)
    // {
    //     currentBlock = currentSubBlock->block;
    // }
    if (isFunction(tmp->ntoken)){
        if(currentBlock->lastStmt!=NULL&& currentBlock->lastStmt->LHS!=NULL&&currentBlock->lastStmt->LHS->type!=ST_RETURN){
            struct statment_value* start = createStatment(currentBlock,  ST_RETURN, tmp, shouldNotUpdateReg(tmp));
            struct statment_value* end = cpyStatment(currentBlock->lastStmt->LHS);
            AddStatement(currentFunction, currentBlock, start, end, tmp, shouldNotUpdateReg(tmp));
        }
        struct statment_value* closure = createStatment(currentBlock, ST_EXIT, tmp, shouldNotUpdateReg(tmp));
        AddStatement(currentFunction, currentBlock, closure, NULL, tmp, shouldNotUpdateReg(tmp));
        currentFunction = currentFunction->next;
        currentBlock = currentFunction->firstBlock;
    }else if(tmp->ntoken==EVAL||tmp->ntoken==PRINT){
        struct statment_value* start = createStatment(currentBlock,  ST_PARAM, tmp, shouldNotUpdateReg(tmp));
        struct statment_value* end = cpyStatment(currentBlock->lastStmt->LHS);
        AddStatement(currentFunction, currentBlock, start, end, tmp, shouldNotUpdateReg(tmp));
        struct statment_value* call_st =createStatment(currentBlock, ST_CALL, tmp, shouldNotUpdateReg(tmp));
        call_st->title = "print";
        AddStatement(currentFunction, currentBlock, call_st, NULL, tmp, shouldNotUpdateReg(tmp));
        struct statment_value* closure = createStatment(currentBlock, ST_EXIT, tmp, shouldNotUpdateReg(tmp));
        AddStatement(currentFunction, currentBlock, closure, NULL, tmp,  shouldNotUpdateReg(tmp));
    }
    // else if(){
    //     struct statment_value* start = createStatment(currentBlock,  ST_PRINT, tmp);
    //     struct statment_value* end = cpyStatment(currentBlock->lastStmt->LHS);
    //     AddStatement(currentBlock, start, end, tmp);
    //     struct statment_value* closure = createStatment(currentBlock,  ST_EXIT, tmp);
    //     AddStatement(currentBlock, closure, NULL, tmp);
    // }
    if (tmp->ntoken==IF){
        struct statment_value* end =createStatment (currentBlock, ST_REG, tmp, shouldNotUpdateReg(tmp));
        struct statment_value* start;
        if (isFunction(tmp->parent->ntoken)){
            start=createStatment (currentBlock, ST_RETURN, tmp, shouldNotUpdateReg(tmp));
        }else{
            start=createStatment (currentBlock, ST_REG, tmp, shouldNotUpdateReg(tmp));
        
        AddStatement(currentFunction, currentBlock, start,end, tmp, shouldNotUpdateReg(tmp));
        updateRegNum(currentBlock->parent);
    }
    if (isParameter(tmp->ntoken)){
        struct statment_value* start =createStatment(currentBlock,  ST_REG, tmp, shouldNotUpdateReg(tmp));
        struct statment_value* end = createStatment(currentBlock, ST_PARAM, tmp, shouldNotUpdateReg(tmp));
        AddStatement(currentFunction, currentBlock, start,end, tmp, shouldNotUpdateReg(tmp));
    }else if(tmp->ntoken == CONST ||tmp->ntoken == TRUE ||tmp->ntoken == FALSE){
        struct statment_value* start= createStatment(currentBlock,  ST_REG, tmp, shouldNotUpdateReg(tmp));
        struct statment_value* end = createStatment(currentBlock,  ST_CONST, tmp, shouldNotUpdateReg(tmp));
        AddStatement(currentFunction, currentBlock, start,end, tmp, shouldNotUpdateReg(tmp));
        
    }else if(tmp->ntoken == EQ ||tmp->ntoken == LT ||tmp->ntoken == GT
        ||tmp->ntoken == GE ||tmp->ntoken == LE
        ||tmp->ntoken == AND ||tmp->ntoken == OR
        ||tmp->ntoken == NOT||tmp->ntoken == PLUS
        ||tmp->ntoken == MINUS ||tmp->ntoken == DIV  
        ||tmp->ntoken == MOD ||tmp->ntoken == MULT){
        if (tmp->ntoken == MULT||tmp->ntoken == PLUS||tmp->ntoken == AND ||tmp->ntoken == OR)
        {
            struct ast_child* t = tmp->child;
            struct statment_s* last=NULL;
            struct statment_value* first=NULL;
            struct statment_value* second=NULL;
            struct statment_value* op_values=NULL;

            while(t!=NULL){
                if (last==NULL){ 
                    op_values=calloc(1,sizeof(struct statment_value));
                    addOperand(op_values,t->id);
                    t=t->next;
                    if(t!=NULL){
                        op_values->op_values_next=calloc(1,sizeof(struct statment_value));
                        addOperand(op_values->op_values_next,t->id); 
                    }
                }else{
                    op_values=cpyStatment(first);
                    op_values->op_values_next=calloc(1,sizeof(struct statment_value));
                    addOperand(op_values->op_values_next, t->id);
                }
                if (first==NULL)
                {
                    first=  createStatment(currentBlock,  ST_REG, tmp, (shouldNotUpdateReg(tmp) && t->next==NULL));
                }else{
                    first = cpyStatment(first);
                }
                second = createStatment(currentBlock,  ST_OP, tmp, shouldNotUpdateReg(tmp));
                second->op_values_next = op_values;
                last = AddStatement(currentFunction, currentBlock, first,second, tmp,(shouldNotUpdateReg(tmp) && t->next==NULL));
                t=t->next;
            }
        }else{
            struct statment_value* start= createStatment(currentBlock,  ST_REG, tmp, shouldNotUpdateReg(tmp));
            struct statment_value* end = createStatment(currentBlock,  ST_OP, tmp, shouldNotUpdateReg(tmp));
            AddStatement(currentFunction, currentBlock, start,end, tmp, shouldNotUpdateReg(tmp));
        }
    }else if (tmp->ntoken == ID){
        struct symbol* var = lookup(tmp->token, tmp->id);
        if (var!=NULL){
            struct statment_value* start;
        struct statment_value* end;
            if(tmp->parent==NULL || tmp->parent->ntoken!=LET || (get_child(tmp->parent, 1)->id!=tmp->id )){
                start= createStatment(currentBlock,  ST_REG, tmp, shouldNotUpdateReg(tmp));
                end = cpyStatment(var->RegValue);
                AddStatement(currentFunction, currentBlock, start,end, tmp, shouldNotUpdateReg(tmp));
            }

        }
    }else if (tmp->ntoken == CALL){
        struct symbol* fun = lookup(tmp->token, tmp->id);
        if (fun!=NULL&&fun->firstArg!=NULL)
        {   struct ast_child* t = tmp->child;
            struct argument* args = fun->firstArg;
            struct statment_value* start;
            struct statment_value* end;
            while(args!=NULL && t!=NULL){
                struct ast* ch = t->id;
                struct statment_s* statment =ch->reg_statment;
                start= cpyStatment(args->argNode->ArgValue);
                end = cpyStatment(statment->LHS);
                AddStatement(currentFunction, currentBlock, start,end, tmp, shouldNotUpdateReg(tmp));
                args = args->next;
                t = t->next;
            }
        }
        struct statment_value* call = createStatment(currentBlock,  ST_CALL, tmp, shouldNotUpdateReg(tmp));
        AddStatement(currentFunction, currentBlock, call, NULL,tmp, shouldNotUpdateReg(tmp));
        struct statment_value* start= createStatment(currentBlock,  ST_REG, tmp, shouldNotUpdateReg(tmp));
        struct statment_value* end = createStatment(currentBlock,  ST_RETURN, tmp, shouldNotUpdateReg(tmp));
        AddStatement(currentFunction, currentBlock, start,end, tmp, shouldNotUpdateReg(tmp));
    }
    if (tmp->parent!=NULL && tmp->parent->ntoken == IF && (get_child(tmp->parent, 1)==tmp)&&currentBlock->lastStmt!=NULL&& currentBlock->lastStmt->LHS!=NULL){
        currentBlock->cond = cpyStatment(currentBlock->lastStmt->LHS);
    }
    if (tmp->parent!=NULL&& tmp->parent->ntoken==LET && (get_child(tmp->parent, 2)->id==tmp->id )){
        struct symbol* var = lookup(get_child(tmp->parent, 1)->token, get_child(tmp->parent, 1)->id);
        if (var!=NULL && var->RegValue==NULL){
                var->RegValue = cpyStatment(currentBlock->lastStmt->LHS);
        }
    }
    return 0;
}
}

void printSTvalue(struct statment_value* statment){
    if (statment==NULL) return;
    int type = statment->type;
    if (type==ST_PARAM){
       printf("a%d ",statment->RGnum);
    }else if (type == ST_REG ){
       printf("v%d ",statment->RGnum);
    }else if(type == ST_CONST){
       printf("%s ",statment->title);
    }else if (type == ST_CALL){
        printf("call %s ",statment->title);
    }else if (type == ST_RETURN ){
        printf("%s ",statment->title);
    }else if (type == ST_PRINT ){
        printf("%s ",statment->title);
    }else if (type == ST_EVAL ){
        printf("%s ",statment->title);
    }else if (type==ST_EXIT){
        printf("br exit");
    }else if (type==ST_OP){
        struct statment_value* value = statment->op_values_next;
        if (statment->isNot){
            printf("%s ", statment->title);
            printSTvalue(value);
        }else{
            while(value!=NULL){
                printSTvalue(value);
                value = value->op_values_next;
                if (value!=NULL){
                    printf("%s ", statment->title);
                }
            }
        }
    }
}
void print_blocks(struct basic_block* block){
   if (block->visited!=1){
    struct statment_s* stat = block->firstStmt;
    block->visited = TRUE;
    while(stat!=NULL){
        printSTvalue(stat->LHS);
        if (stat->RHS!=NULL){
            if (stat->LHS->type!=ST_EVAL && stat->LHS->type!=ST_PRINT) {printf(" := ");}
            printSTvalue(stat->RHS);
        }
        printf("\n");
        stat = stat->next;
    }
    if (block->cond!=NULL)
    {
        printf("br ");
        printSTvalue(block->cond);
        if (block->firstBB!=NULL)
        {
            printf("bb%d",block->firstBB->bbNum);
        }
        if (block->secondBB!=NULL)
        {printf(" bb%d \n", block->secondBB->bbNum);
        }
            // 
    }else if(block->closureBB!=NULL){
        printf("br bb%d \n", block->closureBB->bbNum);
    }
}

}
void printCFG(){
    currentFunction = firstFunction;
    while(currentFunction!=NULL){
    	// generateSSA(currentFunction);
        if (currentFunction->type== B_FUNCTION){
                printf("\n\nfunction %s\n",currentFunction->title );
                printf("\nentry:\n");
            }else if (currentFunction->type== B_EVAL){
                printf("\n\n EVAL \n");
                printf("\nentry:\n");
            }else if (currentFunction->type== B_PRINT){
                printf("\n\n PRINT \n");
                printf("\nentry:\n");
            }
        currentBlock = currentFunction->firstBlock;
        while(currentBlock!=NULL){
            if (currentBlock->bbNum>0){
                printf(" \nbb%d: \n", currentBlock->bbNum);
            }
            print_blocks(currentBlock);
            currentBlock = currentBlock->next;
        }
        currentFunction = currentFunction->next;
    }
}
FILE *fp;
int printFile(struct FunctionBlock* root) {
    struct basic_block* block = root->firstBlock;
    while(block!=NULL){
         if (block->type==ENTRY){
            fprintf(fp, "%d [label=\"entry\", fontname=\"monospace\", style=filled, fillcolor=mintcream];\n ", block->index);
        } else {
            fprintf(fp, "%d [label=\"bb%d\", fontname=\"monospace\"];\n ", block->index, block->bbNum);
        }
        if (block -> cond != NULL){
          fprintf(fp, "%d->%d\n ", block->index, block->firstBB->index);
          fprintf(fp, "%d->%d\n ", block->index, block->secondBB->index);
        }else if (block -> closureBB != NULL){
            fprintf(fp, "%d->%d\n ", block->index, block->closureBB->index);
        }else if (block->closureBB==NULL)
        {
            fprintf(fp, "%d [label=\"exit\", fontname=\"monospace\", style=filled, fillcolor=mintcream];\n ", block->index+1);
            fprintf(fp, "%d->%d\n ", block->index, block->index+1);
        }
        block= block->next;
    }
    return 0;
}

void printCFGfile(){

    fp = fopen("cfg.dot", "w");
    fprintf(fp, "digraph print {\n ");
     visit_cfg(printFile);      
     fprintf(fp, "}\n ");   
    fclose(fp);
}


void free_cfg() {
    while(firstFunction!=NULL){
        clearList(firstFunction->registersSSA);
        clearAvailableAsgn(firstFunction);
         while( firstFunction->firstBlock!=NULL){
	           free_block(firstFunction, firstFunction->firstBlock);
        }
    struct FunctionBlock* tmpFunc = firstFunction;
    firstFunction = firstFunction->next;
    free(tmpFunc);
    }

        clearList(programRegistersSSA);
        clearList(argumentsSSA);
        clearList(returnsSSA);
}

