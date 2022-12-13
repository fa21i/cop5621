/**
 * Author:    Grigory Fedyukovich, Subhadeep Bhattacharya
 * Created:   09/30/2020
 *
 * (c) FSU
 **/

#include "containers.h"

void push_int (int i, struct node_int** r, struct node_int** t){
  if (*r == NULL)   // If root is null - empty list
  {
    *r = (struct node_int*)malloc(sizeof(struct node_int)); //Create a new node a assign the root pointer tp tail
    (*r)->id = i;
    (*r)->next = NULL;
    *t = *r;
  }
  else
  {
    struct node_int* ptr; //Non-empty list
    ptr = (struct node_int*)malloc(sizeof(struct node_int)); // Create a new node, put it after tail as a new node
    ptr->id = i;
    ptr->next = NULL;
    (*t)->next = ptr;
    (*t) = ptr;
  }
}

int pop_int(struct node_int** r) {
  if ((*r)->next == NULL) {             //Check if next node is NULL - check for only one node
    int retval = (*r)->id;              //Store the id of the current node in retval
    *r = NULL;                          //Store current node to NULL
    return retval;                      //return retval
  }

  struct node_int** r1 = r;             //Else store the pointer in r1 - if multiple node, just traverse and then pop
  while ((*r1)->next != NULL) r1 = &((*r1)->next); //traverse the tree

  int retval = (*r1)->id;               //store the last element in retval
  *r1 = NULL;                           //Store current node to NULL
  return retval;                        //return retval
}

int pop_ast(struct node_int** r, struct node_int** t) {
  if ((*r)->next == NULL) {             //Check if next node is NULL - check for only one node
    int retval = (*r)->id;              //Store the id of the current node in retval
    *r = NULL;
    *t = *r;                            //Store current node to NULL
    return retval;                      //return retval
  }

  struct node_int** r1 = r;             //Else store the pointer in r1 - if multiple node, just traverse and then pop
  struct node_int** temp = NULL;
  while ((*r1)->next != NULL) {
      temp = r1;
      r1 = &((*r1)->next);              //traverse the tree
  }

  int retval = (*r1)->id;               //store the last element in retval
  *r1 = NULL;                           //Store current node to NULL
  *t = *temp;
  return retval;                        //return retval
}

void push_str (char* c, struct node_str** r, struct node_str** t){
  if (*r == NULL)                           //If root node is null
  {
    *r = (struct node_str*)malloc(sizeof(struct node_str)); //Create a new node
    (*r)->id = c;                           //set the id for the new node
    (*r)->next = NULL;                                      //set next pointer to null
    *t = *r;                                                //Set tail pointer == root pointer
  }
  else                                    //If root node is not null
  {
    struct node_str* ptr;
    ptr = (struct node_str*)malloc(sizeof(struct node_str));  //Create a temporary node
    ptr->id = c;                                            //Set id for the new node
    ptr->next = NULL;                                         //Set next pointer to NULL
    (*t)->next = ptr;                                         //Set the node after tail
    *t = ptr;                                                 //Set tail as the new pointer
  }
}

char* pop_str(struct node_str** r) {
  if ((*r)->next == NULL) {
    char* retval = (*r)->id;
    *r = NULL;
    free(*r);
    return retval;
  }

  struct node_str** r1 = r;
  while ((*r1)->next != NULL) r1 = &((*r1)->next);

  char* retval = (*r1)->id;
  *r1 = NULL;
  free(*r1);
  return retval;
}

int find_str(char* c, struct node_str* r){
  while (r != NULL)
  {
    if (strcmp(c, r->id) == 0) return 0;
    r = r->next;
  }
  return 1;
}

int print_str(struct node_str* r){
  while (r != NULL)
  {
    printf(" %s", r->id);
    r = r->next;
  }
  printf("\n");
  return 1;
}

void push_dstr (char* c1, char* c2, struct node_dstr** r, struct node_dstr** t){
  if (*r == NULL)                           //If root node is null
  {
    *r = (struct node_dstr*)malloc(sizeof(struct node_dstr)); //Create a new node
    (*r)->id1 = c1;                           //set the id for the new node
    (*r)->id2 = c2;                           //set the id for the new node
    (*r)->next = NULL;                                      //set next pointer to null
    *t = *r;                                                //Set tail pointer == root pointer
  }
  else                                    //If root node is not null
  {
    struct node_dstr* ptr;
    ptr = (struct node_dstr*)malloc(sizeof(struct node_dstr));  //Create a temporary node
    ptr->id1 = c1;                                            //Set id for the new node
    ptr->id2 = c2;                                            //Set id for the new node
    ptr->next = NULL;                                         //Set next pointer to NULL
    (*t)->next = ptr;                                         //Set the node after tail
    *t = ptr;                                                 //Set tail as the new pointer
  }
}

int find_dstr(char* c1, char* c2, struct node_dstr* r){
  while (r != NULL)
  {
    if (strcmp(c1, r->id1) == 0 && strcmp(c2, r->id2) == 0) return 0;
    r = r->next;
  }
  return 1;
}

void insert_parent(struct ast* p) {
  struct ast_child* temp_ast_child_root = p -> child;
  while(temp_ast_child_root != NULL){
    temp_ast_child_root -> id -> parent = p;
    temp_ast_child_root = temp_ast_child_root -> next;
  }
}

