////////////////////////////////////////////////////////////


//#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>

////////////////////////////////////////////////////////////

typedef struct _listnode{
	int item;
	struct _listnode *next;
} ListNode;


void main()
{
	ListNode *head, *p, *temp;
	int n;


	head=NULL;
	p=NULL;		// pointing to the last node in the list
	printf("Enter a list of numbers, terminated by the value -1:\n");

	/////////////////////////////////////////////////////////////////

	//Question 2
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

   //////////////////////////////////////////////////////////////////
}
