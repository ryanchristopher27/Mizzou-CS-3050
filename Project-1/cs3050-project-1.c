// Ryan Christopher
// CS 3050
// Machine Problem 1

// Algorithm Efficiency: 

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Prototype Functions
void findEdgeAndVert(FILE *fp, int *edges, int *vertices);
void printArray(int *vertices, int **arr);
void getArraySort(FILE *fp, int *edges, int *vertices, int **arr);

// Main Function
int main(void) {

    FILE *fptr;

    /* opens file */
    fptr = fopen("projectData.txt", "r");
    if (fptr == NULL) {
        printf("\nFile did not open");
        exit(-1);
    }

    int edges = 0;
    int vertices = 0;

    /* Finds the number of edges and vertices in the graph */
    findEdgeAndVert(fptr, &edges, &vertices);
  
    fclose(fptr);
    fptr = NULL;

    printf("\nNumber of Edges = %d", edges);
    printf("\nNumber of Vertices = %d", vertices);

    /* opens file */
    fptr = fopen("projectData.txt", "r");
    if (fptr == NULL) {
        printf("\nFile did not open");
        exit(-1);
    }

    /* Malloc for 2d array */
    int **arr = (int **)malloc(vertices * sizeof(int *)); 
    if (!**arr) {
        printf("\nMalloc Failed");
        exit(-1);
    }
    for (int i=0; i<vertices; i++) {
         arr[i] = (int *)malloc(vertices * sizeof(int));

         if (!arr[i]) {
             printf("\nMalloc Failed");
             exit(-1);
         }
    }

    /* Get Array function that sorts */
    getArraySort(fptr, &edges, &vertices, arr);

    /* Closes the file and sets the file pointer equal to NULL */
    fclose(fptr);
    fptr = NULL;

    /* Prints the values in each array backwards so they are in ascending order */
    printArray(&vertices, arr);

    /* Free 2d Array and holding array */
    for (int i=0; i<vertices; i++) {
         free(arr[i]);
    }
    free(arr);

    return 0;
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

/* Get Array Function that puts value in position during scan */
void getArraySort(FILE *fp, int *edges, int *vertices, int **arr) {

    int vert = *vertices;
    int edge = *edges;

    int first, second;
    int count = 0;

    /* Initialize 2d array to -1 */
    for (int i=0;i<vert;i++) {
        for (int j=0;j<vert;j++) {
            arr[i][j] = -1;
        }
    }

    /* Scans through the file and puts all the vertices into the array */
    while(count < edge) {

        fscanf(fp, "%d %d\n", &first, &second);

        arr[first-1][second-1] = second;
        //*(*(arr + first) + holdArr[first]) = second;
        arr[second-1][first-1] = first;
        //*(*(arr + second) + holdArr[second]) = first;

        count++;
    }
}

void printArray(int *vertices, int **arr) {

    int vert = *vertices;

    printf("\n\n");
    for (int i=0;i<vert;i++) {
        for (int j=0;j<vert;j++) {
            if (arr[i][j] != -1) {
                printf("%d ", arr[i][j]);
            }
            else {
                continue;
            }
        }
        printf("\n");
    }
}