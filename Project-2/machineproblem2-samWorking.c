/*********************
 *Sam McIntyre       *
 *Rachel Carlson     *
 *Ryan Christopher   *
 *Machine Problem 1  *
 *10-15-20           *
 *********************/

 /*
 This algorithm was based on Tarjan's algoithm, which means it should operate
 in linear time.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct VertexNodeStruct VertexNode;
typedef struct AdjacentNodeStruct AdjacentNode;
//typedef struct EdgeNodeStruct EdgeNode;
typedef struct SCCNodeStruct SCCNode;
typedef struct SCCListStruct SCCList;
typedef struct DFSStackStruct DFSStack;

struct AdjacentNodeStruct{
    VertexNode *adjacent;
    AdjacentNode *nextAdjPtr;
};

struct VertexNodeStruct{
    int vertex;
    int sccID;
    int discoTime;
    int sccRootDiscoTime;
    bool onStack;
    AdjacentNode* adjacentsPtr;//for the adjacency list
    int numAdj;
    VertexNode* prevPtr;//for the stack
};

/*
struct EdgeNodeStruct{
    int u, v;
    EdgeNode *nextEdgePtr;
};
*/

struct SCCNodeStruct{
    VertexNode *vertex;
    int sccID;
    SCCNode *nextSCCPtr;
};

struct SCCListStruct{
    SCCNode *sccRoot;
    int outDeg;
    int numNodes;
    SCCList *nextPtr;
};

struct DFSStackStruct{
    VertexNode* topOfStack;
};

VertexNode* createVertexNode(int);
AdjacentNode* createAdjacentNode(VertexNode*);
//EdgeNode* createEdgeNode(int,int);
SCCNode* createSCCNode(VertexNode*,int);
VertexNode** createVertexArray(int);
void createAdjacencyList(FILE*,VertexNode**);
//EdgeNode* createEdgeList(FILE*);
SCCList* createSCCList(SCCNode*);
void strongConnect(VertexNode*,DFSStack*,int*,int*,SCCList*);
int findNumVertices(FILE*);
int* findOutDegrees(SCCList*,int);
void resetOffLimArr(bool*,int);
void setOffLimArr(bool*,int,SCCNode*);
DFSStack* initDFSStack();
int push(DFSStack*,VertexNode*);
VertexNode* pop(DFSStack*);
void printArr(int*,int);
void freeMemory(VertexNode**,SCCList*,DFSStack*);

int main(void){
    FILE *fp = fopen("machineProblem2Input.txt", "r");
    if(fp == NULL){
        printf("ERROR: Could not open file.\n");
    }else{
        int maxVertex = findNumVertices(fp);
        rewind(fp);
        VertexNode **vertArr = createVertexArray(maxVertex);
        createAdjacencyList(fp, vertArr);

        // <Tarjan's Algorithm for building SCCs>
        int sccCount = 0;
        int time = 0;
        DFSStack *stack = initDFSStack();
        SCCList *sccList = createSCCList(NULL);
        SCCList *curList = sccList;
        SCCNode *cursccNode = NULL;
        for(int i=0; i<maxVertex; i++){
            if(vertArr[i]->discoTime == -1){
                strongConnect(vertArr[i], stack, &time, &sccCount, sccList);
            }
        }
        // </Tarjan's> sccList contains the SCCs

        //find and print the out degrees in increasing order
        int* outDegArr = findOutDegrees(sccList, sccCount);
        printArr(outDegArr, sccCount);
        freeMemory(vertArr, sccList, stack);
    }
    return 0;
}

VertexNode* createVertexNode(int vertex){
/*create and return a new vertex node with the given int value*/

    VertexNode* newNode = malloc(sizeof(VertexNode));
    if(newNode == NULL){
        printf("malloc failed.\n");
    }else{
        newNode->vertex = vertex;
        newNode->sccID = -1;
        newNode->discoTime = -1;
        newNode->sccRootDiscoTime = -1;
        newNode->onStack = false;
        newNode->prevPtr = NULL;
        newNode->adjacentsPtr = NULL;
        newNode->numAdj = 0;
    }
    return newNode;
}

AdjacentNode* createAdjacentNode(VertexNode *vertex){
/*create an adjacent node with value vertex*/

    AdjacentNode* newNode = malloc(sizeof(AdjacentNode));
    if(newNode == NULL){
        printf("malloc failed.\n");
    }else{
        newNode->adjacent = vertex;
        newNode->nextAdjPtr = NULL;
    }
    return newNode;
}

/*
EdgeNode* createEdgeNode(int u, int v){
    EdgeNode* newNode = malloc(sizeof(EdgeNode));
    if(newNode == NULL){
        printf("malloc failed.\n");
    }else{
        newNode->u = u;
        newNode->v = v;
        newNode->nextEdgePtr = NULL;
    }
    return newNode;
}
*/

