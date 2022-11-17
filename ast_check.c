#include<string.h>
#include "y.tab.h"
#include "ast.h"
#include "ast_check.h"


int print_array(int a[], int type_c){
   for (int i = 0; i < type_c; i++)
   {
      printf("index: %d, ",a[i]);
   }  
   printf("\n");
   return 0;
}
int print_char_array(char* a[], int arg_c){
   
   for (int i = 0; i < arg_c; i++)
   {
      printf("index: %s, ",a[i]);
   }  
   printf("\n");
   return 0;
}
int print_scope(int a[][2], int index, int arg_c){
   for (int i = 0; i < arg_c; i++)
   {
      printf("scope %d: %d, ",index,a[i][index]);
   }  
   printf("\n");
   return 0;
}
int print_tokens(struct token tokens[]){
   for (int i = 1; tokens[i].name!=NULL||tokens[i].type!=0 ; i++){
      printf("id: %d,name: %s,type : %d\n",i,tokens[i].name,tokens[i].type);
   }  
   return 0;
}
int get_count(char *a[],char *str, int arg_c){
   int count=0;
   for(int i=0;i<arg_c;i++){
      if(strcmp(a[i],str)==0){
         count++;
      }
   }
   return count;
}
int* get_all_index(char* a[], char* str, int arg_c){
   static int b[10];
   int c=0;
   for(int i=0;i<arg_c;i++){
      if(strcmp(a[i],str)==0){
         b[c++] = i;
      }
   }
   return b;
}
int return_index(char* a[], char* str, int arg_c){
   for(int i=0;i<arg_c;i++){
      if(strcmp(a[i],str)==0){
         return i;
      }
   }
   return -1;
}
int scope_index(int num, int arg_c, int** scope){
   for(int i=0;i<arg_c;i++){
      printf("\\\\%d scope_index: %d\n",num,scope[i][0]);
      if(scope[i][0]==num){
         return i;
      }
   }
   return -1;
}
int count_vars(char* fun_name, int arg_c, char** fun_scope){
   int c=0;
   for(int i=0;i<arg_c;i++){
      if(strcmp(fun_name,fun_scope[i])==0){
         c++;
      }
   }
   return c;
}
