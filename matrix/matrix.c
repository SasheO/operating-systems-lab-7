/*
assumption: all matrices are square
*/


#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

int MAX;

struct matrixCell {
  int row, column, value;
};

void fillMatrix(int** matrix) {
    for(int i = 0; i<MAX; i++) {
        for(int j = 0; j<MAX; j++) {
            matrix[i][j] = rand()%10+1;
        }
    }
}

void printMatrix(int** matrix) {
    for(int i = 0; i<MAX; i++) {
        for(int j = 0; j<MAX; j++) {
            printf("%5d", matrix[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

// Fetches the appropriate coordinates from the argument, and sets
// the cell of matSumResult at the coordinates to the sum of the
// values at the coordinates of matA and matB.
void* computeSum(void* args) { // pass in the number of the ith thread
    // TODO: get the number through args, assign the right cell of matSumResult to value of sum
    struct matrixCell * targetCell = (struct matrixCell*) (args);
    return NULL;
}

// Fetches the appropriate coordinates from the argument, and sets
// the cell of matSumResult at the coordinates to the difference of the
// values at the coordinates of matA and matB.
void* computeDiff(void* args) { // pass in the number of the ith thread
    struct matrixCell * targetCell = (struct matrixCell*) (args);
    return NULL;
}

// Fetches the appropriate coordinates from the argument, and sets
// the cell of matSumResult at the coordinates to the inner product
// of matA and matB.
void* computeProduct(void* args) { // pass in the number of the ith thread
    struct matrixCell * targetCell = (struct matrixCell*) (args);
    return NULL;
}

// Spawn a thread to fill each cell in each result matrix.
// How many threads will you spawn?
int main(int argc, char *argv[]) {
    int **matA; 
    int **matB; 
    int **matSumResult;
    int **matDiffResult; 
    int **matProductResult; 
    int i;
    

    srand(time(0));  // Do Not Remove. Just ignore and continue below.
    
    // 0. Get the matrix size from the command line and assign it to MAX
    if (argc<2){
        fprintf(stderr, "Pass in the size of the matrix through command line\n");
        return 0;
    }
    else{
        MAX = atoi(argv[1]);
        printf("%d\n", MAX);
    }

    // initialize all matrices to size n*n where n = MAX
    
    matA = malloc(MAX * sizeof *matA);
    matB = malloc(MAX * sizeof *matB);
    matSumResult = malloc(MAX * sizeof *matSumResult);
    matDiffResult = malloc(MAX * sizeof *matDiffResult);
    matProductResult = malloc(MAX * sizeof *matProductResult);
    for (i=0; i<MAX; i++)
    {
        matA[i] = malloc(MAX * sizeof *matA[i]);
        matB[i] = malloc(MAX * sizeof *matB[i]);
        matSumResult[i] = malloc(MAX * sizeof *matSumResult[i]);
        matProductResult[i] = malloc(MAX * sizeof *matProductResult[i]);
        matDiffResult[i] = malloc(MAX * sizeof *matDiffResult[i]);
    }
        
    // 1. Fill the matrices (matA and matB) with random values.
    fillMatrix(matA);
    fillMatrix(matB);
    
    // 2. Print the initial matrices.
    printf("Matrix A:\n");
    printMatrix(matA);
    printf("Matrix B:\n");
    printMatrix(matB);
    
    // 3. TODO Create pthread_t objects for our threads.
    pthread_t threads[MAX];
    for (i=0; i<MAX; i++){

        // TODO: add, subtract, dot product
        // pthread_create(&threads[i], NULL, function_name, arguments to function); // look here https://youtube.com/watch?v=ldJ8WGZVXZk
    }
    
    // 4. Create a thread for each cell of each matrix operation.
    // 
    // You'll need to pass in the coordinates of the cell you want the thread
    // to compute.
    // 
    // One way to do this is to malloc memory for the thread number i, populate the coordinates
    // into that space, and pass that address to the thread. The thread will use that number to calcuate 
    // its portion of the matrix. The thread will then have to free that space when it's done with what's in that memory.
    
    // 5. Wait for all threads to finish.
    
    // 6. Print the results.
    printf("Results:\n");
    printf("Sum:\n");
    printMatrix(matSumResult);
    printf("Difference:\n");
    printMatrix(matDiffResult);
    printf("Product:\n");
    printMatrix(matProductResult);

    // TODO: deallocate all arrays https://stackoverflow.com/questions/3911400/how-to-pass-2d-array-matrix-in-a-function-in-c
    return 0;
  
}