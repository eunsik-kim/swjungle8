#ifndef _linkedlist_
#define _linkedlist_
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

typedef struct Node{
    struct Node *_front;
    struct Node *_back;
    int _data;
} Node;

typedef struct linkedlist{
    Node *_head;
    Node *_tail;
} LList;

Node *initNode(int data);
LList *initLList(void); // Empty Linked list exists

Node *insertNode(LList *lst, Node *lastNode);
void printLList(LList *lst);
int deleteNode(LList *lst, int key);
void deleteList(LList *lst);
Node *searchNode(LList *lst, int key);
void updateNode(LList *lst, int key, int new_key);
int getSize(LList *lst);
bool isEmpty(LList *lst);

#endif