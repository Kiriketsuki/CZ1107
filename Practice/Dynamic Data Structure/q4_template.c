////////////////////////////////////////////////////////////

//#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>

////////////////////////////////////////////////////////////

typedef struct _listnode{
	int item;
	struct _listnode *next;
} ListNode;

//////////////////////////////////////////////////////

int searchList(ListNode *head, int value);

//////////////////////////////////////////////////////

void main()
{
	ListNode *head, *p, *temp;
	int n;


	head=NULL;
	p=NULL;		// pointing to the last node in the list
	printf("Enter a list of numbers, terminated by the value -1:\n");

 ////////////////////////////////////////////////////////////////////

 // Question 2
 //Write your program code here
    int user_input;
    scanf("%d", &user_input);
    getchar();
    
    while (user_input != -1) {
        if (head == NULL) {
            head = malloc(sizeof(ListNode));
            p = head;
        } else {
            p->next = malloc(sizeof(ListNode));
            p = p->next;
        }
        p->item = user_input;
        p->next = NULL;
        scanf("%d", &user_input);
        getchar();
        // printf("%d, ", p->item);
    }

    // p = NULL;
    // printf("%d", head->item);
    // printf("%d", head->next->item);
    temp = head;
    while (temp != NULL) {
        printf("%d ", temp->item);
        temp = temp->next;
    }

    printf("\n");

//////////////////////////////////////////////////////////////////////

//search for a value in the list. Q3
    int search;
	printf("Enter value to search for: \n");
	scanf("%d", &n);
	if ((search = searchList(head,n)) != -1) {
        printf("The value %d is found at index %d\n", n, search);
    } else {
        printf("Value %d not found\n", n);
    }

/////////////////////////////////////////////////////////

    //free up memory for question 4
	//this part is wrong. It will cause problems.
    // p = head;
	// while (p != NULL) {
	// 	free(p);
	// 	p=p->next;
	// }
    // Question 4
	//rewrite the free up using temporary pointer temp.

    p = head;

    while (p != NULL) {
        temp = p->next;
        free(p);
        p = temp;
    }

/////////////////////////////////////////////////////////

}

////////////////////////////////////////////////////////////

int searchList(ListNode *head, int value)
{

////////////////////////////////////////////////////////////////////

 // Question 3
 //Write your program code here
    int to_return = -1;
    int index = 0;
    ListNode *temp;

    if (head == NULL) {
        return to_return;
    }

    temp = head;

    while (temp != NULL) {
        if (value == temp->item) {
            to_return = index;
            return to_return;
        } else {
            index++;
        }

        temp = temp->next;
    }

    return to_return;

//////////////////////////////////////////////////////////////////////
}
