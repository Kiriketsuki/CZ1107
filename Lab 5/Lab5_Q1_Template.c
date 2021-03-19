#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define SIZE 80 //The limit of expression length

typedef struct _stackNode{
    char item;
    struct _stackNode *next;
}StackNode;

typedef struct _stack{
   int size;
   StackNode *head;
}Stack;

void push(Stack *sPtr, char item);
int pop(Stack *sPtr);
char peek(Stack s);
int isEmptyStack(Stack s);

void in2Post(char*, char*);

int main()
{
    char infix[SIZE];
    char postfix[SIZE];

    printf("Enter an infix expression:\n");
    gets(infix);

    in2Post(infix,postfix);
    printf("The postfix expression is \n");
    printf("%s\n",postfix);
    return 0;
}

int precendence_checker(char operator) {
    switch (operator) {
        case '+': {
            return 1;
        }

        case '-': {
            return 1;
        }

        case '*': {
            return 2;
        }

        case '/': {
            return 2;
        }

        case '^': {
            return 3;
        }

        default: {
            return -1;
        }
    }
}


void in2Post(char* infix, char* postfix)
{
    Stack stackackack;
    stackackack.head = NULL;
    stackackack.size = 0;
    char *in_ptr = infix;
    char *post_ptr = postfix;

    for (in_ptr; *in_ptr != '\0'; in_ptr++) {
        if (isalnum(*in_ptr)) {
            *post_ptr = *in_ptr;
            post_ptr++;
        } else if (*in_ptr == ')') {
            while (peek(stackackack) != '(') {
                *post_ptr = peek(stackackack);
                pop(&stackackack);
                post_ptr++;
            }
            pop(&stackackack);
        } else if (*in_ptr == '(') {
            push(&stackackack, *in_ptr);
        } else {
            while (!isEmptyStack(stackackack) && peek(stackackack) != '(' && precendence_checker(peek(stackackack)) >= precendence_checker(*in_ptr)) {
                *post_ptr = peek(stackackack);
                pop(&stackackack);
                post_ptr++;
            }
            push(&stackackack, *in_ptr);
        }
    }

    while (!isEmptyStack(stackackack)) {
        *post_ptr = peek(stackackack);
        pop(&stackackack);
        post_ptr++;
    }

    *post_ptr = '\0';
}

void push(Stack *sPtr, char item){
    StackNode *newNode;
    newNode = malloc(sizeof(StackNode));
    newNode->item = item;
    newNode->next = sPtr->head;
    sPtr->head = newNode;
    sPtr->size++;
}

int pop(Stack *sPtr){
    if(sPtr == NULL || sPtr->head == NULL){
        return 0;
    }
    else{
       StackNode *temp = sPtr->head;
       sPtr->head = sPtr->head->next;
       free(temp);
       sPtr->size--;
       return 1;
    }
}

char peek(Stack s){
    return s.head->item;
}

int isEmptyStack(Stack s){
     if(s.size == 0) return 1;
     else return 0;
}
