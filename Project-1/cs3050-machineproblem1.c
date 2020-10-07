// Ryan Christopher
// Pawprint: rdcb2f
// CS 3050
// Machine Problem 1

// Algorithm Efficiency: I created this program with the mindset that it must be made
// in O(N) time. I believe it accomplishes this task. It may not be the most efficient
// way to do so, but I believe it gets it done nevertheless. 

// I was unable to sort the vertices in ascending order in O(N) time, but the TA
// said it did not necessarily have to do so.

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Prototype Functions
void findEdgeAndVert(FILE *fp, int *edges, int *vertices);
void printArray(int *vertices, int **arr, int *holdArr);
void getArray(FILE *fp, int *edges, int *vertices, int **arr, int *holdArr);

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

    /* Finds the number of edges and vertices in the graph */
    findEdgeAndVert(fptr, &edges, &vertices);
  
    fclose(fptr);
    fptr = NULL;

    //printf("\nNumber of Edges = %d", edges);
    //printf("\nNumber of Vertices = %d", vertices);

    /* opens file */
    fptr = fopen("test.txt", "r");
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

    /* Malloc for holding array */
    int *holdarr = (int *)malloc(vertices * sizeof(int));
    if (!*holdarr) {
        printf("\nMalloc Failed");
        exit(-1);
    }

    /* Gets the data from the file and puts it into the array */
    getArray(fptr, &edges, &vertices, arr, holdarr);

    /* Closes the file and sets the file pointer equal to NULL */
    fclose(fptr);
    fptr = NULL;

    /* Prints the values in each array backwards so they are in ascending order */
    printArray(&vertices, arr, holdarr);

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
void getArray(FILE *fp, int *edges, int *vertices, int **arr, int *holdArr) {

    int vert = *vertices;
    int edge = *edges;

    int first, second;
    int count = 0;

    for (int i=0;i<vert;i++) {
        holdArr[i] = 0;
    }

    /* Scans through the file and puts all the vertices into the array */
    while(count < edge) {

        fscanf(fp, "%d %d\n", &first, &second);

        arr[first-1][holdArr[first-1]] = second;
        arr[second-1][holdArr[second-1]] = first;

        holdArr[first-1] = holdArr[first-1] + 1;
        holdArr[second-1] = holdArr[second-1] + 1;

        count++;
    }
}

/* Prints the array in O(N) tie */
void printArray(int *vertices, int **arr, int *holdArr) {

    int vert = *vertices;

    printf("\n\n");
    for (int i=0;i<vert;i++) {
        for (int j=0;j<holdArr[i];j++) {
            printf("%d ", arr[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}