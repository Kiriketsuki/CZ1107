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
    int student_offset;
    int project_offset;
    int mentor_offset;
    int student_dupe_offset;
    int sink_offset;
    ListNode **list;
}Graph;

typedef ListNode QueueNode;

typedef struct _queue{
   int size;
   QueueNode *head;
   QueueNode *tail;
} Queue;

typedef struct _stack
{
	int size;
	StackNode *head;
} Stack;

void print_graph_list(Graph g);
void insertAdjVertex(ListNode** AdjList,int vertex);
void removeAdjVertex(ListNode** AdjList,int vertex);
int matching(Graph g);
int *bfs(Graph g, int start, int goal);
int length_calc(int *visited_array, int start, int goal);
Stack *reform_path(int *visited_array, int start, int goal);
void print_stack(StackNode *curr);
void enqueue(Queue *qPtr, int item);
int dequeue(Queue *qPtr);
int getFront(Queue q);
int isEmptyQueue(Queue q);
void removeAllItemsFromQueue(Queue *qPtr);
void printQ(QueueNode *cur);
//////STACK///////////////////////////////////////////
void push(Stack *sPtr, int vertex);
int pop(Stack *sPtr);
int peek(Stack s);
int isEmptyStack(Stack s);
void removeAllItemsFromStack(Stack *sPtr);
//////////////////////////////////

int main()
{
    int projects, students, mentors; //Project, Student and Mentor;
    int maxMatch;
    scanf("%d %d %d", &students, &projects, &mentors);

    int i, j; //number of projects and number of mentors
	
    //build graph
	Graph g;
    g.V = 2 + projects + students + students + mentors;
    g.list = malloc(g.V * sizeof(ListNode));

    for (i = 0; i < g.V; i++) {
        g.list[i] = NULL;
    }

    // create offsets
    int project_offset = 0;
    int student_offset = project_offset + projects; // 2 2 2, first student index is at 1 + offset. 0 is source, 1 and 2 is project, 3 is student. offset is 2
    int student_dupe_offset = student_offset + students; // 2 2 2. first dupe is at 1 + offset. 0 1 2 3 4. offset is 4
    int mentor_offset = student_dupe_offset + students; // 2 2 2. first mentor at 1 + offset. 0 1 2 3 4 5 6. offset is 6
    int sink_offset = mentor_offset + mentors; // 2 2 2. sink at 1 + offset. 0 // 1 2 // 3 4 // 5 6 // 7 8 // offset is 8.

    g.project_offset = project_offset;
    g.student_offset = student_offset;
    g.student_dupe_offset = student_dupe_offset;
    g.mentor_offset = mentor_offset;
    g.sink_offset = sink_offset;
    
    // connect source to projects
    int project_index;
    for (i = 0; i < projects + 1; i++) {
        project_index = i + project_offset;
        insertAdjVertex(&(g.list[0]), project_index);
    }

    // connect students to student dupes
    int student_index;
    int student_dupe_index;

    for (i = 1; i < students + 1; i++) {
        student_index = i + student_offset;
        student_dupe_index = i + student_dupe_offset;
        insertAdjVertex(&(g.list[student_index]), student_dupe_index);
    }

    // connect mentors to sink;
    int sink = 1 + sink_offset;
    int mentor_index;

    for (i = 1; i < mentors + 1; i++) {
        mentor_index = i + mentor_offset;
        insertAdjVertex(&(g.list[mentor_index]), sink);
    }

    // get student inputs
    int pref_proj, pref_ment;
    for (i = 1; i < students + 1; i++) {
        student_index = i + student_offset;
        student_dupe_index = i + student_dupe_offset;

        scanf("%d %d", &pref_proj, &pref_ment);

        // connect projects to student
        for (j = 1; j < pref_proj + 1; j++) {
            scanf("%d", &project_index);
            project_index += project_offset;
            insertAdjVertex(&(g.list[project_index]), student_index);
        }

        // connect student dupes to mentors
        for (j = 1; j < pref_ment + 1; j++) {
            scanf("%d", &mentor_index);
            mentor_index += mentor_offset;
            insertAdjVertex(&(g.list[student_dupe_index]), mentor_index);
        }
    }

    // use to check if search algo is working and if the graph list is created properly
    // print_graph_list(g);
    // int *test_array = bfs(g, 0, 1 + sink_offset);
    // printf("length from source to sink is %d\n", length_calc(test_array, 0, 1 + sink_offset));
    // Stack *test_path = reform_path(test_array, 0, 1 + sink_offset);
    // print_stack(test_path->head);
	
    //apply Ford Fulkerson algorithm
    // use DFS or BFS to find a path
	maxMatch = matching(g);
    // print_graph_list(g);
    printf("%d\n", maxMatch);
    return 0;
}

