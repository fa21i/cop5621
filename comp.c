#include "y.tab.h"
#include "ast.h"
#include "cfg.h"
#define NOVALUE -9999
int yyparse();


// symbol table
struct node_fun_str* fun_r = NULL;
struct node_fun_str* fun_t = NULL;
struct node_var_str* vars_r = NULL;
struct node_var_str* vars_t = NULL;

// cfg structure
struct cfg* cfg_r = NULL;
struct cfg* cfg_t = NULL;

int sz;

FILE *fp;

// semantic checks

bool is_term (struct ast* node){
  int t = node->ntoken;
  if (t == CONST || t == PLUS || t == MINUS || t == MULT || t == DIV || t == MOD) return true;
  if (t == VARID) {
    struct node_var_str* tmp = find_var_str(node->id, node->token, vars_r);
    if (tmp != NULL && INTID == tmp->type) return true;
  }
  if (t == CALL) {
    struct node_fun_str* tmp = find_fun_str(node->token, fun_r);
    if (tmp != NULL && INT == tmp->type) return true;
  }
  if (t == IF) return is_term (get_child(node, 2));
  if (t == LET) return is_term (get_child(node, 3));
  return false;
}

bool is_fla (struct ast* node){
  int t = node->ntoken;
  if (t == TRUE || t == FALSE || t == EQ || t == LE || t == LT || t == GT || t == GE || t == NOT || t == AND || t == OR) return true;
  if (t == VARID) {
    struct node_var_str* tmp = find_var_str(node->id, node->token, vars_r);
    if (tmp != NULL && BOOLID == tmp->type) return true;
  }
  if (t == CALL){
    struct node_fun_str* tmp = find_fun_str(node->token, fun_r);
    if (tmp != NULL && BOOL == tmp->type) return true;
  }
  if (t == IF) return is_fla (get_child(node, 2));
  if (t == LET) return is_fla (get_child(node, 3));
  return false;
}

int get_fun_var_types(struct ast* node){
  if (node->ntoken == FUNID){
    char* fun_id = node->token;
    if (find_fun_str(fun_id, fun_r) != NULL){
      printf ("Function %s defined twice\n", fun_id);
      return 1;
    }

    int arg_num = get_child_num(node->parent);
    struct ast* body = get_child(node->parent, arg_num);

    struct node_var_str* args = NULL;
    for (int i = 2; i < arg_num - 1; i++) {
      struct ast* var = get_child(node->parent, i);
      if (find_fun_str(var->token, fun_r) != NULL || strcmp(var->token, fun_id) == 0){
        printf ("Variable and function %s have the same name\n", var->token);
        return -1;
      }
      if (find_var_str(var->id, var->token, vars_r) != NULL){
        printf ("Variable %s declared twice in function %s\n", var->token, fun_id);
        return -1;
      }
      push_var_str(var->id, body->id, var->ntoken, var->token, &vars_r, &vars_t);
      if (args == NULL) args = vars_t;
    }
    if (is_term(body)) {
      push_fun_str(fun_id, INT, arg_num - 3, args, &fun_r, &fun_t);
    }
    else if (is_fla(body)) {
      push_fun_str(fun_id, BOOL, arg_num - 3, args, &fun_r, &fun_t);
    }
    else {
      printf ("Unable to detect the type of function %s\n", fun_id);
      return -1;
    }
  }
  if (node->ntoken == LETID) {
    if (find_var_str(node->id, node->token, vars_r) != NULL){
      printf ("Variable %s declared twice\n", node->token);
      return -1;
    }
    int type;
    if (is_fla(get_child(node->parent, 2))) type = BOOLID;
    else if (is_term(get_child(node->parent, 2))) type = INTID;
    else {
      printf ("Unable to detect the type of variable %s\n", node->token);
      return -1;
    }
    push_var_str(node->id, get_child(node->parent, 3)->id, type, node->token, &vars_r, &vars_t);
  }
  return 0;
}

