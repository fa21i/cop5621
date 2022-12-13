extern int is_term(int token);
extern int is_fla(int token);
extern int isParameter(int token);
extern int isFunction(int token);
extern int getReturnType(int token);
extern int checkResultType(struct ast *body);