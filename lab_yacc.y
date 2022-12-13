%{
  #include "containers.h"
  #include "lab_lex.h"
  #include "yacc.h"
  struct argument{
  	int ast_node;
  	struct argument* next;
  }argument;
  struct list{
  	struct argument* first;
  	struct argument* last;
  }list;
  void insertchildrenInOrder(struct list* arg);
// yylval.num = strtoimax(yytext,&endptr2,10);}
%} 

%token VARIABLE CALL INT_FUNCTION BOOL_FUNCTION INT_PARAMETER BOOL_PARAMETER FUNID NUMBER NEGATIVE OPEN_BRACKET CLOSE_BRACKET FUNCTION_DEF GET_INT GET_BOOL EVAL PRINT LET IF NOT INT BOOL TRUE FALSE PLUS MINUS DIV MOD MULT IDENTIFIER NOT_DEFINED EQUAL LESS_THAN GREATER_THAN GREATER_EQUAL LESS_EQUAL AND OR 

%union {int num; char* str; struct list* arg;} 
%start PROGRAM

%type<num>  INT BOOL funid EXP EVAL TYPE var
%type<str>  IDENTIFIER NUMBER NEGATIVE
%type<arg> PARAMETER ALLOWED_MULTI_TERM

/* Rule Section */
%% 
 PROGRAM: function PROGRAM
 	|OPEN_BRACKET PRINT EXP CLOSE_BRACKET {insert_child($3);  insert_node("PRINT", PRINT, lineNumber);}
	| OPEN_BRACKET EVAL EXP CLOSE_BRACKET{insert_child($3); insert_node("EVAL",EVAL, lineNumber); };


function: OPEN_BRACKET FUNCTION_DEF OPEN_BRACKET funid PARAMETER CLOSE_BRACKET TYPE EXP CLOSE_BRACKET  {
  insert_child($4); 
  insertchildrenInOrder($5); 
  insert_child($8); 
  if ($7==INT){
  	insert_node("FUNCTION_DEF",  INT_FUNCTION, lineNumber);
  } else if ($7==BOOL) {
  	insert_node("FUNCTION_DEF",  BOOL_FUNCTION, lineNumber);
  }
}
PARAMETER:	{$$ = NULL;}
	| PARAMETER OPEN_BRACKET IDENTIFIER TYPE CLOSE_BRACKET{
		if($$==NULL){
			$$ =  calloc(1,sizeof(struct list));
			$$->first = $$->last = calloc(1,sizeof(struct argument));
		}else{
			$$->last->next= calloc(1,sizeof(struct argument));
			$$->last= $$->last->next;
		}
		if ($4==INT){
			$$->last->ast_node = insert_node($3, INT_PARAMETER, lineNumber);
		}else if( $4==BOOL ){
			$$->last->ast_node = insert_node($3, BOOL_PARAMETER, lineNumber);
		}
	}
;
TYPE: 	
      INT { $$ = INT;}
      |BOOL {$$ = BOOL;}
