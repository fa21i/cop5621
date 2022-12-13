#include <stdbool.h>
#include <stdio.h>
struct argument
{
	char* name;
	int returnType;
	struct symbol* argNode;
	struct argument* next;
}argument;

struct symbol{
	char* name;
	int type;
	int returnType;
	int argCount;
	int scopeStartNode;
	int scopeEndNode;
	struct statment_value* RegValue;
	struct statment_value* ArgValue;
	struct argument* firstArg;
	struct argument* lastArg;
	struct symbol* listPrevious; // previous symbol in list
	struct symbol* list; // Next symbol in list
	struct symbol* next; // Next symbol in bucket 
	struct symbol* previous; // previous symbol in bucket 
} symbol;
// int MAX_MAP_SIZE = 1023;
struct symbol* symbolMap[1023]; 
struct symbol* first; // First inserted symbol
struct symbol* last; // Last inserted symbol	
int count; 

extern struct symbol* createSymbol(char* name, int type, int returnType, int scopeStartNode, int scopeEndNode); // create new Symbol
extern struct symbol* deleteSymbol(char* name, int nodeId); // delete the symbol and return next in bucket
//extern void initSymbolTable(); // Initialize table
extern void clearSymbolTable(); // Clear symbol table
extern struct symbol* lookup(char*s, int nodeId); // Lookup name s
extern struct symbol* lookupAssign(char*s,unsigned h, int nodeId); // Lookup s with hash h
extern struct symbol* deleteSymbolNode(struct symbol* node);
extern struct symbol* insert(char* s, int type, int returnType, int scopeStartNode, int scopeEndNode, unsigned h); // Insert s with hash h
extern struct symbol* lookupInsert(char* s, int type, int returnType, int scopeStartNode, int scopeEndNode, int nodeId); // Lookup for the symbol and if not found insert it return if inserted
extern bool lookupDeleteSymbol(char* name, int nodeId); // lookup for the symbol and if found delete it return if deleted
extern struct symbol* insertArgument(char* s, int type, int returnType, int scopeStartNode, int scopeEndNode, char* funName, int nodeId);
extern void clearFuntionVariables(char* name, int nodeId);
extern unsigned int hash(void *string);
extern void printSymbolTable();