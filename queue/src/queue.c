#include "../../linkedlist/linkedlist.h"
// init, delete, getSize, isEmpty, printList from Linkedlist 
// ++ pop, append(which is similar to insert)
void append(LList *lst, Node *lastNode){
    if (!lst->_head) lst->_head = lastNode;
    Node *originLastNode = lst->_tail;
    lst->_tail = lastNode;
    if (originLastNode){
        originLastNode->_back = lastNode;
        lastNode->_front = originLastNode;
    }
}

int pop(LList *lst){
    if (isEmpty(lst)) return 0;
    Node *lastNode = lst->_tail;
    int Key = lastNode->_data;
    if (lastNode->_front){
        lst->_tail = lastNode->_front;
        lastNode->_front->_back = NULL;
    } else {
        lst->_tail = NULL;
        if (lastNode == lst->_head){
            lst->_head = NULL;
        }
    } 
    free(lastNode);
    return Key;
}
