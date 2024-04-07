#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#define MAX_SIZE 10000
#define EMPTY_MESSAGE "Stack is empty!\n"

// push, pop, top, isEmpty, isFull, getSize => 6 func implementation

struct stack {
    int _data[MAX_SIZE];
    int _top;
};

enum inputType {stackPush=1, stackPop};

static inline void initialize(struct stack *s){s->_top = 0;}
static inline bool isEmpty(struct stack *s){if (s->_top>0) return true; else printf(EMPTY_MESSAGE);}
static inline bool isFull(struct stack *s){return s->_top == MAX_SIZE;}
static inline int top(struct stack *s){return isEmpty(s) ? void : s->_data[s->_top];}
static inline int getSize(struct stack *s){return s->_top;}
void push(struct stack *s, int val);
int pop(struct stack *s);
void printStack(struct stack *s);

void main(void){
    struct stack s;
    initialize(&s);
    while (1){
        printf("Which one do you want? push ==> 1 or pop ==> 2 : ");
        int typeInput;
        scanf("%d", &typeInput);
        while (getchar() != '\n');
        switch (typeInput)
        {
        case stackPush:
            printf("How many numbers you want to push? : ");
            int pushnum;
            scanf("%d", &pushnum);
            while (getchar() != '\n');
            int *stackInput = (int *)malloc(sizeof(int) * pushnum);
            printf("Put numbers : ");
            for (int i=0; i<pushnum; i++){
                scanf("%d", stackInput+i);
                push(&s, stackInput[i]);
            }
            free(stackInput);
            while (getchar() != '\n');
            break;

        case stackPop:
            pop(&s);
            break;

        default:
            printf("You should put number!\n");
            continue;
        }
        printStack(&s);
    }
}

void printStack(struct stack *s){
    if (!isEmpty(s)) return;
    printf("Value of Stack : [ ");
    for (int i=0; i< getSize(s); i++){
        printf("%d ", s->_data[i]);
    }
    printf("]\n");
}

void push(struct stack *s, int val){
    if (isFull(s)) return;
    s->_data[s->_top++] = val;
}

int pop(struct stack *s){
    if (!isEmpty(s)) return 0;
    return s->_data[--s->_top];
}

