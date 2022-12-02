struct range{
    int bb;
    struct range* next;
};
bool register_allocation (struct cfg* t);
void print_reg_smt();
int* traverse_reg_txt();
extern FILE *fp;