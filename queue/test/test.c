#include "../../linkedlist/linkedlist.h"
#include <assert.h>
#include <stddef.h>

void append(LList *lst, Node *lastNode);
int pop(LList *lst);

void init_list_test(void){
    LList *lst = initLList();
    assert(lst->_head == NULL);
    assert(lst->_tail == NULL);
    deleteList(lst);
}

void init_node_test(int key){
    LList *lst = initLList();
    Node *node = initNode(key);
    assert(node->_front == NULL);
    assert(node->_back == NULL);
    append(lst, node);
    assert(lst->_head == node);
    assert(lst->_tail == node);
    assert(node->_front == NULL);
    assert(node->_back == NULL);
    assert(node->_data == key);
    deleteList(lst);
}

void node_insert(LList *lst, const int *node_key, size_t n){
    for (int i =0; i<n; i++){
        Node *node = initNode(node_key[i]);
        append(lst, node);
        assert(lst->_tail == node);
    }
}

void delete_fixnode_test(){
    LList *lst = initLList();
    const int node_key[] = {32, 25, 323, 78, 56, 97, 25, 73, 99, 1268};
    size_t n = sizeof(node_key) / sizeof(node_key[0]);
    node_insert(lst, node_key, n);
    assert(getSize(lst) == n);
    for (int i = n-1; i>=0; i--){
        int lastNodeKey = pop(lst);
        assert(lastNodeKey == node_key[i]);
    }
    assert(pop(lst) == 0);
    assert(lst->_head == NULL);
    assert(lst->_tail == NULL);
    deleteList(lst);
}

void delete_randnode_test(int n){
    LList *lst = initLList();
    int *node_key = (int *)malloc(sizeof(int)*n);
    for (int i=0; i<n; i++){
        node_key[i] = rand();
    }
    node_insert(lst, node_key, n);
    assert(getSize(lst) == n);
    for (int i = n-1; i>=0; i--){
        int lastNodeKey = pop(lst);
        assert(lastNodeKey == node_key[i]);
    }
    assert(pop(lst) == 0);
    assert(lst->_head == NULL);
    assert(lst->_tail == NULL);
    deleteList(lst);
    free(node_key);
}

int main(void){
    init_list_test(); // LList initialize test
    init_node_test(35); // node initialize & append test
    delete_fixnode_test(); // node delete test
    delete_randnode_test(1000); // random node append & delete randomly 
    printf("All test passed!");
    return 0;
}