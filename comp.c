#include "y.tab.h" 
#include "containers.h"
#include "bblock.h"
#include "symbol.h"
#include "comp.h"
#include "cfg2.h"
#include "optim.h"
#include "assembly.h"
int yyparse();
int checkBody(struct ast* body);
int check_program(struct ast* tmp);
int checkResultType(struct ast* body);
int checkArguments(struct symbol* fun,struct ast* body);
int checkIdentifier(struct ast* body, int need_term, int need_fla);
int chekLetIdentifier(struct ast* body);
int chekIFIdentifier(struct ast* body);

int is_term(int token){
	if (token == NUMBER ||token == NEGATIVE ||token == PLUS ||token == MINUS ||token == DIV  
		||token == MOD ||token == MULT ) return 1;
	return 0;
}
int is_fla(int token){
	if (token == TRUE ||token == FALSE ||token == EQUAL ||token == LESS_THAN ||token == GREATER_THAN
		||token == GREATER_EQUAL ||token == LESS_EQUAL ||token == AND ||token == OR||token == NOT) return 1;
	return 0;
}
int isFunction(int token){
	if (token == INT_FUNCTION ||token == BOOL_FUNCTION  ) return 1;
	return 0;
}
int isParameter(int token){

	if (token == INT_PARAMETER ||token == BOOL_PARAMETER  ) return 1;
	return 0;
}
int getReturnType(int token){
	if (token == INT_FUNCTION || token == INT_PARAMETER) return INT;
	if (token == BOOL_FUNCTION || token == BOOL_PARAMETER) return BOOL;
	return 0;
}

int checkResultType(struct ast* body){
	if (body->ntoken == IF){
		struct ast* tmp = body->child->next->id;
		return checkResultType(tmp);
	}else if (body->ntoken == CALL || body->ntoken == VARIABLE){
		struct symbol* var = lookup(body->token, body->id);
		if (var!=NULL) return var->returnType;
		else return 0 ;
	}else if(body->ntoken == LET){
		struct ast* tmp = body->child->next->next->id;
		return checkResultType(tmp);
	}else{
		if (is_fla(body->ntoken)){
			return BOOL;
		}else if (is_term(body->ntoken)){
			return INT;
		}
	}
		return 0 ;
}

int check_program(struct ast* tmp){
	if (isFunction(tmp->ntoken)){

		struct ast_child* t = tmp->child;
		struct ast* ident = t->id;
		//Each function name can be used in exactly one define-fun per input file

		struct symbol* var = lookup(ident->token, ident->id);
		if(var!=NULL){
			printf("problem in line : %d \n\"%s\" the same identifier is used before \n", ident->line_number, ident->token);
			return 1;
		}else{
			lookupInsert(ident->token,CALL, getReturnType(tmp->ntoken), ident->id, -1, ident->id);
		}
		//add the arguments 
		t = t->next;
		while(t!=NULL && isParameter(t->id->ntoken)){
			struct ast* arg = t->id;
			struct symbol* old = lookup(arg->token, arg->id);
			if(old!=NULL){
				printf("problem in line : %d \n\"%s\" the same var identifier is used before\n", arg->line_number, arg->token);
				return 1;
			}else{
				insertArgument(arg->token, VARIABLE, getReturnType(arg->ntoken), arg->id,  tmp->id, ident->token, arg->id);
			}
			t = t->next;
		}
		//check return body matches the defined return type 
		struct ast* body = t->id;
		int bodyReturnType = checkResultType(body);
		int functionReturnType = getReturnType(tmp->ntoken);
		if ( functionReturnType ==INT && bodyReturnType==BOOL){
			printf("problem in line : %d \n the function \"%s\" return type \"int\" doesn't match the body return type\n", ident->line_number, ident->token);
			return 1;	
		}else if (functionReturnType ==BOOL && bodyReturnType==INT){
			printf("problem in line : %d \n the function \"%s\" return type \"bool\" doesn't match the body return type\n", ident->line_number, ident->token);
			return 1;	
		}
		if(checkBody(body)) return 1;
		 return 0;
	} else if (tmp->ntoken==EVAL||tmp->ntoken==PRINT){
		struct ast_child* t = tmp->child;
		struct ast* body = t->id;
		return checkBody(body);	
	}
	return 0;
}



int checkBody(struct ast* body){
	int cur_token = body->ntoken;
	int need_fla = cur_token == AND ||cur_token == OR||cur_token == NOT;
	int need_term  = cur_token == NUMBER ||cur_token == NEGATIVE ||cur_token == PLUS ||cur_token == MINUS ||cur_token == DIV  
		||cur_token == MOD ||cur_token == MULT || cur_token == EQUAL ||cur_token == LESS_THAN ||cur_token == GREATER_THAN
		||cur_token == GREATER_EQUAL ||cur_token == LESS_EQUAL;

	if (body->ntoken == CALL || body->ntoken == VARIABLE){if(checkIdentifier(body, need_term, need_fla)) return 1;}
	else if (body->ntoken == LET){ if (chekLetIdentifier(body)) return 1;}
	else if (body->ntoken == IF){if(chekIFIdentifier(body)) return 1;}
	else{
		struct ast_child* t = body->child;
		while(t!=NULL){
			struct ast* ch = t->id;
			int childReturnType = checkResultType(ch);
			if(need_term && childReturnType==BOOL){
				printf("problem in line : %d \n\"%s\" should only have terms \n", ch->line_number, body->token);
				return 1;
			}else if(need_fla && childReturnType==INT){
				printf("problem in line : %d \n\"%s\" should only have booleans \n", ch->line_number, body->token);
				return 1;
			}
			if (ch->ntoken == CALL || ch->ntoken == VARIABLE){if(checkIdentifier(ch, need_term, need_fla)) return 1;}
			else if (ch->ntoken == LET){if (chekLetIdentifier(ch)) return 1; }
			else if (ch->ntoken == IF){if(chekIFIdentifier(ch)) return 1;}
			else if (ch->child!=NULL)
			{if(checkBody(ch)) return 1;}

				t = t->next;
		}
	}
	
	return 0;
}