int type_check(struct ast* node){
  int t = node->ntoken;
  int needs_term = (t == EQ || t == LE || t == LT || t == GT || t == GE || t == PLUS || t == MINUS || t == MULT || t == DIV || t == MOD);
  int needs_fla  = (t == NOT || t == AND || t == OR);
  struct ast_child* temp_child_root = node -> child;
  while(temp_child_root != NULL) {
    struct ast* child_node = temp_child_root->id;
    if ((needs_term && !is_term(child_node)) ||
        (needs_fla && !is_fla(child_node))) {
      printf ("Does not type check: operator %s vs operand %s\n", node->token, child_node->token);
      return 1;
    }
    temp_child_root = temp_child_root -> next;
  }

  if (t == IF) {
    if (!is_fla (get_child(node, 1))) {
      printf ("Does not type check: the if-guard %s is not a formula\n", get_child(node, 1)->token);
      return 1;
    }
    else if ((is_fla (get_child(node, 2)) && !is_fla (get_child(node, 3))) ||
             (is_term (get_child(node, 2)) && !is_term (get_child(node, 3)))) {
      printf ("Does not type check: the then-branch %s and the else-branch %s should have the same type\n", get_child(node, 2)->token, get_child(node, 3)->token);
      return 1;
    }
  }

  if (node->ntoken == DEFFUN) {
    char* fun_id = get_child(node, 1)->token;
    struct node_fun_str* deffun = find_fun_str(fun_id, fun_r);
    if (NULL == deffun){
      printf ("Function %s has not been defined\n", fun_id);
      return 1;
    }

    int num_chl = get_child_num(node);
    if (get_child(node, num_chl - 1)->ntoken != deffun->type) {
      printf ("Declared type of function %s does not match the type of the body\n", fun_id);
      return -1;
    }
  }

  if (node->ntoken == CALL) {
    char* call_id = node->token;
    struct node_fun_str* fun = find_fun_str(call_id, fun_r);
    if (NULL == fun){
      printf ("Function %s has not been defined\n", call_id);
      return 1;
    }
    if (get_child_num(node) != fun->arity){
      printf ("Wrong number of arguments of function %s\n", call_id);
      return 1;
    }

    struct node_var_str* args = fun->args;
    for (int i = 1; i <= fun->arity; i++){
      if ((is_fla (get_child(node, i)) && args->type == INTID) ||
          (is_term (get_child(node, i)) && args->type == BOOLID)){
        printf ("Does not type check: argument %s vs type of %s\n", get_child(node, i)->token, args->name);
        return 1;
      }
      args = args->next;
    }
  }

  return 0;
}

int to_cfg(struct ast* node)
{
  if (node->ntoken == FUNID) {
    struct ast *parent = node->parent;
    int how_many_children = get_child_num(parent);
    int id_fun_body = get_child(parent, how_many_children)->id;
    int interm = node->id;
    for (int i = 2; i < how_many_children - 1; i++)
    {
      push_ncfg(interm, get_child(parent, i)->id, true, true, NULL, NULL, node->token, &cfg_r, &cfg_t);
      interm = get_child(parent, i)->id;
      cfg_t->asgns = mk_uasgn(0, interm, -(i - 1), INP);
    }
    push_ncfg(interm, id_fun_body, true, false, NULL, NULL, node->token, &cfg_r, &cfg_t);
    push_cfg(id_fun_body, node->parent->id, true, true,NULL, NULL, &cfg_r, &cfg_t);
  }
  return 0;
}

bool is_op(int n)
{
  return (n == PLUS || n == MINUS || n == MULT || n == DIV || n == MOD ||
          n == NOT || n == OR || n == AND || n == EQ || n == GT || n == GE || n == LT || n == LE);
}

bool is_const_var(int n)
{
  return (n == CONST || n == TRUE || n == FALSE || n == VARID || n == CALL);
}

