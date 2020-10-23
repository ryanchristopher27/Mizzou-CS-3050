// Ryan Christopher
// Pawprint: rdcb2f
// CS 3050
// Machine Problem 2

// Algorithm Efficiency: I was able to get the entire program to run in linear time except
// for the function that finds the out degree for the strongly connected components.
// It is able to run in linear time because it there are no functions taht take longer
// than O(V + E).

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

// Structs
typedef struct nodestruct {
    int destination;
    int scc;
    bool used;
    struct nodestruct *next;
}node;

typedef struct adjListstruct {
    struct nodestruct *head;
}adjList;

typedef struct graphstruct {
    int vertices;
    int *visited;
    struct adjListstruct *array;
}graph;

typedef struct stackstruct {
    int maxsize;
    int top;
    int *items;
}stack;

// Prototype Functions
    // Finds the amount of edges and vertices
void findEdgeAndVert(FILE *fp, int *edges, int *vertices) {

    int first, second;

    while(fscanf(fp, "%d %d\n", &first, &second) != EOF) {

        /* Sets number of lines in file to the number of edges */
        *edges = *edges + 1;

        /* Sets highest number in file to number of vertices */
        if (first > *vertices) {
            *vertices = first;
        }
        if (second > *vertices) {
            *vertices = second;
        }
    }
}

    // Create a new node
node * newListNode(int dest) {

    node *new = (node*)malloc(sizeof(node));

    new->destination = dest;
    new->scc = 0;
    new->used = false;
    new->next = NULL;

    return new;
}

    // Create a graph
graph * createGraph(int vertices) {

    graph *mainG = (graph*)malloc(sizeof(graph));
    mainG->array = (adjList*)malloc(vertices * sizeof(adjList));
    mainG->vertices = vertices;

    for (int i=0;i<vertices;++i) {
        mainG->array[i].head = NULL;
    }

    return mainG;
}

    // Add edge to tranpose graph
void getTranspose(graph *transG, int source, int destination) {

    node *new = newListNode(source);
    new->next = transG->array[destination].head;
    transG->array[destination].head = new;
}

    // Add edges to graph
void addEdge(graph *mainG, graph *transG, int source, int destination) {

    node *new = newListNode(destination);

    new->next = mainG->array[source].head;
    mainG->array[source].head = new;
    getTranspose(transG, source, destination);

}

    // Print graph
void printGraph(graph *mainG) {
    
    for (int i=1;i<mainG->vertices+1;++i) {
        node *temp = mainG->array[i].head;

        printf("%d: ", i);
        while (temp) {
            printf(" -> (%d)", temp->destination);
            temp = temp->next;
        }
        printf("\n");
    }

}

    // Create Stack
stack * createStack(int vertices) {
    
    stack *tempStack = (stack*)malloc(sizeof(stack));

    tempStack->maxsize = vertices;
    tempStack->top = -1;
    tempStack->items = (int*)malloc(sizeof(int) *vertices);

    return tempStack;
}

    // Push Stack
void push(int vert, stack *mainStack) {
    
    if (mainStack->top >= mainStack->maxsize) {
        printf("\nStack is overflowwed");
        exit(-1);
    }
    else {
        mainStack->top++;
        mainStack->items[mainStack->top] = vert;
    }
}

    // Pop Stack
void pop(stack *mainStack) {
    
    if (mainStack->top <= -1) {
        printf("\nStack is underflowwed");
        exit(-1);
    }
    else {
        mainStack->top--;
    }
}

    // Fill the stack
void fillStack(graph *mainG, int vert, bool visited[], stack *mainStack) {

    visited[vert] = true;

    node *temp = mainG->array[vert].head;

    while (temp) {
        if (visited[temp->destination] == false) {
            fillStack(mainG, temp->destination, visited, mainStack);
        }
        temp = temp->next;
    }
    push(vert, mainStack);
}

    // Recursive function to print DFS starting from v
/*
void depthFirstSearch(graph *transG, int vert, bool visited[]) {

    visited[vert] = true;
    printf("%d ", vert);
    node *temp = transG->array[vert].head;

    while (temp) {
        if (visited[temp->destination] == false) {
            depthFirstSearch(transG, temp->destination, visited);
        }
        temp = temp->next;
    }
}*/
void depthFirstSearch(graph *transG, int vert, bool vis[], graph *sccG, graph *sccTransG, int count) {

    vis[vert] = true;
    printf("%d ", vert);
    node *temp = transG->array[vert].head;

    addEdge(sccG, sccTransG, count, vert);

    while (temp) {
        if (vis[temp->destination] == false) {
            depthFirstSearch(transG, temp->destination, vis, sccG, sccTransG, count);
        }
        temp = temp->next;
    }
}

    // Strongly Connected Components