int checkIdentifier(struct ast* body, int need_term, int need_fla){
	if (body->ntoken == CALL || body->ntoken == VARIABLE){
		struct symbol* var = lookup(body->token, body->id);
		if (var==NULL || var->type!=body->ntoken){
			printf("problem in line : %d \n\"%s\" identifier should be defined before calling it.\n ", body->line_number, body->token); return 1;
		}
		if ((var->returnType == INT && need_fla)||(var->returnType == BOOL && need_term)){
			printf("problem in line : %d \n\"%s\" type doesn't match", body->line_number, body->token);
			return 1;
		}
		if (body->ntoken == CALL)
		{
			if(checkArguments(var, body)) return 1;
		}
	}
	return 0;
}

int chekLetIdentifier(struct ast* body){

	if (body->ntoken == LET){
		struct ast_child* t = body->child;
		struct ast* ident = t->id;
		t = t->next;
		struct ast* exp = t->id;
		t = t->next;
		struct ast* expBody = t->id;
		struct symbol* var = lookup(ident->token, ident->id);
		if(var!=NULL){
			printf("problem in line : %d \n\"%s\" The same identifier is used before\n", ident->line_number, ident->token);
			return 1;
		}else{
		int expReturnType = checkResultType(exp);
			if (expReturnType==BOOL){
				lookupInsert(ident->token,VARIABLE, BOOL,ident->id, body->id, ident->id);
			}else if (expReturnType == INT){
				lookupInsert(ident->token,VARIABLE, INT, ident->id, body->id, ident->id);
			}
			if(checkBody(exp)) return 1;
		}
		if(checkBody(expBody)) return 1;
	}
	return 0;
}
int chekIFIdentifier(struct ast* body){

	if (body->ntoken == IF){
		struct ast_child* t = body->child;
		struct ast* cond = t->id;
		t = t->next;
		struct ast* succesEXP = t->id;
		t = t->next;
		struct ast* failEXP = t->id;
		int successExpReturnType = checkResultType(succesEXP);
		int failExpReturnType = checkResultType(failEXP);
		if(checkResultType(cond)==INT){
			printf("problem in line : %d \nThe if condition should be boolean check not term\n",cond->line_number);
			return 1;
		}
		if (successExpReturnType!=failExpReturnType)
		{
			printf("problem in line : %d \nThe if condition then and else should have identical types\n",succesEXP->line_number);
			return 1;
		}
		if(checkBody(cond)) return 1;
		if(checkBody(succesEXP)) return 1;
		if(checkBody(failEXP)) return 1;
	}
	return 0;
}
int checkArguments(struct symbol* fun,struct ast* body){
	struct ast_child* t = body->child;
	struct argument* args = fun->firstArg;
	while(args!=NULL && t!=NULL){
		struct ast* ch = t->id;
		int childReturnType = checkResultType(ch);
		if((args->returnType==INT &&childReturnType==BOOL)||(args->returnType==BOOL && childReturnType==INT)){
			printf("problem in line : %d \nWrong parameter type, while calling \"%s\" function the \"%s\" parameter type doesn't match \n", ch-> line_number, fun->name,args->name);
			return 1;
		}

		if (ch->ntoken == CALL || ch->ntoken == VARIABLE){if(checkIdentifier(ch, args->returnType==INT, args->returnType==BOOL)) return 1;}
		else if (ch->ntoken == LET){if (chekLetIdentifier(ch)) return 1;}
		else if (ch->ntoken == IF){if(chekIFIdentifier(ch)) return 1;}
		else if(ch->child!=NULL){if(checkBody(ch)) {return 1;}}
		args = args->next;
		t = t->next;
	}
	if((args!=NULL && t==NULL)||(args==NULL && t!=NULL)){
		printf("problem in line : %d \n\"%s\" parameters count does not match\n", body->line_number , fun->name);
		return 1;
	}
	return 0;
}

int main(int argc, char *argv[]) { 
   int i=0;
   

   int ret = yyparse();
   if (ret ==0){
   lookupInsert("get-int",CALL, INT, 0, -1 , 0);
   lookupInsert("get-bool",CALL, BOOL, 0,  -1, 0);
   }
   if(ret ==0) {ret = visit_ast(check_program);}
   if(ret ==0) {print_ast();}
   if (ret == 0){
	   visit_ast(create_blocks);
	   initCFG();
	   visit_ast(create_cfg);
	   int i ;
        for (i= 0; i < argc; i++) {
			if(strcmp(argv[i],"--cass")==0){
				combineRedundant();
			}else if(strcmp(argv[i],"--cp")==0){
				constantPropagation();
			}else if(strcmp(argv[i],"--cse")==0){
				commonSubexpression();
			}else if(strcmp(argv[i],"--simpcfg")==0){
				simplifyCFG();
			}
        }
        AllocateRegisters();
        int valid = visit_cfg(validateCFGforAssembly);
        if (valid==0){
			createAssemblyFile();
			visit_cfg(printAssembly);
			finishAssembly();
			printCFG();
			printCFGfile();
        }
   }
 //   if(ret ==0) {
 //   		initSmtCreator();
	//    	visit_cfg(check_const_creation);
	//    	printSMT();
	// }
   clearSymbolTable();
   free_ast();
   free_cfg();
   return ret;
} 
