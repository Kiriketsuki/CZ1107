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

    printf("Enter an prefix expression:\n");
    gets(prefix);

    createExpTree(&root, prefix);

    printf("\nThe Infix Expression:\n");
    printTree(root);
    printf("\n");
    printf("The Postfix (Reverse Polish) Expression:\n");
    printTreePostfix(root);
    printf("\n");
    printf("Answer ");
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

int is_operator(char input) {
    if (input == '*' || input == '/' || input == '+' || input == '-' || input == '^') {
        return 1;
    }
    return 0;
}

void createExpTree(BTNode** root,char* prefix)
{
    char *pre_ptr = prefix;
    int temp_array[SIZE];
    int to_insert = 0;
    int index = 0;
    int prev_scan = 1;
    BTNode *curr = malloc(sizeof(BTNode));
    BTNode *tree_head = curr;
    Stack prev_nodes;
    prev_nodes.head = NULL;
    prev_nodes.size = 0;

    // for (pre_ptr; *pre_ptr != '\0'; pre_ptr++) {
    //     if (*pre_ptr == ' ' && !update) {
    //         continue;
    //     } else if (*pre_ptr >= 48 && *pre_ptr <= 57) {
    //         to_insert *= 10;
    //         to_insert += *pre_ptr - '0';
    //         // printf("to insert is %d\n", to_insert);
    //         prev_scan = 0;
    //         update = 1;
    //     } else {
    //         if (!prev_scan && *pre_ptr == ' ') {
    //             temp_array[index] = to_insert;
    //             // printf("inserted %c\n", to_insert);
    //             to_insert = 0;
    //             index++;
    //             update = 1;
    //         }

    //         if (*pre_ptr != ' ') {
    //             temp_array[index] = *pre_ptr;
    //             // printf("temp array is %c\n", temp_array[index]);
    //             index++;
    //         }

    //         prev_scan = 1;
    //         update = 1;
    //     }
    // }

    for (pre_ptr; *pre_ptr != '\0'; pre_ptr++) {
        if (*pre_ptr == ' ') {
            if (prev_scan == 0) {
                temp_array[index] = to_insert;
                index++;
                to_insert = 0;
            }

            prev_scan = 2;
        } else if (is_operator(*pre_ptr)) {
            if (prev_scan == 0) {
                temp_array[index] = to_insert;
                index++;
                to_insert = 0;
            }

            temp_array[index] = *pre_ptr;
            index++;
            prev_scan = 1;
        } else {
            to_insert *= 10;
            to_insert += *pre_ptr - '0';
            prev_scan = 0;
        }
    }

    if (!prev_scan) {
        temp_array[index] = to_insert;
    } else {
        index--;
    }

    // printf("index is %d\n", index);

    // for (int i = 0; i < index+1; i++) {
    //     printf("temp array is %c ", temp_array[i]);
    // }
    int *temp_ptr = temp_array;

    for (index; index != -1; index--) {
        if (*temp_ptr == ' ') {
            continue;
        }

        insert_node(&curr, *temp_ptr);

        if (is_operator(curr->item)) {
            // printf("isnt alnum, curr item is %c\n", curr->item);
            push(&prev_nodes, curr);
            // printf("top of stack is %c\n", peek(prev_nodes)->item);
            curr->right = malloc(sizeof(BTNode));
            curr->left = malloc(sizeof(BTNode));
            curr = curr->left;
        } else {
            // printf("is alnum, curr item is %c\n", curr->item);
            if (!isEmptyStack(prev_nodes)) {
                curr = peek(prev_nodes)->right;
                pop(&prev_nodes);
            }
        }
        temp_ptr++;
        // printf("pre ptr is %c\n", *pre_ptr);
    }
    // printf("exited for loop \n");
    // printTree(tree_head);
    // printf("\n");
    //  +99    *         -  88  77     +/  66-55       44  1000 
    *root = tree_head;
}

void printTree(BTNode *node){
    if (node == NULL) {
        return;
    }

    printTree(node->left);
    if (is_operator(node->item)) {
        printf("%c ", node->item);
    } else {
        printf("%d ", node->item);
    }
    printTree(node->right);
}


void printTreePostfix(BTNode *node){
    Stack stackack;
    Stack stackackack;
    BTNode *curr;
    stackack.head = NULL;
    stackack.size = 0;
    stackackack.head = NULL;
    stackackack.size = 0;

    if (node == NULL) {
        return;
    }

    push(&stackack, node);

    while (!isEmptyStack(stackack)) {
        curr = peek(stackack);
        // printf("curr is %c\n", curr->item);
        // printf("curr left is %c\n", curr->left->item);
        pop(&stackack);
        push(&stackackack, curr);

        if (curr->left != NULL) {
            // printf("entered\n");
            push(&stackack, curr->left);
        }

        if (curr->right != NULL) {
            // printf("entered too\n");
            push(&stackack, curr->right);
        }
    }

    while (!isEmptyStack(stackackack)) {
        if (is_operator(peek(stackackack)->item)) {
            printf("%c ", peek(stackackack)->item);
        } else {
            printf("%d ", peek(stackackack)->item);
        }
        
        pop(&stackackack);
    }
}

double computeTree(BTNode *node){
    if (node == NULL) {
        return 0;
    }

    double left;
    double right;

    if (is_operator(node->item)) {
        left = computeTree(node->left);
        right = computeTree(node->right);

        switch (node->item) {
            case '*':
                return left * right;
            case '/':
                return left / right;
            case '+':
                return left + right;
            case '-':
                return left - right;
            default:
                return 0;
        }
    } else {
        return (node->item);
    }
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