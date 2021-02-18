////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>

////////////////////////////////////////////////////////////////////


typedef struct _btnode{
	int item;
	struct _btnode *left;
	struct _btnode *right;

} BTNode;

////////////////////////////////////////////////////////////////////


void mirrorTree(BTNode *node);

void printSmallerValues(BTNode *node, int m);
int smallestValue(BTNode *node);
int hasGreatGrandchild(BTNode *node);
int maxDepth(BTNode *node);
void printTree_InOrder(BTNode *node);

////////////////////////////////////////////////////////////////////

int main(int argc, const char * argv[]){

	int i;
	BTNode *root, *root2;
	BTNode btn[15];

	// Create the tree in Q1
	// Using manual dynamic allocation of memory for BTNodes

	root = malloc(sizeof(BTNode));
	root->item = 4;

	root->left = malloc(sizeof(BTNode));
	root->left->item = 5;

	root->right = malloc(sizeof(BTNode));
	root->right->item = 2;

	root->left->left = NULL;

	root->left->right = malloc(sizeof(BTNode));
	root->left->right->item = 6;

	root->left->right->left = NULL;
	root->left->right->right = NULL;

	root->right->left = malloc(sizeof(BTNode));
	root->right->left->item = 3;

	root->right->right = malloc(sizeof(BTNode));
	root->right->right->item = 1;

	root->right->left->left = NULL;

	root->right->left->right = NULL;

	root->right->right->left = NULL;

	root->right->right->right = NULL;

	printTree_InOrder(root);
	printf("\n");
	mirrorTree(root);
	printTree_InOrder(root);
	printf("\n\n");

	//question 2
	printf("\nInput m for question 2: ");
	scanf("%d", &i);
	printf("The values smaller than %d are: ", i);
	printSmallerValues(root, i);
	printf("\n\n");

	//question 3
	printf("The smallest value in the tree is: %d\n", smallestValue(root));

	//question 4
	// Create a tree for Q4: Tall enough so some nodes have great-grandchildren
	// Use array of BTNodes, create tree by linking nodes together
	for (i = 0; i <= 6; i++){
		btn[i].item = i;
		btn[i].left = &(btn[i * 2 + 1]);
		btn[i].right = &(btn[i * 2 + 2]);
	}

	for (i = 7; i <= 14; i++){
		btn[i].item = i;
		btn[i].left = NULL;
		btn[i].right = NULL;
	}
	root2 = &btn[0];

	printf("The tree for question 4 visited by in-order is \n");
	printTree_InOrder(root2);
	printf("\nThe values of the nodes in the tree that have at least one great-grandchild are: \n");

	hasGreatGrandchild(root2);

	return 0;
}

void mirrorTree(BTNode *node){

    if (node == NULL) {
        return;
    }

    BTNode *temp;
    temp = node->left;
    node->left = node->right;
    node->right = temp;

    if (node->left != NULL) {
        mirrorTree(node->left);
    }

    if (node->right != NULL) {
        mirrorTree(node->right);
    }
}

int hasGreatGrandchild(BTNode *node){
    int curr_depth = maxDepth(node);

    if (curr_depth >= 3) {
        printf("%d ", node->item);
    }

    if (node->left != NULL) {
        hasGreatGrandchild(node->left);
    }

    if (node->right != NULL) {
        hasGreatGrandchild(node->right);
    }
}

int maxDepth(BTNode *node)
{
    int to_return = 0;
    int left = 1;
    int right = 1;
    int max;

    if (node->left == NULL && node->right == NULL) {
        return to_return;
    } 
    
    if (node->left != NULL) {
        left += maxDepth(node->left);
    }

    if (node->right != NULL) {
        right += maxDepth(node->right);
    }

    if (right > left) {
        max = right;
    } else {
        max = left;
    }

    to_return += max;
    return to_return;
}

void printSmallerValues(BTNode *node, int m){
    if (node == NULL) {
        return;
    }

    if (node->item < m) {
        printf("%d ", node->item);
    }

    if (node->left != NULL) {
        printSmallerValues(node->left, m);
    }

    if (node->right != NULL) {
        printSmallerValues(node->right, m);
    }

}

int smallestValue(BTNode *node) {
	int l, r;

    if (node == NULL) {
        return 0;
    }

    if (node->left == NULL && node->right == NULL) {
        return node->item;
    }

    if (node->left != NULL) {
        l = smallestValue(node->left);
    }

    if (node->right != NULL) {
        r = smallestValue(node->right);
    }

    if (l < r) {
        return l;
    } else {
        return r;
    }
}


//////////////////////////////////////////////////////////////////

void printTree_InOrder(BTNode *node){

	if (node == NULL) return;
	printTree_InOrder(node->left);
	printf("%d ", node->item);
	printTree_InOrder(node->right);
	return;
}