void fill_ast_node(struct ast** t, int val, char* token, bool is_leaf, int ntoken, int line_number){
  (*t) = (struct ast*)malloc(sizeof(struct ast));
  (*t) -> id = val;                                     //set value into id field
  (*t) -> next = NULL;                                  //set next pointer to NULL
  (*t) -> token = token;
  (*t) -> is_leaf = is_leaf;
  (*t) -> ntoken = ntoken;
  (*t) -> line_number = line_number;
  if (ast_child_root != NULL){                          //if child doesnot exist,
    (*t) -> child = ast_child_root;                     //set current child root pointer to child field
    ast_child_root = NULL;                              //Set the child root to NULL as we intend to set a new list
    insert_parent(*t);
  } else {
    (*t) -> child = NULL;                               //
  }
}

void insert_node_tmp(int val, char* token, bool is_leaf, int ntoken, int line_number){
  if (ast_list_root == NULL){                               //Create a new node and set the value if root is empty
    fill_ast_node(&ast_list_root, val, token, is_leaf, ntoken,line_number);
  } else {                                                  //Else create a ast graph node and assign it to next
    struct ast* ptr;                                        //Create a temp pointer
    fill_ast_node(&ptr, val, token, is_leaf, ntoken,line_number);

    //Traverse root and set the new pointer at the next of last node
    struct ast** tmp = &ast_list_root;
    while ((*tmp)->next != NULL) {
      tmp = &((*tmp) -> next);    // last_node will store the pointer to last node
    }
    (*tmp)->next = ptr;
  }
}

int insert_node(char* token, int ntoken, int line_number) {
  current_node_id++;
  insert_node_tmp(current_node_id, token, ast_child_root == NULL, ntoken, line_number);
  return current_node_id;
}


void insert_child(int val){        //This function helps to fill the child list
  struct ast* node = find_ast_node(val);
  if (ast_child_root == NULL)      //If null list
  {
    (ast_child_root) = (struct ast_child*)malloc(sizeof(struct ast_child));  //allocate memory
    // Set id and next field of the ast_child_root
    (ast_child_root) -> id = node;
    (ast_child_root) -> next = NULL;
  } else {
    //Create the node
    struct ast_child* ptr;
    ptr = (struct ast_child*)malloc(sizeof(struct ast_child));
    ptr -> id = node;
    ptr -> next = NULL;

    //Traverse the list and set the node at top
    struct ast_child** temp = &ast_child_root;     // Set child root in a temporary variable
    struct ast_child** last_child_node = NULL;
    while ((*temp)->next != NULL) {           // Until next is NULL traverse the list
      temp = &((*temp) -> next);              // temp will hold the address of next node
    }
    (*temp)->next = ptr;
  }
}

void insert_children (int n, ...)
{
  va_list vl;
  va_start(vl,n);
  int i;
  for (i = 0; i < n; i++)
    insert_child(va_arg(vl, int));
  va_end(vl);
}

struct ast* find_ast_node(int id){
  struct ast* temp_root = ast_list_root;
  while(temp_root != NULL){
    if (id == temp_root->id) return temp_root;
    temp_root = temp_root -> next;
  }
  return NULL;
}

struct ast* get_child(struct ast* ast_node, int id){
  struct ast_child* temp_child_root = ast_node -> child;
  int child_num = 0;
  while(temp_child_root != NULL) {
    child_num++;
    if (child_num == id) {
      return temp_child_root->id;
    }
    temp_child_root = temp_child_root -> next;
  }
  return NULL;
}

int visit_ast(int (*f)(struct ast* ast_node)){
  struct ast* temp_root = ast_list_root;
  while(temp_root != NULL){
    if (f(temp_root) == 1) return 1;
    temp_root = temp_root -> next;
  }
  return 0;
}
FILE *fp;
int print(struct ast* temp_root) {
  if (! temp_root->is_leaf){
    fprintf(fp, "%d [label=\"%s\", fontname=\"monospace\", style=filled, fillcolor=mintcream];\n ", temp_root->id, temp_root->token);
  } else {
    fprintf(fp, "%d [label=\"%s\", fontname=\"monospace\"];\n ", temp_root->id, temp_root->token);
  }
  if (temp_root -> child != NULL){
    struct ast_child* temp_ast_child_root = temp_root -> child;
    while(temp_ast_child_root != NULL){
      fprintf(fp, "%d->%d\n ", temp_root->id, temp_ast_child_root->id->id);
      temp_ast_child_root = temp_ast_child_root -> next;
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
}

void free_ast() {
  struct ast* temp_root = ast_list_root;
  struct ast* current_root = NULL;
  while(temp_root != NULL){
    if (temp_root -> child != NULL){
      struct ast_child* temp_ast_child_root = temp_root -> child;
      struct ast_child* current_child = NULL;
      while(temp_ast_child_root != NULL){
        current_child = temp_ast_child_root;
        temp_ast_child_root = temp_ast_child_root -> next;
        free(current_child);
      }
    }
    current_root = temp_root;
    temp_root = temp_root -> next;
    free(current_root);
  }
}
