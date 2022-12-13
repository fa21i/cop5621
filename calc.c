#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "y.tab.h" 
#include "yacc.h"
#include "containers.h"
#include "symbol.h"
#include "bblock.h"
#include "comp.h"
#include "ssa.h"
#include "optim.h"
#include "calc.h"

long to_int(char *str)
{
  long result;
  long puiss;

  result = 0;
  puiss = 1;
  if (*str == '-'){
        puiss = puiss * -1;
      str++;
  }
  while ((*str >= '0') && (*str <= '9'))
  {
      result = (result * 10) + ((*str) - '0');
      str++;
  }
  return (result * puiss);
}

long calculateNum(struct statment_value* first, struct statment_value* second, int operator) {
	long num1 = to_int(first->title);
	// printf("num1 %ld\n",num1 );
	long num2= to_int(second->title);
	// printf("num2 %ld\n",num2 );
	
	if (operator == PLUS){
		return (num1 + num2);
	}else if (operator == MINUS){
		return (num1 - num2);
	}else if (operator == DIV){
                return (num1 / num2);
        }else if (operator == MOD){
                return (num1 % num2);
        }else if (operator == MULT){
                return (num1 * num2);
        }
	return 0;

}

int calculateFla(struct statment_value* first, struct statment_value* second, int operator) {
		long num1;
		if (strcmp(first->title, "TRUE")==0){
            num1 = 1;
        }else if (strcmp(first->title, "FALSE")==0){
          	num1 = 0;
          }else{
          	num1 = to_int(first->title);
          }

	long num2;
          if (second!=NULL)
          {
          	if (strcmp(second->title, "TRUE")==0){
            num2 = 1;
        }else if (strcmp(second->title, "FALSE")==0){
          	num2 = 0;
          }else{
          	
          	num2 = to_int(second->title);
          }
          }
	
        if (operator == EQUAL){
		return (num1 == num2);	
        }else if (operator == LESS_THAN){
                return (num1 < num2);
        }else if (operator == GREATER_THAN){
                return (num1 > num2);
        }else if (operator == LESS_EQUAL ){
                return (num1 <= num2);
        }else if (operator == GREATER_EQUAL){
                return (num1 >= num2);
        }else if (operator == AND) {
		return (num1 && num2);
	}else if (operator == OR) {
		return (num1 || num2);
	}else if (operator == NOT) {
		if (num1 == 1){
			return 0;
		}else if (num1 ==0) {
			return 1;
		}
	}



	return -1;

}







