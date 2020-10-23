// Ryan Christopher
// Pawprint: rdcb2f
// CS 3050
// Machine Problem 2

// Algorithm Efficiency:

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Structs
typedef struct nodestruct {
    int vertex;
    int discover;
    int finish;
    struct nodestruct *next;
}node;

typedef struct adjListstruct {
    struct nodestuct *head;
}adjList;

typedef struct graphstruct {
    int vertices;
    int *visited;

    node **lists;
}graph;

// Prototype Functions
void findEdgeAndVert(FILE *fp, int *edges, int *vertices);
node * createNode(int i);
graph * createGraph(int vert);
void addEdges (FILE *fp, graph *mainG, int edges);
void printGraph(graph *mainG);
void DepthFirstSearch(graph *mainG, int vert, int *timeP);
void transposeGraph(graph *mainG, graph *tG);

// Main Function
int main(void) {

    FILE *fptr;

    /* opens file */
    fptr = fopen("test.txt", "r");
    if (fptr == NULL) {
        printf("\nFile did not open");
        exit(-1);
    }

    int edges = 0;
    int vertices = 0;
    int time;

    findEdgeAndVert(fptr, &edges, &vertices);

    fclose(fptr);
    fptr = NULL;

    /* opens file */
    fptr = fopen("test.txt", "r");
    if (fptr == NULL) {
        printf("\nFile did not open");
        exit(-1);
    }

    graph *mainGraph = createGraph(vertices);
    addEdges(fptr, mainGraph, edges);
    printGraph(mainGraph);
    time = 0;
    DepthFirstSearch(mainGraph, 1, &time);

    graph *tGraph = createGraph(vertices);
    transposeGraph(mainGraph, tGraph);
    printGraph(tGraph);
    time = 0;
    

    fclose(fptr);
    fptr = NULL;

}

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

node * createNode(int i) {
    node *new = malloc(sizeof(node));
    new->next = NULL;
    new->vertex = i;
    new->discover = 0;
    new->finish = 0;
    return new;
}

graph * createGraph(int vert) {
    graph *new = malloc(sizeof(graph));
    new->vertices = vert;
    new->visited = malloc(sizeof(int) * vert);
    new->lists = malloc(sizeof(node*) * vert);

    for (int i=0;i<vert;i++) {
        new->visited[i] = 0;
        new->lists[i] = NULL;
    }
    return new;
}

void addEdges (FILE *fp, graph *mainG, int edges) {

    int start, end;
    for (int i=0;i<edges;i++) {

        fscanf(fp, "%d %d\n", &start, &end);

        node *new = createNode(end);
        new->next = mainG->lists[start];
        mainG->lists[start] = new;
    }
}

void printGraph(graph *mainG) {
    for (int i=1;i<mainG->vertices+1;i++) {
        node *temp = mainG->lists[i];
        printf("\nAdjacency list of vertex %d\n", i);
        while (temp) {
            printf("%d / %d -> ", temp->vertex, temp->finish);
            temp = temp->next;
        }
        printf("\n");
    }
}

void DepthFirstSearch(graph *mainG, int vert, int *timeP) {
    node *temp = mainG->lists[vert];
    // node *temp = List;

    int time = *timeP;

    time++;
    mainG->lists[vert]->discover = time;

    mainG->visited[vert] = 1;
    printf("Visited %d\t", vert);
    printf("discovered = %d\t", mainG->lists[vert]->discover);
    printf("finished = %d\n", mainG->lists[vert]->finish);

    while (temp != NULL) {
        int connectedV = temp->vertex;

        if (mainG->visited[connectedV] == 0) {
            DepthFirstSearch(mainG, connectedV, &time);
        }
        temp = temp->next;
        printf("test");
    }

    time++;
    mainG->lists[vert]->finish = time;
    
    printf("done");
}

void transposeGraph(graph *mainG, graph *tG) {

    int endTemp;

    for (int i=1;i<mainG->vertices+1;i++) {
        node *temp = mainG->lists[i];

        while (temp) {
            endTemp = temp->vertex;
            node *new = createNode(i);
            new->next = tG->lists[endTemp];
            tG->lists[endTemp] = new;
            temp = temp->next;

            // printf("\n%d %d", tG->lists[mainG->lists[i]->vertex]->vertex, endTemp);
        }
    }
}

