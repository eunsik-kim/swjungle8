#ifndef _binarysearchtree_
#define _binarysearchtree_
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

typedef enum { BsTREE_RED, BsTREE_BLACK } color_t; // test코드와 호환

typedef int key_t;
typedef enum { MIN_VALUE, MAX_VALUE } flag;
typedef struct node_t {
  color_t color; // test코드와 호환
  key_t key; 
  struct node_t *parent, *left, *right;
} node_t;

typedef struct {
  node_t *root;
  // node_t *nil;  // for sentinel
} bstree;

bstree *new_bstree(void);

node_t *bstree_insert(bstree *bst, const key_t key);
node_t *bstree_find(const bstree *bst, const key_t key);
node_t *bstree_min(const bstree *bst);
node_t *bstree_max(const bstree *bst);
node_t *subtree_min_max(node_t *node, flag direction); // 새로추가

int bstree_erase(bstree *bst, node_t *);
void delete_bstree(bstree *bst);
void postOrderTraverse(node_t *deleteNode); // 새로추가

int bstree_to_array(const bstree *bst, key_t *arr, const size_t n);
size_t inOrderTraverse(node_t *treeNode, key_t *arr, size_t i, size_t n); // 새로추가

# endif