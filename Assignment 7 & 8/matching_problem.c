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
    int project_offset;
    int student_offset;
    int mentor_offset;
    int sink_offset;
    ListNode **list;
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

// function declarations
// ------------------- graph functions -------------------
Graph *create_graph(int projects, int students, int mentors);
void add_adjacency(ListNode **adjacent_list, int **adjacent_matrix, int adjacent_to);
void remove_adjacency(ListNode **adjacent_list, int **adjacent_matrix, int was_adjacent_to);
void print_graph_list(Graph g);
void print_graph_matrix(int **matrix, int size);
int **copy_graph_matrix(int **original_matrix, int size);
int matching(Graph *graph, int projects, int students, int mentors, int **original_matrix);
int *BFS (Graph *g, int start, int goal);
int *BFS_list (Graph g, int v, int w);
int length_calc(int *visited, int v, int w);
Stack *reform_path(int *visited, int v, int w);
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
    int **original_graph_matrix;
    Graph *graph;
    printf("Enter number of students, projects, and mentors:\n");
    scanf("%d %d %d", &students, &projects, &mentors);

    graph = create_graph(projects, students, mentors);
    // printf("successfully created graph\n");
    // print_graph_list(*graph);
    // print_graph_matrix(graph->matrix, graph->V);
    original_graph_matrix = copy_graph_matrix(graph->matrix, graph->V);


    printf("finding path from source to sink\n");
    int *visited_array_matrix = BFS(graph, 1, 1 + graph->sink_offset);
    int *visited_array_list = BFS_list(*graph, 1, 1 + graph->sink_offset);
    // printf("length of path is %d\n", length_calc(visited_array, 1, graph->sink_offset + 1));
    printf("matrix path is \n");
    Stack *path = reform_path(visited_array_matrix, 1, graph->sink_offset + 1);
    print_stack(path->head);

    printf("list path is \n");
    path = reform_path(visited_array_list, 1, graph->sink_offset + 1);
    print_stack(path->head);

    int max_matches = matching(graph, projects, students, mentors, original_graph_matrix);
    printf("%d\n", max_matches);
}

// ------------------- graph functions -------------------
Graph *create_graph(int projects, int students, int mentors) {
    int i, j;
    Graph *to_return = malloc(sizeof(Graph));
    to_return->E = 0;
    to_return->V = 3 + projects + students + mentors; // the 3 are zeroth, src, and sink

    // form graph's matrix. matrix[i][j] represents capacity from i to j
    to_return->matrix = (int **)malloc(to_return->V * sizeof(int *));

    for (i = 0; i < to_return->V; i++) {
        to_return->matrix[i] = (int *)malloc(to_return->V *sizeof(int)); // first layer
    }

    for (i = 0; i < to_return->V; i++) {
        for (j = 0; j < to_return->V; j++) {
            to_return->matrix[i][j] = NO_CONNECTION; // every vertex isnt connected to another node
        }
    }

    // form graph's list;
    to_return->list = malloc(to_return->V * sizeof(ListNode));
    for (i = 0; i < to_return->V; i++) {
        to_return->list[i] = NULL;
    }

    // set offsets; index of vertex is offset plus whichever value it is
    int source_offset = 0; // source is 1 + offset
    int project_offset = 1; // 1st project is at 1 + offset
    int student_offset = 1 + projects; // 1st student is at 1 + offset
    int mentor_offset = 1 + projects + students; // 1st mentor is at 1 + offset
    int sink_offset = 1 + projects + students + mentors; // sink is last node or 1 + offset

    // connect offsets
    to_return->project_offset = project_offset;
    to_return->student_offset = student_offset;
    to_return->mentor_offset = mentor_offset;
    to_return->sink_offset = sink_offset;

    // make source point to all projects
    int project_index;
    for (i = 1; i < projects + 1; i++) {
        project_index = i + project_offset;
        add_adjacency(&(to_return->list[1]), &(to_return->matrix[1]), project_index);
        to_return->matrix[project_index][i] = 0; // make capacity in opposite direction 0
        to_return->E++;
    }

    // make all mentors point to sink
    int mentor_index;
    for (i = 1; i < mentors + 1; i++) {
        mentor_index = i + mentor_offset;
        add_adjacency(&(to_return->list[mentor_index]), &(to_return->matrix[mentor_index]), 1 + sink_offset);
        to_return->matrix[1+sink_offset][mentor_index] = 0; // make capacity in opposite direction 0
        to_return->E++;
    }

    // prompt student for inputs
    int student_index;
    for (i = 1; i < students + 1; i++) {
        int preferred_projects_amount, preferred_mentors_amount;
        student_index = i + student_offset;
        // printf("For student %d: enter number of preferred projects, number of preferred mentors, IDs of preferred projects, and IDs of preferred mentors\n", i);
        scanf("%d %d", &preferred_projects_amount, &preferred_mentors_amount);

        // get project id for student
        for (j = 1; j < preferred_projects_amount + 1; j++) {
            scanf("%d", &project_index);
            project_index = project_index + project_offset;

            // connect project to student
            add_adjacency(&(to_return->list[project_index]), &(to_return->matrix[project_index]), student_index);
            to_return->matrix[student_index][project_index] = 0; // make capacity in opposite direction 0
            to_return->E++;
        }

        // get mentor id for student
        for (j = 1; j < preferred_mentors_amount + 1; j++) {
            scanf("%d", &mentor_index);
            mentor_index = mentor_index + mentor_offset;

            // connect student to mentor
            add_adjacency(&(to_return->list[student_index]), &(to_return->matrix[student_index]), mentor_index);
            to_return->matrix[mentor_index][student_index] = 0; // make capacity in opposite direction 0
            to_return->E++;
        }
    }

    return to_return;
}

