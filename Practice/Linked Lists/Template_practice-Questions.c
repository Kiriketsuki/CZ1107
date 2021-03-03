////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>

////////////////////////////////////////////////////////////////////


typedef struct _listnode{
	int num;
	struct _listnode *next;
} ListNode;
typedef struct _linkedlist{
	ListNode *head;
	int size;
} LinkedList;
typedef struct _dbllistnode{
	int num;
	struct _dbllistnode *pre;
	struct _dbllistnode *next;
} DblListNode;

////////////////////////////////////////////////////////////////////
////////////
int removeNode(ListNode **ptrHead, int index);
int removeNode2(LinkedList *ll, int index);
int split(ListNode *head, ListNode **ptrEvenList, ListNode **ptrOddList);
int duplicateReverse(ListNode *head, ListNode **ptrNewHead);

void printList(ListNode *head);
ListNode * findNode(ListNode *head, int index);
int insertNode(ListNode **ptrHead, int index, int value);


// these are for the practice questions
int moveMaxToFront(ListNode **ptrHead);
int concatenate(ListNode **ptrHead1, ListNode *head2);
int combineAlternating(ListNode **ptrHead, ListNode *head1, ListNode *head2);
int insertDbl(DblListNode **ptrHead, int index, int value);
void printDblList(DblListNode *head);

////////////////////////////////////////////////////////////////////
////////////

int main()
{
	int i;
	ListNode *head = NULL;
	ListNode *dupRevHead = NULL;
	ListNode *oddHead = NULL;
	ListNode *evenHead = NULL;
	ListNode *list1Head = NULL, *list2Head = NULL;
	ListNode *combAltHead = NULL;
	DblListNode *dblHead = NULL;
	LinkedList llist;
	int size = 0;

	//build a linked list
	if (insertNode(&head, 0, 6) == 0) size++;
	if (insertNode(&head, 0, 4) == 0) size++;
	if (insertNode(&head, 0, 2) == 0) size++;
	printf("After inserting 3 values.");
	printList(head);


	//removeNode(): question 1
	if (removeNode(&head, 3) == 0) size--;//this one can't be removed.
	if (removeNode(&head, 1) == 0) size--;
	if (removeNode(&head, 0) == 0) size--;

	printf("\nafter remove using removeNode(), ");
	printList(head);

	//insert some nodes
	if (insertNode(&head, 2, 3) == 0) size++; // this one can't be inserted.
	if (insertNode(&head, 1, 1) == 0) size++;
	if (insertNode(&head, 0, 0) == 0) size++;

	printf("\nafter insert, ");
	printList(head);

	//removeNode2(): question 2
	llist.head = head;
	llist.size = size;
	removeNode2(&llist, 2);
	removeNode2(&llist, 0);
	removeNode2(&llist, 0);
	head = llist.head;


	printf("\nafter remove using removeNode2(), ");
	printList(llist.head);
	// empty linked list now

	//split(): question 3
	for (i = 1; i <10; i++)//build a new linked list
		insertNode(&head, 0, i);
	printf("\n\ninsert 1-9 to the linked list,");
	printList(head);
	printf("split the current list, results:\n");
	split(head, &evenHead, &oddHead);
	printf("the even list: ");
	printList(evenHead);
	printf("the odd list: ");
	printList(oddHead);

	//duplicateReverse(): question 4
	printf("\n");
	printList(head);
	duplicateReverse(head, &dupRevHead);
	printf("After duplicateReverse(),");
	printList(dupRevHead);

	////////////////////////////////////////////////////////////////////////
	//the following are for the PRACTICE QUESTIONS

	printf("\n\nNow for the practice questions");

	//1.moveMaxToFront()
	printf("\n************** moveMaxToFront *******************\n");

	printList(head);
	moveMaxToFront(&head);
	printf("after moveMaxToFront()");
	printList(head);

	//2. concatenate()
	printf("\n************** concatenate() *******************\n");
	concatenate(&head, dupRevHead);
	printf("concatenate(current list, duplicate reverse list)\n  ");
	printList(head);

	//3.combineAlternating()
	printf("\n************** combineAlternating() *******************\n");
	for (i = 3; i > 0; i--)				//build linked list 1
		insertNode(&list1Head, 0, i);
	for (i = 13; i > 10; i--)			//build linked list 2
		insertNode(&list2Head, 0, i);
	printf("List 1:");
	printList(list1Head);
	printf("List 2: ");
	printList(list2Head);
	combineAlternating(&combAltHead, list1Head, list2Head);
	printf("After combineAlternating(): ");
	printList(combAltHead);

	//4. insertDbl()
	printf("\n************** insertDbl() *******************\n");
	printf("insertDbl()\nDoubly-linked list: ");
	insertDbl(&dblHead, 0, 10);
	insertDbl(&dblHead, 0, 20);
	insertDbl(&dblHead, 1, 30);
	insertDbl(&dblHead, 2, 40);
	printDblList(dblHead);
	return 0;
}

