#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define SIZE 80 //The limit of expression length

typedef struct _stackNode{
    double item;
    struct _stackNode *next;
}StackNode;

typedef struct _stack{
   int size;
   StackNode *head;
}Stack;

void push(Stack *sPtr, double item);
int pop(Stack *sPtr);
double peek(Stack s);
int isEmptyStack(Stack s);

double exePostfix(char*);

int main()
{
    char postfix[SIZE];

    printf("Enter a postfix expression:\n");
    gets(postfix);

    printf("The answer is %.2lf.\n",exePostfix(postfix));

    return 0;
}


double exePostfix(char* postfix)
{
    char *post_ptr  = postfix;
    Stack stackackack;

    for (post_ptr; *post_ptr != '\0'; post_ptr++) {
        if (isdigit(*post_ptr)) {
            push(&stackackack, (double)(*post_ptr - '0'));
        } else {
            int num_0 = peek(stackackack);
            pop(&stackackack);
            int num_1 = peek(stackackack);
            pop(&stackackack);

            double to_push;
            switch (*post_ptr) {
                case '+':
                    to_push = num_1 + num_0;
                    break;
                case '-':
                    to_push = num_1 - num_0;
                    break;
                case '*':
                    to_push = num_1 * num_0;
                    break;
                case '/':
                    to_push = num_1 / num_0;
                    break;
            }

            push(&stackackack, to_push);
        }
    }

    return peek(stackackack);
}

void push(Stack *sPtr, double item){
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

double peek(Stack s){
    return s.head->item;
}

int isEmptyStack(Stack s){
     if(s.size == 0) return 1;
     else return 0;
}