/*
void stronglyConnectedComponents(graph *mainG, graph *transG, int vert, stack *mainStack) {

    bool visited[vert];
    for (int i=1;i<vert+1;i++) {
        visited[i] = false;
    }

    for (int j=1;j<vert+1;j++) {
        if(visited[j] == false) {
            fillStack(mainG, j, visited, mainStack);
        }
    }

    int count = 1;
    for (int k=1;k<vert+1;k++) {
        visited[k] = false;
    }

    while (mainStack->top != -1) {
        int hold = mainStack->items[mainStack->top];
        pop(mainStack);

        if (visited[hold] == false) {
            printf("Strongly connected component %d: \n", count++);
            depthFirstSearch(transG, hold, visited);
            printf("\n");
        }
    }
    printf("\nCount = %d", count-1);
}*/
graph * stronglyConnectedComponents(graph *mainG, graph *transG, int vert, stack *mainStack, int *count) {

    graph *sccG = createGraph(vert);
    graph *sccTransG = createGraph(vert);

    bool visited[vert];
    for (int i=1;i<vert+1;i++) {
        visited[i] = false;
    }

    for (int j=1;j<vert+1;j++) {
        if(visited[j] == false) {
            fillStack(mainG, j, visited, mainStack);
        }
    }

    int counter = *count;
    for (int k=1;k<vert+1;k++) {
        visited[k] = false;
    }

    while (mainStack->top != -1) {
        int hold = mainStack->items[mainStack->top];
        pop(mainStack);

        if (visited[hold] == false) {
            printf("Strongly connected component %d: \n", counter++);
            depthFirstSearch(transG, hold, visited, sccG, sccTransG, counter-1);
            printf("\n");
        }
    }
    *count = counter;

    for (int m=0;m<vert;m++) {
        free((graph*)sccTransG->array[m].head);
        // free((int*)mainStack->items[m]);
    }
    free(sccTransG);
    // free((stack*)mainStack);
    

    return sccG;
}

// Attempt to find the out degree of the stronlgy connected components

void compareSCC(graph *mainG, graph *sccG1, graph *sccG2, int vert, int *outD, int count) {

    for (int k=0;k<vert;k++) {
        outD[k] = 0;
    }

    /*
    for (int i=1;i<vert;i++) {
        for (int j=1;j<count;j++) {
            while (mainG->array[i].head) {
                while (sccG1->array[j].head) {
                    if (i == sccG1->array[j].head->destination) {
                        mainG->array[i].head->scc = j;
                    }
                    sccG1->array[j].head = sccG1->array[j].head->next;
                }
                mainG->array[i].head = mainG->array[i].head->next; 
            }
        }
    }
    printf("\n");
    printGraph(mainG);*/
    

}

// Main Function
int main(void) {
    
    FILE *fptr;

    fptr = fopen("test.txt", "r");
    if (fptr == NULL) {
        printf("\nFile did not open");
        exit(-1);
    }

    int edges = 0;
    int vertices = 0;
    int start, end;
    
    findEdgeAndVert(fptr, &edges, &vertices);

    graph *mainG = createGraph(vertices);
    graph *transG = createGraph(vertices);

    rewind(fptr);

    for (int i=0;i<edges;i++) {
        fscanf(fptr, "%d %d\n", &start, &end);

        addEdge(mainG, transG, start, end);
    }

    fclose(fptr);
    fptr = NULL;

    // printf("\nNormal Graph\n");
    // printGraph(mainG);

    // printf("\nTranspose Graph\n");
    // printGraph(transG);

    // printf("\n\n");

    stack *mainStack = createStack(vertices);

    int count = 1;
    graph *sccGraph = stronglyConnectedComponents(mainG, transG, vertices, mainStack, &count);
    // printGraph(sccGraph);

    int outDarray[vertices];
    compareSCC(mainG, sccGraph, sccGraph, vertices, outDarray, count);

    printf("\n\n");
    for (int j=0;j<count-1;j++) {
        printf("%d ", outDarray[j]);
    }

    for (int m=0;m<vertices;m++) {
        free((graph*)mainG->array[m].head);
        free((graph*)transG->array[m].head);
        free((graph*)sccGraph->array[m].head);
    }
    free(mainG);
    free(transG);
    free(sccGraph);

    return 0;
}
