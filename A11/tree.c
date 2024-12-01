/*----------------------------------------------
 * Author: Rebecca Lassman
 * Date: 12/1/24
 * Description: Implements a binary search tree.
 ---------------------------------------------*/
#include "tree.h"
#include "stdio.h"
#include "stdlib.h"
#include <string.h>

struct tree_node* find(const char* name, struct tree_node* root) {
    if (root == NULL) {
      return NULL;
    }
    if (strcmp(name, root->data.name) < 0) {
      return (find(name, root->left));
    } else if (strcmp(name, root->data.name) > 0) {
      return (find(name, root->right));
    } else {
      return root;
    }
}

struct tree_node* insert(const char* name, struct tree_node* root) {
  if (root == NULL) {
    root = malloc (sizeof (struct tree_node)); 
    if(root == NULL) {
      return NULL;
    } else {
      strncpy(root->data.name, name, sizeof(root->data.name));
      root->left = NULL;
      root->right = NULL;
    }
  } else {
    if (strcmp(name, root->data.name) < 0) {
      root->left = insert(name, root->left);
    } else if (strcmp(name, root->data.name) > 0) {
      root->right = insert(name, root->right);
    } 
  }
  return root;
}

void clear(struct tree_node* root) {
  if (root == NULL) 
    return;
  clear(root->left);
  clear(root->right);
  free(root);
}

void print(struct tree_node* root) {
  if (root == NULL)
    return;
  printf(" %s\n", root->data.name);
  print(root->left);
  print(root->right);
}

void printSorted(struct tree_node* root) {
  if (root == NULL)
    return;
  printSorted(root->left);
  printf(" %s\n", root->data.name);
  printSorted(root->right);
}

