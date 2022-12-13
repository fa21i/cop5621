%{
#include <stdio.h>
#include <string.h>
#include "y.tab.h" 
#include "lab_lex.h"
int lineNumber = 1;
%}

%option caseless
%%
[\t\r" "]* {}  //SPACE
";".*\n {lineNumber++;}    //comment
[\n] {lineNumber ++;} //new line 
"(" { return OPEN_BRACKET;} 
")" { return CLOSE_BRACKET;} 
"define-fun" {return FUNCTION_DEF;} 
"get-int" {return GET_INT;}
"get-bool" {return GET_BOOL;}
"eval" {return EVAL;}
"print" {return PRINT;}
"let" {return LET;}
"if" {return IF;}
"not" {return NOT;}
"int" {return INT;}
"bool" {return BOOL;}
"true" {return TRUE;}
"false" {return FALSE;}
"-" {return MINUS;}
"+" {return PLUS;}
"*" {return MULT;} 
"div" {return DIV;}
"mod" {return MOD;}
"=" {return EQUAL;}
"<" {return LESS_THAN;}
">" {return GREATER_THAN;}
">=" {return GREATER_EQUAL;}
"<=" {return LESS_EQUAL;}
"and" {return AND;}
"or" {return OR;}
[-][0-9]+ {yylval.str = strdup(yytext);return NEGATIVE; }
[0-9]+ {yylval.str = strdup(yytext); return NUMBER;}
[a-zA-Z][a-zA-Z0-9]* {setStringToSmallCase(yytext); yylval.str = strdup(yytext); return IDENTIFIER;}
[0-9.]+ {return NOT_DEFINED;} 
[-a-zA-Z0-9]+ {return NOT_DEFINED;} 
. {return NOT_DEFINED;} 
%%

int yywrap() 
{ 

 return 1; 
} 

//case insensitive
void setStringToSmallCase(char* str){
	int len = strlen(str);
	int i;
        for(i = 0; i < len; i++){
          if(str[i] >='A'&& str[i] <= 'Z'){
             str[i] = (char)(str[i]-'A' + 'a');
          }
        }
}