;
EXP: OPEN_BRACKET PLUS EXP EXP ALLOWED_MULTI_TERM CLOSE_BRACKET {insert_children(2,$3,$4); insertchildrenInOrder($5); $$ =  insert_node("PLUS",PLUS, lineNumber);}
 	| OPEN_BRACKET MULT EXP EXP ALLOWED_MULTI_TERM CLOSE_BRACKET {insert_children(2,$3,$4); insertchildrenInOrder($5); $$ = insert_node("MULT",MULT, lineNumber);}

 	| OPEN_BRACKET MINUS EXP EXP CLOSE_BRACKET {insert_children(2,$3,$4); $$ = insert_node("MINUS",MINUS, lineNumber);}
	| OPEN_BRACKET DIV EXP EXP CLOSE_BRACKET {insert_children(2,$3,$4); $$ = insert_node("DIV",DIV, lineNumber);}
 	| OPEN_BRACKET MOD EXP EXP CLOSE_BRACKET {insert_children(2,$3,$4); $$ = insert_node("MOD",MOD, lineNumber);}
	| OPEN_BRACKET GET_INT CLOSE_BRACKET {$$ = insert_node("get-int",CALL, lineNumber);}
	| NUMBER { $$ = insert_node($1,NUMBER, lineNumber);} 
    	| NEGATIVE {$$ = insert_node($1, NEGATIVE, lineNumber);}
	
    | OPEN_BRACKET EQUAL EXP EXP CLOSE_BRACKET {insert_children(2,$3,$4); $$ = insert_node("EQUAL",EQUAL, lineNumber);}
	| OPEN_BRACKET LESS_THAN EXP EXP CLOSE_BRACKET {insert_children(2,$3,$4); $$ = insert_node("LESS_THAN",LESS_THAN, lineNumber);}
	| OPEN_BRACKET GREATER_THAN EXP EXP  CLOSE_BRACKET {insert_children(2,$3,$4); $$ = insert_node("GREATER_THAN", GREATER_THAN, lineNumber);}
	| OPEN_BRACKET GREATER_EQUAL EXP EXP  CLOSE_BRACKET {insert_children(2,$3,$4); $$ = insert_node("GREATER_EQUAL", GREATER_EQUAL, lineNumber);}
	| OPEN_BRACKET LESS_EQUAL EXP EXP  CLOSE_BRACKET {insert_children(2,$3,$4); $$ = insert_node("LESS_EQUAL",LESS_EQUAL, lineNumber);}

	| OPEN_BRACKET AND EXP EXP ALLOWED_MULTI_TERM  CLOSE_BRACKET {insert_children(2,$3,$4); insertchildrenInOrder($5); $$ = insert_node("AND",AND, lineNumber);}
	| OPEN_BRACKET OR EXP EXP ALLOWED_MULTI_TERM  CLOSE_BRACKET {insert_children(2,$3,$4); insertchildrenInOrder($5); $$ = insert_node("OR",OR, lineNumber);}
	| OPEN_BRACKET NOT EXP CLOSE_BRACKET {insert_child($3); $$ = insert_node("NOT",NOT, lineNumber);}
	| OPEN_BRACKET GET_BOOL CLOSE_BRACKET {$$ = insert_node("get-bool",CALL, lineNumber);}

	| OPEN_BRACKET IF EXP EXP EXP CLOSE_BRACKET {insert_children(3,$3,$4,$5); $$ = insert_node("IF",IF, lineNumber);}
	| OPEN_BRACKET IDENTIFIER ALLOWED_MULTI_TERM CLOSE_BRACKET {insertchildrenInOrder($3); $$ = insert_node($2,CALL, lineNumber); }
	| OPEN_BRACKET LET OPEN_BRACKET var EXP CLOSE_BRACKET EXP CLOSE_BRACKET {insert_children(3,$4,$5,$7 );$$= insert_node("LET",LET, lineNumber);}
	| TRUE {$$ = insert_node("TRUE",TRUE, lineNumber);}
	| FALSE {$$ = insert_node("FALSE",FALSE, lineNumber);}
    | var
 
; 
var: IDENTIFIER {$$ = insert_node($1, VARIABLE, lineNumber);};

funid: IDENTIFIER {$$ = insert_node($1, FUNID, lineNumber);};

ALLOWED_MULTI_TERM:  {$$ = NULL;}
	|ALLOWED_MULTI_TERM EXP{
		if($$==NULL){
			$$ =  calloc(1,sizeof(struct list));
			$$->first = $$->last = calloc(1,sizeof(struct argument));
		}else{
			$$->last->next= calloc(1,sizeof(struct argument));
			$$->last= $$->last->next;
		}
		$$->last->ast_node = $2;
	}
	;
%% 
void insertchildrenInOrder(struct list* arg){
	while(arg!=NULL && arg->first!=NULL){
  		insert_child(arg->first->ast_node);
  		struct argument* temp = arg->first;
  		arg->first = arg->first->next;
  		free(temp);
	}
	if (arg!=NULL)
		 free(arg);
}

void yyerror(char* s) 
{  printf("%s in line : %d\n", s ,lineNumber);}



