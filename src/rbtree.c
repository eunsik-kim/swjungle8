#include "rbtree.h"
#include <stdlib.h>

// prototype
typedef enum {LEFT, RIGHT} dir; // direction to look when going down
void switchChildParent(rbtree *rbt, node_t *childNode, node_t *parentNode, node_t *grparentNode);
void insert_color_fixing(rbtree *rbt, node_t *childNode);
void postOrderTraverse(rbtree *rbt, node_t *deleteNode);
node_t *subtree_min_max(const rbtree *rbt, node_t *subTreeNode, dir direction);
size_t inOrderTraverse(const rbtree *rbt, node_t *treeNode, key_t *arr, size_t i, size_t n);
void rearrange_tree(rbtree *rbt, node_t *replaceNode, dir direction);
//
rbtree *new_rbtree(void)
{
	rbtree *rbt = (rbtree *)malloc(sizeof(rbtree));
	node_t *nilNode = (node_t *)malloc(sizeof(node_t));
	nilNode->color = RBTREE_BLACK;
	rbt->nil = nilNode;
	rbt->root = nilNode;
	return rbt;
}

void delete_rbtree(rbtree *rbt)
{
	if (rbt->root != rbt->nil)
		postOrderTraverse(rbt, rbt->root);
	free(rbt->nil);
	free(rbt);
}

void postOrderTraverse(rbtree *rbt, node_t *deleteNode)
{
	if (deleteNode != rbt->nil){
		postOrderTraverse(rbt, deleteNode->left);
		postOrderTraverse(rbt, deleteNode->right);
		free(deleteNode);
	}
}

node_t *rbtree_insert(rbtree *rbt, const key_t key)
{
	node_t *insertNode = (node_t *)malloc(sizeof(node_t));
	insertNode->key = key;
	insertNode->left = rbt->nil;
	insertNode->right = rbt->nil;
	insertNode->color = RBTREE_RED;
	// find a position
	node_t *parentNode = rbt->root;
	while (parentNode != rbt->nil)
	{
		if (parentNode->key <= key)
		{
			if (parentNode->right != rbt->nil)
				parentNode = parentNode->right;
			else
			{
				parentNode->right = insertNode;
				break;
			}
		}
		else
		{
			if (parentNode->left != rbt->nil)
				parentNode = parentNode->left;
			else
			{
				parentNode->left = insertNode;
				break;
			}
		}
	}
	// set a color of nodes
	if (rbt->root == rbt->nil)
	{
		rbt->root = insertNode;
		insertNode->parent = rbt->nil;
		insertNode->color = RBTREE_BLACK;
	}
	else
	{
		insertNode->parent = parentNode;
		insert_color_fixing(rbt, insertNode);
	}
	return insertNode;
}

void insert_color_fixing(rbtree *rbt, node_t *childNode)
{
	node_t *parentNode = childNode->parent;
	// case 1: a color of the parentNode is black
	if (parentNode->color == RBTREE_BLACK)
	{
		if (parentNode->left == childNode)
			parentNode->left = childNode;
		else
			parentNode->right = childNode;
		return;
	}
	node_t *grparentNode = parentNode->parent;
	node_t *uncleNode = (grparentNode->left == parentNode) ? grparentNode->right : grparentNode->left;
	// case 2: a color of the uncleNode is RED
	if (uncleNode->color == RBTREE_RED)
	{
		parentNode->color = RBTREE_BLACK;
		uncleNode->color = RBTREE_BLACK;
		grparentNode->color = (rbt->root == grparentNode) ? RBTREE_BLACK : RBTREE_RED;
		// recursive fixing until the parentNode is black(before reaching rootNode)
		if (grparentNode->parent->color == RBTREE_RED)
			insert_color_fixing(rbt, grparentNode);
	}
	else
	{ // case 3: a color of the uncleNode is black
		if ((grparentNode != rbt->nil) && ((grparentNode->left == parentNode) != (parentNode->left == childNode)))
		{
			switchChildParent(rbt, childNode, parentNode, grparentNode);
			parentNode = childNode;
		}
		node_t *grgrparentNode = grparentNode->parent;
		switchChildParent(rbt, parentNode, grparentNode, grgrparentNode);
		grparentNode->color = RBTREE_RED;
		parentNode->color = RBTREE_BLACK;
	}
}
// change connection the parentNode with the grparentNode into the childNode with the grparentNode(4 case exists)
void switchChildParent(rbtree *rbt, node_t *childNode, node_t *parentNode, node_t *grparentNode)
{
	// connect the childNode and the grparentNode
	childNode->parent = grparentNode;
	if (parentNode == rbt->root)
		rbt->root = childNode;
	else if (grparentNode->left == parentNode)
		grparentNode->left = childNode;
	else
		grparentNode->right = childNode;
	// change relationship with the parentNode and the childNode
	if (parentNode->left == childNode)
	{
		parentNode->left = childNode->right;
		if (childNode->right != rbt->nil)
			childNode->right->parent = parentNode;
		childNode->right = parentNode;
		parentNode->parent = childNode;
	}
	else
	{
		parentNode->right = childNode->left;
		if (childNode->left != rbt->nil)
			childNode->left->parent = parentNode;
		childNode->left = parentNode;
		parentNode->parent = childNode;
	}
}

node_t *rbtree_find(const rbtree *rbt, const key_t key)
{
	node_t *findNode = rbt->root;
	while ((findNode != rbt->nil) && (findNode->key != key))
	{
		if (findNode->key <= key)
			findNode = findNode->right;
		else
			findNode = findNode->left;
	}
	if (findNode == rbt->nil)
		return NULL;
	return findNode;
}

