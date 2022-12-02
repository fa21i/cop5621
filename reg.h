struct range{
    int start;
    int end;
};
bool register_allocation (struct cfg* t);
void print_reg_smt();
int* traverse_reg_txt();
extern FILE *fp;