////////////////////////////////////////////////////////////////////
////////////

void printList(ListNode *head){
	ListNode *cur = head;

	if (cur == NULL)
		return;

	printf("the current linked list is:\n");
	while (cur != NULL){
		printf("%d ", cur->num);
		cur = cur->next;
	}
	printf("\n");
}

void printDblList(DblListNode *head){
	if (head == NULL) return;
	while (head != NULL){
		printf("%d ", head->num);
		head = head->next;
	}
	printf("\n");
}

ListNode * findNode(ListNode *head, int index){
	if (head == NULL || index < 0) return NULL;
	while (index > 0){
		head = head->next;
		if (head == NULL)
			return NULL;
		index--;
	}
	return head;
}


int insertNode(ListNode **ptrHead, int index, int value){
	ListNode *pre, *cur;
	// If empty list or inserting first node, need to update head pointer
	if (*ptrHead == NULL || index == 0){
		cur = *ptrHead;
		*ptrHead = malloc(sizeof(ListNode));
		(*ptrHead)->num = value;
		(*ptrHead)->next = cur;
		return 0;
	}
	// Find the nodes before and at the target position
	// Create a new node and reconnect the links
	if ((pre = findNode(*ptrHead, index - 1)) != NULL){
		cur = pre->next;
		pre->next = malloc(sizeof(ListNode));
		pre->next->num = value;
		pre->next->next = cur;
		return 0;
	}
	return -1;
}

////////////////////////////////////////////////////////////////////
////////////

int removeNode(ListNode **ptrHead, int index){
	ListNode *current_node = *ptrHead;
    ListNode *prev_node;
    int i = 0;

    if (current_node == NULL) {
        return 0;
    }

    if (index == 0) {
        if (current_node->next != NULL) {
            *ptrHead = current_node->next;
            return 1;
        } else {
            return 0;
        }
    }

    while (current_node != NULL) {
        if (i == index) {
            prev_node->next = current_node->next;
            free(current_node);
            return 1;
        }

        prev_node = current_node;
        current_node = current_node->next;
        i++;
    }
}

////////////////////////////////////////////////////////////////////////////////
////////////

int removeNode2(LinkedList *ll, int index) {

	ListNode *current_node = ll->head;
    ListNode *prev_node;
    int i = 0;

    if (current_node == NULL) {
        return 0;
    }

    if (index == 0) {
        if (current_node->next != NULL) {
            ll->head = current_node->next;
            return 1;
        } else {
            return 0;
        }
    }

    while (current_node != NULL) {
        if (i == index) {
            prev_node->next = current_node->next;
            free(current_node);
            return 1;
        }

        prev_node = current_node;
        current_node = current_node->next;
        i++;
    }
}



int split(ListNode *head, ListNode **ptrEvenList, ListNode **ptrOddList)
{

	ListNode *current_node = head;
    int i = 0;
    
    while (current_node != NULL) {
        if (i == 0) {
            insertNode(ptrEvenList, 0, current_node->num);
        } else if (i == 1) {
            insertNode(ptrOddList, 0, current_node->num);
        } else {
            if (i % 2 == 0) {
                insertNode(ptrEvenList, i/2, current_node->num);
            } else {
                insertNode(ptrOddList, i/2, current_node->num);
            }
        }
        current_node = current_node->next;
        i++;
    }

}

