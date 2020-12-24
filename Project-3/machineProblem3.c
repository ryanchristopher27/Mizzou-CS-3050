// Name: Ryan Christopher
// Pawprint: rdcb2f
// Date: 11/18/2020
// CS 3050
// Machine Problem 3

// All basic Heap functions have been implemented according to array that starts at A[1]
// Also implemented using the cyclic-shift idea

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

// Prototype Functions
void maxHeapify(int *heap, int size, int index);
void buildMaxHeap(int *heap, int size);
int insert(int *heap, int *size, int key);
void delete(int *heap,int *size, int index);
void heapChangeKey(int *heap, int *size, int index, int key);
void heapIncreaseKey(int *heap, int size, int index, int key);
void printHeap(int *heap, int size);

// Main Function
int main(void) {

    int size;
    int heap[1001];
    char command;
    int number;
    int index;

    FILE *fp = fopen("test.txt", "r");
    if (fp == NULL) { // Checks if file opened up correctly
        printf("Error: File could not be openeds\n");
    }
    else {
        fscanf(fp, "%d\n", &size); // Scans first value in the document as the size

        for (int i=1; i<size+1; i++) {
            fscanf(fp, "%d\n", &heap[i]);
        }

        buildMaxHeap(heap, size); // Builds the heap

        // printHeap(heap, size);
        
        while (!feof(fp)) { // Loop goes through file until it reaches the end
            fscanf(fp, " %c", &command); // All values after size are predefined by a command
            
            switch(command) { 
                case 'E': // Extract Max
                    fscanf(fp, "\n");
                    // printf("\nMax value is %d\n", heap[1]);
                    delete(heap, &size, 1);
                    // printHeap(heap, size);
                    break;
                case 'I': // Insert new key
                    fscanf(fp, " %d\n", &number);
                    insert(heap, &size, number);
                    // printHeap(heap, size);
                    break;
                case 'C': // Change value at A[i] to key
                    fscanf(fp, " %d %d\n", &index, &number);
                    heapChangeKey(heap, &size, index, number);
                    // printHeap(heap, size);
                    break;
                case 'D': // Delete value at A[i]
                    fscanf(fp, " %d\n", &index);
                    delete(heap, &size, index);
                    // printHeap(heap, size);
                    break;
                default:
                    printf("Invalid Command\n");
            }
        }

        printHeap(heap, size); // Prints the heap

    }

    return 0;
}

// Max Heapify implememnted to run from top to bottom to make heap follow required property
void maxHeapify(int *heap, int size, int index) {

    int left = 2*index; // Left child of index
    int right = 2*index + 1; // Right child of index
    int largest = index;

    if (index >= size) {
        return;
    }

    if (left <= size && heap[left] > heap[index]) {
        largest = left;
    }
    else {
        largest = index;
    }

    if (right <= size && heap[right] > heap[largest]) {
        largest = right;
    }

    if (largest != index) {
        int temp = heap[largest];
        heap[largest] = heap[index];
        heap[index] = temp;
        maxHeapify(heap, size, largest);
    }
}

// Build Max Heap goes from index size/2 to 1, ensures that all child nodes satisfy required property
void buildMaxHeap(int *heap, int size) {
    for (int i=size/2; i>0; i--) { // Loop from size/2 to 1
        maxHeapify(heap, size, i);
    }
}

// Inserts key into the heap and then rebuilds the heap
int insert(int *heap, int *size, int key) {

    if (*size == 0) { // Checks if heap is empty
        heap[1] = key;
        *size = *size + 1;
    }
    else {
        *size = *size + 1;
        heap[*size] = key;

        buildMaxHeap(heap, *size);

        // int index = *size;
        // heap[index] = 0;
        // heapIncreaseKey(heap, *size, index, key);
    }

    return 0;
}

// Deletes the element by replacing the desired index with the last element and then rebuilding the heap
void delete(int *heap, int *size, int index) {
    int last = heap[*size];

    heap[index] = last; // Sets desired index to last element

    *size = *size - 1;

    buildMaxHeap(heap, *size);
    // maxHeapify(heap, *size, index);
}

// Heap Change Key inserts element in desired index using heapIncreaseKey or buildMaxHeap, Also checks if index is available
void heapChangeKey(int *heap, int *size, int index, int key) {

    if ((index > *size) || (index < 1)) { // Checks if index is available
        printf("Error: Chosen index is not available");
        return;
    }
    
    if (key > heap[index]) { // Checks if key is greater than value at index
        heapIncreaseKey(heap, *size, index, key);
    }
    else {
        int hold = heap[index];
        heap[index] = key;
        if (key < hold) {
            // maxHeapify(heap, *size, index);
            buildMaxHeap(heap, *size);
        }
    }
    // delete(heap, size, index);
    // insert(heap, size, key);
}

// Heap Increase Key checks if key is smaller than current index value and then increases the priority key for the desired index
void heapIncreaseKey(int *heap, int size, int index, int key) {

    if (key < heap[index]) { // Checks if key is smaller than current value at index
        printf("Error: New Key is smaller than Current Key");
        return;
    }

    while ((index > 1) && (heap[index/2] < key)) {
        heap[index] = heap[index/2];
        index = index/2;
    }

    heap[index] = key;
}

// Print Heap prints the heap in order based on the array
void printHeap(int *heap, int size) {
    printf("\n");
    printf("Content of array is: ");
    for (int i=1; i<size+1; i++) { // Loops through the array until size+1
        printf("%d ", heap[i]);
    }
}