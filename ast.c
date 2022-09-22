/**
 * Author:    Grigory Fedyukovich, Subhadeep Bhattacharya
 * Created:   09/30/2020
 *
 * (c) FSU
 **/

#include "ast.h"

void insert_parent(struct ast* p) {
  struct ast_child* temp_ast_child_root = p->child;
  while(temp_ast_child_root != NULL){
    temp_ast_child_root->id->parent = p;
    temp_ast_child_root = temp_ast_child_root->next;
  }
}

void fill_ast_node(struct ast** t, int val, char* token, bool is_leaf, int ntoken){
  (*t) = (struct ast*)malloc(sizeof(struct ast));
  (*t)->id = val;                                     //set value into id field
  (*t)->next = NULL;                                  //set next pointer to NULL
  (*t)->token = token;
  (*t)->is_leaf = is_leaf;
  (*t)->ntoken = ntoken;
  if (ast_child_root != NULL){                          //if child doesnot exist,
    (*t)->child = ast_child_root;                     //set current child root pointer to child field
    ast_child_root = NULL;                              //Set the child root to NULL as we intend to set a new list
    insert_parent(*t);
  } else {
    (*t)->child = NULL;                               //
  }
}

void insert_node_tmp(int val, char* token, bool is_leaf, int ntoken){
  if (ast_list_root == NULL){                               //Create a new node and set the value if root is empty
    fill_ast_node(&ast_list_root, val, token, is_leaf, ntoken);
  } else {                                                  //Else create a ast graph node and assign it to next
    struct ast* ptr;                                        //Create a temp pointer
    fill_ast_node(&ptr, val, token, is_leaf, ntoken);

    //Traverse root and set the new pointer at the next of last node
    struct ast** tmp = &ast_list_root;
    while ((*tmp)->next != NULL) {
      tmp = &((*tmp)->next);    // last_node will store the pointer to last node
    }
    (*tmp)->next = ptr;
  }
}

int insert_node(char* token, int ntoken) {
  current_node_id++;
  insert_node_tmp(current_node_id, token, ast_child_root == NULL, ntoken);
  return current_node_id;
}

void insert_child(int val){        //This function helps to fill the child list
  struct ast* node = find_ast_node(val);
  if (ast_child_root == NULL) {     //If null list
    (ast_child_root) = (struct ast_child*)malloc(sizeof(struct ast_child));  //allocate memory
    // Set id and next field of the ast_child_root
    (ast_child_root)->id = node;
    (ast_child_root)->next = NULL;
  } else {
    //Create the node
    struct ast_child* ptr;
    ptr = (struct ast_child*)malloc(sizeof(struct ast_child));
    ptr->id = node;
    ptr->next = NULL;

    //Traverse the list and set the node at top
    struct ast_child** temp = &ast_child_root;     // Set child root in a temporary variable
    struct ast_child** last_child_node = NULL;
    while ((*temp)->next != NULL) {           // Until next is NULL traverse the list
      temp = &((*temp)->next);              // temp will hold the address of next node
    }
    (*temp)->next = ptr;
  }
}

void insert_children (int n, ...){
  va_list vl;
  va_start(vl,n);
  for (int i = 0; i < n; i++)
    insert_child(va_arg(vl, int));
  va_end(vl);
}

struct ast* find_ast_node(int id){
  struct ast* temp_root = ast_list_root;
  while(temp_root != NULL){
    if (id == temp_root->id) return temp_root;
    temp_root = temp_root->next;
  }
  return NULL;
}

struct ast* find_parent(struct ast* ast_node, struct ast* ast_parent){
  struct ast* tmp = ast_node;
  while(tmp != NULL) {
    if (tmp == ast_parent) return tmp;
    tmp = tmp->parent;
  }
  return NULL;
}

struct ast* get_root(struct ast* ast_node){
  struct ast* tmp = ast_node;
  while(tmp->parent != NULL) {
    tmp = tmp->parent;
  }
  return tmp;
}

struct ast* get_child(struct ast* ast_node, int id){
  struct ast_child* temp_child_root = ast_node->child;
  int child_num = 0;
  while(temp_child_root != NULL) {
    child_num++;
    if (child_num == id) {
      return temp_child_root->id;
    }
    temp_child_root = temp_child_root->next;
  }
  return NULL;
}

int get_child_num(struct ast* ast_node){
  struct ast_child* temp_child_root = ast_node->child;
  int child_num = 0;
  while(temp_child_root != NULL) {
    child_num++;
    temp_child_root = temp_child_root->next;
  }
  return child_num;
}

int visit_ast(int (*f)(struct ast* ast_node)){
  struct ast* temp_root = ast_list_root;
  while(temp_root != NULL){
    if (f (temp_root) != 0) return 1;
    temp_root = temp_root->next;
  }
  return 0;
}

FILE *fp;
int print(struct ast* temp_root) {
  if (!temp_root->is_leaf){
    fprintf(fp, "%d [label=\"%s\", fontname=\"monospace\", style=filled, fillcolor=mintcream];\n ", temp_root->id, temp_root->token);
  } else {
    fprintf(fp, "%d [label=\"%s\", fontname=\"monospace\"];\n ", temp_root->id, temp_root->token);
  }
  if (temp_root->child != NULL){
    struct ast_child* temp_ast_child_root = temp_root->child;
    while(temp_ast_child_root != NULL){
      fprintf(fp, "%d->%d\n ", temp_root->id, temp_ast_child_root->id->id);
      temp_ast_child_root = temp_ast_child_root->next;
    }
  }
  return 0;
}

void print_ast(){
  fp = fopen("ast.dot", "w");
  fprintf(fp, "digraph print {\n ");
  visit_ast(print);
  fprintf(fp, "}\n ");
  fclose(fp);
  system("dot -Tpdf ast.dot -o ast.pdf");
}

void free_ast() {
  struct ast* temp_root = ast_list_root;
  struct ast* current_root = NULL;
  while(temp_root != NULL){
    if (temp_root->child != NULL){
      struct ast_child* temp_ast_child_root = temp_root->child;
      struct ast_child* current_child = NULL;
      while(temp_ast_child_root != NULL){
        current_child = temp_ast_child_root;
        temp_ast_child_root = temp_ast_child_root->next;
        free(current_child);
      }
    }
    current_root = temp_root;
    temp_root = temp_root->next;
    free(current_root);
  }
}
