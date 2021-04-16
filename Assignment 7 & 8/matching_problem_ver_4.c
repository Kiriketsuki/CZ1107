#include <stdio.h>
#include <stdlib.h>
#define NO_CONNECTION 99999

typedef struct _listnode
{
    int vertex;
	struct _listnode *next;
} ListNode;
typedef ListNode StackNode;

typedef struct _graph{
    int V;
    int E;
    int **matrix; // matrix is for capacity!!
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

// ------------------- graph functions -------------------
Graph *create_graph(int projects, int students, int mentors);
Graph *create_graph_ver_2(int projects, int students, int mentors);
void add_adjacency(ListNode **adjacent_list, int **adjacent_matrix, int adjacent_to);
void remove_adjacency(ListNode **adjacent_list, int **adjacent_matrix, int was_adjacent_to);
void print_graph_list(Graph g);
void print_graph_matrix(int **matrix, int size);
int **copy_graph_matrix(int **original_matrix, int size);
int *UCS(int **capacity_matrix, Graph *graph, int start, int goal, int minimum, int maximum);
int *UCS_ver_2(int **capacity_matrix, Graph *graph, int start, int goal);
int *DFS(int **capacity_matrix, Graph *graph, int start, int goal);
int **edmonds_karp(Graph *graph); // returns flow matrix
int **edmonds_karp_ver_2(Graph *original_graph);
int flow_calc_in(int **flow_matrix, int flow_into, int size); // calculate flow into a vertex
int flow_calc_out(int **flow_matrix, int flow_out, int size); // calculate flow from a vertex
int matching(Graph *graph);
int matching_ver_2(Graph *graph, int **original_matrix);
int length_calc(int *visited, int v, int w);
Stack *reform_path(Graph *g, int *visited);
int is_in_matrix(int **matrix, int pointer, int pointed);
// ------------------- queue functions -------------------
void enqueue(Queue *qPtr, int vertex);
int dequeue(Queue *qPtr);
int get_front(Queue q);
int is_empty_queue(Queue q);
void remove_all_items_from_queue(Queue *qPtr);
void print_queue(QueueNode *curr);
// ------------------- stack functions -------------------
void push(Stack *sPtr, int vertex);
int pop(Stack *sPtr);
int is_empty_stack(Stack s);
int peek(Stack s);
void remove_all_items_from_stack(Stack *sPtr);
void print_stack(StackNode *curr);
// ------------------------ main -------------------------

int main() {
    int projects, students, mentors;
    scanf("%d %d %d", &students, &projects, &mentors);


    Graph *graph = create_graph(projects, students, mentors);
    printf("%d", matching(graph));
}

Graph *create_graph(int projects, int students, int mentors) {
    int i, j;

    Graph *graph = malloc(sizeof(Graph));
    graph->V = projects + students + students + mentors + 2; // sink source
    graph->E = 0;
    graph->matrix = (int **)malloc(graph->V * sizeof(int *));

    for (i = 0; i < graph->V; i++) {
        graph->matrix[i] = malloc(graph->V * sizeof(int));
        for (j = 0; j < graph->V; j++) {
            graph->matrix[i][j] = NO_CONNECTION;
        }
    }

    // connections and indices:
    // 0 to projects - 1 are projects
    // projects to (students - 1) are students
    // students to (students + students - 1) are student dupes
    // students + students to (mentors - 1) are mentors
    // gv - 2 is src
    // gv - 1 is sink

    int project_offset = -1;
    int student_offset = project_offset + projects;
    int student_dupe_offset = student_offset + students;
    int mentor_offset = student_dupe_offset + students;

    // source to project
    for (i = 1; i < projects + 1; i++) {
        graph->matrix[graph->V - 2][i] = 1;
        graph->matrix[i][graph->V - 2] = 0;
    }

    // student to student dupe
    for (i = 1; i < students + 1; i++) {
        graph->matrix[i + student_offset][i + student_dupe_offset] = 1;
        graph->matrix[i + student_dupe_offset][i + student_offset] = 0;
    }

    // mentor to sink
    for (i = 1; i < mentors + 1; i++) {
        graph->matrix[i + mentor_offset][graph->V - 1] = 1;
        graph->matrix[graph->V - 1][i + mentor_offset] = 0;
    }

    // get input from student
    int pref_proj, pref_ment;

    for (i = 1; i < students + 1; i++) {
        scanf("%d %d", &pref_proj, &pref_ment);

        int proj_id;

        for (j = 0; j < pref_proj; j++) {
            scanf("%d", &proj_id);
            // project to student
            graph->matrix[proj_id + project_offset][i + student_offset] = 1;
            graph->matrix[i + student_offset][proj_id + project_offset] = 0;
        }

        int ment_id;

        for (j = 0; j < pref_ment; j++) {
            scanf("%d", &ment_id);
            // student dupe to ment
            graph->matrix[i + student_dupe_offset][ment_id + mentor_offset] = 1;
            graph->matrix[ment_id + mentor_offset][i + student_dupe_offset] = 0;
        }
    }

    return graph;
}

int *bfs(Graph *graph) {
    int *visited = malloc(graph->V * sizeof(int));
    int i, index;

    for (i = 0; i < graph->V; i++) {
        visited[i] = -1;
    }

    Queue *queueue = malloc(sizeof(Queue));
    queueue->head = NULL;
    queueue->tail = NULL;
    queueue->size = 0;

    index = (graph->V - 2); // src
    enqueue(queueue, index);
    visited[index] = index;

    while (!is_empty_queue(*queueue)) {
        index = queueue->head->vertex;
        dequeue(queueue);

        if (index == (graph->V - 1)) { // sink
            break;
        }

        for (i = 0; i < graph->V; i++) {
            if (graph->matrix[index][i] == 1 && visited[i] == -1) {
                enqueue(queueue, i);
                visited[i] = index;
            }
        }
    }

    return visited;
}

int path_length(Graph *graph, int *visited) {
    int curr = visited[graph->V - 1];
    int length = 0;

    while (curr != graph->V - 2) {
        if (curr == -1) {
            return -1;
        }
        curr = visited[curr];
        length++;
    }

    return length;
}

Stack *reform_path(Graph *graph, int *visited) {
    Stack *path = malloc(sizeof(Stack));
    path->head = NULL;
    path->size = 0;

    int curr = graph->V - 1; // sink

    while (1) {
        push(path, curr);
        if (curr == (graph->V - 2)) { // src
            break;
        }
        curr = visited[curr];
    }

    return path;
}

int matching(Graph *graph) {
    int i;
    int to_return = 0;

    while (1) {
        int *visited = bfs(graph);
        int length = path_length(graph, visited);

        if (length == -1) {
            break;
        }

        Stack *path = reform_path(graph, visited);
        // print_stack(path->head);
        int first = peek(*path);
        int second;
        pop(path);

        while (!is_empty_stack(*path)) {
            second = peek(*path);

            graph->matrix[first][second] = 0;
            graph->matrix[second][first] = 1;
        
            first = second;
            pop(path);
        }
    }

    while (1) {
        int *visited = bfs(graph);
        int length = path_length(graph, visited);

        if (length == -1) {
            break;
        }

        Stack *path = reform_path(graph, visited);
        int first = peek(*path);
        int second;
        pop(path);

        while (!is_empty_stack(*path)) {
            second = peek(*path);

            graph->matrix[first][second] = 0;
            graph->matrix[second][first] = 1;
        
            first = second;
            pop(path);
        }
    }

    for (i = 0; i < graph->V; i++) {
        if (graph->matrix[graph->V - 1][i] == 1) {
            to_return++;
        }
    }

    return to_return;
}

void enqueue(Queue *qPtr, int vertex) {
    QueueNode *newNode;
    newNode = malloc(sizeof(QueueNode));
    if(newNode == NULL) exit(0);

    newNode->vertex = vertex;
    newNode->next = NULL;

    if(is_empty_queue(*qPtr))
        qPtr->head=newNode;
    else
        qPtr->tail->next = newNode;

    qPtr->tail = newNode;
    qPtr->size++;
}

int is_empty_queue(Queue q) {
    if(q.size == 0) return 1;
    else return 0;
}

int dequeue(Queue *qPtr) {
    if(qPtr == NULL || qPtr->head == NULL){ //Queue is empty or NULL pointer
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

void push(Stack *sPtr, int vertex) {
	StackNode *newNode;
    newNode= malloc(sizeof(StackNode));
    newNode->vertex = vertex;
    newNode->next = sPtr->head;
    sPtr->head = newNode;
    sPtr->size++;
}

int peek(Stack s) {
    return s.head->vertex;
}

int pop(Stack *sPtr) {
    if(sPtr == NULL || sPtr->head == NULL){
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

int is_empty_stack(Stack s) {
     if(s.size == 0) return 1;
     else return 0;
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