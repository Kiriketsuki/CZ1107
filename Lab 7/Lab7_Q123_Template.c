#include <stdio.h>
#include <stdlib.h>

enum GraphType {ADJ_MATRIX, ADJ_LIST}; // Types of Graph Representation

typedef struct _listnode
{
    int vertex; // weight/value
	struct _listnode *next;
} ListNode;

union GraphForm{
    int **matrix;
    ListNode **list;
};

typedef struct _graph{
    int V;
    int E;
    enum GraphType type;
    union GraphForm adj; // either double int array aka matrix or list
} Graph;

void printGraphMatrix(Graph );
void adjM2adjL(Graph *);
void adjL2adjM(Graph *);
void printGraphList(Graph );
void calDegreeV(Graph,int *);
void printDegreeV(int *,int );

int main()
{
    Graph g;
    int i, j;

    int* degreeV; // int array, degree is number of edges connected to this vertice
    printf("Enter the number of vertices:\n");
    scanf("%d", &g.V);

    g.E = 0; // number of edges aka links
    g.adj.matrix = (int **)malloc(g.V*sizeof(int *)); // initialized as a matrix

    for(i = 0; i < g.V; i++)
        g.adj.matrix[i] = (int *)malloc(g.V*sizeof(int)); // initialize first level of matrix

    for(i = 0; i < g.V; i++)
        for(j = 0; j < g.V; j++)
            g.adj.matrix[i][j] = 0; // sets everything in the matrix to 0

    g.type = ADJ_MATRIX;

    degreeV = (int *) malloc(g.V*sizeof(int));

    for(i = 0; i < g.V; i++) {
        degreeV[i] = 0;
    }

    int V1, V2; // temp

    printf("Enter two vertices which are adjacent to each other:\nnegative numbers to exit\n");
    while(scanf("%d %d", &V1, &V2) == 2) // scanf returns number of inputs given aka 2
    {
        if(V1 > 0 && V1 <= g.V && V2 > 0 && V2 <= g.V) // g.v is number of vertices aka nodes
        {
            g.adj.matrix[V1-1][V2-1] = 1;
            g.adj.matrix[V2-1][V1-1] = 1;
            g.E++; // number of edges increase
        }
        else
            break;
        printf("Enter two vertices which are adjacent to each other:\nnegative numbers to exit\n");
    }

    calDegreeV(g,degreeV);

    printGraphMatrix(g);
    printDegreeV(degreeV,g.V);

    adjM2adjL(&g);

    calDegreeV(g,degreeV);

    printGraphList(g);
    printDegreeV(degreeV,g.V);

    adjL2adjM(&g);
    printGraphMatrix(g);

    return 0;
}

void printGraphMatrix(Graph g)
{
    int i, j;
    if(g.type == ADJ_LIST) {printf("Error"); return;}

    for(i = 0; i < g.V; i++){
        for(j = 0; j < g.V; j++)
            printf("%d\t", g.adj.matrix[i][j]);
        printf("\n");
    }

}

void printDegreeV(int *degreeV,int V)
{
    int i;
    for(i=0;i<V;i++)
        printf("%d: %d degree\n",i+1,degreeV[i]);
}

void printGraphList(Graph g){
    int i;
    ListNode* temp;

    if(g.type == ADJ_MATRIX) {printf("Error"); return;}

    for(i = 0; i < g.V; i++)
    {
        printf("%d:\t", i+1);
        temp = g.adj.list[i];
        while(temp != NULL){
            printf("%d -> ",temp->vertex);
            temp = temp->next;
        }
        printf("\n");
    }
}

void adjM2adjL(Graph *g)
{
    if (g->type == ADJ_LIST) {
        printf("Error"); 
        return;
    }

    int i, j;
    ListNode **list_array = (ListNode **)malloc(g->V*sizeof(ListNode *)); // create array of listnodes containing V number of listnode addresses
    ListNode *pointer;

    for (i = 0; i < g->V; i++) {
        ListNode *head = malloc(sizeof(ListNode));
        head->vertex = i+1;
        pointer = head;

        for (j = 0; j < g->V; j++) {
            ListNode *curr = malloc(sizeof(ListNode));
            curr->vertex = j+1;

            if (g->adj.matrix[i][j] != 0) { // is adjacent
                pointer->next = curr;
                pointer = curr;
            } else {
                free(curr);
            }
        }
        pointer->next = NULL;
        list_array[i] = head;
    }

    g->adj.list = list_array;
    g->type = ADJ_LIST;
}

void adjL2adjM(Graph *g){
	if (g->type == ADJ_MATRIX) {
        printf("Error"); 
        return;
    }

    int i;
    int curr_int;
    ListNode *curr_node;
    int **temp_matrix = (int **)malloc(g->V*sizeof(int *));

    for(i = 0; i < g->V; i++) {
        temp_matrix[i] = (int *)malloc(g->V*sizeof(int)); // IMPORTANT need to allocate the 2nd layer of array memory too
    }

    for (i = 0; i < g->V; i++) {
        for (int j = 0; j < g->V; j++) {
            temp_matrix[i][j] = 0;
        }
    }

    for (i = 0; i < g->V; i++) {
        curr_node = g->adj.list[i];
        while (curr_node != NULL) {
            curr_int = curr_node->vertex;
            if (curr_int-1 != i) {
                temp_matrix[i][curr_int - 1] = 1;
            }
            curr_node = curr_node->next;
        }
    }

    g->adj.matrix = temp_matrix;
    g->type = ADJ_MATRIX;
}

void calDegreeV(Graph g, int *degreeV)
{
    int i;

	if (g.type == ADJ_LIST) {
        ListNode *curr;
        for (i = 0; i < g.V; i++) {
            curr = g.adj.list[i];
            int counter = -1; // lazy way to make it not count itself
            while (curr != NULL) {
                counter++;
                curr = curr->next;
            }
            degreeV[i] = counter;
        }
    } else {
        for (i = 0; i < g.V; i++) {
            int counter = 0;
            for (int j = 0; j < g.V; j++) {
                if (g.adj.matrix[i][j] != 0) {
                    counter++;
                } 
            }
            degreeV[i] = counter;
        }
    }
}