int print_nodes(struct ast* node)
{
  if (node->ntoken == IF || is_op(node->ntoken) || is_const_var(node->ntoken) || node->ntoken == FUNID
    || node->ntoken == LET) {
    fprintf(fp, "%d [label=\"%s\"];\n", node->id, node->token);
  }
  return 0;
}

int print_nodes_ir(struct ast* node)
{
  if (node->ntoken == CONST || node->ntoken == TRUE ||
      node->ntoken == FALSE || node->ntoken == VARID) {
    fprintf(fp, "%d [label=\"v%d := %s\"];\n", node->id, node->id, node->token);
  }
  if (node->ntoken == FUNID) {
    fprintf(fp, "%d [label=\"%s\"];\n", node->id, node->token);
  }
  if (node->ntoken == LETID) {
    struct ast* def = get_child(node->parent, 2);
    fprintf(fp, "%d [label=\"%s := v%d\"];\n", node->id, node->token, def->id);
  }
  if (node->ntoken == LET) {
    struct ast* def = get_child(node, 3);
    fprintf(fp, "%d [label=\"v%d := v%d\"];\n", node->id, node->id, def->id);
  }
  if (node->ntoken == IF) {
    fprintf(fp, "%d [label=\"IF v%d = true, then v%d := v%d, else v%d := v%d\"];\n",
           node->id, get_child(node, 1)->id,
           node->id, get_child(node, 2)->id, node->id,
           get_child(node, 3)->id);
  }
  if (is_op(node->ntoken))
  {
    fprintf(fp, "%d [label=\"v%d := ", node->id, node->id);
    int n = get_child_num(node);
    if (n == 1) fprintf(fp, " NOT ");
    for (int i = 1; i <= n; i++) {
      fprintf(fp, "v%d", get_child(node, i)->id);
      if (i < n) fprintf(fp, " %s ", node->token);
      else fprintf(fp, "\"];\n");
    }
  }
  if (node->ntoken == CALL)
  {
    fprintf(fp, "%d [label=\"v%d := %s (", node->id, node->id, node->token);
    int n = get_child_num(node);
    for (int i = 1; i <= n; i++) {
      fprintf(fp, "v%d", get_child(node, i)->id);
      if (i < n) fprintf(fp, ", ");
    }
    fprintf(fp, ")\"];\n");
  }
  else if (node->ntoken == DEFFUN || node->ntoken == PRINT)
  {
    fprintf(fp, "%d [label=\"EXIT\"];\n", node->id);
  }
  return 0;
}

int find_reach(int n, int prev, int dst)
{
  struct ast* tmp = find_ast_node(n);
  if (tmp != NULL) {
    if (tmp->ntoken == IF) {
      if (get_child(tmp, 1)->id == dst) {
        if (get_child(tmp, 2)->id == prev) return n;
        if (get_child(tmp, 3)->id == prev) return -n;
      }
    }
  }
  int n1, n2;
  get_next(cfg_r, n, &n1, &n2);
  return find_reach(n1, n, dst);
}

void add_br_instr(struct cfg** r)
{
  int n1, n2;
  get_next(cfg_r, (*r)->dst, &n1, &n2);
  struct br_instr* br = NULL;
  if (n1 == 0)
    br = mk_ubr(0, 0);
  else if (n2 == 0 && n1 != 0)
    br = mk_ubr(0, n1);
  else if (n2 != 0 && n1 != 0)
  {
    int n3 = find_reach(n1, 0, (*r)->dst);
    br = mk_cbr(0, (*r)->dst, (n3 > 0 ? n1 : n2), (n3 > 0 ? n2 : n1));
  }
  (*r)->br = br;

  if (n1 == 0) (*r)->asgns = mk_uasgn(0, 0, (*r)->src, 0);
}

