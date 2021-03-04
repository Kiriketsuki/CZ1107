//////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#define MIN_INT -1000

//////////////////////////////////   linked list /////////////////////////////////

typedef struct _listnode{
   int item;
   struct _listnode *next;
} ListNode;

typedef struct _linkedlist{
   int size;
   ListNode *head;
   ListNode *tail;
} LinkedList;

////////////////////////////////// stack    ///////////////////////////////////////////////////////

typedef struct stack{
	LinkedList ll;
} Stack;

//////////////////////////////////// queue ////////////////////////////////////////////////////////

typedef struct _queue{
	LinkedList ll;
} Queue;

///////////////////////////////////////////////////////////////////////////////////////////////////

void insert_overlap(LinkedList *ll1, LinkedList *ll2, LinkedList *ll3);

///////////////////////////////////////////////////////////////////////////////////////////////////

void push(Stack *s, int item);
int pop(Stack *s);
int peek(Stack *s);
int isEmptyStack(Stack *s);

void enqueue(Queue *q, int item);
int dequeue(Queue *q);
int isEmptyQueue(Queue *s);

///////////////////////////////////////////////////////////////////////////////////////////////////
void printList(LinkedList *ll);
ListNode * findNode(LinkedList *ll, int index);
int insertNode(LinkedList *ll, int index, int value);
int removeNode(LinkedList *ll, int index);
void removeAllItems(LinkedList *ll);
///////////////////////////////////////////////////////////////////////////////////////////////////

void push(Stack *s, int item){
   insertNode(&(s->ll), 0, item);
}

int pop(Stack *s){
   int item;
   if(!isEmptyStack(s)){
    item = ((s->ll).head)->item;
    removeNode(&(s->ll), 0);
    return item;
   }
    return INT_MIN;
}

int peek(Stack *s){
   return ((s->ll).head)->item;
}

int isEmptyStack(Stack *s){
   if ((s->ll).size == 0)
      return 1;
   return 0;
}

void enqueue(Queue *q, int item){
   insertNode(&(q->ll), q->ll.size, item);
}

int dequeue(Queue *q){
   int item;
   item = ((q->ll).head)->item;
   removeNode(&(q->ll), 0);
   return item;
}

int isEmptyQueue(Queue *q){
   if ((q->ll).size == 0)
      return 1;
   return 0;
}

//////////////////////////////////////////////////////////////////////////////////

void printList(LinkedList *ll){

	ListNode *cur;
	if (ll == NULL)
		return;
	cur = ll->head;

	if (cur == NULL)
		printf("Empty");
	while (cur != NULL)
	{
		printf("%d ", cur->item);
		cur = cur->next;
	}
	printf("\n");
}

ListNode * findNode(LinkedList *ll, int index){

	ListNode *temp;

	if (ll == NULL || index < 0 || index >= ll->size)
		return NULL;

	temp = ll->head;

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

int insertNode(LinkedList *ll, int index, int value){

	ListNode *pre, *cur;

	if (ll == NULL || index < 0 || index > ll->size + 1)
		return -1;

	// If empty list or inserting first node, need to update head pointer
	if (ll->head == NULL || index == 0){
		cur = ll->head;
		ll->head = malloc(sizeof(ListNode));
		ll->head->item = value;
		ll->head->next = cur;
		ll->size++;
		return 0;
	}


	// Find the nodes before and at the target position
	// Create a new node and reconnect the links
	if ((pre = findNode(ll, index - 1)) != NULL){
		cur = pre->next;
		pre->next = malloc(sizeof(ListNode));
		pre->next->item = value;
		pre->next->next = cur;
		ll->size++;
		return 0;
	}

	return -1;
}


int removeNode(LinkedList *ll, int index){

	ListNode *pre, *cur;

	// Highest index we can remove is size-1
	if (ll == NULL || index < 0 || index >= ll->size)
		return -1;

	// If removing first node, need to update head pointer
	if (index == 0){
		cur = ll->head->next;
		free(ll->head);
		ll->head = cur;
		ll->size--;

		return 0;
	}

	// Find the nodes before and after the target position
	// Free the target node and reconnect the links
	if ((pre = findNode(ll, index - 1)) != NULL){

		if (pre->next == NULL)
			return -1;

		cur = pre->next;
		pre->next = cur->next;
		free(cur);
		ll->size--;
		return 0;
	}

	return -1;
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
}

int main () {
    int a = 1;
    int b = 1;
    LinkedList ll1;
    ll1.head = NULL;
    ll1.tail = NULL;
    ll1.size = 0;

    LinkedList ll2;
    ll2.head = NULL;
    ll2.tail = NULL;
    ll2.size = 0;

    LinkedList ll3;
    ll3.head = NULL;
    ll3.tail = NULL;
    ll3.size = 0;

    while (a != -1) {
        printf("Input an integer you want to insert into the first linked list, -1 to stop:\n");
        scanf("%d", &a);
        if (a == -1) {
            break;
        } else {
            insertNode(&ll1, ll1.size, a);
        }
        printf("The linked list is: ");
        printList(&ll1);
    }

    while (b != -1) {
        printf("Input an integer you want to insert into the second linked list, -1 to stop:\n");
        scanf("%d", &b);
        if (b == -1) {
            break;
        } else {
            insertNode(&ll2, ll2.size, b);
        }
        printf("The linked list is: ");
        printList(&ll2);
    }

    printf("The overlaps between ll1 and ll2 are: ");
    insert_overlap(&ll1, &ll2, &ll3);
    printList(&ll3);

}

void insert_overlap(LinkedList *ll1, LinkedList *ll2, LinkedList *ll3) {
    if (ll1->head == NULL && ll2->head == NULL) {
        return;
    }

    ListNode *curr_1, *head_1, *curr_2, *head_2;
    curr_1 = ll1->head;
    curr_2 = ll2->head;
    head_1 = curr_1;

    int curr;
    while (curr_2 != NULL) { //iterate through the elements of ll2 one by one
        curr = curr_2->item; 

        if (curr_2->next != NULL && curr == curr_2->next->item) { //ll2 is sorted so if duplicates, the next in line will have the same value. NULL checker comes first so won't crash
            curr_2 = curr_2->next; //ignore first duplicate, jumps straight to second duplicate
        } else {
            while (curr_1 != NULL) { //iterate through elements of ll1 one by one
                if (curr_1->next != NULL && curr_1->item == curr_1->next->item) { //duplicate checker in ll1
                    curr_1 = curr_1->next; //ll1 duplciate skipper
                } else {
                    if (curr_1->item == curr) {
                        insertNode(ll3, ll3->size, curr); //insert node if overlap
                    }
                    curr_1 = curr_1->next; //move to next iteration of ll1
                }
            }
            curr_2 = curr_2->next; //move to next iteration of ll2
        }
        curr_1 = head_1; //resets ll1 iterator so can iterate through ll1 again during next iteration of ll2
    } 
}