int duplicateReverse(ListNode *head, ListNode **ptrNewHead){

	ListNode *current_node = head;
    ListNode *to_insert;
    int size = 0, i = 0;

    while (current_node->next != NULL) {
        size++;
        current_node = current_node->next;
    }

    for (size; size >= 0; size--) {
        to_insert = findNode(head, size);
        insertNode(ptrNewHead, i, to_insert->num);
        i++;
    }

}

////////////////////////////////////////////////////////////////////
////////////

int moveMaxToFront(ListNode **ptrHead){
    ListNode *curr = *ptrHead;
    ListNode *largest = *ptrHead;
    ListNode *old_head = *ptrHead;
    ListNode *prev;
    ListNode *before_largest;
    int to_return = -1;
    int index = 0;
    int curr_val = curr->num;
    int largest_val = curr_val;

    while (curr != NULL) {
        curr_val = curr->num;

        if (curr_val > largest_val) {
            largest = curr;
            before_largest = prev;
            largest_val = curr_val;
            to_return = index;
        }
        prev = curr;
        curr = curr->next;
        index++;
    }

    if (old_head == largest) {
        return to_return;
    } else {
        before_largest->next = largest->next;
        largest->next = old_head;
        *ptrHead = largest;
        return to_return;
    }


}


int concatenate(ListNode **ptrHead1, ListNode *head2){
    ListNode *temp = *ptrHead1;
    ListNode *prev;

    if (*ptrHead1 == NULL && head2 != NULL) {
        *ptrHead1 = head2;
        return 1;
    }

    if (head2 == NULL && *ptrHead1 != NULL) {
        return 1;
    }

    if (*ptrHead1 ==  NULL && head2 == NULL) {
        return 0;
    }

    while (temp != NULL) {
        if (temp != *ptrHead1) {
            prev = temp;
        }
        temp = temp->next;
    }

    prev->next = head2;
    return 1;
}


int combineAlternating(ListNode **ptrHead, ListNode *head1, ListNode *head2){
    ListNode *curr_1, *curr_2, *ptr;
    ListNode **curr_0;
    curr_0 = ptrHead;
    curr_1 = head1;
    curr_2 = head2;
    int index = 0;

    if (curr_1 == NULL && curr_2 != NULL) {
        *ptrHead = curr_2;
        return 1;
    } else if (curr_2 == NULL && curr_1 != NULL) {
        *ptrHead = curr_1;
        return 1;
    } else if (curr_1 == NULL && curr_2 == NULL) {
        return 0;
    }

    while (curr_1 != NULL && curr_2 != NULL) {
        insertNode(curr_0, index, curr_1->num);
        index++;
        insertNode(curr_0, index, curr_2->num);
        index++;
        curr_1 = curr_1->next;
        curr_2 = curr_2->next;
    }

    if (curr_1 == NULL && curr_2 != NULL) {
        while (curr_2 != NULL) {
            insertNode(curr_0, index, curr_2->num);
            index++;
            curr_2 = curr_2->next;
        }
    } else if (curr_1 != NULL && curr_2 == NULL) {
        while (curr_1 != NULL) {
            insertNode(curr_0, index, curr_1->num);
            index++;
            curr_1 = curr_1->next;
        }
    }
    return 1;

}


int insertDbl(DblListNode **ptrHead, int index, int value){

    DblListNode *prev, *curr;
	if (*ptrHead == NULL || index == 0){
		curr = *ptrHead;
		*ptrHead = malloc(sizeof(DblListNode));
		(*ptrHead)->num = value;
		(*ptrHead)->next = curr;
		(*ptrHead)->pre = NULL;
		if (curr != NULL)
			curr->pre = *ptrHead;
		return 0;
	}

	prev = *ptrHead;
	while (index > 1){
		prev = prev->next;
		index--;
	}
	// Create a new node and reconnect the links
	if (prev != NULL){
		curr = prev->next;
		prev->next = malloc(sizeof(DblListNode));
		prev->next->pre = prev;
		prev->next->next = curr;
		if (curr != NULL)
			prev->next->next->pre = prev->next;
		prev->next->num = value;
		return 0;
	}
	return 0;


}


////////////////////////////////////////////////////////////////////
