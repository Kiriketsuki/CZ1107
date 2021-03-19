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

void in2Pre(char*, char*);

int main()
{
    char infix[SIZE];
    char prefix[SIZE];

    printf("Enter an infix expression:\n");
    gets(infix);

    in2Pre(infix,prefix);
    printf("The prefix expression is \n");
    printf("%s\n",prefix);

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

        return -1;
    }
}

void in2Pre(char* infix, char* prefix)
{
    Stack reverse_stack;
    Stack prefix_stack;
    Stack output_stack;
    reverse_stack.head = NULL;
    reverse_stack.size = 0;
    prefix_stack.head = NULL;
    prefix_stack.size = 0;
    output_stack.head = NULL;
    output_stack.size = 0;
    char *in_ptr = infix;
    char *temp;
    char scanned;
    char *pre_ptr = prefix;
    int str_len = 0;

    do {
        if (*in_ptr == '(') {
            push(&reverse_stack, ')');
        } else if (*in_ptr == ')') {
            push(&reverse_stack, '(');
        } else {
            push(&reverse_stack, *in_ptr);
        }

        in_ptr++;
    } while (*in_ptr != '\0');

    do {
        scanned = peek(reverse_stack);
        pop(&reverse_stack);

        if (isalnum(scanned)) {
            push(&output_stack, scanned);
        } else if (scanned == ')') {
            while (peek(prefix_stack) != '(') {
                push(&output_stack, peek(prefix_stack));
                pop(&prefix_stack);
            }
            pop(&prefix_stack);
        } else if (scanned == '(') {
            push(&prefix_stack, scanned);
        } else {
            while (!isEmptyStack(prefix_stack) && peek(prefix_stack) != '(' && precendence_checker(peek(prefix_stack)) > precendence_checker(scanned)) {
                push(&output_stack, peek(prefix_stack));
                pop(&prefix_stack);
            }
            push(&prefix_stack, scanned);
        }
    } while (!isEmptyStack(reverse_stack));

    while (!isEmptyStack(prefix_stack)) {
        push(&output_stack, peek(prefix_stack));
        pop(&prefix_stack);
    }

    while (!isEmptyStack(output_stack)) {
        *pre_ptr = peek(output_stack);
        pop(&output_stack);
        pre_ptr++;
    }

    *pre_ptr = '\0';
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