SCCNode* createSCCNode(VertexNode* v, int sccID){
/*insert the given vertex v into the SCC with ID sccID*/

    SCCNode* newNode = malloc(sizeof(SCCNode));
    if(newNode == NULL){
        printf("malloc failed.\n");
    }else{
        newNode->vertex = v;
        newNode->sccID = sccID;
        newNode->nextSCCPtr = NULL;
    }
    return newNode;
}

VertexNode** createVertexArray(int numVertex){
/*creates an array of vertices from 1 to the numVertex*/

    VertexNode** vertArr = malloc(sizeof(VertexNode*) * numVertex);
    if(vertArr == NULL){
        printf("malloc failed.\n");
    }else{
        for(int i=0; i<numVertex; i++){
            vertArr[i] = createVertexNode(i+1);
        }
    }
    return vertArr;
}

void createAdjacencyList(FILE* fp, VertexNode** vertArr){
/*creates an adjacency list of the given graph*/

    AdjacentNode *curAdj;
    int u, v;

    while(feof(fp) == 0){
        fscanf(fp, "%d %d\n", &u, &v);
        curAdj = vertArr[u-1]->adjacentsPtr;
        if(curAdj == NULL){
            curAdj = createAdjacentNode(vertArr[v-1]);
            vertArr[u-1]->adjacentsPtr = curAdj;
        }else{
            while(curAdj->nextAdjPtr != NULL){
                curAdj = curAdj->nextAdjPtr;
            }
            curAdj->nextAdjPtr = createAdjacentNode(vertArr[v-1]);
        }
        (vertArr[u-1]->numAdj)++;
    }
}

/*
EdgeNode* createEdgeList(FILE* fp){
    int u, v, maxVertex=0;

    //create the head node
    EdgeNode *headNode = malloc(sizeof(EdgeNode)+sizeof(int));
    if(headNode == NULL){
        printf("malloc failed.\n");
    }else{
        headNode = (EdgeNode*)(((int*)headNode)+1);
        headNode->u = -1;
        headNode->v = -1;
        headNode->nextEdgePtr = NULL;

        //create the rest...
        EdgeNode* curNode = headNode;
        while(feof(fp) == 0){
            fscanf(fp, "%d %d\n", &u, &v);

            //find max vertex...
            if(u > maxVertex)
            maxVertex = u;
            if(v > maxVertex)
            maxVertex = v;

            //build the edgelist...
            curNode->nextEdgePtr = createEdgeNode(u,v);
            curNode = curNode->nextEdgePtr;
        }
        //hide the maxVertex
        *((int*)headNode-1) = maxVertex;
    }
    return headNode;
}
*/

SCCList* createSCCList(SCCNode *sccRoot){
/*creates and returns a new SCC list node with the given root of an SCC*/

    SCCList *newList = malloc(sizeof(SCCList));
    if(newList == NULL){
        printf("malloc failed.\n");
    }else{
        newList->sccRoot = sccRoot;
        newList->outDeg = 0;
        newList->nextPtr = NULL;
    }
    return newList;
}

void strongConnect(VertexNode *v, DFSStack *stack, int *time, int *sccCount, SCCList* curList){
/*the backbone of Tarjan's algorithm. Will fill curList with SCCs*/

    (*time)++;
    v->discoTime = *time;
    v->sccRootDiscoTime = *time;
    push(stack, v);
    v->onStack = true;

    AdjacentNode *curAdj = v->adjacentsPtr;
    VertexNode *u;
    while(curAdj != NULL){
        u = curAdj->adjacent;
        if(u->discoTime == -1){
            strongConnect(u, stack, time, sccCount, curList);
            if(u->sccRootDiscoTime < v->sccRootDiscoTime){
                v->sccRootDiscoTime = u->sccRootDiscoTime;
            }
        }else if(u->onStack){
            if(u->discoTime < v->sccRootDiscoTime){
                v->sccRootDiscoTime = u->discoTime;
            }
        }
        curAdj = curAdj->nextAdjPtr;
    }

    if(v->discoTime == v->sccRootDiscoTime){
    //this is the scc root, lets make the scc...
        (*sccCount)++;
        SCCNode *sccHead = createSCCNode(NULL, *sccCount);
        SCCNode *curNode = sccHead;
        int nodeCount = 0;
        VertexNode *u;

        do{
            u = pop(stack);
            u->onStack = false;
            u->sccID = *sccCount;
            curNode->nextSCCPtr = createSCCNode(u, *sccCount);
            curNode = curNode->nextSCCPtr;
            nodeCount++;
        }while(v != u);

        while(curList->nextPtr!=NULL){
            curList=curList->nextPtr;
        }
        curList->nextPtr = createSCCList(sccHead);
        curList = curList->nextPtr;
        curList->numNodes = nodeCount;
    }
}