node_t *rbtree_min(const rbtree *rbt)
{
	node_t *minNode = subtree_min_max(rbt, rbt->root, LEFT);
	return minNode;
}

node_t *rbtree_max(const rbtree *rbt)
{
	node_t *maxNode = subtree_min_max(rbt, rbt->root, RIGHT);
	return maxNode;
}

node_t *subtree_min_max(const rbtree *rbt, node_t *subTreeNode, dir direction)
{
	// travel in one direction and find the last left(right) node
	node_t *minmaxNode = direction ? subTreeNode->right : subTreeNode->left;
	while (minmaxNode != rbt->nil)
	{
		subTreeNode = minmaxNode;
		minmaxNode = direction ? subTreeNode->right : subTreeNode->left;
	}
	return subTreeNode;
}

int rbtree_erase(rbtree *rbt, node_t *eraseNode)
{
	node_t *replaceNode;
	dir direction;
	// find the repalceNode
	if (eraseNode->right != rbt->nil)
		replaceNode = subtree_min_max(rbt, eraseNode->right, LEFT);
	else if (eraseNode->left != rbt->nil)
		replaceNode = subtree_min_max(rbt, eraseNode->left, RIGHT);
	else
	{
		replaceNode = eraseNode;
		if (rbt->root == replaceNode)
		{
			rbt->root = rbt->nil;
			free(eraseNode);
			return 0;
		}
	} // rearrange the tree
	direction = (replaceNode->parent->left == replaceNode) ? LEFT : RIGHT;
	rearrange_tree(rbt, replaceNode, direction);
	// connect the childNode with the parentNode
	if (replaceNode == eraseNode)
	{
		if (direction == LEFT)
			eraseNode->parent->left = rbt->nil;
		else
			eraseNode->parent->right = rbt->nil;
	}
	else if (direction == RIGHT)
	{
		if (replaceNode->parent == eraseNode)
		{
			replaceNode->parent->right = replaceNode->right;
			if (replaceNode->right != rbt->nil)
				replaceNode->right->parent = replaceNode->parent;
		}
		else
		{
			replaceNode->parent->right = replaceNode->left;
			if (replaceNode->left != rbt->nil)
					replaceNode->left->parent = replaceNode->parent;
		}
			
	}
	else
	{	
		if (replaceNode->parent == eraseNode)
		{
			replaceNode->parent->left = replaceNode->left;
			if (replaceNode->left != rbt->nil)
				replaceNode->left->parent = replaceNode->parent;
		}
		else 
		{
			replaceNode->parent->left = replaceNode->right;
			if (replaceNode->right != rbt->nil)
				replaceNode->right->parent = replaceNode->parent;
		}
		
	} // erase replaceNode
	eraseNode->key = replaceNode->key;
	free(replaceNode);
	return 0;
}
// adjust a subtree starting from the replaceNode to satisfy the RB condition
void rearrange_tree(rbtree *rbt, node_t *replaceNode, dir direction)
{
	if (replaceNode->color == RBTREE_RED)
		return;
	// case0 : if replaceNode is red, there's nothing to rearrange
	node_t *parentNode = replaceNode->parent;
	node_t *siblingNode, *nearNpNode, *farNpNode;
	if (direction == LEFT)
	{ // If the replaceNode is black, it's not possible that the siblingNode is nil
		siblingNode = parentNode->right;
		nearNpNode = siblingNode->left;
		farNpNode = siblingNode->right;
	}
	else
	{
		siblingNode = parentNode->left;
		nearNpNode = siblingNode->right;
		farNpNode = siblingNode->left;
	}
	// case1 : a color of the siblingNode is red => recursively call case2(acyclic), case3, case4
	if (siblingNode->color == RBTREE_RED)
	{
		switchChildParent(rbt, siblingNode, parentNode, parentNode->parent);
		siblingNode->color = RBTREE_BLACK;
		parentNode->color = RBTREE_RED;
		rearrange_tree(rbt, replaceNode, direction);
		// case2 : if a color the parentNode is red => end of rearrangement, else recursively call case1(acyclic), case2, case4
	}
	else if ((nearNpNode->color == RBTREE_BLACK) && (farNpNode->color == RBTREE_BLACK))
	{
		siblingNode->color = RBTREE_RED;
		if (parentNode->color == RBTREE_RED)
			parentNode->color = RBTREE_BLACK;
		else if (parentNode != rbt->root)
		{ // root exception
			dir direction = (parentNode->parent->left == parentNode) ? LEFT : RIGHT;
			rearrange_tree(rbt, parentNode, direction);
		}
	}
	else
	{ // case3 : process of converting trees to case4
		if (farNpNode->color == RBTREE_BLACK)
		{
			switchChildParent(rbt, nearNpNode, siblingNode, parentNode);
			farNpNode = siblingNode;
			siblingNode = nearNpNode;
		} // case4 : end of rearrangement
		switchChildParent(rbt, siblingNode, parentNode, parentNode->parent);
		farNpNode->color = RBTREE_BLACK;
		siblingNode->color = parentNode->color;
		parentNode->color = RBTREE_BLACK;
	}
}

int rbtree_to_array(const rbtree *rbt, key_t *arr, const size_t n)
{
	node_t *rootNode = rbt->root;
	inOrderTraverse(rbt, rootNode, arr, 0, n);
	return 0;
}

size_t inOrderTraverse(const rbtree *rbt, node_t *treeNode, key_t *arr, size_t i, size_t n)
{
	if (i == n)
		return n;
	if (treeNode->left != rbt->nil)
		i = inOrderTraverse(rbt, treeNode->left, arr, i, n);
	*(arr + i++) = treeNode->key;
	if (treeNode->right != rbt->nil)
		i = inOrderTraverse(rbt, treeNode->right, arr, i, n);
	return i;
}