  #include "y.tab.h"
  #include "containers.h"
  #include "symbol.h"
  #include "bblock.h"
  #include "comp.h"
  #include "ssa.h"
  #include "optim.h"

  char* s_argument[6] = {"%rsi", "%rdi", "%rdx", "%rcx", "%r8", "%r9"};
  char* base = "  .section  .rodata\nmsg0:\n  .string \"%d\"\nmsg1:\n  .string \"%d\\n\"\n  .text\n  .globl  main\n  .type   main, @function\n\nget_int:\n.bbg:\n  pushq %rbp\n  movq  %rsp, %rbp\n  subq  $16, %rsp\n  leaq  -8(%rbp), %rax\n  movq  %rax, %rsi\n  movq  $msg0, %rdi\n  movq  $0, %rax\n  call scanf\n  movq  -8(%rbp), %rax\n  leave\n  ret\n\n\nprint:\n.bbp:\n  pushq %rbp\n  movq $msg1, %rdi\n  call printf\n  popq %rbp\n  ret";

  char* startFunction = "  pushq %rbp\n  movq  %rsp, %rbp\n  subq  $64, %rsp";

  FILE *fp;
  int validateCFGforAssembly(struct FunctionBlock* root){
      clearAvailableAsgn(root);
      struct basic_block* block = root->firstBlock;
      while(block!=NULL){
        struct statment_s* statment = block->firstStmt;
        while(statment!=NULL){
          if (statment->LHS->type==ST_REG){
            lookupInsertAsgn(root, statment->LHS);
          }
         statment = statment->next;
        }

        block = block->next;
      }
      struct available_regs* tmp= root->firstAvailableReg;
      int count = 0;
      while(tmp!=NULL){
        count++;
        if (count>7){
          printf("%s function is too large try to make it more modular\n",root->title );
          return 1;
        }
        tmp = tmp->next;
      }
       struct symbol* fun = lookup(root->title , root->firstBlock->index);
       count=0;
      if (fun!=NULL && fun->firstArg!=NULL){ 
        struct argument* args = fun->firstArg;
        while(args!=NULL){
          count++;
          if (count>6){
            printf("%s function arguments are more than 6 try to decrease that\n",root->title );
            return 1;
          }
          args = args->next;
        }
      }
      
      return 0;
  }
  void printRegisterPhisicalNum(struct FunctionBlock* root, struct statment_value* value){
      if(value->type==ST_PARAM){
         fprintf(fp," %s",  s_argument[value->RGnum-1]);
          return;
      }else if (value->type==ST_REG){   
        struct available_regs* tmp= root->firstAvailableReg;
        int i=0;
        while(tmp!=NULL){
          i++;
          if(tmp->asgn->RGnum == value->RGnum){
            fprintf(fp," -%d(%%rbp)", i*8);
              return;
          }
          tmp = tmp->next;
        }
      }else if (value->type==ST_CONST){   
       if (strcmp(value->title, "TRUE")==0){
            fprintf(fp," $1");
          }else if (strcmp(value->title, "FALSE")==0){
            fprintf(fp," $0");
          }else{
            fprintf(fp," $%s", value->title);
          }
      }else if(value->type==ST_RETURN){
        fprintf(fp," %%rax");        
      }
  }

 void print_from_mem_to_reg(struct FunctionBlock* root,  struct statment_value* value, char* regName){
      printRegisterPhisicalNum(root, value);
      fprintf(fp,", %%%s \n", regName);
  }
 void print_from_reg_to_mem(struct FunctionBlock* root,  struct statment_value* value, char* regName){
  		fprintf(fp," %%%s,", regName);
      printRegisterPhisicalNum(root, value);
      fprintf(fp,"\n");
  }

void print_move(struct FunctionBlock* root, struct statment_value* LHS, struct statment_value* RHS ){

    if (RHS==NULL ||LHS==NULL) return;

    if (LHS->type == ST_REG && RHS->type == ST_REG){
  		fprintf(fp,"  movq");
    	print_from_mem_to_reg(root,RHS, "rbx");
  		fprintf(fp,"  movq");
    	print_from_reg_to_mem(root,LHS, "rbx");
    }else{
       fprintf(fp,"  movq");
        printRegisterPhisicalNum(root, RHS);
      fprintf(fp,",");
        printRegisterPhisicalNum(root, LHS);
        fprintf(fp,"\n");
    }

}