void to_cfg_rec()
{
  struct cfg* r = cfg_r;
  while (r != NULL){
    if (r->valid && !r->final) {
      struct ast *node = find_ast_node(r->dst);
      if (node->is_leaf)
      {
        int op, ty;
        bool f = false;
        if (node->ntoken == CONST) { op = atoi(node->token); ty = CONST; f = true; }
        if (node->ntoken == TRUE) { op = 1; ty = CONST; f = true; }
        if (node->ntoken == FALSE) { op = 0; ty = CONST; f = true; }
        if (node->ntoken == LETID) { op = get_child(node->parent, 2)->id; ty = LETID; f = true; }
        if (node->ntoken == VARID) {
          struct node_var_str* tmp = find_var_str(node->id, node->token, vars_r);
          if (tmp == NULL) exit(1);
          op = tmp->begin_id; ty = VARID; f = true; }
        if (f)
          r->asgns = mk_asgn(0, r->dst, 0, op, 0, ty);
        if (node->ntoken == CALL) {
          int dst = r->dst;
          r->dst = ++sz;
          r->id = r->dst;
          r->asgns = mk_casgn(0, 0, node->token);
          push_ncfg(sz, dst, true, true, NULL, NULL, r->fun, &cfg_r, &cfg_t);
          cfg_t->asgns = mk_casgn(0, dst, node->token);
        }
      }
      else{
        r->valid = false;
        int interm = r->src;

        if (node->ntoken == IF){
          int cur = get_child(node, 1)->id;
          push_ncfg(interm, cur, true, false, NULL, NULL, r->fun, &cfg_r, &cfg_t);
          push_cfg(cur,  get_child(node, 2)->id, true, false, NULL, NULL, &cfg_r, &cfg_t);
          push_cfg(cur,  get_child(node, 3)->id, true, false, NULL, NULL, &cfg_r, &cfg_t);
          push_cfg(get_child(node, 2)->id, r->dst, true, true, NULL, NULL, &cfg_r, &cfg_t);
          push_cfg(get_child(node, 3)->id, r->dst, true, true,NULL, NULL,  &cfg_r, &cfg_t);
        }
        else if (is_op(node->ntoken) || node->ntoken == CALL){
          for (int i = 1; i <= get_child_num(node); i++) {
            int cur = get_child(node, i)->id;
            push_ncfg(interm, cur, true, false, NULL, NULL, r->fun, &cfg_r, &cfg_t);
            interm = cur;
          }

          if (is_op(node->ntoken)) {
            push_ncfg(interm, r->dst, true, true, NULL, NULL, r->fun, &cfg_r, &cfg_t);
            if (node->ntoken == NOT)
              cfg_t->asgns = mk_asgn(0, r->dst, 0, get_child(node, 1)->id, 0, node->ntoken);
            else
              cfg_t->asgns = mk_asgn(0, r->dst, 1, get_child(node, 1)->id, get_child(node, 2)->id, node->ntoken);
          }
          else
          {
            // function call
            for (int i = 1; i <= get_child_num(node); i++) {
              int cur = ++sz;
              push_ncfg(interm, cur, true, true, NULL, NULL, r->fun, &cfg_r, &cfg_t);
              cfg_t->asgns = mk_uasgn(0, -i, get_child(node, i)->id, 0);
              interm = cur;
            }
            push_ncfg(interm, ++sz, true, true, NULL, NULL, r->fun, &cfg_r, &cfg_t);
            cfg_t->asgns = mk_casgn(0, 0, node->token);
            push_cfg(sz, r->dst, true, true, NULL, NULL, &cfg_r, &cfg_t);
            cfg_t->asgns = mk_casgn(0, r->dst, node->token);
          }
        }
        else if (node->ntoken == LET)
        {
          int cur = get_child(node, 2)->id;
          push_ncfg(interm, cur, true, false, NULL, NULL, r->fun, &cfg_r, &cfg_t);
          interm = cur;
          cur = get_child(node, 1)->id;
          push_cfg(interm, cur, true, false,NULL, NULL,  &cfg_r, &cfg_t);
          interm = cur;
          cur = get_child(node, 3)->id;
          push_cfg(interm, cur, true, false, NULL, NULL, &cfg_r, &cfg_t);
          interm = cur;
          push_cfg(interm, r->dst, true, true, NULL, NULL, &cfg_r, &cfg_t);
          cfg_t->asgns = mk_asgn(0,  r->dst, 0,  interm,0, LET);
        }
      }
    }

    r = r->next;
  }
}

