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
int *UCS(int **capacity_matrix, Graph *graph, int start, int goal);
int **edmonds_karp(Graph *graph, int **original_matrix, int start, int goal); // returns flow matrix
int flow_calc_in(int **flow_matrix, int flow_into, int size); // calculate flow into a vertex
int flow_calc_out(int **flow_matrix, int flow_out, int size); // calculate flow from a vertex
int matching(Graph *graph, int **original_matrix);
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
    // printf("Enter number of students, projects, and mentors:\n");
    scanf("%d %d %d", &students, &projects, &mentors);

    graph = create_graph(projects, students, mentors);
    // printf("successfully created graph\n");
    // print_graph_list(*graph);
    // print_graph_matrix(graph->matrix, graph->V);
    original_graph_matrix = copy_graph_matrix(graph->matrix, graph->V);

    int max_matches = matching(graph, original_graph_matrix);
    printf("%d\n", max_matches);
}

// ------------------- graph functions -------------------
Graph *create_graph(int projects, int students, int mentors) {
    int i, j;
    Graph *to_return = malloc(sizeof(Graph));
    to_return->E = 0;
    to_return->V = 5 + projects + students + mentors; // the 5 are zeroth, source 1 source 2, and sink 1 sink 2. source and sink 1 are for project student matching. source and sink 2 are for student mentor matching

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
    int source_offset = 0; // source is 1 + offset. offset is zeroth
    int project_offset = 2; // 1st project is at 1 + offset. offsets are the 2 sources
    int student_offset = 2 + projects; // 1st student is at 1 + offset
    int mentor_offset = 2 + projects + students; // 1st mentor is at 1 + offset
    int sink_offset = 2 + projects + students + mentors; // sink is last node or 1 + offset

    // connect offsets
    to_return->project_offset = project_offset;
    to_return->student_offset = student_offset;
    to_return->mentor_offset = mentor_offset;
    to_return->sink_offset = sink_offset;

    // make source 1 point to all projects. src_1 -> project -> student -> sink_1
    int project_index;
    for (i = 1; i < projects + 1; i++) {
        project_index = i + project_offset;
        add_adjacency(&(to_return->list[1]), &(to_return->matrix[1]), project_index); // [1] is first source
        to_return->matrix[project_index][1] = 0; // make capacity in opposite direction 0
        to_return->E++;
    }

    // make source 2 point to all students. src_2 -> student -> mentor -> sink_2
    int student_index;
    for (i = 1; i < students + 1; i++) {
        student_index = i + student_offset;
        add_adjacency(&(to_return->list[2]), &(to_return->matrix[2]), student_index);
        to_return->matrix[student_index][2] = 0; // make capacity in opposite direction 0
        to_return->E++;
    }

    // make all students point to sink 1. src_1 -> project -> student -> sink_1
    for (i = 1; i < students + 1; i++) {
        student_index = i + student_offset;
        add_adjacency(&(to_return->list[student_index]), &(to_return->matrix[student_index]), 1 + sink_offset);
        to_return->matrix[1 + sink_offset][student_index] = 0; // make capacity in opposite direction 0
        to_return->E++;
    }

    // make all mentors point to sink 2. src_2 -> student -> mentor -> sink_2
    int mentor_index;
    for (i = 1; i < mentors + 1; i++) {
        mentor_index = i + mentor_offset;
        add_adjacency(&(to_return->list[mentor_index]), &(to_return->matrix[mentor_index]), 2 + sink_offset);
        to_return->matrix[1+sink_offset][mentor_index] = 0; // make capacity in opposite direction 0
        to_return->E++;
    }

    // prompt student for inputs. src_1 -> project -> student -> sink_1
    for (i = 1; i < students + 1; i++) {
        int preferred_projects_amount, preferred_mentors_amount;
        student_index = i + student_offset;
        // printf("For student %d: enter number of preferred projects, number of preferred mentors, IDs of preferred projects, and IDs of preferred mentors\n", i);
        scanf("%d %d", &preferred_projects_amount, &preferred_mentors_amount);

        // get project id for student. 
        for (j = 1; j < preferred_projects_amount + 1; j++) {
            scanf("%d", &project_index);
            project_index = project_index + project_offset;

            // connect project to student
            add_adjacency(&(to_return->list[project_index]), &(to_return->matrix[project_index]), student_index);
            to_return->matrix[student_index][project_index] = 0; // make capacity in opposite direction 0
            to_return->E++;
        }

        // get mentor id for student. src_2 -> student -> mentor -> sink_2
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

int *UCS(int **capacity_matrix, Graph *graph, int start, int goal) { // UCS == BFS when cost is the same. Capacity is 1 for every edge so implementing BFS first
    int *visited_array = malloc(graph->V * sizeof(int));
    int i;
    int index;
    for (i = 0; i < graph->V; i++) {
        visited_array[i] = 0;
    }

    Queue *queueueue = malloc(sizeof(Queue));
    queueueue->head = NULL;
    queueueue->tail = NULL;
    queueueue->size = 0;

    enqueue(queueueue, start);
    visited_array[start] = start; // start parent's is itself

    while (!is_empty_queue(*queueueue)) {
        index = get_front(*queueueue);
        dequeue(queueueue);
        if (index == goal) {
            break;
        }
        
        for (i = 0; i < graph->V; i++) {
            if (capacity_matrix[index][i] == 1 && visited_array[i] == 0) { // there is capacity along the edge from index to i. change == 1 for to something else for true UCS
                enqueue(queueueue, i);
                visited_array[i] = index; // set i's parent as index. aka i was visited because of index
            }
        }
    }

    // remove_all_items_from_queue(queueueue);

    return visited_array;
}

int **edmonds_karp(Graph *graph, int **original_matrix, int start, int goal) {
    int to_return = 0;
    int i, j;
    int minimum_flow = 1; // matching so flow is 1

    // initialize flow;
    int **flow_matrix = copy_graph_matrix(original_matrix, graph->V);

    for (i = 0; i < graph->V; i++) {
        for (j = 0; j < graph->V; j++) {
            if (flow_matrix[i][j] != NO_CONNECTION) { // NO_CONNECTION remains as NO_CONNECTION
                flow_matrix[i][j] = 0; // set flow to and from every existing edge to 0
            }
        }
    }

    // find path
    int path_length = 1;
    int *visited_array;
    Stack *path;
    int first_vertex, second_vertex;

    while (path_length != -1) {
        // find path from start to goal
        visited_array = UCS(graph->matrix, graph, start, goal); // graph is used as residual graph. pass in graph->matrix. original_matrix is for preserving the graph matrix before matching flips the edges
        path_length = length_calc(visited_array, start, goal);
        if (path_length == -1) {
            break;
        }

        path = reform_path(visited_array, start, goal);

        // start for loop
        first_vertex = peek(*path);
        pop(path);

        while (!is_empty_stack(*path)) {
            second_vertex = peek(*path);

            // add new flow
            if (is_in_matrix(original_matrix, first_vertex, second_vertex)) { // first_vertex -> second_vertex exists in the original graph
                flow_matrix[first_vertex][second_vertex] += minimum_flow;
            } else {
                flow_matrix[second_vertex][first_vertex] -= minimum_flow;
            }

            // update residual graph
            remove_adjacency(&(graph->list[first_vertex]), &(graph->matrix[first_vertex]), second_vertex); // function modifies the matrix, changes from 1 to 0, which is the same as subtracting minimum flow
            add_adjacency(&(graph->list[second_vertex]), &(graph->matrix[second_vertex]), first_vertex); // function modifies the matrix, changes from 0 to 1, which is the same as adding minimum flow

            // update vertices
            first_vertex = second_vertex;
            pop(path); // second vertex will be update at the next iteration
        }

        // printf("after flipping\n");
        // print_graph_list(*graph);
    }

    // free
    // remove_all_items_from_stack(path);
    // free(visited_array);
    
    return flow_matrix;
}

int flow_calc_in(int **flow_matrix, int flow_into, int size) {
    int to_return = 0;
    for (int i = 0; i < size; i++) {
        if (flow_matrix[i][flow_into] == 1) {
            to_return++;
        }
    }

    return to_return;
}

int flow_calc_out(int **flow_matrix, int flow_out, int size) {
    int to_return = 0;
    for (int i = 0; i < size; i++) {
        if (flow_matrix[flow_out][i] == 1) {
            to_return++;
        }
    }

    return to_return;
}

int matching(Graph *graph, int **original_matrix) {
    int **p_to_s_matrix;
    int **s_to_m_matrix;
    int p_to_s, s_to_m;

    // src_1 -> project -> student -> sink_1
    // printf("finding source 1 to project to student to sink 1\n");
    p_to_s_matrix = edmonds_karp(graph, original_matrix, 1, graph->sink_offset + 1);

    // calculate flow from student into sink 1
    p_to_s = flow_calc_in(p_to_s_matrix, graph->sink_offset + 1, graph->V);

    // src_2 -> student -> mentor -> sink_2
    // printf("finding source 2 to student to mentor to sink 2\n");
    s_to_m_matrix = edmonds_karp(graph, original_matrix, 2, graph->sink_offset + 2);

    // calculate flow from source 2 to student
    s_to_m = flow_calc_out(s_to_m_matrix, 2, graph->V);

    // free
    // free(p_to_s_matrix);
    // free(s_to_m_matrix);

    if (s_to_m <= p_to_s) {
        return s_to_m;
    } else {
        return p_to_s;
    }
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