void print_three_lines_operation(struct FunctionBlock* root,  struct statment_value* LHS, struct statment_value* RHS , char* operator){
       		fprintf(fp,"  movq");
    		print_from_mem_to_reg(root, RHS->op_values_next, "rbx");
       		fprintf(fp,"  %s", operator);
    		print_from_mem_to_reg(root, RHS->op_values_next->op_values_next, "rbx");
       		fprintf(fp,"  movq");
    		print_from_reg_to_mem(root, LHS, "rbx");
}

 void print_operation(struct FunctionBlock* root, struct statment_value* LHS, struct statment_value* RHS ){

    if (RHS==NULL ||LHS==NULL||RHS->type != ST_OP) return;
    if (is_term(RHS->token)){
    	if (RHS->token==PLUS||RHS->token==MINUS||RHS->token==MULT){
    		if (RHS->token==PLUS){
       			print_three_lines_operation(root, LHS,RHS,"addq");
	       	}else if(RHS->token==MINUS){
       			print_three_lines_operation(root, LHS,RHS,"subq");
	       	}else if(RHS->token==MULT){
       			print_three_lines_operation(root, LHS,RHS,"imulq");
	       	}
    	}else if (RHS->token==DIV||RHS->token==MOD){
    		fprintf(fp, "  movq %%rdx, %%r10\n");
    		fprintf(fp, "  movq");
    		print_from_mem_to_reg(root, RHS->op_values_next, "rax");
    		fprintf(fp, "  movq $0, %%rdx\n");
    		fprintf(fp, "  divq");
           	printRegisterPhisicalNum(root, RHS->op_values_next->op_values_next);
       		fprintf(fp,"\n  movq");
       		if (RHS->token==DIV){
       			print_from_reg_to_mem(root, LHS,"rax");
       		}else{
       			print_from_reg_to_mem(root, LHS,"rdx");
       		}
    		fprintf(fp, "  movq %%r10, %%rdx\n");
    	}
     	
    }else if (is_fla(RHS->token)){
		if (RHS->token==AND||RHS->token==OR){
    		if (RHS->token==AND){
       			print_three_lines_operation(root, LHS,RHS,"andq");
	       	}else if(RHS->token==OR){
       			print_three_lines_operation(root, LHS,RHS,"orq");
	       	}
		}else if(RHS->token==EQUAL||RHS->token==LESS_THAN||RHS->token==LESS_EQUAL||RHS->token==GREATER_THAN||RHS->token==GREATER_EQUAL){
			fprintf(fp,"  movq");
    		print_from_mem_to_reg(root, RHS->op_values_next, "rbx");
       		fprintf(fp,"  cmpq");
    		print_from_mem_to_reg(root, RHS->op_values_next->op_values_next, "rbx");
    		char* comp_op;
    		if (RHS->token==EQUAL){
    			comp_op = "sete";
    		}else if (RHS->token==LESS_THAN){
    			comp_op = "setl";
    		}else if (RHS->token==LESS_EQUAL){
    			comp_op = "setle";
    		}else if (RHS->token==GREATER_THAN){
    			comp_op = "setg";
    		}else if (RHS->token==GREATER_EQUAL){
    			comp_op = "setge";
    		}
       		fprintf(fp,"  %s %%al\n  movzbl %%al, %%rbx", comp_op);
       		fprintf(fp,"\n  movq");
    		print_from_reg_to_mem(root, LHS, "rbx");
		}
    }

}
  void print_statment(struct FunctionBlock* root, struct statment_s* statment, int type){
    if (statment==NULL) return;
    if (type==ST_OP){
      struct statment_value* value = statment->RHS->op_values_next;
      if (statment->RHS->isNot){
      	fprintf(fp,"  movq");
    		print_from_mem_to_reg(root, statment->RHS->op_values_next, "rbx");
       		fprintf(fp,"  notq %%rbx\n  andq $1, %%rbx\n  movq");
    		print_from_reg_to_mem(root, statment->LHS, "rbx");
      }else{
         print_operation(root,statment->LHS, statment->RHS );
      }
    }else{
      print_move(root, statment->LHS, statment->RHS);
    }
  }
  void print_block_Statments(struct FunctionBlock* root, struct basic_block* block){
    if (block==NULL) return;
    struct statment_s* statment = block->firstStmt;
    while(statment!=NULL){
      if (statment->LHS->type==ST_EXIT){
        fprintf(fp, "  leave\n  ret\n");
      }else if (statment->LHS->type==ST_CALL){
       if (strcmp(statment->LHS->title, "get-bool")==0||strcmp(statment->LHS->title, "get-int")==0){
        	fprintf(fp,"  call get_int\n");
      	}else{
        	fprintf(fp,"  call %s\n", statment->LHS->title);
    	}
      }else if (statment->LHS->type==ST_RETURN|| statment->LHS->type==ST_PARAM){
        print_move(root, statment->LHS, statment->RHS);
      }else if (statment->RHS!=NULL){
        print_statment(root, statment, statment->RHS->type);
      }
     statment = statment->next;
    }
  }

void print_branching(struct FunctionBlock* root,struct basic_block* block){
  if (block->cond!=NULL){ 
        fprintf(fp, "  cmp $0,");
          printRegisterPhisicalNum(root, block->cond);
          fprintf(fp, "\n  je .bb%d\n  jmp .bb%d\n" , block->secondBB->bbNum, block->firstBB->bbNum);
  }else if(block->closureBB!=NULL){
      fprintf(fp,"  jmp .bb%d \n", block->closureBB->bbNum);
  }
}

int printAssembly(struct FunctionBlock* root) {
  if (root->type== B_FUNCTION){
      fprintf(fp,"\n\n%s:",root->title );
  }else if (root->type== B_EVAL||root->type== B_PRINT){
      fprintf(fp,"\n\nmain:");
  }
  struct basic_block* block = root->firstBlock;
  while(block!=NULL){
      if (block->type==ENTRY){
        if (root->type== B_FUNCTION){
          fprintf(fp,"\n.bb%s:\n",root->title);
        }else if (root->type== B_EVAL||root->type== B_PRINT){
          fprintf(fp,"\n.bb0:\n");
        }
      fprintf(fp, "%s\n", startFunction);
      }else{
        if (block->bbNum>0){
            fprintf(fp,"\n.bb%d: \n", block->bbNum);
          }
    }
    print_block_Statments(root, block);
    print_branching(root,block);
    block = block->next;

  }
  return 0;
}

  void createAssemblyFile(){
     fp = fopen("result.s", "w");
     fprintf(fp, "%s\n", base);
  }

  void finishAssembly(){
      fclose(fp);
  }