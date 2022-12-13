#include <stdio.h>

#include <stdlib.h>

#include <string.h>
#include "containers.h"
#include "symbol.h"
#include "bblock.h"
#include "ssa.h"

void checkUsedInStatments(struct ssaItem* item,struct basic_block* block, struct statment_s* statment){
  if (item!=NULL){
      struct basic_block* tmpBlock = block;
      struct statment_s* tmpStatment = statment;
      while(tmpBlock!=NULL){
        if(tmpBlock->cond!=NULL&&tmpBlock->cond->RGnum==item->reg->RGnum && tmpBlock->cond->type ==  item->reg->type){
        	tmpBlock->cond->RGnum = item->reg->RGnum;
            tmpBlock->cond->SSAnum = item->reg->SSAnum;
        }
        while(tmpStatment!=NULL){
          if (tmpStatment->RHS!=NULL){
            if (tmpStatment!=NULL&& tmpStatment->RHS->RGnum==item->reg->RGnum && tmpStatment->RHS->type ==  item->reg->type){
            	tmpStatment->RHS->RGnum = item->reg->RGnum;
            	tmpStatment->RHS->SSAnum = item->reg->SSAnum;
              addUsedInStatment(item, tmpStatment);
            }else if(tmpStatment->RHS->op_values_next!=NULL && tmpStatment->RHS->op_values_next->op_values_next!= NULL
             && tmpStatment->RHS->op_values_next->op_values_next->RGnum==item->reg->RGnum&& tmpStatment->RHS->op_values_next->op_values_next->type==item->reg->type){
            	tmpStatment->RHS->op_values_next->op_values_next->RGnum = item->reg->RGnum;
            	tmpStatment->RHS->op_values_next->op_values_next->SSAnum = item->reg->SSAnum;
              addUsedInStatment(item, tmpStatment);
            }else if(tmpStatment->RHS->op_values_next!=NULL && tmpStatment->RHS->op_values_next->RGnum==item->reg->RGnum && tmpStatment->RHS->op_values_next->type==item->reg->type ){ 
            	tmpStatment->RHS->op_values_next->RGnum = item->reg->RGnum;
            	tmpStatment->RHS->op_values_next->SSAnum = item->reg->SSAnum;
              addUsedInStatment(item, tmpStatment);
            }
          }
          tmpStatment = tmpStatment->next;
        }

        tmpBlock = tmpBlock->next;
        if (tmpBlock!=NULL){
        	tmpStatment =tmpBlock->firstStmt;
        }
      }
    }


}
void addUsedInStatment(struct ssaItem* item, struct statment_s* statment){
	if (item==NULL) return;
	if (item->firstUsed==NULL){
		item->firstUsed = item->lastUsed = calloc(1, sizeof(struct usedChain));
	}else{

		struct usedChain* tmp = calloc(1, sizeof(struct usedChain));

		tmp->prev = item->lastUsed;

		item->lastUsed->next = tmp;

		item->lastUsed = tmp;
	}
	item->lastUsed->statment = statment;
}

struct ssaList* createSSAlist(enum StatementType listRegType){
	struct ssaList* node =  calloc(1,sizeof(struct ssaList));
	if (node!=NULL){
		node->listRegType =listRegType;
	}
	return node;
}

struct ssa* createSSA(struct statment_value* value){
	struct ssa* node =  calloc(1,sizeof(struct ssa));
	if (node!=NULL&& value!=NULL){
		node->rgNum = value->RGnum;
		node->ssaReached = 0;
	}
	return node;
}

struct ssaItem* createSSAItem(struct statment_value* value, struct statment_s* statment, int num){
	struct ssaItem* node =  calloc(1,sizeof(struct ssaItem));
	if (node!=NULL&& statment !=NULL&& value !=NULL){
		node->ssaNum = num;
		node->reg = value;
		node->statment = statment;
		node->firstUsed = NULL;
	}
	return node;
}

int lookupRemoveSSAItem(struct ssaList*  list, struct statment_value* value){
	if (value==NULL) return 0;
	// struct ssaList* list = getListByType(block, value->type);
	if (list==NULL || list->listRegType!= value->type) return 0;
	struct ssa* tmp= list->firstSSA;
	while(tmp!=NULL){
		if(tmp->rgNum==value->RGnum){
			if (removeSSAItem(tmp, tmp->firstItem, value)){
				removeSSA(tmp,list);
				return 1;
			}
		}
		tmp = tmp->next;
	}
	return 0;
}

int removeSSA(struct ssa* tmp, struct ssaList* list){
	if (tmp->firstItem==NULL)
	{
			if (tmp->prev==NULL&& tmp->next==NULL){
				free(tmp);
				free(list);
				return 1;
			}else if(tmp->prev==NULL){
				list->firstSSA = tmp->next;
			}else{
				tmp->prev->next = tmp->next;
			}
			free(tmp);
			return 1;
	}
	return 0;
}

int removeSSAItem(struct ssa* tmp, struct ssaItem* item, struct statment_value* value){
	struct ssaItem* currentItem = item;
	while(currentItem!=NULL){
		if(currentItem->ssaNum == value->SSAnum){
			if (currentItem->prev==NULL&& currentItem->next==NULL){
				tmp->firstItem = tmp->lastItem = NULL;
				free(currentItem);
				return 1;
			}else if(currentItem->prev==NULL){
				tmp->firstItem = tmp->firstItem->next;
			}else{
				currentItem->prev->next = currentItem->next;
			}
			free(currentItem);
			updateSSA(tmp);
			return 1;
		}

		currentItem = currentItem->next;

	}

	return 0;

}