void add_adjacency(ListNode **adjacent_list, int **adjacent_matrix, int adjacent_to) {
    // pass in graph.list[1], graph.matrix[1], and 2, to make 1 point to 2

    // adjacency list first
    ListNode *curr = *adjacent_list;

    if (curr == NULL) { // wasnt adjacent to anything in the past
        *adjacent_list = malloc(sizeof(ListNode));
        (*adjacent_list)->vertex = adjacent_to;
        (*adjacent_list)->next = NULL;
    } else {
        while (curr->next != NULL) {
            curr = curr->next;
        }
        
        curr->next = malloc(sizeof(ListNode));
        curr->next->vertex = adjacent_to;
        curr->next->next = NULL;
    }

    // adjacency matrix

    (*adjacent_matrix)[adjacent_to] = 1;
}

void remove_adjacency(ListNode **adjacent_list, int **adjacent_matrix, int was_adjacent_to) {
    // pass in graph.list[1], graph.matrix[1], and 2, to make 1 no longer point to 2. use when sure they were previously connected
    // for the matrix, use in conjunction with add adjacency, or you'll have to manually set to NO_CONNECTION

    // adjacency list first
    ListNode *curr = *adjacent_list;
    ListNode *prev;
    if (curr->vertex == was_adjacent_to) { // first item
        *adjacent_list = curr->next;
        free(curr);
    } else {
        while (curr->vertex != was_adjacent_to) {
            prev = curr;
            curr = curr->next;
        }
        prev->next = curr->next;
        free(curr);
    }

    // adjacency matrix

    (*adjacent_matrix)[was_adjacent_to] = 0; // because the edge exists just in a different direction so capacity is 0 and not NO_CONNECTION
}

void print_graph_list(Graph g){
    int i;
    ListNode *curr;
    for(i = 1; i < g.V; i++) {
        printf("%d:\t", i);
        curr = g.list[i];
        while(curr != NULL) {
            printf("%d -> ",curr->vertex); 
            curr = curr->next;
        }
        printf("\n");
    }
}

void print_graph_matrix(int **matrix, int size) {
    int i, j;
    for(i = 1; i < size; i++){
        for(j = 1; j < size; j++)
            if (matrix[i][j] != NO_CONNECTION) {
                printf("%d\t", matrix[i][j]);
            } else {
                printf("x\t");
            }
        printf("\n");
    }
}

int **copy_graph_matrix(int **original_matrix, int size) {
    int **to_return = (int **)malloc(size * sizeof(int *));
    int i, j;
    for (i = 0; i < size; i++) {
        to_return[i] = (int *)malloc(size *sizeof(int)); // first layer
    }

    for (i = 0; i < size; i++) {
        for (j = 0; j < size; j++) {
            to_return[i][j] = original_matrix[i][j];
        }
    }

    return to_return;
}

