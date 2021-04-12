#include <stdio.h>
#include <stdlib.h>
#define NO_CONNECTION 999
typedef struct _listnode
{
    int vertex;
	struct _listnode *next;
} ListNode;
typedef ListNode StackNode;

typedef struct _graph{
    int V;
    int E;
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

void printGraphList(Graph );
int *BFS(Graph g, int v, int w); // now returns visited array
int BFS_len_calc(int *visited, int v, int w);
Stack *reform_path(int *visited, int v, int w);
Queue *reform_path_queue(int *visited, int v, int w);
void insert_adjacency(ListNode *list, int value);
void remove_adjacency(ListNode *list, int value);
int matching(Graph g);

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
    int projects, students, mentors; //Project, Student and Mentor
    int pref_projs_no, pref_ments_no;
    int maxMatch;
    int i, j;
    int index;
    int student_number;
    // char dummychar;

    // get number of students projects and mentors
    printf("Enter number of students, number of projects, and number of mentors: \n");
    scanf("%d %d %d", &students, &projects, &mentors);

    int total_vertices = students + projects + mentors + 2; // assuming 2 2 2, there are 8 total vertices including sink and source. excluding zeroth

    // create graph
    Graph *original_graph = malloc(sizeof(Graph));
    original_graph->E = 0;
    original_graph->V = total_vertices + 1; // include 0th vertex
    original_graph->list = (ListNode **)malloc(original_graph->V*sizeof(ListNode *));

    for (i = 0; i < original_graph->V; i++) {
        original_graph->list[i] = NULL;
    }

    // create 0th vertex to soak up indexing offsets
    ListNode *zero_th = malloc(sizeof(ListNode));
    zero_th->vertex = NO_CONNECTION;
    zero_th->next = NULL; // set to null first
    original_graph->list[0] = zero_th; // add 0th vertex to graph

    // create offsets
    // source_offset = 0 offset is 0th vertex
    int projects_offset = 1; // offset is source
    int students_offset = 1 + projects; // offset is source + no of projs
    int mentors_offset = 1 + projects + students; // offset is source + no of projs + no of students
    // sink_offset = 1 + projects + students + mentors
    
    // create source and sink
    ListNode *source = malloc(sizeof(ListNode));
    source->vertex = NO_CONNECTION; // 1st vertex is source. value = 1 + offset. offset is 0
    source->next = NULL;
    original_graph->list[1] = source; // add source to graph

    ListNode *sink = malloc(sizeof(ListNode));
    sink->vertex = NO_CONNECTION;  // sink is no 8. no need offset it's the last node, or it's 1 + offset which is source + projects + students + mentors
    sink->next = NULL;
    original_graph->list[total_vertices] = sink; // add sink to graph

    // create projects
    for (i = 1; i < projects + 1; i++) { // start indexing at 1
        ListNode *temp_project = malloc(sizeof(ListNode));
        index = i + projects_offset; // 0th is zeroth, 1st is source, 2 is project 1
        temp_project->vertex = NO_CONNECTION;
        temp_project->next = NULL;
        original_graph->list[index] = temp_project;
    }

    // create students
    for (i = 1; i < students + 1; i++) {
        ListNode *temp_student = malloc(sizeof(ListNode));
        index = i + students_offset; // 0th is zeroth, 1st is source, 2 and 3 are projects, 4 is student 1
        temp_student->vertex = NO_CONNECTION;
        temp_student->next = NULL;
        original_graph->list[index] = temp_student;
    }
    
    // create mentors
    for (i = 1; i < mentors + 1; i++) {
        ListNode *temp_mentor = malloc(sizeof(ListNode));
        index = i + mentors_offset; //  0th is zeroth, 1st is source, 2 and 3 are projects, 4 and 5 are students, 6 is mentor 1
        temp_mentor->vertex = NO_CONNECTION; // 1st mentor has value of 1 + offset = 1 + 5 = 6.
        temp_mentor->next = NULL;
        original_graph->list[index] = temp_mentor;
    }

    // connect source to projects
    for (i = 1; i < projects + 1; i++) {
        index = i + projects_offset;
        insert_adjacency(original_graph->list[1], index); 
        
    }

    // connect mentors to sink
    for (i = 1; i < mentors + 1; i++) {
        index = i + mentors_offset;
        insert_adjacency(original_graph->list[index], total_vertices);
    }

    // get inputs from each student
    for (i = 1; i < students + 1; i++) { // starting indexing from 1 instead of 0
        // student index
        int student_index;
        student_index = i + students_offset;

        // get input from student i
        printf("For student %d: enter number of preferred projects, number of preferred mentors, IDs of preferred projects, and IDs of preferred mentors\n", i);
        
        // get number of pref projs for student i, and number of pref mentors for student i
        scanf("%d %d", &pref_projs_no, &pref_ments_no);

        // get ids of projs for student i
        int pref_proj_index;
        ListNode *project_pointer;
        for (j = 1; j < pref_projs_no+1; j++) {
            scanf("%d", &pref_proj_index);
            pref_proj_index = pref_proj_index + projects_offset;

            // connect project to student
            insert_adjacency(original_graph->list[pref_proj_index], student_index);
            
        }

        // get ids of mentors for student i
        int pref_ment_index;
        ListNode *student_pointer;
        for (j = 1; j < pref_ments_no + 1; j++) {
            scanf("%d", &pref_ment_index);
            pref_ment_index = pref_ment_index + mentors_offset;

            // connect student to mentor
            insert_adjacency(original_graph->list[student_index], pref_ment_index);
        }
    }
    printGraphList(*original_graph);
    
    // Stack *path = reform_path(BFS(*original_graph, 1, 2 + mentors_offset), 1, 2 + mentors_offset);
    // printf("path is \n");
    // while (!isEmptyStack(*path)) {
    //     printf("%d ", peek(*path));
    //     pop(path);
    // }

    int max_flow = matching(*original_graph);
    printf("\n%d\n", max_flow);
    return 0;
}

