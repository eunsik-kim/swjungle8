#include "linkedlist.h"

int main(void){
    LList *lst;
    Node *n1, *n2;
    
    n1 = initNode(1);
    n2 = initNode(2);
    lst = initLList();
    // inseart test
    insertNode(lst, n1);
    insertNode(lst, n2);
    printLList(lst);
    printf("Total %d Nodes exist\n", getSize(lst));
    // delete test
    deleteNode(lst, n2->_data);
    printLList(lst);
    deleteNode(lst, n1->_data);
    printLList(lst);
    deleteNode(lst, 2);
    printf("Total %d Nodes exist\n", getSize(lst));
    // update test
    Node *n3, *n4;
    n3 = initNode(3);
    n4 = initNode(4);
    insertNode(lst, n3);
    insertNode(lst, n4);
    updateNode(lst, 3, 5);
    printLList(lst);
    updateNode(lst, 6, 4);
    printLList(lst);
    // meemory test
    for (int i=0; i<100; i++){
        Node *a;
        a = initNode(i);
        inseartNode(lst, a);
    }
    printLList(lst);
    deleteNode(lst, 50);
    deleteNode(lst, 5);
    deleteNode(lst, 5);
    deleteNode(lst, 99);
    printLList(lst);
    deleteList(lst);
    
    return 0;
}