int matching(Graph *graph, int projects, int students, int mentors, int **original_matrix) {
    int minimum_flow = 1;
    int i, j;

    // initialize flow
    int **flow_matrix = copy_graph_matrix(original_matrix, graph->V);

    for (i = 0; i < graph->V; i++) {
        for (j = 0; j < graph->V; j++) {
            if (flow_matrix[i][j] != NO_CONNECTION) {
                flow_matrix[i][j] = 0;
            }
        }
    }

    // start finding path
    int path_length = 1;
    int *visted_array;
    Stack *path;
    int first_vertex, second_vertex;

    while (path_length != -1) { // length calc returns -1 if no path
        // find path from source, 1, to sink, 1 + sink offset
        visted_array = BFS(graph, 1, graph->sink_offset + 1);
        path_length = length_calc(visted_array, 1, graph->sink_offset + 1);
        if (path_length == -1) {
            break;
        }
        path = reform_path(visted_array, 1, graph->sink_offset + 1);
        printf("path found in matching is \n");
        print_stack(path->head);

        printf("matrix before\n");
        print_graph_matrix(graph->matrix, graph->V);

        // start for loop
        first_vertex = peek(*path);
        pop(path);
        while (!is_empty_stack(*path)) {
            second_vertex = peek(*path);

            // start adding flow
            if (is_in_matrix(original_matrix, first_vertex, second_vertex)) {
                // printf("added something!!\n");
                flow_matrix[first_vertex][second_vertex] += minimum_flow;
            } else {
                // printf("minused something!!\n");
                flow_matrix[first_vertex][second_vertex] -= minimum_flow;
            }

            // update capacity of residual graph (original graph is enough)
            remove_adjacency(&(graph->list[first_vertex]), &(graph->matrix[first_vertex]), second_vertex); // function sets to 0 capacity instead of NO_CAPACITY
            add_adjacency(&(graph->list[second_vertex]), &(graph->matrix[second_vertex]), first_vertex);

            first_vertex = second_vertex;
            pop(path);
        }
        printf("matrix after\n");
        print_graph_matrix(graph->matrix, graph->V);
    }

    int to_return = 0;

    printf("printing flow matrix\n");
    print_graph_matrix(flow_matrix, graph->V);

    // loop through every student. if they have flow from a project and to a mentor they have a match
    int student_index;
    int project_index;
    int mentor_index;
    int has_project;
    int has_mentor;
    for (i = 1; i < students + 1; i++) {
        has_project = 0;
        has_mentor = 0;
        student_index = i + graph->student_offset;
        for (j = 1; j < projects + 1; j++) {
            project_index = j + graph->project_offset;
            if (flow_matrix[project_index][student_index] == 1) {
                has_project = 1;
            }
        }

        for (j = 1; j < mentors + 1; j++) {
            mentor_index = j + graph->mentor_offset;
            if(flow_matrix[student_index][mentor_index] == 1) {
                has_mentor = 1;
            }
        }

        if (has_project * has_project) { // easter!! egg. code written in april so this checks out
            to_return++;
        }
    }

    return to_return;
}

int *BFS(Graph *g, int start, int goal) {
    int *visited_array = malloc(g->V *sizeof(int));
    int i = 0;
    int j = 0;
    for (i = 0; i < g->V; i++) {
        visited_array[i] = 0;
    }

    Queue *queueueueue = malloc(sizeof(Queue));
    queueueueue->head = NULL;
    queueueueue->tail = NULL;
    queueueueue->size = 0;

    ListNode *curr_node;

    enqueue(queueueueue, start);
    visited_array[start] = start;

    while (!is_empty_queue(*queueueueue)) {
        int index = get_front(*queueueueue);
        if (index == goal) {
            break;
        }
        dequeue(queueueueue);

        for (i = 0; i < g->V; i++) {
            if (g->matrix[index][i] == 1) {
                enqueue(queueueueue, i);
                visited_array[i] = index;
            }
        }
    }

    return visited_array;
}

int *BFS_list(Graph g, int v, int w) {
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

    while (!is_empty_queue(*queueueue)) {
        int index = get_front(*queueueue);
        if (index == w) {
            break;
        }
        dequeue(queueueue);

        curr_node = g.list[index];
        while (curr_node != NULL) { // goes through the adjacency list
            if (visited_array[curr_node->vertex] == 0) { // curr_node is unvisited
                enqueue(queueueue, curr_node->vertex);
                visited_array[curr_node->vertex] = index; // value is parent
            }
            curr_node = curr_node->next;
        }
    }

    return visited_array;
}

int length_calc(int *visited, int v, int w) {
    int curr = visited[w];
    int length = 1;
    while(curr != v) {
        if (curr == 0) {
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

int is_in_matrix(int **matrix, int pointer, int pointed) { // returns 1 if pointer points to pointed according to input graph matrix
    if (matrix[pointer][pointed] == 1) {
        return 1;
    } else {
        return 0;
    }
}
// ------------------- queue functions -------------------
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

int get_front(Queue q) {
    return q.head->vertex;
}

int is_empty_queue(Queue q) {
    if(q.size == 0) return 1;
    else return 0;
}

void remove_all_items_from_queue(Queue *qPtr) {
	while(dequeue(qPtr));
}

void print_queue(QueueNode *curr) {
    if(curr == NULL) {
        printf("Empty");
    } 

    while (curr != NULL){
        printf("%d ", curr->vertex);
        curr = curr->next;
    }
    printf("\n");
}
// ------------------- stack functions -------------------
void push(Stack *sPtr, int vertex) {
	StackNode *newNode;
    newNode= malloc(sizeof(StackNode));
    newNode->vertex = vertex;
    newNode->next = sPtr->head;
    sPtr->head = newNode;
    sPtr->size++;
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

int peek(Stack s) {
    return s.head->vertex;
}

void remove_all_items_from_stack(Stack *sPtr) {
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