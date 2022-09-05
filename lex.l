%{
int count = 1;
%}

%%
print 	{printf("line %d: program entry:\t\t%s\n", count,yytext);}
[)]   	{printf("line %d: right paranthesis:\t%s\n", count,yytext);}
[(]   	{printf("line %d: left paranthesis:\t%s\n", count,yytext);}
[a-z]+ 	{printf("line %d: variable/function:\t%s\n", count,yytext);}
[0-9]+ 	{printf("line %d: integer constant:\t%s\n", count,yytext);}
[+] 	{printf("line %d: arithmetic constant:  %s\n", count,yytext);}
[<|>|<=|>=|=] {printf("line %d: arithmetic comparison:\t\t%s\n", count,yytext);}
\n    	{count++;}
%%

int yywrap(){}
int main(){

// Explanation:
// yywrap() - wraps the above rule section
/* yyin - takes the file pointer which contains the input*/
/* yylex() - this is the main flex function which runs the Rule Section*/
// yytext is the text in the buffer

FILE *fp;
char filename[50];
//printf("Enter the filename: \n");
//scanf("%s",filename);
fp = fopen("D:\\Education\\fsu mscs\\Compiler construction\\Assign 2 scanner\\sample.txt","r");
yyin = fp;

yylex();
printf("\nNumber of lines - %d\n", count);

return 0;
}