void add_brs(){
  struct cfg* t = cfg_r;
  while (t != NULL){
    if (t->valid) {
      add_br_instr(&t);
      struct cfg* c1;
      struct cfg* c2;
      struct cfg* c3;
      struct cfg* c4;
      get_prev(cfg_r, t->dst, &c1, &c2);
      if (c1 != NULL && c2 != NULL) {
        get_prev(cfg_r, c1->src, &c3, &c4);
        if (c3 != NULL) {
          if (c3->asgns == NULL)
            c3->asgns = mk_asgn(0, c1->id, 0, c1->src, 0, c1->src);
          else
            c3->asgns->lhs = c1->id;
        }
        if (c4 != NULL) {
          if (c4->asgns == NULL)
            c4->asgns = mk_asgn(0, c1->id, 0, c1->src, 0, c1->src);
          else
            c4->asgns->lhs = c1->id;
        }
        get_prev(cfg_r, c2->src, &c3, &c4);
        if (c3 != NULL) {
          if (c3->asgns == NULL)
            c3->asgns = mk_asgn(0, c2->id, 0, c2->src, 0, c2->src);
          else
            c3->asgns->lhs = c2->id;
        }
        if (c4 != NULL) {
          if (c4->asgns == NULL)
            c4->asgns = mk_asgn(0, c2->id, 0, c2->src, 0, c2->src);
          else
            c4->asgns->lhs = c2->id;
        }
      }
    }
    t = t->next;
  }
}

void print_cfg(struct cfg* r){
  fp = fopen("cfg.dot", "w");
  fprintf(fp, "digraph print {\n");
  visit_ast(print_nodes_ir);
  struct cfg* t = r;
  while (t != NULL){
    if (t->valid) fprintf(fp, "%d->%d\n", t->src, t->dst);
    t = t->next;
  }
  fprintf(fp, "}\n");
  fclose(fp);
  // system("dot -Tpdf cfg.dot -o cfg.pdf");
}

int main (int argc, char **argv) {
  int opt = 0;

  int register_values[50];
  for(int i = 0; i < 50; i++){register_values[i] = NOVALUE;}

  
  if (argc > 1){
    if (strcmp("--opt", argv[1]) == 0){
      if (argc == 2) opt = 999;
      else opt = atoi(argv[2]);
    }
  }
  printf("Running with optimizations level %d\n", opt);

  int retval = yyparse();
  push_fun_str("GET-INT", INT, 0, NULL, &fun_r, &fun_t);
  push_fun_str("GET-BOOL", BOOL, 0, NULL, &fun_r, &fun_t);
  if (retval == 0) retval = visit_ast(get_fun_var_types);
  if (retval == 0) retval = visit_ast(type_check);
//  if (retval == 0) print_ast();
//  print_fun_str(fun_r);

  sz = get_ast_size();
  visit_ast(to_cfg);
  to_cfg_rec();
  add_brs();
  get_bb_ids(cfg_r);

  bool to_cont = true;
  while (opt && to_cont)
  {
    to_cont = cfg_compact(cfg_r);
    if (opt > 1) to_cont |= cfg_unreach(cfg_r);
    if (opt > 2) to_cont |= cfg_dupl(cfg_r);
    if (opt > 3) to_cont |= opt_cp(cfg_r,register_values);
    if (opt > 4) to_cont |= opt_arithm(cfg_r,register_values);
  }
  print_cfg(cfg_r);
  print_cfg_ir(cfg_r, sz, fun_r);

  // TODO: add CFG cleaning
  free_ast();
  return retval;
}

