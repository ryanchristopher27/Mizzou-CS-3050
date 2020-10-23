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

struct VertexNodeStruct;

typedef struct EdgeStruct{
    int u, v;
    struct EdgeStruct *nextEdgePtr;
}EdgeNode;

typedef struct AdjacentNodeStruct{
    struct VertexNodeStruct *adjacent;
    struct AdjacentNodeStruct *nextAdjPtr;
}AdjacentNode;

typedef struct VertexNodeStruct{
    int vertex;
    int discoTime;
    int sccRootDiscoTime;
    bool onStack;
    AdjacentNode* adjacentsPtr;//for the adjacency list
    struct VertexNodeStruct* prevPtr;//for the stack
}VertexNode;

typedef struct IntLLStruct{
    int val;
    struct IntLLStruct *nextPtr;
}IntLL;

typedef struct SCCNodeStruct{
    VertexNode *vertex;
    struct SCCNodeStruct *nextSCCPtr;
}SCCNode;

typedef struct SCCListStruct{
    SCCNode *sccRoot;
    int outDeg;
    struct SCCListStruct *nextPtr;
}SCCList;

typedef struct DFSStackStruct{
    VertexNode* topOfStack;
}DFSStack;

EdgeNode* createEdgeList(FILE*);
EdgeNode* createEdgeNode(int,int);
VertexNode* createVertexNode(int);
IntLL* createIntLL(int);
AdjacentNode* createAdjacentNode(VertexNode*);
SCCNode* createSCCNode(VertexNode*);
VertexNode** createVertexArray(int);
void createAdjacencyList(FILE*,VertexNode**);
int findNumVertices(FILE*);
SCCNode* strongConnect(VertexNode*, DFSStack*, int*, SCCList*);
int* findOutDegrees(SCCList*);
bool offLimits(int, IntLL*);
SCCList* createSCCList(SCCNode*);
DFSStack* initDFSStack();
int push(DFSStack*,VertexNode*);
VertexNode* pop(DFSStack*);
//void freeList(vertexNode*);

int main(void){
    FILE *fp = fopen("machineProblem2Input.txt", "r");
    if(fp == NULL){
        printf("ERROR: Could not open file.\n");
    }else{
        //EdgeNode *edgeList = createEdgeList(fp);
        int maxVertex = findNumVertices(fp);
        rewind(fp);
        VertexNode **vertArr = createVertexArray(maxVertex);
        createAdjacencyList(fp, vertArr);
/*
        AdjacentNode *curAdj;
        for(int i=0; i<maxVertex; i++){
            printf("%d : ", vertArr[i]->vertex);

            curAdj = vertArr[i]->adjacentsPtr;
            while(curAdj != NULL){
                printf(" %d ", curAdj->adjacent->vertex);
                curAdj = curAdj->nextAdjPtr;
            }
            printf("\n");
        }
*/

        int time = 0;
        DFSStack *stack = initDFSStack();
        SCCList *sccList = createSCCList(NULL);
        SCCList *curNode = sccList;
        SCCNode *cursccNode = NULL;
        for(int i=0; i<maxVertex; i++){
            if(vertArr[i]->discoTime == -1){
                strongConnect(vertArr[i], stack, &time, sccList);
            }
        }
        curNode = sccList->nextPtr;

        /*while(curNode != NULL){
            cursccNode = curNode->sccRoot;
            //printf("zzz");
            while(cursccNode->nextSCCPtr != NULL){
                cursccNode = cursccNode->nextSCCPtr;
                printf(" %d ", cursccNode->vertex->vertex);
            }
            //printf("lll");
            curNode = curNode->nextPtr;
            //printf("vvv");
            printf("\n");
            //printf("ooo\n");
        }
        //printf("eee\n");
        */
        int* outDegs = findOutDegrees(sccList);
    }
    return 0;
}

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

VertexNode* createVertexNode(int vertex){
    VertexNode* newNode = malloc(sizeof(VertexNode));
    if(newNode == NULL){
        printf("malloc failed.\n");
    }else{
        newNode->vertex = vertex;
        newNode->discoTime = -1;
        newNode->sccRootDiscoTime = -1;
        newNode->onStack = false;
        newNode->prevPtr = NULL;
        newNode->adjacentsPtr = NULL;
    }
    return newNode;
}

