#include <stdio.h>
#include <stdlib.h>

typedef struct _listnode
{
    int vertex;
	struct _listnode *next;
} ListNode;
typedef ListNode StackNode;

typedef struct _graph{
    int V;
    int E;
    int **matrix;
}Graph;

typedef struct _linkedlist
{
	int size;
	ListNode *head;
} Stack;

// You should not change the prototypes of these functions
//////STACK///////////////////////////////////////////
void push(Stack *sPtr, int vertex);
int pop(Stack *sPtr);
int peek(Stack s);
int isEmptyStack(Stack s);
void removeAllItemsFromStack(Stack *sPtr);
int is_in_stack(Stack s, int to_find);
////GRAPH////////////////////////////////////////////
void printGraphMatrix(Graph );
////////////////////////////////////////////


void DFS_I (Graph , int );

int main()
{
    Graph g;
    int i,j;

    printf("Enter the number of vertices:\n");
    scanf("%d",&g.V);

    g.E = 0;
    g.matrix = (int **)malloc(g.V*sizeof(int *));
    for(i=0;i<g.V;i++)
        g.matrix[i] = (int *)malloc(g.V*sizeof(int));

    for(i=0;i<g.V;i++)
       for(j=0;j<g.V;j++)
           g.matrix[i][j] = 0;

    int V1, V2;
    printf("Enter two vertices which are adjacent to each other:\n");
    while(scanf("%d %d",&V1,&V2)==2)
    {
        if(V1>0 && V1<=g.V && V2>0 && V2<=g.V)
        {
            g.matrix[V1-1][V2-1] = 1;
            g.matrix[V2-1][V1-1] = 1;
            g.E++;
        }
        else
            break;
        printf("Enter two vertices which are adjacent to each other:\n");
    }
    scanf("%*c");
    printf("Enter a start vertex for DFS:\n");
    scanf("%d", &i);
    printGraphMatrix(g);
    DFS_I(g,i);

    return 0;
}

void DFS_I (Graph g, int v)
{
    Stack *stackackack = malloc(sizeof(Stack));
    stackackack->head = NULL;
    stackackack->size = 0;

    Stack *visited_stack = malloc(sizeof(Stack));
    visited_stack->head = NULL;
    visited_stack->size = 0;
    int i;
    int found_non_visited;
    int vertex;
    int index;

    push(stackackack, v); // stack holds vertex not index
    push(visited_stack, v); // ??

    while (!isEmptyStack(*stackackack)) {
        found_non_visited = 0;
        vertex = peek(*stackackack);
        index = vertex - 1;
        if (!is_in_stack(*visited_stack, vertex)) {
            push(visited_stack, vertex);
        }

        for (i = 0; i < g.V; i++) { // loop through rows of the matrix i is index of vertex adjacent to current vertex
            if (g.matrix[i][index] == 1 && !is_in_stack(*visited_stack, i+1)) { // adjacent to node vertex(i+1) and (i+1) not already visited
                found_non_visited = 1;
                push(stackackack, i+1);
                push(visited_stack, i+1);
                break;
            }
        }

        if (!found_non_visited) {
            pop(stackackack);
        }
    }

    while (!isEmptyStack(*visited_stack)) {
        printf("%d ", peek(*visited_stack));
        pop(visited_stack);
    }
}

int is_in_stack(Stack s, int to_find) {
    ListNode *curr = s.head;

    while (curr != NULL) {
        if (curr->vertex == to_find) {
            return 1;
        }
        curr = curr->next;
    }

    return 0;
}


void printGraphMatrix(Graph g)
{
    int i,j;

    for(i=0;i<g.V;i++){
        for(j=0;j<g.V;j++)
            printf("%d\t",g.matrix[i][j]);
        printf("\n");
    }

}

void push(Stack *sPtr, int vertex)
{
	StackNode *newNode;
    newNode= malloc(sizeof(StackNode));
    newNode->vertex = vertex;
    newNode->next = sPtr->head;
    sPtr->head = newNode;
    sPtr->size++;
}

int pop(Stack *sPtr)
{
    if(sPtr==NULL || sPtr->head==NULL){
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

int isEmptyStack(Stack s)
{
     if(s.size==0) return 1;
     else return 0;
}

int peek(Stack s){
    return s.head->vertex;
}

void removeAllItemsFromStack(Stack *sPtr)
{
	while(pop(sPtr));
}
