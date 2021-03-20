#include <stdio.h>
#include <stdlib.h>

#define SIZE 80 //The size of the array

enum ExpType {OPT,OPERAND};

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

void insertNode(LinkedList *llPtr, int item, enum ExpType type);
int deleteNode(LinkedList *llPtr);
void removeAllNodes(LinkedList *llPtr);
int isEmptyLinkedList (LinkedList ll);

void expressionLL(char* infix, LinkedList *inExpLL);

void printExpLL(LinkedList inExpLL, int seed);

int main()
{
    char infix[SIZE];

    printf("Enter an infix expression:\n");
    gets(infix);

    LinkedList inExpLL;
    inExpLL.head = NULL;
    inExpLL.size = 0;

    expressionLL(infix, &inExpLL);

    int seed;
    printf("Enter a seed: \n");
    scanf("%d",&seed);
    printExpLL(inExpLL, seed);

    removeAllNodes(&inExpLL);
    return 0;
}


// void expressionLL(char* infix, LinkedList *inExpLL)
// {
//     char *in_ptr = infix;
//     int length = 0;
//     int to_insert = 0;
//     int prev_insert = 0; // check latest insert. 0 is alnum, 1 is operator.

    
//     for (in_ptr; *in_ptr != '\0'; in_ptr++) {
//         printf("currently pointing at %c\n", *in_ptr);
//         printf("decimal value is %d\n", *in_ptr);

//         if (*in_ptr <= 57 && *in_ptr >= 48) { // 48 is '0', 57 is '9'
//             to_insert *= 10;
//             to_insert += (*in_ptr - '0');
//             prev_insert = 0;
//             printf("alnum!! to_insert is at %d\n", to_insert);
//         } else {
//             if (!prev_insert) {
//                 printf("else! to_insert is at %d\n", to_insert);
//                 insertNode(inExpLL, to_insert, OPERAND);
//                 printf("inserted %d\n", to_insert);
//             }   
//             insertNode(inExpLL, *in_ptr, OPT);
//             printf("inserted %c\n", (char)*in_ptr);
//             to_insert = 0;
//             prev_insert = 1;
//             // printf("to_insert is at %d\n", to_insert);
//         }
//     }
//     if (!prev_insert) {
//         insertNode(inExpLL, to_insert, OPERAND);
//     }
// }

void expressionLL(char* infix, LinkedList *inExpLL) {
    char *in_ptr = infix;
    int to_insert = 0;
    int multiplier = 1;
    int prev_scan = 1; // check latest scan. 0 is alnum, 1 is operator
    int temp_int;
    char temp_char;
    int str_len = 0;

    for (in_ptr; *in_ptr != '\0'; in_ptr++) {
        str_len++;
    }

    in_ptr--;

    for (str_len; str_len != 0; str_len--) {
        if (*in_ptr <= 57 && *in_ptr >= 48) { // alnum
            temp_int = *in_ptr - '0';
            temp_int *= multiplier;
            to_insert += temp_int;
            multiplier *= 10;
            prev_scan = 0;
        } else {
            temp_char = *in_ptr;
            if (!prev_scan) { // last scan was alnum
                insertNode(inExpLL, to_insert, OPERAND);
            }

            insertNode(inExpLL, temp_char, OPT);
            to_insert = 0;
            multiplier = 1;
            prev_scan = 1;

        }
        in_ptr--;
    }

    if (!prev_scan) {
        insertNode(inExpLL, to_insert, OPERAND);
    }
}

void printExpLL(LinkedList inExpLL, int seed)
{
    ListNode* temp = NULL;
    temp = inExpLL.head;
    while(temp!= NULL){
        if(temp->type == OPERAND)
            printf(" %d ",temp->item+ seed);
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
    if(LLPtr==NULL || LLPtr->size==0){ //Queue is empty or NULL pointer
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
