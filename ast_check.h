

struct args {
   char* name;
   int type;
   int s1;
   int s2;
   char* root_fun;
   int id; 
};

struct token{
   char* name;
   int type;
};

int print_array(int a[], int type_c);
int print_char_array(char* a[], int arg_c);
int print_scope(int a[][2], int index, int arg_c);
int print_tokens(struct token tokens[]);
int get_count(char *a[],char *str, int arg_c);
int* get_all_index(char* a[], char* str, int arg_c);
int return_index(char* a[], char* str, int arg_c);
int scope_index(int num, int arg_c, int** scope);
int count_vars(char* fun_name, int arg_c, char** fun_scope);