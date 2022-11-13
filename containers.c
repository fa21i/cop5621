/**
 * Author:    Grigory Fedyukovich, Subhadeep Bhattacharya
 * Created:   09/30/2020
 *
 * (c) FSU
 **/

#include "containers.h"

void push_int (int i, struct node_int** r, struct node_int** t){
  if (*r == NULL){   // If root is null - empty list
    *r = (struct node_int*)malloc(sizeof(struct node_int)); //Create a new node a assign the root pointer tp tail
    (*r)->id = i;
    (*r)->next = NULL;
    *t = *r;
  }
  else{
    struct node_int* ptr; //Non-empty list
    ptr = (struct node_int*)malloc(sizeof(struct node_int)); // Create a new node, put it after tail as a new node
    ptr->id = i;
    ptr->next = NULL;
    (*t)->next = ptr;
    (*t) = ptr;
  }
}

void push_unique_int (int i, struct node_int** r){
  if (*r == NULL){   // If root is null - empty list
    *r = (struct node_int*)malloc(sizeof(struct node_int)); //Create a new node a assign the root pointer tp tail
    (*r)->id = i;
    (*r)->next = NULL;
  }
  else{
    if ((*r)->id == i) return;
    push_unique_int(i, &(*r)->next);
  }
}

int find_int(int c, struct node_int* r){
  while (r != NULL){
    if (c == r->id) return 0;
    r = r->next;
  }
  return 1;
}

int pop_int(struct node_int** r, struct node_int** t) {
  if ((*r)->next == NULL) {             //Check if next node is NULL - check for only one node
    int retval = (*r)->id;              //Store the id of the current node in retval
    free(*r);
    *r = NULL;                          //Store current node to NULL
    *t = NULL;
    return retval;                      //return retval
  }

  struct node_int** r1 = r;             //Else store the pointer in r1 - if multiple node, just traverse and then pop
  while ((*r1)->next != NULL) {
    *t = *r1;
    r1 = &((*r1)->next); //traverse the tree
  }

  int retval = (*r1)->id;               //store the last element in retval
  *r1 = NULL;                           //Store current node to NULL
  return retval;                        //return retval
}

void dequeue_int (struct node_int** r) {
  struct node_int* tmp = *r;
  (*r) = (*r)->next;
  free(tmp);
}

void push_str (char* c, struct node_str** r, struct node_str** t){
  if (*r == NULL){                           //If root node is null
    *r = (struct node_str*)malloc(sizeof(struct node_str)); //Create a new node
    (*r)->id = c;                           //set the id for the new node
    (*r)->next = NULL;                                      //set next pointer to null
    *t = *r;                                                //Set tail pointer == root pointer
  }
  else {                                   //If root node is not null
    struct node_str* ptr;
    ptr = (struct node_str*)malloc(sizeof(struct node_str));  //Create a temporary node
    ptr->id = c;                                            //Set id for the new node
    ptr->next = NULL;                                         //Set next pointer to NULL
    (*t)->next = ptr;                                         //Set the node after tail
    *t = ptr;                                                 //Set tail as the new pointer
  }
}

char* pop_str(struct node_str** r, struct node_str** t) {
  if ((*r)->next == NULL) {
    char* retval = (*r)->id;
    *r = NULL;
    free(*r);
    *t = *r;
    return retval;
  }

  struct node_str** r1 = r;
  while ((*r1)->next != NULL){
    (*t) = *r1;
    r1 = &((*r1)->next);
  }

  char* retval = (*r1)->id;
  *r1 = NULL;
  free(*r1);
  return retval;
}

int find_str(char* c, struct node_str* r){
  while (r != NULL){
    if (strcmp(c, r->id) == 0) return 0;
    r = r->next;
  }
  return 1;
}

int print_str(struct node_str* r){
  while (r != NULL){
    printf(" %s,", r->id);
    r = r->next;
  }
  printf("\n");
  return 1;
}

int print_int(struct node_int* r){
  while (r != NULL){
    printf(" %d,", r->id);
    r = r->next;
  }
  printf("\n");
  return 1;
}

void clean_int (struct node_int** r){
  while (*r != NULL){
    struct node_int* tmp = *r;
    (*r) = (*r)->next;
    free(tmp);
  }
}

void clean_str(struct node_str** r){
  while (*r != NULL){
    struct node_str* tmp = *r;
    (*r) = (*r)->next;
    free(tmp);
  }
}

void push_var_str (int begin_id, int end_id, int type, char* name, struct node_var_str** r, struct node_var_str** t){
  if (*r == NULL){
    *r = (struct node_var_str*)malloc(sizeof(struct node_var_str)); //Create a new node
    (*r)->begin_id = begin_id;
    (*r)->end_id = end_id;
    (*r)->type = type;
    (*r)->name = name;
    (*r)->next = NULL;
    *t = *r;
  }
  else {
    struct node_var_str* ptr;
    ptr = (struct node_var_str*)malloc(sizeof(struct node_var_str));  //Create a temporary node
    ptr->begin_id = begin_id;
    ptr->end_id = end_id;
    ptr->type = type;
    ptr->name = name;
    ptr->next = NULL;
    (*t)->next = ptr;                                         //Set the node after tail
    *t = ptr;
  }
}

void push_fun_str (char* name, int type, int arity, struct node_var_str* args, struct node_fun_str** r, struct node_fun_str** t) {
  if (*r == NULL){
    *r = (struct node_fun_str*)malloc(sizeof(struct node_fun_str)); //Create a new node
    (*r)->name = name;
    (*r)->type = type;
    (*r)->args = args;
    (*r)->next = NULL;
    (*r)->arity = arity;
    *t = *r;
  }
  else {
    struct node_fun_str* ptr;
    ptr = (struct node_fun_str*)malloc(sizeof(struct node_fun_str));  //Create a temporary node
    ptr->args = args;
    ptr->type = type;
    ptr->name = name;
    ptr->arity = arity;
    ptr->next = NULL;
    (*t)->next = ptr;                                         //Set the node after tail
    *t = ptr;
  }
}

struct node_var_str* find_var_str(int loc_id, char* name, struct node_var_str* r){
  while (r != NULL){
    if (strcmp(name, r->name) == 0 &&
        r->begin_id <= loc_id && loc_id <= r->end_id) return r;
    else r = r->next;
  }
  return NULL;
}

struct node_fun_str* find_fun_str(char* name, struct node_fun_str* r){
  while (r != NULL){
    if (strcmp(name, r->name) == 0) return r;
    else r = r->next;
  }
  return NULL;
}

void clean_var_str(struct node_var_str** r){
  while (*r != NULL){
    struct node_var_str* tmp = *r;
    (*r) = (*r)->next;
    free(tmp);
  }
}

void clean_fun_str(struct node_fun_str** r){
  while (*r != NULL){
    struct node_fun_str* tmp = *r;
    (*r) = (*r)->next;
    free(tmp);
  }
}