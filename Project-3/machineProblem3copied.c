// Ryan Christopher
// CS 3050
// Machine Program 3


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

// Structs
int arr[100005]; // variable for array to store up to 100005 elements
int n;

// Prototype Functions

void max_heapify(int index){//run from top to bottom to make heap property follow
    if(index >= n)return;
    int left = 2*index+1;// add 1 to get left child index
    int right = 2*index + 2;// add two to get right child index
    int mx_ind = index;
    if(left < n && arr[left] > arr[mx_ind]){//use arr[left]<arr[mx_ind],we are creating min heap
    mx_ind = left;
    }
    if(right < n && arr[right] > arr[mx_ind]){
    mx_ind = right;
    }
    if(mx_ind != index){
    int tmp=arr[mx_ind];
    arr[mx_ind]=arr[index];
    arr[index]=tmp;
    max_heapify(mx_ind);
    }

}
int insert(int num, int location)//insert into heap moves from bottom to top
{
    int parentnode;
    while (location > 0)
    {
    parentnode =(location - 1)/2;
    if (num >= arr[parentnode])
    {
    arr[location] = arr[parentnode];
    }
    else break;
    location = parentnode;
    }
    arr[location] = num;
    return 0;
}
void delete_elem(int index)//replace the element to be deleted to the value of last element in array and use heapify
{                         // at index where value if replaced
    arr[index] = arr[n - 1];
    n = n - 1;
    // simply use heapify at index i and the subtree will follow heap property after running function
    max_heapify(index);

}
void build_max_heap(){
    for(int i=n/2-1;i>=0;i--){// start from index n/2-1 to 0, these indices have child nodes and satify heap property
    max_heapify(i);
}
}

void display(){
    for(int i=0;i<n;i++)printf("%d ",arr[i]);
    printf("\n");
}
int main()
{
    printf("Enter number of elements ");
    scanf(" %d",&n);
    printf("Enter the value of elements ");
    for(int i=0;i<n;i++)scanf(" %d",&arr[i]);
    build_max_heap();
    int num,index;
    char choice;
    while(1)
    {
        printf("E: means extract max \n");
        printf("I v: means insert new key vt \n");
        printf("C i v: change key at A[i] to v \n");
        printf("D i: delete key at A[i] \n");
        printf("Q: Exit() \n");
        printf("Enter your choice : ");
        scanf(" %c", &choice);
        switch(choice)
        {
        case 'E':
            printf("Max value if %d\n",arr[0]);
            printf("Content of array is: ");
            display();
            break;
        case 'I':
            scanf(" %d", &num);
            insert(num,n);n++;
            printf("Content of array is: ");
            display();
            break;
        case 'C':
            scanf(" %d %d",&index,&num);
            arr[index]=num;
            if(arr[(index-1)/2]>num)max_heapify(index);
            else insert(num,index);
            printf("Content of array is: ");
            display();
            break;
        case 'D':
            scanf(" %d",&index);
            delete_elem(index);
            printf("Content of array is: ");
            display();
            break;
        case 'Q':exit(1);
            break;
        default:printf("INVALID CHOICE \n");

        }
    }
}



// Main Function
// int main(void) {
    
// }