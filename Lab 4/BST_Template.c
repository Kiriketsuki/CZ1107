//////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>

///////////////////////////////////////////////////////////////////////

typedef struct _btnode{
int item;
struct _btnode *left;
struct _btnode *right;
} BTNode;

///////////////////////////////////////////////////////////////////////

void insertBSTNode(BTNode **node, int value);
void printBSTInOrder(BTNode *node);
int isBST(BTNode *node, int min, int max);
BTNode *removeBSTNode(BTNode *node, int value);
BTNode *findMin(BTNode *p);

///////////////////////////////////////////////////////////////////////

int main(){
	int i=0;

	BTNode *root=NULL;

	//question 1
	do{
		printf("input a value you want to insert(-1 to quit):");

		scanf("%d",&i);
		if (i!=-1)
			insertBSTNode(&root,i);
	}while(i!=-1);

	//question 2
	printf("\n");
	printBSTInOrder(root);

	// question 3
	if ( isBST(root,-1000000, 1000000)==1)
		printf("It is a BST!\n");
	else
		printf("It is not a BST!\n");

	//question 4
	do{
		printf("\ninput a value you want to remove(-1 to quit):");
		scanf("%d",&i);
		if (i!=-1)
		{
			root=removeBSTNode(root,i);
			printBSTInOrder(root);
		}
	}while(i!=-1);


	return 0;
}

//////////////////////////////////////////////////////////////////////

void insertBSTNode(BTNode **node, int value)
{
	if (*node == NULL) {
        *node = malloc(sizeof(BTNode));
        (*node)->item = value;
        (*node)->right = NULL;
        (*node)->left = NULL;
    } else {
        if (value < (*node)->item) {
            insertBSTNode(&((*node)->left), value);
        } else if (value > (*node)->item) {
            insertBSTNode(&((*node)->right), value);
        } else {
            return; //so the bst doesnt have duplicates
        }
    }
}

//////////////////////////////////////////////////////////////////////

void printBSTInOrder(BTNode *node)
{
	if (node == NULL) {
        return;
    }

    printBSTInOrder(node->left);
    printf("%d ", node->item);
    printBSTInOrder(node->right);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int isBST(BTNode *node, int min, int max) // the item stored in node has to be smaller than max and larger than min
{
	int to_return = 0;

    if (node == NULL) {
        to_return = 1;
        return to_return;
    }

    int curr = node->item;
    if (node->left != NULL) {
        if (node->left->item >= curr) {
            return to_return;
        } else {
            return isBST(node->left, min, curr);
        }
    }

    if (node->right != NULL) {
        if (node->right->item <= curr) {
            return to_return;
        } else {
            return isBST(node->right, curr, max);
        }
    }

    if (to_return == 0) { //if the recursion makes it through the first 2 left right checkers and doesn't return 0, it probs is a BST so just return 1 to break the recursion;
        return 1;
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

BTNode *removeBSTNode(BTNode *node, int value)
{
    BTNode *temp;
	if (node == NULL) {
        printf("Value not found in BST/n");
        return NULL;
    }

    if (node->item == value) {
        if (node->left != NULL && node->right != NULL) { // 2 children
            temp = findMin(node->right); //finding successor
            node->item = temp->item; //swapping with successor
            node->right = removeBSTNode(node->right, temp->item); 
        } else {
            temp = node;
            if (node->left != NULL) { //swapping with left child
                node = node->left;
            } else {
                node = node->right; //swapping with right child or right == NULL
            }

            free(temp);
        }
    } else if (node->item > value) {
        node->left = removeBSTNode(node->left, value);
    } else {
        node->right = removeBSTNode(node->right, value);
    }

    return node;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

BTNode *findMin(BTNode *p)
{
	if (p->left == NULL) {
        return p;
    } else {
        findMin(p->left);
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