void updateSSA(struct ssa* item){

	item->ssaReached=0;

	struct ssaItem* currentItem = item->firstItem;

	while(currentItem!=NULL){

		item->ssaReached++;

		currentItem->reg->SSAnum = item->ssaReached;

		currentItem->ssaNum = item->ssaReached;

		currentItem = currentItem->next;

	}

}

void clearAllSSAlists(struct SSABlock* block){
	if (block==NULL) return;
	clearList(block->regeistersList);
	clearList(block->argumentsList);
	clearList(block->returnList);
}
void clearList(struct ssaList* list){
	if (list==NULL) return;

	while(list->firstSSA!=NULL){

		while(list->firstSSA->firstItem!=NULL){

			while(list->firstSSA->firstItem->firstUsed!=NULL){
				struct usedChain* prevItem = list->firstSSA->firstItem->firstUsed;

				list->firstSSA->firstItem->firstUsed= list->firstSSA->firstItem->firstUsed->next;

				free(prevItem);
			}

			struct ssaItem* prevItem = list->firstSSA->firstItem;

			list->firstSSA->firstItem = list->firstSSA->firstItem->next;

			free(prevItem);

		}

		struct ssa* prevSSA = list->firstSSA;

		list->firstSSA = list->firstSSA->next;

		free(prevSSA);

	}

	free(list);

}

struct ssaItem* lookupReg(struct ssaList*  list, struct statment_value* reg){

	// struct ssaList* list = getListByType(block, reg->type);

	if (list==NULL || list->listRegType!= reg->type) return NULL;

	struct ssa* tmp= list->firstSSA;

	while(tmp!=NULL){

		if(tmp->rgNum==reg->RGnum){

			struct ssaItem* tmpItem = tmp->firstItem;
// 
			while(tmpItem!=NULL){

				if (tmpItem->ssaNum==reg->SSAnum){

					return tmpItem;

				}

				tmpItem = tmpItem->next;

			}

		}

		tmp = tmp->next;

	}

	return NULL;

}


struct ssa* insertSSA(struct ssaList* list, struct statment_s* statment, struct statment_value* value){

	if (list->firstSSA==NULL){

		list->firstSSA = list->lastSSA = createSSA(value);

	}else{

		struct ssa* tmp = createSSA(value);

		tmp->prev = list->lastSSA;

		list->lastSSA->next = tmp;

		list->lastSSA = tmp;

	}

	insertReg(list->lastSSA,statment,value);

	return list->lastSSA ;

}


struct ssaItem* insertReg(struct ssa* item, struct statment_s* statment, struct statment_value* value){

	item->ssaReached += 1 ;
	if (item->firstItem==NULL){

		item->firstItem = item->lastItem = createSSAItem(value, statment, item->ssaReached);

	}else{

		struct ssaItem*
		tmp = createSSAItem(value, statment, item->ssaReached);

		tmp->prev = item->lastItem;

		item->lastItem->next = tmp;

		item->lastItem = tmp;

	}

	value->SSAnum =  item->lastItem->ssaNum;

	return item->lastItem ;

}


struct ssaItem* lookupInsertReg( struct ssaList*  list, struct statment_s* statment, struct statment_value* value){

	if (statment==NULL||value==NULL) return NULL;

	// struct ssaList* list = getListByType(block, value->type);

	if (list==NULL || list->listRegType!= value->type) return NULL;

	struct ssa* tmp= list->firstSSA;

	while(tmp!=NULL){

		if(tmp->rgNum==value->RGnum){

			return insertReg(tmp, statment, value);

		}

		tmp = tmp->next;

	}


	struct ssa* result = insertSSA(list, statment, value);

	return result->firstItem;
}


struct ssaList* getListByType(struct SSABlock* block, enum StatementType listRegType){
	if (listRegType==ST_REG){

		if (block->regeistersList==NULL){

			block->regeistersList= createSSAlist(ST_REG);

		}

		return block->regeistersList;

	}else if (listRegType==ST_PARAM){

		if (block->argumentsList==NULL){

			block->argumentsList= createSSAlist(ST_PARAM);

		}

		return block->argumentsList;

	}else if (listRegType==ST_RETURN){

		if (block->returnList==NULL){

			block->returnList= createSSAlist(ST_RETURN);

		}

		return block->returnList;

	}

	return NULL;

}
int visit_SSA(struct ssaList* list, int (*f)(struct ssa* ssa)){
	struct ssa* temp_root;
	if (list==NULL) return 0;
	temp_root = list->firstSSA;
	while(temp_root!=NULL){
		if (f(temp_root) == 1) return 1;
		temp_root = temp_root->next;
	}
	// if (block->argumentsList!=NULL){
	// 	temp_root = block->argumentsList->firstSSA;
	// 	while(temp_root!=NULL){
	// 		if (f(temp_root) == 1) return 1;
	// 		temp_root = temp_root->next;
	// 	}
	// }
	// if (block->returnList!=NULL){
	// 	temp_root = block->returnList->firstSSA;
	// 	while(temp_root!=NULL){
	// 		if (f(temp_root) == 1) return 1;
	// 		temp_root = temp_root->next;
	// 	}
	// }
return 0;
}