IntLL* createIntLL(int val){
    IntLL* newNode = malloc(sizeof(IntLL));
    if(newNode == NULL){
        printf("malloc failed.\n");
    }else{
        newNode->val = val;
        newNode->nextPtr = NULL;
    }
    return newNode;
}

AdjacentNode* createAdjacentNode(VertexNode *vertex){
    AdjacentNode* newNode = malloc(sizeof(AdjacentNode));
    if(newNode == NULL){
        printf("malloc failed.\n");
    }else{
        newNode->adjacent = vertex;
        newNode->nextAdjPtr = NULL;
    }
    return newNode;
}

SCCNode* createSCCNode(VertexNode* v){
    SCCNode* newNode = malloc(sizeof(SCCNode));
    if(newNode == NULL){
        printf("malloc failed.\n");
    }else{
        newNode->vertex = v;
        newNode->nextSCCPtr = NULL;
    }
    return newNode;
}

VertexNode** createVertexArray(int numVertex){
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

SCCNode* strongConnect(VertexNode *v, DFSStack *stack, int *time, SCCList* curList){
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
            strongConnect(u, stack, time, curList);
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
        SCCNode *sccHead = createSCCNode(NULL);
        SCCNode *curNode = sccHead;
        VertexNode *u;
        do{
            u = pop(stack);
            u->onStack = false;
            curNode->nextSCCPtr = createSCCNode(u);
            curNode = curNode->nextSCCPtr;
        }while(v != u);
        //return the newly created scc :)
        while(curList->nextPtr!=NULL){curList=curList->nextPtr;}
        curList->nextPtr = createSCCList(sccHead);
        curList = curList->nextPtr;
    }
    return NULL; //we aint there yet...RECURSE
}

int* findOutDegrees(SCCList* list){
    //printf("zzz");
    int i=0;
    IntLL *offLim = createIntLL(-1);
    IntLL *curInt = offLim->nextPtr;
    //printf("ppp");
    int outDeg = 0;
    SCCList *curListNode = list->nextPtr;
    SCCNode *curNode;
    AdjacentNode *curAdj;
    VertexNode *v;
    while(curListNode != NULL){
        curNode = curListNode->sccRoot;
        //curInt = curInt->nextPtr;
        while(curNode->nextSCCPtr != NULL){
            curNode = curNode->nextSCCPtr;
            v = curNode->vertex;
            curInt = createIntLL(v->vertex);
            curInt = curInt->nextPtr;
            //printf("hey1 %d\n", v->vertex);
        }
        curNode = curListNode->sccRoot;
        while(curNode->nextSCCPtr != NULL){
            i++;
            curNode = curNode->nextSCCPtr;
            v = curNode->vertex;
            curAdj = v->adjacentsPtr;
            while(curAdj != NULL){
                if(!offLimits(curAdj->adjacent->vertex, offLim)){
                    //printf("outDeg: %d\n", curAdj->adjacent->vertex);
                    outDeg++;
                    curInt = createIntLL(curAdj->adjacent->vertex);//fucky
                    curInt = curInt->nextPtr;
                }
                curAdj = curAdj->nextAdjPtr;
            }

        }
        if(outDeg>0)
            outDeg -= i+1;
        curListNode->outDeg = outDeg;
        printf("%d ", outDeg);
        offLim = createIntLL(-1);
        curInt = offLim->nextPtr;
        curListNode = curListNode->nextPtr;
        outDeg = 0;
        i=0;
    }
    printf("\n");
    return 0;
}

bool offLimits(int val, IntLL* oL){
    IntLL *curNode = oL->nextPtr;
    //printf("\nhere %d\n", val);
    while(curNode != NULL){
    //    printf("\n\n %d %d \n", curNode->val, val);
        if(curNode->val == val)
        //    printf("offlim^\n\n");
            return true;
        curNode = curNode->nextPtr;
    }
    return false;
}

SCCList* createSCCList(SCCNode *sccRoot){
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

DFSStack* initDFSStack(){
    DFSStack *newStack = malloc(sizeof(DFSStack));
    if(newStack == NULL){
        printf("malloc failed.\n");
    }else{
        newStack->topOfStack = createVertexNode(-1);
    }
    return newStack;
}

int push(DFSStack* stack, VertexNode* v){
    v->prevPtr = stack->topOfStack;
    stack->topOfStack = v;
    return 0; //success
}

VertexNode* pop(DFSStack* stack){
    VertexNode* hold = stack->topOfStack;
    stack->topOfStack = hold->prevPtr;
    return hold;
}
