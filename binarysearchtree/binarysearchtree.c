#include "binarysearchtree.h"

// implementation in rbtree.h function into binarysearchtree

bstree *new_bstree(void){
    bstree *bst = (bstree *)malloc(sizeof(bstree));
    bst->root = NULL;
    return bst;
}

node_t *bstree_insert(bstree *bst, const key_t key){
    node_t *insertNode = (node_t *)malloc(sizeof(node_t));
    insertNode->key = key;
    node_t *minNode = bst->root;
    while (minNode){
        if (minNode->key <= key){
            if (minNode->right){
                minNode = minNode->right;
            } else {
                minNode->right = insertNode;
                break;
            }
        } else {
            if (minNode->left){
                minNode = minNode->left;
            } else {
                minNode->left = insertNode;
                break;
            }
        }
    }
    if (!bst->root) {bst->root = insertNode;}
    insertNode -> parent = minNode;
    insertNode -> left = NULL;
    insertNode -> right = NULL;
    return insertNode;
}

node_t *bstree_find(const bstree *bst, const key_t key){
    node_t *insertNode = bst->root;
    while (insertNode && (insertNode->key != key)){
        if (insertNode->key <= key){
            insertNode = insertNode->right;
        } else {
            insertNode = insertNode->left;
        }
    }
    return insertNode;
}
node_t *bstree_min(const bstree *bst){
    node_t *minNode = subtree_min_max(bst->root, MIN_VALUE);
    return minNode;
}
node_t *bstree_max(const bstree *bst){
    node_t *maxNode = subtree_min_max(bst->root, MAX_VALUE);
    return maxNode;
}

node_t *subtree_min_max(node_t *subTreeNode, flag direction){
    node_t *minmaxNode = direction ? subTreeNode->right: subTreeNode->left;
    while (minmaxNode){
        subTreeNode = minmaxNode;
        minmaxNode = direction ? subTreeNode->right: subTreeNode->left;
    }
    return subTreeNode;
}

int bstree_erase(bstree *bst, node_t *eraseNode){
    node_t *replaceNode;
    if (eraseNode->right){
        replaceNode = subtree_min_max(eraseNode->right, MIN_VALUE);
        if (replaceNode != eraseNode->right){
            replaceNode->parent->left = replaceNode->right;
        } else {
            replaceNode->parent->right = replaceNode->right;
        }
        if (replaceNode->right){
            replaceNode->right->parent = replaceNode->parent;
        }
    } else if (eraseNode->left){
        replaceNode = subtree_min_max(eraseNode->left, MAX_VALUE);
        if (replaceNode != eraseNode->left){
            replaceNode->parent->right = replaceNode->left;
        } else {
            replaceNode->parent->left = replaceNode->left;
        }
        if (replaceNode->left){
            replaceNode->left->parent = replaceNode->parent;
        }
    } else {
        replaceNode = eraseNode;
        if (bst->root == eraseNode){
            bst->root = NULL;
        } else if (eraseNode->parent->key > eraseNode->key) {
            eraseNode->parent->left = NULL;
        } else {
            eraseNode->parent->right = NULL;
        }
    }
    eraseNode->key = replaceNode->key;
    free(replaceNode);
    return 0;
}

void delete_bstree(bstree *bst){
    if (bst->root) {postOrderTraverse(bst->root);}
    free(bst);
}

void postOrderTraverse(node_t *deleteNode){
    if (deleteNode->left) {postOrderTraverse(deleteNode->left);}
    if (deleteNode->right) {postOrderTraverse(deleteNode->right);}
    free(deleteNode);
}

int bstree_to_array(const bstree *bst, key_t *arr, const size_t n){
    node_t *rootNode = bst->root;
    inOrderTraverse(rootNode, arr, 0, n);
    return 0;
}

size_t inOrderTraverse(node_t *treeNode, key_t *arr, size_t i, size_t n){
    if (i == n) {return n;}
    if (treeNode->left) {i=inOrderTraverse(treeNode->left, arr, i, n);}
    *(arr+i++) = treeNode->key;
    if (treeNode->right) {i=inOrderTraverse(treeNode->right, arr, i, n);}
    return i;
}
