#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 80 //The size of the array

enum ExpType {OPT,OPERAND};

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

typedef struct _listnode
{
    int  item;
    enum ExpType type;
	struct _listnode *next;
} ListNode;

typedef struct _linkedlist{
   int size;
   ListNode *head;
} LinkedList;

void insertNode(LinkedList *llPtr, int item,enum ExpType type);
int deleteNode(LinkedList *llPtr);
void removeAllNodes(LinkedList *llPtr);
int isEmptyLinkedList (LinkedList ll);


void in2PreLL(char* infix, LinkedList *inExpLL);

void printExpLL(LinkedList inExpLL);

int main()
{
    char infix[SIZE];

    printf("Enter an infix expression:\n");
    gets(infix);

    LinkedList inExpLL;
    inExpLL.head = NULL;
    inExpLL.size = 0;

    in2PreLL(infix, &inExpLL);

    printExpLL(inExpLL);

    removeAllNodes(&inExpLL);
    return 0;
}

int precedence_checker(char operator) {
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

// void in2PreLL(char* infix, LinkedList *inExpLL)
// {
//     Stack stackackack;
//     stackackack.head = NULL;
//     stackackack.size = 0;

//     char *in_ptr = infix;
//     char scanned;
//     int scanned_int;
//     int str_len = 0;
//     int to_insert = 0;
//     int multiplier = 1;
//     int prev_scan = 1; //0 for alnum, 1 for operator

//     for (in_ptr; *in_ptr != '\0'; in_ptr++) {
//         str_len++;
//     }
//     in_ptr--;

//     do {
//         scanned = *in_ptr;

//         if (scanned <= 57 && scanned >= 48) { // is numeric
//             scanned = scanned - '0';
//             scanned_int = (int)scanned;
//             scanned_int *= multiplier;
//             printf("scanned_int is %d and multiplier is %d\n", scanned_int, multiplier);
//             to_insert += scanned_int;
//             printf("to_insert is %d\n", to_insert);
//             multiplier *= 10;
//             prev_scan = 0;

//         } else {
//             printf("to_insert is %d\n", to_insert);
//             if (!prev_scan) { // if the previous scan was alnum
//                 insertNode(inExpLL, to_insert, OPERAND);
//             }
            
//             if (scanned == '(') {
//                 while (peek(stackackack) != ')') {
//                     insertNode(inExpLL, peek(stackackack), OPT);
//                     pop(&stackackack);
//                 }
//                 pop(&stackackack);
//             } else if (scanned == ')') {
//                 push(&stackackack, scanned);
//             } else {
//                 while (!isEmptyStack(stackackack) && peek(stackackack) != ')' && precedence_checker(peek(stackackack)) > precedence_checker(scanned)) {
//                     insertNode(inExpLL, peek(stackackack), OPT);
//                     pop(&stackackack);
//                 }
//                 push(&stackackack, scanned);
//             }
//             to_insert = 0;
//             multiplier = 1;
//             prev_scan = 1;
//         }
//         in_ptr--;
//         str_len--;

//     } while (str_len != 0);

//     if (!prev_scan) { // if the previous scan was alnum
//         insertNode(inExpLL, to_insert, OPERAND);
//         to_insert = 0;
//         multiplier = 1;
//     }

//     while (!isEmptyStack(stackackack)) {
//         insertNode(inExpLL, peek(stackackack), OPT);
//         pop(&stackackack);
//     }
// }

int precedence(char c)
{
    if (c == '*' || c == '/')
    {
        return 2;
    }

    if (c == '+' || c == '-')
    {
        return 1;
    }

    return 0;
}


void in2PreLL(char* infix, LinkedList *inExpLL)
{
    
    int i = 0;
    int num = 0;
    int prev = 0;
    int index=0;
    char c = infix[i];
    char input[SIZE];


    // TO DO: change to int array not char array
    while (c != '\0')
    {
        printf("c is %c\n", c);
        if (c >= 48 && c <= 57) {
            num = num * 10;
            num = num + (c -'0');
            prev = 1;
            i++;
            c = infix[i];
        } else {
            if (prev) {
                input[index] = num;
                index++;
                num = 0;
            }

            input[index] = c;
            index++;
            prev = 0;
            i++;
            c = infix[i];
        }
        // printf("input index is %c\n", input[index-1]);
    }

    if (prev) {
        input[index] = num;
    } else {
        index--;
    }

    printf("index is %d\n", index);

    // index++;
    // input[index] = '\0';

    // printf("converted string is %s\n", input);
    // index--;
    // printf("last char is %c\n", input[index]);

    Stack s;
    s.head = NULL;
    s.size = 0;
    // index--;

    while (index != -1) {
        c = input[index];
        // printf("c is %c\n", c);

        if (precedence(c) != 0) {
            printf("c is %c\n", c);
            while (!(isEmptyStack(s)) && precedence(peek(s)) > precedence(c)) {
                // printf("popped\n");
                insertNode(inExpLL, peek(s), OPT);
                pop(&s);
            }
            push(&s, c);
            // printf("top of stack is %c\n", peek(s));

        } else if (c == '(') {
            while (peek(s)!= ')') {
                insertNode(inExpLL, peek(s), OPT);
                pop(&s);
            }
            pop(&s); 

        } else if (c == ')') {
            // printf("c is actually )\n");
            push(&s, c);

        } else {
            // printf("c is %d\n", c);
            insertNode(inExpLL, c, OPERAND);
        }

        index--;
    }

    while (!isEmptyStack(s)) {
        insertNode(inExpLL, peek(s), OPT);
        pop(&s);
    }
}


void printExpLL(LinkedList inExpLL)
{
    ListNode* temp = NULL;
    temp = inExpLL.head;
    while(temp!= NULL){
        if(temp->type == OPERAND)
            printf(" %d ",temp->item);
        else
            printf(" %c ",(char)(temp->item));
        temp = temp->next;
    }
    printf("\n");
}

void insertNode(LinkedList *LLPtr, int item, enum ExpType type) {
    ListNode *newNode;
    newNode = malloc(sizeof(ListNode));
    if(newNode==NULL) exit(0);

    newNode->item = item;
    newNode->type = type;
    newNode->next = LLPtr->head;
    LLPtr->head=newNode;
    LLPtr->size++;
}

int deleteNode(LinkedList *LLPtr) {
    if(LLPtr==NULL || LLPtr->size==0){
        return 0;
    }
    else{
       ListNode *temp = LLPtr->head;
       LLPtr->head = LLPtr->head->next;

       free(temp);
       LLPtr->size--;
       return 1;
    }
}

int isEmptyLinkedList (LinkedList ll) {
    if(ll.size==0) return 1;
    else return 0;
}

void removeAllNodes(LinkedList *LLPtr)
{
	while(deleteNode(LLPtr));
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
