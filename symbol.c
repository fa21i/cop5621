#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <string.h>
#include "y.tab.h" 
#include "containers.h"
#include "bblock.h"
#include "symbol.h"


struct symbol*  createSymbol(char* name, int type, int returnType, int scopeStartNode, int scopeEndNode){
	struct symbol* node =  calloc(1,sizeof(struct symbol));
	if (node!=NULL){
		node->type = type;
		node->name = name;
		node->returnType = returnType;
		node->scopeStartNode = scopeStartNode;
		node->scopeEndNode = scopeEndNode;
	}
	return node;
}
struct symbol*  deleteSymbolNode(struct symbol* node){
	if (node!=NULL){
		struct symbol* resultNode = node->list;
		if ((strcmp(first->name,node->name)==0)&&(strcmp(last->name,node->name)==0))
		{
			last = first = NULL;
		}else if (strcmp(last->name,node->name)==0){
			last= node->listPrevious;
		} else if (strcmp(first->name,node->name)==0){
			first= node->list;
		} else if (resultNode!=NULL && count<3){
			last = first = resultNode;
		} else if(resultNode==NULL && count<3){
			last = first = node->listPrevious;
		}
		if (node->listPrevious!=NULL){
			node->listPrevious->list = resultNode;
		}
		while(node->firstArg!=NULL){
			struct argument* arg = node->firstArg;
			free(arg);
			node->firstArg = node->firstArg->next;
		}
		
		if (node-> previous==NULL){
			unsigned index = hash(node->name) % 1023;
			symbolMap[index] = node->next;
		}else{
			node-> previous->next = node->next;
		}
		
		free(node);
		count--;
		return resultNode; 
	}
	return NULL;
}
struct symbol*  deleteSymbol(char* name, int nodeId){
	struct symbol* node = lookup(name,nodeId);
	return deleteSymbolNode(node);
}


void clearSymbolTable(){
	while(first!=NULL){
		deleteSymbolNode(first);
	}
}

void clearFuntionVariables(char* name, int nodeId){
	struct symbol* node = lookup(name, nodeId);
	if (node!=NULL && node->list!=NULL){
		struct symbol* child = node->list;
		while(child!=NULL){
			child = deleteSymbolNode(child);
		}
		last = node;
	}
}

struct symbol* lookup(char* s, int nodeId) {
	unsigned h = hash(s);
	return lookupAssign(s,h,nodeId);
}

struct symbol* lookupAssign(char* s, unsigned h, int nodeId) {
	unsigned index = h % 1023;
	struct symbol* sym  = symbolMap[index];
	while (sym != NULL) {
		if (strcmp(sym->name, s) == 0 && sym->scopeStartNode<=nodeId && (sym->scopeEndNode>=nodeId||sym->scopeEndNode==-1)) break;
		sym = sym->next;
	}
	return sym;
}


struct symbol* insert(char* s, int type, int returnType, int scopeStartNode, int scopeEndNode, unsigned h) {
	unsigned index = h % 1023;
	struct symbol* sym = createSymbol(s,type,returnType,scopeStartNode,scopeEndNode);
	sym->next = symbolMap[index];
	if(symbolMap[index]!=NULL){
		symbolMap[index]->previous = sym;
	}
	symbolMap[index] = sym;

	if (count == 0) { first = last = sym; }
	else {
		sym -> listPrevious = last;
		last -> list = sym;
		last = sym;
	}

	count++;
	return sym;
}

struct symbol* lookupInsert(char* s, int type, int returnType, int scopeStartNode, int scopeEndNode, int nodeId) {
	unsigned h = hash(s); // Computed once
	struct symbol* sym = lookupAssign(s,h, nodeId); // Locate symbol first
	if (sym == NULL) { // If not found
		sym = insert(s,type,returnType,scopeStartNode,scopeEndNode,h); // Insert a new symbol
	}
	return sym;
}
bool lookupDeleteSymbol(char* name, int nodeId){
	
	struct symbol* node = lookup(name, nodeId);
	if (node!=NULL)
	{
		deleteSymbolNode(node);
		return true;
	}
	return false;
}
struct symbol* insertArgument(char* s, int type, int returnType, int scopeStartNode, int scopeEndNode, char* funName, int nodeId) {
	struct symbol* sym  = lookupInsert(s, type,returnType,scopeStartNode,scopeEndNode,nodeId);
	struct symbol* fun = lookup(funName, nodeId);
	if (sym!=NULL && fun != NULL)
	{
		if ( fun->firstArg==NULL)
		{
			fun->firstArg = fun->lastArg = calloc(1,sizeof(struct argument));
		}else{
			fun->lastArg->next = calloc(1,sizeof(struct argument));
			fun->lastArg = fun->lastArg->next;
		}
		fun->lastArg->name = s;
		fun->lastArg->argNode = sym;
		fun->lastArg->returnType = returnType;
	}
	return sym;
}

unsigned int hash(void *string)
{
	unsigned int result = 5381;
	unsigned char *p;

	p = (unsigned char *) string;
	while (*p != '\0') {
		result = (result << 5) + result + (unsigned int) tolower(*p);
		++p;
	}

	return result;
}

void printSymbolTable(){
	struct symbol* fSym = first;
	while(fSym!=NULL){
		printf("var = %s | scope start at index = %d |  scope end at index = %d \n", fSym->name, fSym->scopeStartNode, fSym->scopeEndNode);
		fSym = fSym->list;
	}

}