int matching(Graph g)
{
    int path_length = 0;
    int *visited_array;
    Stack *path;

    while (path_length != -1) {
        visited_array = bfs(g, 0, 1 + g.sink_offset);
        path_length = length_calc(visited_array, 0, 1 + g.sink_offset);
        if (path_length == -1) {
            break;
        }
        path = reform_path(visited_array, 0, 1 + g.sink_offset);
        int first_vertex = peek(*path);
        pop(path);
        int second_vertex;

        while (!isEmptyStack(*path)) {
            second_vertex = peek(*path);
            removeAdjVertex(&(g.list[first_vertex]), second_vertex);
            insertAdjVertex(&(g.list[second_vertex]), first_vertex);
            first_vertex = second_vertex;
            pop(path);
        }
    }

    ListNode *curr;
    int to_return = 0;
    curr = g.list[1 + g.sink_offset];
    while (curr != NULL) {
        to_return++;
        curr = curr->next;
    }

    return to_return;
}

int *bfs(Graph g, int start, int goal) {
    int *visited_array = malloc(g.V * sizeof(int));
    int i;
    int index;

    for (i = 0; i < g.V; i++) {
        visited_array[i] = -1;
    }

    Queue *queueueueue = malloc(sizeof(Queue));
    queueueueue->head = NULL;
    queueueueue->tail = NULL;
    queueueueue->size = 0;

    enqueue(queueueueue, start);
    visited_array[start] = start;
    ListNode *curr;

    while (!isEmptyQueue(*queueueueue)) {
        index = getFront(*queueueueue);
        dequeue(queueueueue);

        if (index == goal) {
            break;
        }
        curr = g.list[index];

        while (curr != NULL) {
            if (visited_array[curr->vertex] == -1) {
                enqueue(queueueueue, curr->vertex);
                visited_array[curr->vertex] = index;
            }
            curr = curr->next;
        }
    }

    return visited_array;
}

int *dfs(Graph g, int start, int goal) {
    int *visited_array = malloc(g.V * sizeof(int));
    int i;
    int index;

    for (i = 0; i < g.V; i++) {

    }
}

int length_calc(int *visited_array, int start, int goal) {
    int curr = visited_array[goal];
    int length = 1;
    while (curr != start) {
        if (curr == -1) {
            return -1;
        }

        curr = visited_array[curr];
        length++;
    }
    return length;
}

Stack *reform_path(int *visited_array, int start, int goal) {
    Stack *stackackack = malloc(sizeof(Stack));
    stackackack->head = NULL;
    stackackack->size = 0;

    int curr = goal;
    while (1) {
        push(stackackack, curr);
        if (curr == start) {
            break;
        }
        curr = visited_array[curr];
    }

    return stackackack;
}

void print_graph_list(Graph g) {
    int i;
    ListNode *curr;
    for(i = 0; i < g.V; i++) {
        printf("%d:\t", i);
        curr = g.list[i];
        while(curr != NULL) {
            printf("%d -> ",curr->vertex); 
            curr = curr->next;
        }
        printf("\n");
    }
}

void removeAdjVertex(ListNode** AdjList,int vertex)
{
    ListNode *temp, *preTemp;
    if(*AdjList != NULL)
    {
        if((*AdjList)->vertex ==vertex){//first node
            temp = *AdjList;
            *AdjList = (*AdjList)->next;
            free(temp);
            return;
        }
        preTemp = *AdjList;
        temp = (*AdjList)->next;
        while(temp!=NULL && temp->vertex != vertex)
        {
            preTemp= temp;
            temp = temp->next;
        }
        preTemp->next = temp->next;
        free(temp);
    }

}
void insertAdjVertex(ListNode** AdjList,int vertex)
{
    ListNode *temp;
    if(*AdjList==NULL)
    {
        *AdjList = (ListNode *)malloc(sizeof(ListNode));
        (*AdjList)->vertex = vertex;
        (*AdjList)->next = NULL;
    }
    else{
        temp = (ListNode *)malloc(sizeof(ListNode));
        temp->vertex = vertex;
        temp->next = *AdjList;
        *AdjList = temp;
    }
}
void enqueue(Queue *qPtr, int vertex) {
    QueueNode *newNode;
    newNode = malloc(sizeof(QueueNode));
    if(newNode==NULL) exit(0);

    newNode->vertex = vertex;
    newNode->next = NULL;

    if(isEmptyQueue(*qPtr))
        qPtr->head=newNode;
    else
        qPtr->tail->next = newNode;

    qPtr->tail = newNode;
    qPtr->size++;
}

int dequeue(Queue *qPtr) {
    if(qPtr==NULL || qPtr->head==NULL){ //Queue is empty or NULL pointer
        return 0;
    }
    else{
       QueueNode *temp = qPtr->head;
       qPtr->head = qPtr->head->next;
       if(qPtr->head == NULL) //Queue is emptied
           qPtr->tail = NULL;

       free(temp);
       qPtr->size--;
       return 1;
    }
}

int getFront(Queue q){
    return q.head->vertex;
}

int isEmptyQueue(Queue q) {
    if(q.size==0) return 1;
    else return 0;
}

void removeAllItemsFromQueue(Queue *qPtr)
{
	while(dequeue(qPtr));
}

void printQ(QueueNode *cur){
    if(cur==NULL) printf("Empty");

    while (cur != NULL){
        printf("%d ", cur->vertex);
        cur = cur->next;
    }
    printf("\n");
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

void print_stack(StackNode *curr) {
    if(curr == NULL) {
        printf("Empty");
    } 

    while (curr != NULL){
        printf("%d ", curr->vertex);
        curr = curr->next;
    }
    printf("\n");
}