int findNumVertices(FILE* fp){
    int u, v, maxVertex=0;
    while(feof(fp) == 0){
        fscanf(fp, "%d %d\n", &u, &v);
        if(u > maxVertex)
            maxVertex = u;
        if(v > maxVertex)
            maxVertex = v;
  }
  return maxVertex;
}

int* findOutDegrees(SCCList* list, int numSCC){
/*find the out degrees of the SCCs, return in an int array*/

    //malloc offLimArr
    bool *offLimArr = malloc(sizeof(bool)*numSCC);
    if(offLimArr == NULL){
        printf("malloc failed.\n");
        return NULL;
    }

    //malloc outDegArr;
    int* outDegArr = malloc(sizeof(int)*numSCC + sizeof(int));
    if(outDegArr == NULL){
        printf("malloc failed.\n");
        return NULL;
    }
    *(outDegArr+0) = numSCC; //hiding the size
    outDegArr = outDegArr+1;

    //find out degrees and store them
    int outDegCount = 0, index = 0;
    SCCList *curList = list->nextPtr;
    SCCNode *cursccNode;
    VertexNode *v, *u;
    AdjacentNode *curAdj;

    while(curList != NULL){
        cursccNode = curList->sccRoot;
        setOffLimArr(offLimArr, numSCC, cursccNode);//scc members off limits
        for(int i=0; i<(curList->numNodes); i++){//for each member of the scc
            cursccNode = cursccNode->nextSCCPtr;
            v = cursccNode->vertex;
            curAdj = v->adjacentsPtr;
            for(int j=0; j<(v->numAdj); j++){//for each adjacent to each member
                u = curAdj->adjacent;
                if(!offLimArr[(u->sccID)-1]){
                    outDegCount++;//if it's not off limits, its an outDeg
                    offLimArr[(u->sccID)-1] = true;//...and make it off limits
                }
                curAdj = curAdj->nextAdjPtr;
            }
        }
        *(outDegArr+index) = outDegCount;
        curList->outDeg = outDegCount;
        outDegCount = 0;
        curList = curList->nextPtr;
        index++;
    }

    return outDegArr;
}

void resetOffLimArr(bool *offLimArr, int numSCC){
/*reset all bool values of the off limits array*/

    for(int i=0; i<numSCC; i++){
        offLimArr[i] = false;
    }
}

void setOffLimArr(bool *offLimArr, int numSCC, SCCNode *sccRoot){
/*set the current SCC's ID to off limits*/

    resetOffLimArr(offLimArr, numSCC);
    offLimArr[(sccRoot->sccID)-1] = true;
}

DFSStack* initDFSStack(){
/*initialize a DFSStack*/

    DFSStack *newStack = malloc(sizeof(DFSStack));
    if(newStack == NULL){
        printf("malloc failed.\n");
    }else{
        newStack->topOfStack = createVertexNode(-1);
    }
    return newStack;
}

int push(DFSStack* stack, VertexNode* v){
/*push vertex node v onto the stack*/

    v->prevPtr = stack->topOfStack;
    stack->topOfStack = v;
    return 0; //success
}

VertexNode* pop(DFSStack* stack){
/*pop the stack*/

    VertexNode* hold = stack->topOfStack;
    stack->topOfStack = hold->prevPtr;
    return hold;
}

void printArr(int *arr, int numSCC){
/*print in increasing order*/

    for(int i=0; i<numSCC; i++){
        for(int j=0; j<*(arr-1); j++){
            if(*(arr+j) == i){
                printf("%d ", *(arr+j));
            }
        }
    }
    printf("\n");
}

void freeMemory(VertexNode** vertArr, SCCList* sccList, DFSStack* stack){
/*free all dynamically allocated memory*/

    VertexNode *tempVert;
    AdjacentNode *tempAdj, *curAdj;
    SCCNode *tempSCC, *curSCC;
    SCCList *tempList;

    while(sccList != NULL){ //for each SCC list node
        tempSCC = sccList->sccRoot;
        if(tempSCC != NULL){
            while(tempSCC != NULL){ //for each SCC node
                if(tempSCC->vertex != NULL){//if there is a vertex
                    tempVert = tempSCC->vertex;
                    tempAdj = tempVert->adjacentsPtr;

                    while(tempAdj != NULL){ //free each adjacent of the vertex
                        curAdj = tempAdj->nextAdjPtr;
                        free(tempAdj);
                        tempAdj = curAdj;
                    }

                    free(tempVert); //free the vertex;
                }

                curSCC = tempSCC->nextSCCPtr; //free the SCC node
                free(tempSCC);
                tempSCC = curSCC;
            }
        }
        tempList = sccList->nextPtr; //free the SCC list node
        free(sccList);
        sccList = tempList;
    }

    while(stack->topOfStack != NULL){ //for each item still in the stack
        tempVert = pop(stack); //pop it off
        if(tempVert != NULL){ //...and if it isn't NULL...
            free(tempVert); //free it
        }
    }
    //free(stack); // free the stack itself

    //free(vertArr); // free the vertex array pointer

}
