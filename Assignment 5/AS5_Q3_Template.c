#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define SIZE 200 //The number digit limitation

typedef struct _btnode{
	int item;
	struct _btnode *left;
	struct _btnode *right;
} BTNode;   // You should not change the definition of BTNode

typedef struct _node{
     BTNode* item;
     struct _node *next;
}StackNode;

typedef struct _stack{
   int size;
   StackNode *head;
}Stack;

void deleteTree(BTNode **root);

void createExpTree(BTNode** root,char* prefix);
void printTree(BTNode *node);
void printTreePostfix(BTNode *node);
double computeTree(BTNode *node);

void push(Stack *sPtr,  BTNode* item);
int pop(Stack *sPtr);
BTNode *peek(Stack s);
int isEmptyStack(Stack s);

int main()
{
    char prefix[SIZE];
    BTNode* root=NULL;

    //printf("Enter an prefix expression:\n");
    gets(prefix);

    createExpTree(&root, prefix);

    // printf("The Infix Expression:\n");
    printTree(root);
    printf("\n");
    //printf("The Postfix (Reverse Polish) Expression:\n");
    printTreePostfix(root);
    printf("\n");
    //printf("Answer ");
    printf("%.2f\n", computeTree(root));
    deleteTree(&root);
    return 0;
}

void insert_node(BTNode **node, int value) {
    if (*node == NULL) {
		*node = malloc(sizeof(BTNode));
    }

	if (*node != NULL) {
        (*node)->item = value;
        (*node)->left = NULL;
        (*node)->right = NULL;
    }
}


void createExpTree(BTNode** root,char* prefix)
{
    char *pre_ptr = prefix;
    BTNode *curr = *root;
    Stack prev_nodes;
    prev_nodes.head = NULL;
    prev_nodes.size = 0;

    for (pre_ptr; *pre_ptr != '\0'; pre_ptr++) {
        if (*pre_ptr == ' ') {
            continue;
        } else if (!isalnum(*pre_ptr)) {
            insert_node(curr, *pre_ptr);
            push(&prev_nodes, curr);
            curr = curr->left;
        } else {
            if (curr == NULL) {
                insert_node(curr, *pre_ptr);
                curr = peek(prev_nodes);
                pop(&prev_nodes);
            } else {
                insert_node(curr->right, *pre_ptr);
                curr = peek(prev_nodes);
                pop(&prev_nodes);
            }
            
        }
    }

}

void printTree(BTNode *node){
    //Write your code here
}


void printTreePostfix(BTNode *node){
   //Write your code here

}

double computeTree(BTNode *node){
//Write your code here
}

void push(Stack *sPtr, BTNode *item){
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

BTNode *peek(Stack s){
    return s.head->item;
}

int isEmptyStack(Stack s){
     if(s.size == 0) return 1;
     else return 0;
}

void deleteTree(BTNode **root){
    BTNode* temp;
    if(*root !=NULL)
    {
        temp = (*root)->right;
        deleteTree(&((*root)->left));
        free(*root);
        *root = NULL;
        deleteTree(&temp);
    }
}