void printGraphList(Graph g){
    int i;
    ListNode* temp;
    for(i = 1; i<g.V;i++)
    {
        printf("%d:\t",i);
        temp = g.list[i];
        while(temp!=NULL && temp->vertex != NO_CONNECTION){ // deal with no connection
            printf("%d -> ",temp->vertex); 
            temp = temp->next;
        }
        printf("\n");
    }
}

int *BFS (Graph g, int v, int w) {
    int *visited_array = malloc(g.V * sizeof(int));
    int i;
    int found = 0;
    for (i = 0; i < g.V; i++) { // array of visited nodes. 0 means unvisited. index is the node that was visited. initally all nodes are unvisited. value stored is the parent that lead to this node
        visited_array[i] = 0;
    }

    Queue *queueueue = malloc(sizeof(Queue));
    queueueue->size = 0;
    queueueue->head = NULL;
    queueueue->tail = NULL;

    ListNode *curr_node;

    enqueue(queueueue, v);
    visited_array[v] = v; // start node's own parent is itself

    while (!isEmptyQueue(*queueueue)) {
        int index = getFront(*queueueue);
        if (index == w) {
            found = 1;
            break; // found, jump to length calc
        }
        dequeue(queueueue);

        curr_node = g.list[index];
        while (curr_node != NULL && curr_node->vertex != NO_CONNECTION) { // goes through the adjacency list
            if (visited_array[curr_node->vertex] == 0) { // curr_node is unvisited
                enqueue(queueueue, curr_node->vertex);
                visited_array[curr_node->vertex] = index; // value is parent
            }
            curr_node = curr_node->next;
        }
    }

    return visited_array;
}

int BFS_len_calc(int *visited, int v, int w) {
    int curr = visited[w];
    int length = 1;
    while(curr != v) {
        if (curr == 0 || curr == NO_CONNECTION) {
            return -1;
        }
        curr = visited[curr];
        length++;
    }
    return length;
}

Stack *reform_path(int *visited, int v, int w) {
    Stack *stackackack = malloc(sizeof(Stack));
    stackackack->head = NULL;
    stackackack->size = 0;

    int curr = w;
    while (1) {
        push(stackackack, curr);
        if (curr == v) {
            break;
        }
        curr = visited[curr];
    }

    return stackackack;
}

int matching(Graph g)
{
    int to_return = 0;
    int *visited;
    Stack *curr_path;
    int top;
    int next;

    while(1) {
        // find path
        visited = BFS(g, 1, g.V - 1); // 1 is source, g.V is total nodes including 0th, g.V - 1 is index of sink
        if (BFS_len_calc(visited, 1, g.V - 1) == -1) { // path doesnt exist
            break;
        }

        // increase counter
        to_return++;

        // form path
        curr_path = reform_path(visited, 1, g.V - 1); // top of stack is source, bottom is sink top points to next currently, ie 1 points to 2, 2 points to 5, 5 points to 7
        printf("\npath found is\n");
        while (!isEmptyStack(*curr_path)) {
            printf("%d ", peek(*curr_path));
            pop(curr_path);
        }
        curr_path = reform_path(visited, 1, g.V - 1);

        // reverse path
        top = peek(*curr_path);
        pop(curr_path);

        while (!isEmptyStack(*curr_path)) {
            next = peek(*curr_path);
            pop(curr_path);

            // remove adjacency
            remove_adjacency(g.list[top], next);

            // add new adjacency
            insert_adjacency(g.list[next], top);

            // make top next for next iteration
            top = next;
        }
    }
    
    return to_return;
}

void remove_adjacency(ListNode *list, int value) {
    ListNode *curr;
    ListNode *prev;

    // find value
    int i = 0;
    curr = list;

    if (curr->vertex == value && curr->next == NULL) { // first value in the list. only value in the list
        curr->vertex = NO_CONNECTION;
        return;
    } else if (curr->vertex == value) { // first value but not only value
        curr->vertex = curr->next->vertex;
        curr->next->vertex = value;
    }

    while (curr->vertex != value) {
        prev = curr;
        curr = curr->next;
    }

    prev->next = curr->next;
    free(curr);
}

void insert_adjacency(ListNode *list, int value) {
    ListNode *curr;
    curr = list;
    if (curr->vertex == NO_CONNECTION) {
        curr->vertex = value;
    } else {
        while (curr->next != NULL) {
            curr = curr->next;
        }
        curr->next = malloc(sizeof(ListNode));
        curr->next->vertex = value;
        curr->next->next = NULL;
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