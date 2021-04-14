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
int *UCS(int **capacity_matrix, Graph *graph, int start, int goal, int minimum, int maximum);
int **edmonds_karp(Graph *graph); // returns flow matrix
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

    // test UCS
    // int *visited_test = UCS(graph->matrix, graph, 1, 2 + graph->sink_offset, -1, NO_CONNECTION);
    // printf("length from src 1 to sink 2 is %d\n", length_calc(visited_test, 1, 2 + graph->sink_offset));
    // printf("path found is\n");
    // Stack *test_path = reform_path(visited_test, 1, 2 + graph->sink_offset);
    // print_stack(test_path->head);

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

        // neuter the student if it inputs 0 for either input. neuter after scanf so it doesnt ruin future inputs.
        if (!(preferred_mentors_amount * preferred_projects_amount)) {
            for (j = 0; j < to_return->V; j++) {
                // it points to nothing
                add_adjacency(&(to_return->list[student_index]), &(to_return->matrix[student_index]), j); // bypass the remove_adjacency's requirement of having previous connection
                remove_adjacency(&(to_return->list[student_index]), &(to_return->matrix[student_index]), j);
                to_return->matrix[student_index][j] = NO_CONNECTION; // because remove function makes capacity 0 not NO_CONNECTION

                // nothing points to it
                add_adjacency(&(to_return->list[j]), &(to_return->matrix[j]), student_index); // bypass the remove_adjacency's requirement of having previous connection
                remove_adjacency(&(to_return->list[j]), &(to_return->matrix[j]), student_index);
                to_return->matrix[j][student_index] = NO_CONNECTION; // because remove function makes capacity 0 not NO_CONNECTION
            }
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

Graph *copy_graph(Graph *original_graph) {
    Graph *to_return = malloc(sizeof(Graph));
    int i, j;

    // copy ints
    to_return->V = original_graph->V;
    to_return->E = original_graph->E;
    to_return->student_offset = original_graph->student_offset;
    to_return->project_offset = original_graph->project_offset;
    to_return->mentor_offset = original_graph->mentor_offset;
    to_return->sink_offset = original_graph->sink_offset;

    // copy matrix
    to_return->matrix = copy_graph_matrix(original_graph->matrix, original_graph->V);

    // copy list
    to_return->list = malloc(to_return->V * sizeof(ListNode)); // malloc list array

    ListNode *curr;
    ListNode *temp;
    ListNode *prev;

    for (i = 0; i < to_return->V; i++) {
        curr = original_graph->list[i];

        if (curr == NULL) {
            to_return->list[i] = NULL;
        } else {
            to_return->list[i] = malloc(sizeof(ListNode));
            temp = to_return->list[i];
            while (curr != NULL) {
                temp->vertex = curr->vertex;
                temp->next = malloc(sizeof(ListNode));
                prev = temp;
                temp = temp->next;
                curr = curr->next;
            }
            prev->next = NULL;
        }
    }

    // return

    return to_return;
}

int *UCS(int **capacity_matrix, Graph *graph, int start, int goal, int minimum, int maximum) {
    int *visited_array = malloc(graph->V * sizeof(int));
    int i;
    int index;

    // create visited array to keep track of visited vertices. 0 means unvisited.
    for (i = 0; i < graph->V; i++) {
        visited_array[i] = 0;
    }

    // create queue for searching algorithm
    Queue *queueueueue = malloc(sizeof(Queue));
    queueueueue->head = NULL;
    queueueueue->tail = NULL;
    queueueueue->size = 0;

    // start algorithm
    // enqueue start
    enqueue(queueueueue, start);
    visited_array[start] = start; // start's parent is itself

    while (!is_empty_queue(*queueueueue)) {
        index = get_front(*queueueueue);
        dequeue(queueueueue);

        if (index == goal) {
            break;
        }

        for (i = 0; i < graph->V; i++) {
            if (i <= minimum && i != 1 && i != 2) { // 1 and 2 are sources. if minimum is set, means UCS shouldnt go into those vertices, usually project vertices. only allowed to go to source, sink, students, mentors
                continue;
            } else if (i >= maximum && i != (1 + graph->sink_offset) && i != (2 + graph->sink_offset)) { // if maximum is set, UCS shouldnt go into those vertices, only allowed to source, sink, projects, students
                continue;
            } else {
                if (capacity_matrix[index][i] == 1 && visited_array[i] == 0) { // index is adjacent to i, and i wasnt visited previously
                    enqueue(queueueueue, i);
                    visited_array[i] = index; // i was visited from index. set i's parent as index
                }
            }
        }
    }

    return visited_array;
}

int **edmonds_karp(Graph *original_graph) {
    int  i, j;

    // create residual graph
    Graph *residual_graph = copy_graph(original_graph);

    // create capacity matrix of residual graph
    int **capacity_matrix = copy_graph_matrix(residual_graph->matrix, residual_graph->V);

    // create flow matrix of residual graph
    int **flow_matrix = copy_graph_matrix(residual_graph->matrix, residual_graph->V);

    for (i = 0; i < residual_graph->V; i++) {
        for (j = 0; j < residual_graph->V; j++) {
            if (flow_matrix[i][j] != NO_CONNECTION) { // NO_CONNECTION remains as NO_CONNECTION
                flow_matrix[i][j] = 0; // set flow to and from every existing edge to 0
            }
        }
    }

    // start algorithm for source 1 to sink 1, ie match project to students
    int source_1 = 1;
    int sink_1 = 1 + residual_graph->sink_offset;
    int *first_visited_array;
    Stack *first_path;
    int first_bipartite_length = 1;

    while (first_bipartite_length != -1) { // while finding path

        // path finding
        first_visited_array = UCS(residual_graph->matrix, residual_graph, source_1, sink_1, -1, 1 + residual_graph->mentor_offset); // can go from index 0 to right before first mentor
        first_bipartite_length = length_calc(first_visited_array, 1, 1 + residual_graph->sink_offset);

        if (first_bipartite_length == -1) {
            break;
        }

        first_path = reform_path(first_visited_array, source_1, sink_1);

        // set minimum flow
        int minimum_flow = 1;

        // start for loop
        int first_vertex = peek(*first_path);
        int second_vertex;
        pop(first_path);

        while (!is_empty_stack(*first_path)) {
            second_vertex = peek(*first_path);

            // add flow
            if (original_graph->matrix[first_vertex][second_vertex] == 1) { // first_vertex -> second_vertex was an edge in the original graph
                flow_matrix[first_vertex][second_vertex] += minimum_flow;
            } else {
                original_graph->matrix[second_vertex][first_vertex] -= minimum_flow;
            }

            // update residual graph by subtracting flow
            residual_graph->matrix[first_vertex][second_vertex] -= minimum_flow; // capacity shld flip from 1 to 0, means UCS won't go through this edge
            residual_graph->matrix[second_vertex][first_vertex] += minimum_flow; // capacity shld flip from 0 to 1, means UCS can go through this edge

            // update vertices
            first_vertex = second_vertex;
            pop(first_path);
        }

    }

    // start algorithm for source 2 to sink 2, ie match students to mentors
    int source_2 = 2;
    int sink_2 = 2 + residual_graph->sink_offset;
    int *second_visited_array;
    Stack *second_path;
    int second_bipartite_length = 1;

    // get minimum index for UCS. UCS cannot go into projects. minimum is index of last project
    int number_of_projects = residual_graph->student_offset - residual_graph->project_offset;

    while (second_bipartite_length != -1) { // length calc returns -1 if no path
        // path finding
        second_visited_array = UCS(residual_graph->matrix, residual_graph, source_2, sink_2, number_of_projects + residual_graph->project_offset, NO_CONNECTION); // can go from index of second source, skip project indices, and go till sink 2
        second_bipartite_length = length_calc(second_visited_array, source_2, sink_2);
        
        if (second_bipartite_length == -1) {
            break;
        }

        second_path = reform_path(second_visited_array, source_2, sink_2);

        // set minimum flow
        int minimum_flow = 1;

        // start for loop
        int first_vertex = peek(*second_path);
        int second_vertex;
        pop(second_path);

        while (!is_empty_stack(*second_path)) {
            second_vertex = peek(*second_path);

            // add flow
            if (original_graph->matrix[first_vertex][second_vertex] == 1) {
                flow_matrix[first_vertex][second_vertex] += minimum_flow;
            } else {
                flow_matrix[second_vertex][first_vertex] -= minimum_flow;
            }

            // update residual graph by subtracting flow
            residual_graph->matrix[first_vertex][second_vertex] -= minimum_flow;
            residual_graph->matrix[second_vertex][first_vertex] += minimum_flow;

            // update vertices
            first_vertex = second_vertex;
            pop(second_path);
        }
    }

    return flow_matrix;
}

int flow_calc_in(int **flow_matrix, int flow_into, int size) {
    int to_return = 0;
    for (int i = 0; i < size; i++) {
        if (flow_matrix[i][flow_into] == 1) {
            to_return++;
        } else if (flow_matrix[i][flow_into] == -1) {
            to_return--;
        }
    }

    return to_return;
}

int matching(Graph *graph, int **original_matrix) {
    // call edmond karp
    int **flow_matrix = edmonds_karp(graph);

    // count flow into sink 1
    int sink_1 = flow_calc_in(flow_matrix, 1 + graph->sink_offset, graph->V);

    // count flow into sink 2
    int sink_2 = flow_calc_in(flow_matrix, 2 + graph->sink_offset, graph->V);

    // return minimum of the 2
    if (sink_1 <= sink_2) {
        return sink_1;
    } else {
        return sink_2;
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