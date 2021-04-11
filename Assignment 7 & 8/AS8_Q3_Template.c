#include <stdio.h>
#include <stdlib.h>

typedef struct _arraynode
{
    int *itemArray; // the sequence
    int sizeArray; // the size of this particular sequence
    struct _arraynode *next;
}ArrayNode;

typedef struct _arraylist{
   int size; // total number of possible sequences
   ArrayNode *head;
} ArrayList;

typedef struct _listnode
{
    int item;
	struct _listnode *next;
} ListNode;

typedef struct _linkedlist{
   int sum; //the number of possible sequences
   int size;
   ListNode *head;
} LinkedList;

// You should not change the prototypes of these functions
//////LinkedList///////////////////////////////////////////
int insertNode(LinkedList *ll, int index, int coin);
int removeNode(LinkedList *ll, int index);
ListNode *findNode(LinkedList ll, int index);
void removeAllItems(LinkedList *ll);
///////////////////////////////////////////////////

void sumToC(LinkedList* ll, int C, ArrayList* al);
int main()
{
    int C;
    printf("Enter a number:\n");
    scanf("%d",&C);

    LinkedList ll;
    ll.head = NULL;
    ll.sum = 0;
    ll.size = 0;
    ArrayList al;
    al.head = NULL;
    al.size = 0;

    sumToC(&ll,C,&al);
	
	//printing all possible sequences
    ArrayNode* temp;
    int i,j;
    temp = al.head;
    for(i=0;i<al.size;i++){
        for(j=0;j<temp->sizeArray;j++)
            printf(" %d ",temp->itemArray[j]);
        printf("\n");
        temp = temp->next;
    }

    return 0;
}

void sumToC(LinkedList* ll, int C, ArrayList* al)
{
    // recursion base case returns

    if (ll->sum > C) {
        removeNode(ll, ll->size - 1); // remove last item in ll then return to previous state to go to next recursion
        return;
    }

    if (ll->size == C && ll->sum != C) {
        removeNode(ll, ll->size - 1); // remove last item in ll then return to previous state to go to next recursion
        return;
    }

    // recursion found solution return

    if (ll->sum == C) {
        // solution found!! increase size of al
        al->size++;

        // create arraynode from linked list
        ListNode *ll_ptr = ll->head;

        // calc how many non zeroes in linked list
        int i = 0;
        while (ll_ptr != NULL) {
            if (ll_ptr->item != 0) {
                i++;
            }
            ll_ptr = ll_ptr->next;
        }

        // i is number of items to add to itemArray
        ArrayNode *linked_list = malloc(sizeof(ArrayNode)); // create array node
        linked_list->sizeArray = 0; // size set to 0
        linked_list->itemArray = malloc(i * sizeof(int)); // allocate size of array, set to how many non zero items needed to be added

        // add items from ll to array node
        ll_ptr = ll->head;
        i = 0;
        while (ll_ptr != NULL) {
            if (ll_ptr->item != 0) {
                linked_list->itemArray[i] = ll_ptr->item; // add item from ll to node
                i++; // increase index of array
                linked_list->sizeArray++; // increase size of array
            }
            ll_ptr = ll_ptr->next;
        }

        // attach array node to al
        if (al->head == NULL) { // new al
            al->head = linked_list; // make temp array node the new head of al
            al->head->next = NULL; // next is null
        } else { // not new al
            ArrayNode *al_ptr = al->head;
            ArrayNode *prev;

            while (al_ptr != NULL) {
                prev = al_ptr;
                al_ptr = al_ptr->next;
            }
            prev->next = linked_list;
            prev->next->next = NULL;
            // al_ptr = linked_list;
        }
        
        // remove latest node in linked list to carry on backtracking
        removeNode(ll, ll->size - 1);
        return;
    }

    // begin recursion

    int index = ll->size;
    int to_insert = index + 1; // insert 3 at [2] or insert 0 at [2]

    // dfs go to left child. left child is insert index + 1 at index. aka 3 at [2]

    insertNode(ll, index, to_insert);
    // printf("entering recursion\n");
    sumToC(ll, C, al);

    // dfs go to right child. right child is insert 0 at index, aka 0 at [2]
    // it will get here after returning from the previous statement

    insertNode(ll, index, 0);
    sumToC(ll, C, al);

    // exit out of recursion OR for backtracking
    removeNode(ll, ll->size-1);
    return;
}

///////////////////////////////////////////////////////
int insertNode(LinkedList *ll, int index, int value){

	ListNode *pre, *cur;

	if (ll == NULL || index < 0 || index > ll->size)
		return 0;

        if (index == 0){
		cur = ll->head;
		ll->head = (ListNode*) malloc(sizeof(ListNode));
		ll->head->item = value;
		ll->head->next = cur;
		ll->sum += value;
		ll->size++;
		return 1;
	}


	// Find the nodes before and at the target position
	// Create a new node and reconnect the links
	if ((pre = findNode(*ll, index - 1)) != NULL){
		cur = pre->next;
		pre->next = malloc(sizeof(ListNode));
		pre->next->item = value;
		pre->next->next = cur;
		ll->sum += value;
		ll->size++;
		return 1;
	}

	return 0;
}


int removeNode(LinkedList *ll, int index){

	ListNode *pre, *cur;

	// Highest index we can remove is size-1
	if (ll == NULL || index < 0 || index > ll->size)
		return 0;

	// If removing first node, need to update head pointer
	if (index == 0){
		cur = ll->head->next;
		ll->sum -= ll->head->item;
		free(ll->head);
		ll->head = cur;
		ll->size--;
		return 1;
	}

	// Find the nodes before and after the target position
	// Free the target node and reconnect the links
	if ((pre = findNode(*ll, index - 1)) != NULL){

		if (pre->next == NULL)
			return 0;

		cur = pre->next;
		ll->sum -= cur->item;
		pre->next = cur->next;
		free(cur);
		ll->size--;
		return 1;
	}

	return 0;
}

ListNode *findNode(LinkedList ll, int index){

	ListNode *temp;

	if (index < 0 || index >= ll.size)
		return NULL;

	temp = ll.head;

	if (temp == NULL || index < 0)
		return NULL;

	while (index > 0){
		temp = temp->next;
		if (temp == NULL)
			return NULL;
		index--;
	}

	return temp;
}

void removeAllItems(LinkedList *ll)
{
	ListNode *cur = ll->head;
	ListNode *tmp;

	while (cur != NULL){
		tmp = cur->next;
		free(cur);
		cur = tmp;
	}
	ll->head = NULL;
	ll->size = 0;
	ll->sum =0;
}

