/*
David Bazler G01582055
CS 262 Lab Section 007
Lab 6
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int* get_memory(size_t size);
void InitArray(int *numArray, int arrayLength);
void printArray(int *arr, int len);
void ShuffleArray(int *numArray, int arrayLength);
int comp(const void *a, const void *b);

int main(int argc, char const *argv[])
{
    if (argc < 3 || argc > 3){
        printf("Usage:\n%s seed size\n" , argv[0]);
        exit(1);        
    }
    srand(atoi(argv[1]));
    int arrSize = atoi(argv[2]);
    int* numArray = get_memory(arrSize);
    for (int i = 0; i < 15; i++)
    {
        printf("Initalize an array\n");
        InitArray(numArray, arrSize);
        printArray(numArray, arrSize);
        printf("Shuffle an array\n");
        ShuffleArray(numArray, arrSize);
        printArray(numArray, arrSize);
        printf("Sort an array\n");
        qsort(numArray, arrSize, sizeof(int), comp);
        printArray(numArray, arrSize);
        
    }
    
    free(numArray);
    return 0;
}

int* get_memory(size_t size){
    if (size < 2 || size > 200){
        printf("Error! size must be within 2-200\n");
        exit(1);
    }
    int* ret = malloc(size*sizeof(int));
    if (ret == NULL){
        printf("Error allocating memory!");
        exit(-1);
    }
    return ret;
}

void InitArray(int *numArray, int arrayLength){
    for (int i = 0; i < arrayLength; i++)
    {
        numArray[i] = rand()%arrayLength+1;
    }
    
}

void printArray(int *arr, int len){
    for (int i = 0; i < len; i++)
    {
        if(i == len-1){
            printf("%d", arr[i]);
            break;
        }
        printf("%d, ", arr[i]);
    }
    printf("\n");
    
}

void ShuffleArray(int *numArray, int arrayLength){
    for(int i = arrayLength-1; i > 0; i--){
        int j = rand()%(i+1);
        
        int t = numArray[j];
        numArray[j] = numArray[i];
        numArray[i] = t;
    }
}

int comp(const void *a, const void *b){
    return (*(int*)b - *(int*)a);
}