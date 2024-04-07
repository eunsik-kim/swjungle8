#include "linkedlist.h"
// init, insert, delete, search, update, getSize, isEmpty, printList => 9 func implementation

void printLList(LList *lst){
    printf("Linked List elements = [ ");
    int cnt = 0;
    Node *node = lst->_head;
    while (node && ++cnt && node != lst->_tail){
        printf("%d ", node->_data);
        node = node->_back;
    }
    if (node){
        printf("%d ] total nodes : %d\n", node->_data, cnt);
    } else{
        printf("] total nodes : %d\n", cnt);
    }
    
}

LList *initLList(void){
    LList *lst = (LList *) malloc(sizeof(LList));
    lst->_head = NULL;
    lst->_tail = NULL;
    return lst;
}

Node *initNode(int data){
    Node *node = (Node *) malloc(sizeof(Node));
    node->_data = data;
    node->_front = NULL;
    node->_back = NULL;
    return node;
}

Node *insertNode(LList *lst, Node *lastNode){
    if (!lst->_head) lst->_head = lastNode;
    Node *originLastNode = lst->_tail;
    lst->_tail = lastNode;
    if (originLastNode){
        originLastNode->_back = lastNode;
        lastNode->_front = originLastNode;
    }
    printf("%d Node has been inserted\n", lastNode->_data);
    return lastNode;
}

int deleteNode(LList *lst, int key){
    if (isEmpty(lst)) return 0;
    Node *node = searchNode(lst, key);
    if (!node) return 0;
    if (lst->_head == lst->_tail) {
        lst->_head = NULL;
        lst->_tail = NULL;
    } else if (node == lst->_head){
        lst->_head = node->_back;
        if (node->_back){
            node->_back->_front = NULL;
        }
    } else if (node->_back){
        node->_back->_front = node->_front;
        node->_front->_back = node->_back; 
    } else {
        lst->_tail = node->_front;
        node->_front->_back = NULL;
    }
    free(node);
    return key;
}

void deleteList(LList *lst){
    if (!isEmpty(lst)){
        Node *node = lst->_head;
        while ((node = lst->_head) != NULL){
            lst->_head = node->_back;
            free(node);
        }
    }
    free(lst);
}


Node *searchNode(LList *lst, int key){
    Node *node = lst->_head;
    do {if (node->_data == key) return node;
        node = node->_back;
    } while (node != NULL);
    printf("%d Node doesn't exist\n", key);
    return NULL;
}

void updateNode(LList *lst, int key, int new_key){
    if (isEmpty(lst)) return;
    Node *node = searchNode(lst, key);
    if (node){
        node->_data = new_key;
        printf("%d Node has been changed into %d Node\n", key, new_key);
    }
}

int getSize(LList *lst){
    int cnt = 0;
    Node *node = lst->_head;
    while (node && ++cnt && node != lst->_tail){
        node = node->_back;
    }
    return cnt;
}

bool isEmpty(LList *lst){
    if ((lst != NULL) && (lst ->_head != NULL)){
        return false;
    } 
    return true;}
